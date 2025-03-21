import tkinter as tk
from controllers.polygon_controller import PolygonController
from controllers.file_controller import FileController
from views.main_view import MainView


class StraightSkeletonApp:
    def __init__(self, root: tk.Tk, max_polygons: int = 1):
        self.root = root
        self.max_polygons = max_polygons
        self.polygon_controller = PolygonController(self, max_polygons)
        self.file_controller = FileController()
        self.main_view = MainView(root, self)
        self.main_view.pack(fill=tk.BOTH, expand=True)
        self._setup_button_commands()


    def _setup_button_commands(self) -> None:
        # self.main_view.open_file_button.config(command=self.file_controller.open_file) removed this button from the GUI #TODO: rework this functionality
        self.main_view.finish_polygon_button.config(command=self.polygon_controller.finish_polygon)
        # self.main_view.export_pdf_button.config(command=self.file_controller.export_to_pdf) removed this button from the GUI #TODO: rework this functionality
        # self.main_view.export_png_button.config(command=self.file_controller.export_to_png) removed this button from the GUI #TODO: rework this funcitonality
        #self.main_view.toggle_skeleton_button.config(command=self.polygon_controller.toggle_skeleton)
        #self.main_view.toggle_perpendiculars_button.config(command=self.polygon_controller.toggle_perpendiculars) #TODO: has to be implemented
        self.main_view.delete_vertex_button.config(command=self.polygon_controller.delete_selected_vertex)
        self.main_view.add_vertex_button.config(command=self.polygon_controller.add_point_to_polygon)
        self.main_view.delete_polygon_button.config(command=self.polygon_controller.delete_selected_polygon)
        self.main_view.toggle_grid_button.config(command=self.polygon_controller.toggle_grid)

        canvas = self.main_view.canvas_view.canvas
        canvas.bind("<Shift-Button-1>", self._on_shift_left_click)                                      # Shift+LeftClick to add a point
        canvas.bind("<Button-3>", lambda e: self.polygon_controller.finish_polygon())                   # Right-click to finish the polygon
        self.root.bind_all("<Control-z>", lambda e: self.polygon_controller.undo_last_action())         # Ctrl+Z for undo
        canvas.bind("<Return>", self.main_view.canvas_view.center_canvas)                               # Return key to center the canvas


    def _on_shift_left_click(self, event: tk.Event) -> None:
        canvas = self.main_view.canvas_view.canvas
        canvas_x = canvas.canvasx(event.x)
        canvas_y = canvas.canvasy(event.y)
        lx, ly = self.main_view.canvas_view.canvas_to_logic(canvas_x, canvas_y)
        self.polygon_controller.add_point(lx, ly)
