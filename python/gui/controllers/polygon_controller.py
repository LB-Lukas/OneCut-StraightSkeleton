import tkinter as tk
import math
from tkinter import messagebox
import geometry
from utils.last_action import LastAction
from utils.intersection_helper import IntersectionHelper

# Define canvas bounds as constants.
CANVAS_WIDTH = 600
CANVAS_HEIGHT = 600

class PolygonController:
    def __init__(self, app, max_polygons: int = 1):
        self.app = app
        self.max_polygons = max_polygons
        self.points = []  # In-progress polygon (logic coordinates).
        self.polygons = []  # Completed polygons: {"points": [...], "skeleton_line_ids": [...]}
        self.last_actions = []  # Stack for undo actions.
        self.intersection_helper = IntersectionHelper()

        # For moving a vertex.
        self._moving_poly_index = None
        self._moving_vertex_index = None

    def _redraw(self):
        self.app.main_view.canvas_view._redraw()

    def _record_action(self, action, data: dict):
        self.last_actions.append((action, data))

    def add_point(self, lx: float, ly: float):
        if len(self.polygons) >= self.max_polygons:
            messagebox.showinfo("Error", f"Maximum number of polygons is {self.max_polygons}")
            return

        if not (0 <= lx <= CANVAS_WIDTH and 0 <= ly <= CANVAS_HEIGHT):
            print("Point out of bounds")
            return

        # Check for intersections with existing lines.
        intersects = self.intersection_helper.check_intersection(
            x=lx,
            y=ly,
            polygons=self.polygons,
            lines=[],
            canvas=self.app.main_view.canvas_view.canvas,
            points=self.points,
        )
        if intersects:
            messagebox.showerror("Error", "The new line intersects an existing line.")
            return

        self.points.append((lx, ly))
        self._record_action(LastAction.ADD_POINT, {"point": (lx, ly)})
        self._redraw()

    def finish_polygon(self):
        if len(self.points) < 3:
            messagebox.showinfo("Info", "A polygon must have at least 3 points.")
            return

        first_x, first_y = self.points[0]
        intersects = self.intersection_helper.check_intersection(
            x=first_x,
            y=first_y,
            polygons=self.polygons,
            lines=[],
            canvas=self.app.main_view.canvas_view.canvas,
            points=self.points,
        )
        if intersects:
            messagebox.showerror("Error", "Closing line intersects with an existing line.")
            return

        new_polygon = {"points": self.points.copy(), "skeleton_line_ids": []}
        self.polygons.append(new_polygon)
        self.points.clear()
        self._record_action(LastAction.FINISH_POLYGON, {"polygon": new_polygon})
        self._redraw()

    def toggle_skeleton(self):
        """
        Toggle the skeleton for the first polygon:
          - If the skeleton is already drawn, remove it.
          - Otherwise, generate and display it.
        """
        if not self.polygons:
            messagebox.showinfo("Info", "No polygon available to toggle skeleton.")
            return

        poly = self.polygons[0]
        if poly["skeleton_line_ids"]:
            # Skeleton is already drawn: remove it.
            old_skeleton = poly["skeleton_line_ids"].copy()
            self._record_action(LastAction.REMOVE_SKELETON, {"skeleton_line_ids": old_skeleton})
            poly["skeleton_line_ids"].clear()
            self._redraw()
        else:
            # Generate and display the skeleton.
            self._generate_skeleton()

    def _generate_skeleton(self):
        """Generate the skeleton for the first polygon."""
        try:
            from geometry import StraightSkeleton  # Import here to catch errors gracefully.
            poly = self.polygons[0]
            points = [geometry.Point(px, py) for px, py in poly["points"]]
            skeleton = StraightSkeleton(points)
            edges = skeleton.get_edges()
            if not edges:
                messagebox.showinfo("Info", "No skeleton edges generated.")
                self._redraw()
                return

            skeleton_data = []
            for edge in edges:
                src, tgt = edge
                skeleton_data.append({"coords": (src.x(), src.y(), tgt.x(), tgt.y())})
            poly["skeleton_line_ids"] = skeleton_data
            self._record_action(LastAction.GENERATE_SKELETON, {"skeleton_line_ids": skeleton_data.copy()})
            self._redraw()
        except Exception as e:
            messagebox.showerror("Error", f"Error generating skeleton:\n{e}")

    def _update_skeleton(self):
        """
        Regenerate the skeleton for the first polygon if it is visible.
        This method is called during vertex dragging so that the skeleton
        reflects the updated vertex positions without toggling it off.
        """
        if not self.polygons:
            return

        poly = self.polygons[0]
        # Only update if the skeleton is currently displayed.
        if not poly["skeleton_line_ids"]:
            self._redraw()
            return

        try:
            from geometry import StraightSkeleton
            points = [geometry.Point(px, py) for px, py in poly["points"]]
            skeleton = StraightSkeleton(points)
            edges = skeleton.get_edges()
            if edges:
                skeleton_data = []
                for edge in edges:
                    src, tgt = edge
                    skeleton_data.append({"coords": (src.x(), src.y(), tgt.x(), tgt.y())})
                poly["skeleton_line_ids"] = skeleton_data
            else:
                # If no edges are generated, clear the skeleton.
                poly["skeleton_line_ids"].clear()
            self._redraw()
        except Exception as e:
            messagebox.showerror("Error", f"Error generating skeleton:\n{e}")
            self._redraw()

    def drag_vertex(self, lx: float, ly: float):
        if self._moving_poly_index is None:
            return

        # Clamp the new vertex coordinates to the canvas bounds.
        lx = max(0, min(lx, CANVAS_WIDTH))
        ly = max(0, min(ly, CANVAS_HEIGHT))

        poly = self.polygons[self._moving_poly_index]
        if 0 <= self._moving_vertex_index < len(poly["points"]):
            poly["points"][self._moving_vertex_index] = (lx, ly)
        
        # Instead of toggling, update the skeleton if it is visible.
        if poly["skeleton_line_ids"]:
            self._update_skeleton()
        else:
            self._redraw()

    def delete_vertex(self, poly_index: int, vertex_index: int):
        """
        Delete a vertex from a finished polygon.
        Ensures that a polygon always has at least 3 vertices.
        """
        if poly_index < 0 or poly_index >= len(self.polygons):
            messagebox.showerror("Error", "Polygon not found.")
            return

        poly = self.polygons[poly_index]
        if len(poly["points"]) <= 3:
            messagebox.showinfo("Info", "Polygon must have at least 3 vertices.")
            return

        deleted_point = poly["points"].pop(vertex_index)
        self._record_action(LastAction.DELETE_VERTEX, {
            "polygon_index": poly_index,
            "vertex_index": vertex_index,
            "deleted_point": deleted_point
        })
        # If the skeleton is currently drawn, update it.
        if poly["skeleton_line_ids"]:
            self._update_skeleton()
        else:
            self._redraw()

    def insert_vertex(self, poly_index: int, vertex_index: int, new_point: tuple):
        """
        Insert a new vertex into a finished polygon at the specified index.
        """
        if poly_index < 0 or poly_index >= len(self.polygons):
            messagebox.showerror("Error", "Polygon not found.")
            return

        poly = self.polygons[poly_index]
        poly["points"].insert(vertex_index, new_point)
        self._record_action(LastAction.INSERT_VERTEX, {
            "polygon_index": poly_index,
            "vertex_index": vertex_index,
            "new_point": new_point
        })
        if poly["skeleton_line_ids"]:
            self._update_skeleton()
        else:
            self._redraw()

    def undo_last_action(self):
        if not self.last_actions:
            messagebox.showinfo("Info", "No actions to undo.")
            return

        action, data = self.last_actions.pop()
        undo_actions = {
            LastAction.ADD_POINT: self._undo_add_point,
            LastAction.FINISH_POLYGON: self._undo_finish_polygon,
            LastAction.MOVE_POINT: self._undo_move_point,
            LastAction.GENERATE_SKELETON: self._undo_generate_skeleton,
            LastAction.REMOVE_SKELETON: self._undo_remove_skeleton,
            LastAction.DELETE_VERTEX: self._undo_delete_vertex,
            LastAction.INSERT_VERTEX: self._undo_insert_vertex,
        }
        undo_func = undo_actions.get(action)
        if undo_func:
            undo_func(data)
        self._redraw()

    def _undo_add_point(self, data: dict):
        point = data.get("point")
        if point in self.points:
            self.points.remove(point)

    def _undo_finish_polygon(self, data: dict):
        polygon = data.get("polygon")
        if polygon in self.polygons:
            self.polygons.remove(polygon)

    def _undo_move_point(self, data: dict):
        poly_index = data.get("polygon_index")
        vertex_index = data.get("vertex_index")
        old_x = data.get("old_x")
        old_y = data.get("old_y")
        if 0 <= poly_index < len(self.polygons):
            poly = self.polygons[poly_index]
            if 0 <= vertex_index < len(poly["points"]):
                poly["points"][vertex_index] = (old_x, old_y)

    def _undo_generate_skeleton(self, data: dict):
        skeleton_ids = data.get("skeleton_line_ids", [])
        if self.polygons:
            poly = self.polygons[0]
            for sk in skeleton_ids:
                if sk in poly["skeleton_line_ids"]:
                    poly["skeleton_line_ids"].remove(sk)

    def _undo_remove_skeleton(self, data: dict):
        skeleton_ids = data.get("skeleton_line_ids", [])
        if self.polygons:
            poly = self.polygons[0]
            poly["skeleton_line_ids"].extend(skeleton_ids)

    def _undo_delete_vertex(self, data: dict):
        poly_index = data.get("polygon_index")
        vertex_index = data.get("vertex_index")
        deleted_point = data.get("deleted_point")
        if poly_index is not None and 0 <= poly_index < len(self.polygons):
            self.polygons[poly_index]["points"].insert(vertex_index, deleted_point)

    def _undo_insert_vertex(self, data: dict):
        poly_index = data.get("polygon_index")
        vertex_index = data.get("vertex_index")
        if poly_index is not None and 0 <= poly_index < len(self.polygons):
            poly = self.polygons[poly_index]
            if 0 <= vertex_index < len(poly["points"]):
                poly["points"].pop(vertex_index)

    def begin_move_vertex(self, lx: float, ly: float):
        threshold = 10.0
        best_dist_sq = threshold ** 2
        found_poly_idx = None
        found_vert_idx = None

        for i, poly in enumerate(self.polygons):
            for j, (vx, vy) in enumerate(poly["points"]):
                dist_sq = (vx - lx) ** 2 + (vy - ly) ** 2
                if dist_sq < best_dist_sq:
                    best_dist_sq = dist_sq
                    found_poly_idx = i
                    found_vert_idx = j

        if found_poly_idx is not None:
            self._moving_poly_index = found_poly_idx
            self._moving_vertex_index = found_vert_idx
            vx, vy = self.polygons[found_poly_idx]["points"][found_vert_idx]
            self._record_action(LastAction.MOVE_POINT, {
                "polygon_index": found_poly_idx,
                "vertex_index": found_vert_idx,
                "old_x": vx,
                "old_y": vy,
            })

    def end_move_vertex(self):
        self._moving_poly_index = None
        self._moving_vertex_index = None
