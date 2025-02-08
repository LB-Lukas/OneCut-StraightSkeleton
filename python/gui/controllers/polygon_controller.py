import tkinter as tk
from tkinter import messagebox
import geometry
from geometry import StraightSkeleton
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
        self.selected_vertex: tuple[int, int] = None
        self.show_skeleton: bool = False
        self.show_perpendiculars: bool = False

        # For moving a vertex.
        self._moving_poly_index = None
        self._moving_vertex_index = None
        
        
    def _redraw(self):
        self.app.main_view.canvas_view._redraw()


    def _record_action(self, action, data: dict):
        self.last_actions.append((action, data))
        
    
    def select_vertex(self, lx: float, ly: float):
        threshold = 10.0
        best_dist_sq = threshold ** 2
        found_polygon_idx = None
        found_vertex_idx = None
        
        for i, poly in enumerate(self.polygons):
            for j, (vx, vy) in enumerate(poly["points"]):
                dist_sq = (vx - lx) ** 2 + (vy - ly) ** 2
                if dist_sq < best_dist_sq:
                    best_dist_sq = dist_sq
                    found_polygon_idx = i
                    found_vertex_idx = j
        
        if found_polygon_idx is not None:
            self.selected_vertex = (found_polygon_idx, found_vertex_idx)
            return True
        else:
            self.selected_vertex = None
            return False
        
    
    def clear_selection(self):
        self.select_vertex = None    
        
            
    def add_point(self, lx: float, ly: float):
        if len(self.polygons) >= self.max_polygons:
            messagebox.showinfo("Error", f"Maximum number of polygons is {self.max_polygons}")
            return

        if not (0 <= lx <= CANVAS_WIDTH and 0 <= ly <= CANVAS_HEIGHT):
            print("Point out of bounds")
            return

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
        
        
    def add_point_to_polygon(self):
        if not self.polygons:
            messagebox.showinfo("Info", "No polygon available to add point.")
            return
        
        if self.selected_vertex is None:
            return
        
        poly_index, vertex_index = self.selected_vertex
        poly = self.polygons[poly_index]
        
        has_next_vertex: bool = vertex_index + 1 < len(poly["points"])
        if has_next_vertex:
            next_vertex_index = vertex_index + 1
        else:
            next_vertex_index = 0
        vx1, vy1 = poly["points"][vertex_index]
        vx2, vy2 = poly["points"][next_vertex_index]
        
        new_x = (vx1 + vx2) / 2
        new_y = (vy1 + vy2) / 2
        
        offset = 5
        new_point = (new_x + offset, new_y + offset)
        self.insert_vertex(poly_index, next_vertex_index, new_point)


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
        
        if self.show_skeleton:
            self._generate_skeleton()
        
        self._record_action(LastAction.FINISH_POLYGON, {"polygon": new_polygon})
        self._redraw()


    def toggle_skeleton(self):
        """
        Toggle the skeleton for all polygons:
        - If at least one polygon’s skeleton is already drawn, remove skeletons for all.
        - Otherwise, generate and display skeletons for all polygons.
        """
        if not self.polygons:
            messagebox.showinfo("Info", "No polygon available to toggle skeleton.")
            return

        # Toggle the show_skeleton flag.
        self.show_skeleton = not self.show_skeleton

        if self.show_skeleton:
            # Generate and display skeletons for all polygons.
            self._generate_skeleton()
        else:
            # Remove skeletons from all polygons.
            old_skeleton_data = []
            for poly in self.polygons:
                # Save a copy of the current skeleton for undo purposes.
                old_skeleton_data.append(poly["skeleton_line_ids"].copy())
                poly["skeleton_line_ids"].clear()
            self._record_action(LastAction.REMOVE_SKELETON, {"skeleton_line_ids": old_skeleton_data})
            self._redraw()


    def _generate_skeleton(self):
        """
        Generate the skeleton for each polygon.
        """
        any_generated = False
        skeleton_data_all = []
        try:
            for poly in self.polygons:
                points = [geometry.Point(px, py) for px, py in poly["points"]]
                skeleton = StraightSkeleton(points)
                edges = skeleton.get_edges()
                skeleton_data = []
                if edges:
                    for edge in edges:
                        src, tgt = edge
                        skeleton_data.append({"coords": (src.x(), src.y(), tgt.x(), tgt.y())})
                    poly["skeleton_line_ids"] = skeleton_data
                    any_generated = True
                else:
                    poly["skeleton_line_ids"].clear()
                skeleton_data_all.append(skeleton_data)
            if not any_generated:
                messagebox.showinfo("Info", "No skeleton edges generated for any polygon.")
            self._record_action(LastAction.GENERATE_SKELETON, {"skeleton_line_ids": skeleton_data_all.copy()})
            self._redraw()
        except Exception as e:
            messagebox.showerror("Error", f"Error generating skeleton:\n{e}")


    def _update_skeleton(self):
        for poly in self.polygons:
            if poly["skeleton_line_ids"]:
                try:
                    from geometry import StraightSkeleton
                    points = [geometry.Point(px, py) for px, py in poly["points"]]
                    skeleton = StraightSkeleton(points)
                    edges = skeleton.get_edges()
                    skeleton_data = []
                    if edges:
                        for edge in edges:
                            src, tgt = edge
                            skeleton_data.append({"coords": (src.x(), src.y(), tgt.x(), tgt.y())})
                        poly["skeleton_line_ids"] = skeleton_data
                    else:
                        poly["skeleton_line_ids"].clear()
                except Exception as e:
                    messagebox.showerror("Error", f"Error generating skeleton:\n{e}")
        self._redraw()

    # Ray casting algorithm
    def point_in_polygon(self, x: float, y: float, poly_points: list[tuple[float, float]]) -> bool:
        inside = False
        n = len(poly_points)
        if n < 3:
            return False  # Not a valid polygon
        
        p1x, p1y = poly_points[0]
        for i in range(1, n + 1):
            p2x, p2y = poly_points[i % n]
            # Check if point is between the y-coordinates of the edge
            if y > min(p1y, p2y):
                if y <= max(p1y, p2y):
                    if x <= max(p1x, p2x):
                        # Compute intersection of the horizontal ray with the edge.
                        if p1y != p2y:
                            xinters = (y - p1y) * (p2x - p1x) / (p2y - p1y) + p1x
                        else:
                            xinters = p1x
                        if p1x == p2x or x <= xinters:
                            inside = not inside
            p1x, p1y = p2x, p2y
        return inside
    
    
    # Ray casting algorithm 
    def point_in_polygon(self, x: float, y: float, poly_points: list[tuple[float, float]]) -> bool:
        inside = False
        n = len(poly_points)
        if n < 3:
            return False  # Not a valid polygon
        p1x, p1y = poly_points[0]
        for i in range(1, n + 1):
            p2x, p2y = poly_points[i % n]
            if y > min(p1y, p2y):
                if y <= max(p1y, p2y):
                    if x <= max(p1x, p2x):
                        if p1y != p2y:
                            xinters = (y - p1y) * (p2x - p1x) / (p2y - p1y) + p1x
                        else:
                            xinters = p1x
                        if p1x == p2x or x <= xinters:
                            inside = not inside
            p1x, p1y = p2x, p2y
        return inside


    def drag_vertex(self, lx: float, ly: float):
        if self._moving_poly_index is None:
            return

        # Clamp the new vertex coordinates to the canvas bounds.
        lx = max(0, min(lx, CANVAS_WIDTH))
        ly = max(0, min(ly, CANVAS_HEIGHT))
        
        # Prevent dragging the vertex into any finished polygon.
        # (Assuming finished polygons are stored in self.polygons.)
        for i, poly in enumerate(self.polygons):
            if i != self._moving_poly_index and len(poly["points"]) >= 3:
                if self.point_in_polygon(lx, ly, poly["points"]):
                    # Optionally: messagebox.showinfo("Invalid Move", "Cannot move vertex inside another polygon.")
                    return

        # Create a temporary copy of the points for the polygon being edited,
        # with the moving vertex replaced by the new coordinates.
        poly = self.polygons[self._moving_poly_index]
        new_points = poly["points"].copy()
        new_points[self._moving_vertex_index] = (lx, ly)
        n = len(new_points)

        # 1. Check for self-intersections in the updated (moving) polygon.
        # Loop over all pairs of non-adjacent edges.
        for i in range(n):
            a1 = new_points[i]
            a2 = new_points[(i + 1) % n]
            for j in range(i + 1, n):
                # Skip adjacent edges or edges sharing a vertex.
                if j == i or (j + 1) % n == i or (i + 1) % n == j:
                    continue
                b1 = new_points[j]
                b2 = new_points[(j + 1) % n]
                if self.intersection_helper.is_intersecting((a1, a2), (b1, b2)):
                    # Optionally: messagebox.showinfo("Invalid Move", "Moving the vertex causes a self-intersection.")
                    return

        # 2. Check for intersections between any edge of the updated polygon
        #    and any edge of any other finished polygon.
        for i, other_poly in enumerate(self.polygons):
            if i == self._moving_poly_index:
                continue  # Skip the polygon being moved.
            other_points = other_poly["points"]
            m = len(other_points)
            if m < 2:
                continue
            for j in range(n):
                # Edge from updated polygon.
                edge_new = (new_points[j], new_points[(j + 1) % n])
                for k in range(m):
                    # Edge from the other polygon.
                    edge_other = (other_points[k], other_points[(k + 1) % m])
                    if self.intersection_helper.is_intersecting(edge_new, edge_other):
                        # Optionally: messagebox.showinfo("Invalid Move", "Moving the vertex would cause an intersection with another polygon.")
                        return

        # If all checks pass, update the vertex in the active polygon.
        poly["points"][self._moving_vertex_index] = (lx, ly)
        
        # Update skeleton if visible; otherwise, just redraw.
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
            self.delete_polygon(poly_index)
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
            
            
    def delete_selected_vertex(self):
        if self.selected_vertex is None:
            return
        poly_index, vertex_index = self.selected_vertex
        self.delete_vertex(poly_index, vertex_index)
        self.selected_vertex = None
        
        
    def delete_selected_polygon(self):
        if self.selected_vertex is None:
            return
        poly_index, _ = self.selected_vertex
        self.delete_polygon(poly_index)    
        
    
    def delete_polygon(self, poly_index: int):
        if poly_index < 0 or poly_index >= len(self.polygons):
            messagebox.showerror("Error", "Polygon not found.")
            return

        self.polygons.pop(poly_index)
        self._record_action(LastAction.FINISH_POLYGON, {"polygon": None})
        self._redraw()
        

    def insert_vertex(self, poly_index: int, vertex_index: int, new_point: tuple):
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
