import tkinter as tk
from tkinter import messagebox
import one_cut
from utils.last_action import LastAction
from utils.intersection_helper import IntersectionHelper
from utils.undo_manager import UndoManager
from models.polygon_model import PolygonModel


CANVAS_WIDTH = 600
CANVAS_HEIGHT = 600


class PolygonController:
    def __init__(self, app, max_polygons: int = 1):
        self.app = app
        self.max_polygons = max_polygons
        self.grid_enabled = False
        self.GRID_SPACING = 20

        # In-progress polygon (list of points)
        self.current_points: list[tuple[float, float]] = []
        # Finished polygons as models
        self.polygons: list[PolygonModel] = []

        self.undo_manager = UndoManager()
        self.intersection_helper = IntersectionHelper()
        self.selected_vertex: tuple[int, int] = None  # (polygon_index, vertex_index)
        self.selected_edge: tuple[int, int] = None  # (polygon_index, edge_index)
        self.show_skeleton: bool = False
        self.show_perpendiculars: bool = False
        self.show_crease_pattern: bool = False

        # For dragging a vertex.
        self._moving_poly_index: int = None
        self._moving_vertex_index: int = None
        
        
    def toggle_grid(self):  # Add this method
        self.grid_enabled = not self.grid_enabled
        self._redraw()


    def _redraw(self):
        self.app.main_view.canvas_view._redraw()


    def _record_action(self, action, data: dict):
        self.undo_manager.record_action(action, data)


    def select_vertex(self, lx: float, ly: float) -> bool:
        result = self._get_closest_vertex(lx, ly)
        if result:
            poly_idx, vertex_idx, _ = result
            self.selected_vertex = (poly_idx, vertex_idx)
            self.selected_edge = None
            return True
        else:
            self.selected_vertex = None
            return False
        
    def select_edge(self, lx: float, ly: float) -> bool:
        result = self._get_closest_edge(lx, ly)
        if result:
            poly_idx, vertex_idx, _ = result
            self.selected_edge = (poly_idx, vertex_idx)
            self.selected_vertex = None
            return True
        else:
            self.selected_edge = None
            return False


    def clear_selection(self):
        self.selected_vertex = None


    def add_point(self, lx: float, ly: float):
        if len(self.polygons) >= self.max_polygons:
            messagebox.showinfo("Error", f"Maximum number of polygons is {self.max_polygons}")
            return
        
        if self.grid_enabled:
            lx = round(lx / self.GRID_SPACING) * self.GRID_SPACING
            ly = round(ly / self.GRID_SPACING) * self.GRID_SPACING

        if not (0 <= lx <= CANVAS_WIDTH and 0 <= ly <= CANVAS_HEIGHT):
            print("Point out of bounds")
            return
        
        for poly in self.polygons:
            if PolygonModel.point_in_polygon(lx, ly, poly.points):
                messagebox.showerror("Error", "The new point is inside an existing polygon.")
                return

        if self.intersection_helper.check_intersection(
            x=lx,
            y=ly,
            polygons=self.polygons,
            lines=[],
            canvas=self.app.main_view.canvas_view.canvas,
            points=self.current_points,
        ):
            messagebox.showerror("Error", "The new line intersects an existing line.")
            return

        self.current_points.append((lx, ly))
        self._record_action(LastAction.ADD_POINT, {"point": (lx, ly)})
        self._redraw()


    def finish_polygon(self):
        if len(self.current_points) < 3:
            messagebox.showinfo("Info", "A polygon must have at least 3 points.")
            return

        first_x, first_y = self.current_points[0]
        if self.intersection_helper.check_intersection(
            x=first_x,
            y=first_y,
            polygons=self.polygons,
            lines=[],
            canvas=self.app.main_view.canvas_view.canvas,
            points=self.current_points,
        ):
            messagebox.showerror("Error", "Closing line intersects with an existing line.")
            return

        new_poly = PolygonModel(self.current_points)
        self.polygons.append(new_poly)
        self.current_points = []  # reset in-progress polygon


        if self.show_crease_pattern or self.show_skeleton or self.show_perpendiculars:
            try:
                new_poly.generate_creases()
            except Exception as e:
                messagebox.showerror("Error", str(e))
                print(e)

        self._record_action(LastAction.FINISH_POLYGON, {"polygon": new_poly})
        self._redraw()


    def add_point_to_polygon(self):
        if not self.polygons:
            messagebox.showinfo("Info", "No polygon available to add point.")
            return

        if self.selected_edge is None:
            return

        poly_index, edge_index = self.selected_edge
        poly = self.polygons[poly_index]
        p1 = poly.points[edge_index]
        p2 = poly.points[(edge_index + 1) % len(poly.points)]
        new_x = (p1[0] + p2[0]) / 2
        new_y = (p1[1] + p2[1]) / 2
        new_point = (new_x, new_y)
        self.insert_vertex(poly_index, edge_index + 1, new_point)
        self.selected_edge = None
        
        
    def update_drawing_mode(self, mode: str):
        print(f"Drawing mode changed to: {mode}")
        
        if not self.polygons:
            messagebox.showinfo("Info", "no polygon available to toggle skeleton.")
            return

        if mode == "None":
            self.show_crease_pattern = False
            self.show_skeleton = False
            self.show_perpendiculars = False
            self._redraw()
            return
        
        for poly in self.polygons:
            try:
                poly.generate_creases()
            except Exception as e:
                messagebox.showerror("Error", str(e))
        if mode == "Foldpattern":
            self.show_crease_pattern = True
            self.show_skeleton = False
            self.show_perpendiculars = False
            self._redraw()
        elif mode == "Skeleton":
            self.show_crease_pattern = False
            self.show_skeleton = True
            self.show_perpendiculars = False
            self._redraw()
        elif mode == "Perpendiculars":
            self.show_crease_pattern = False
            self.show_skeleton = False
            self.show_perpendiculars = True
            self._redraw()
        elif mode == "Skeleton and Perpendiculars":
            self.show_crease_pattern = False
            self.show_skeleton = True
            self.show_perpendiculars = True
            self._redraw()
        else:
            messagebox.showerror("Error", "Drawing mode not available")
            return
        
    def _redraw_or_update_creases(self, polygon=None):
        if polygon is not None:
            try:
                polygon.update_creases()
            except Exception as e:
                messagebox.showerror("Error", str(e))
                print(e)
        else:
            for poly in self.polygons:
                try:
                    poly.update_creases()
                except Exception as e:
                    messagebox.showerror("Error", str(e))
                    print(e)
        self._redraw()
        

    def drag_vertex(self, lx: float, ly: float):
        if self._moving_poly_index is None:
            return

        lx = max(0, min(lx, CANVAS_WIDTH))
        ly = max(0, min(ly, CANVAS_HEIGHT))
        
        if self.grid_enabled:
            lx = round(lx / self.GRID_SPACING) * self.GRID_SPACING
            ly = round(ly / self.GRID_SPACING) * self.GRID_SPACING

        for i, poly in enumerate(self.polygons):
            if i != self._moving_poly_index and len(poly.points) >= 3:
                if PolygonModel.point_in_polygon(lx, ly, poly.points):
                    return

        poly = self.polygons[self._moving_poly_index]
        new_points = poly.points.copy()
        new_points[self._moving_vertex_index] = (lx, ly)
        n = len(new_points)

        for i in range(n):
            a1 = new_points[i]
            a2 = new_points[(i + 1) % n]
            for j in range(i + 1, n):
                if j == i or (j + 1) % n == i or (i + 1) % n == j:
                    continue
                b1 = new_points[j]
                b2 = new_points[(j + 1) % n]
                if self.intersection_helper.is_intersecting((a1, a2), (b1, b2)):
                    return

        for i, other_poly in enumerate(self.polygons):
            if i == self._moving_poly_index:
                continue
            m = len(other_poly.points)
            if m < 2:
                continue
            for j in range(n):
                edge_new = (new_points[j], new_points[(j + 1) % n])
                for k in range(m):
                    edge_other = (other_poly.points[k], other_poly.points[(k + 1) % m])
                    if self.intersection_helper.is_intersecting(edge_new, edge_other):
                        return

        poly.move_point(self._moving_vertex_index, (lx, ly))
        self._redraw_or_update_creases()


    def delete_vertex(self, poly_index: int, vertex_index: int):
        if poly_index < 0 or poly_index >= len(self.polygons):
            messagebox.showerror("Error", "Polygon not found.")
            return

        poly = self.polygons[poly_index]
        if len(poly.points) <= 3:
            self.delete_polygon(poly_index)
            return

        try:
            deleted_point = poly.points.pop(vertex_index)
        except Exception as e:
            messagebox.showerror("Error", str(e))
            return

        self._record_action(LastAction.DELETE_VERTEX, {
            "polygon_index": poly_index,
            "vertex_index": vertex_index,
            "deleted_point": deleted_point
        })
        self._redraw_or_update_creases(poly)


    def delete_selected_vertex(self):
        if self.selected_vertex is None:
            return
        poly_index, vertex_index = self.selected_vertex
        self.delete_vertex(poly_index, vertex_index)
        self.selected_vertex = None


    def delete_selected_polygon(self):
        if self.selected_edge is not None:
            poly_index, _ = self.selected_edge
            self.delete_polygon(poly_index)
            self.selected_edge = None
        elif self.selected_vertex is not None:
            poly_index, _ = self.selected_vertex
            self.delete_polygon(poly_index)


    def delete_polygon(self, poly_index: int):
        if poly_index < 0 or poly_index >= len(self.polygons):
            messagebox.showerror("Error", "Polygon not found.")
            return
        removed_poly = self.polygons.pop(poly_index)
        self._record_action(LastAction.FINISH_POLYGON, {"polygon": removed_poly})
        self._redraw()


    def insert_vertex(self, poly_index: int, vertex_index: int, new_point: tuple):
        if poly_index < 0 or poly_index >= len(self.polygons):
            messagebox.showerror("Error", "Polygon not found.")
            return
        poly = self.polygons[poly_index]
        poly.insert_point(vertex_index, new_point)
        self._record_action(LastAction.INSERT_VERTEX, {
            "polygon_index": poly_index,
            "vertex_index": vertex_index,
            "new_point": new_point
        })
        self._redraw_or_update_creases(poly)


    def undo_last_action(self):
        if not self.undo_manager.has_actions():
            messagebox.showinfo("Info", "No actions to undo.")
            return

        action, data = self.undo_manager.pop_action()
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
        if point in self.current_points:
            self.current_points.remove(point)


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
            self.polygons[poly_index].points[vertex_index] = (old_x, old_y)


    def _undo_generate_skeleton(self, data: dict):
        skeleton_data = data.get("skeleton_line_ids", [])
        if self.polygons:
            poly = self.polygons[0]
            for sk in skeleton_data:
                if sk in poly.skeleton_line_ids:
                    poly.skeleton_line_ids.remove(sk)


    def _undo_remove_skeleton(self, data: dict):
        skeleton_data = data.get("skeleton_line_ids", [])
        if self.polygons:
            poly = self.polygons[0]
            poly.skeleton_line_ids.extend(skeleton_data)


    def _undo_delete_vertex(self, data: dict):
        poly_index = data.get("polygon_index")
        vertex_index = data.get("vertex_index")
        deleted_point = data.get("deleted_point")
        if poly_index is not None and 0 <= poly_index < len(self.polygons):
            self.polygons[poly_index].points.insert(vertex_index, deleted_point)


    def _undo_insert_vertex(self, data: dict):
        poly_index = data.get("polygon_index")
        vertex_index = data.get("vertex_index")
        if poly_index is not None and 0 <= poly_index < len(self.polygons):
            poly = self.polygons[poly_index]
            if 0 <= vertex_index < len(poly.points):
                poly.points.pop(vertex_index)


    def begin_move_vertex(self, lx: float, ly: float):
        result = self._get_closest_vertex(lx, ly)
        if result:
            poly_idx, vertex_idx, _ = result
            self._moving_poly_index = poly_idx
            self._moving_vertex_index = vertex_idx
            vx, vy = self.polygons[poly_idx].points[vertex_idx]
            self._record_action(
                LastAction.MOVE_POINT,
                {
                    "polygon_index": poly_idx,
                    "vertex_index": vertex_idx,
                    "old_x": vx,
                    "old_y": vy,
                }
            )


    def end_move_vertex(self):
        self._moving_poly_index = None
        self._moving_vertex_index = None
        
        
    def _get_closest_vertex(self, lx: float, ly: float, threshold: float = 10.0) -> tuple[int, int, float] | None:
        best_dist_sq = threshold ** 2
        found_poly_idx = None
        found_vertex_idx = None

        for i, poly in enumerate(self.polygons):
            for j, (vx, vy) in enumerate(poly.points):
                dist_sq = (vx - lx) ** 2 + (vy - ly) ** 2
                if dist_sq < best_dist_sq:
                    best_dist_sq = dist_sq
                    found_poly_idx = i
                    found_vertex_idx = j

        if found_poly_idx is not None:
            return (found_poly_idx, found_vertex_idx, best_dist_sq)
        return None
    
    
    def _distance_to_segment_squared(self, px: float, py: float, a: tuple[float, float], b: tuple[float, float]) -> float:
        ax, ay = a
        bx, by = b
        abx = bx - ax
        aby = by - ay
        apx = px - ax
        apy = py - ay
        dot_product = apx * abx + apy * aby
        if dot_product <= 0.0:
            return (px - ax)**2 + (py - ay)**2
        ab_len_sq = abx**2 + aby**2
        if dot_product >= ab_len_sq:
            return (px - bx)**2 + (py - by)**2
        t = dot_product / ab_len_sq
        proj_x = ax + t * abx
        proj_y = ay + t * aby
        return (px - proj_x)**2 + (py - proj_y)**2


    def _get_closest_edge(self, lx: float, ly: float, threshold: float = 10.0) -> tuple[int, int, float] | None:
        best_dist_sq = threshold ** 2
        found_poly_idx = None
        found_edge_idx = None
        for poly_idx, poly in enumerate(self.polygons):
            points = poly.points
            n = len(points)
            for edge_idx in range(n):
                p1 = points[edge_idx]
                p2 = points[(edge_idx + 1) % n]
                dist_sq = self._distance_to_segment_squared(lx, ly, p1, p2)
                if dist_sq < best_dist_sq:
                    best_dist_sq = dist_sq
                    found_poly_idx = poly_idx
                    found_edge_idx = edge_idx
        if found_poly_idx is not None:
            return (found_poly_idx, found_edge_idx, best_dist_sq)
        return None