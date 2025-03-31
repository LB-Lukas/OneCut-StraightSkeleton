import tkinter as tk
from controllers.polygon_controller import PolygonController
from controllers.file_controller import FileController
from views.main_view import MainView

class StraightSkeletonApp:
    """@brief Main application container managing MVC components and event bindings"""
    
    def __init__(self, root: tk.Tk, max_polygons: int = 1):
        """@brief Initialize application with root window and max polygon limit"""
        self.root = root
        self.max_polygons = max_polygons
        self.polygon_controller = PolygonController(self, max_polygons)  # Handles polygon operations
        self.file_controller = FileController()                         # Manages file I/O
        self.main_view = MainView(root, self)                           # Main UI container
        self.main_view.pack(fill=tk.BOTH, expand=True)
        self._setup_button_commands()

    def _setup_button_commands(self) -> None:
        """@brief Bind UI controls to controller methods and set up key shortcuts"""
        # Button commands
        self.main_view.finish_polygon_button.config(command=self.polygon_controller.finish_polygon)
        self.main_view.delete_vertex_button.config(command=self.polygon_controller.delete_selected_vertex)
        self.main_view.add_vertex_button.config(command=self.polygon_controller.add_point_to_polygon)
        self.main_view.delete_polygon_button.config(command=self.polygon_controller.delete_selected_polygon)
        self.main_view.toggle_grid_button.config(command=self.polygon_controller.toggle_grid)

        # Canvas event bindings
        canvas = self.main_view.canvas_view.canvas
        canvas.bind("<Shift-Button-1>", self._on_shift_left_click)      # Shift+Click: Add point
        canvas.bind("<Button-3>", lambda e: self.polygon_controller.finish_polygon())  # Right-click: Finish

    def _on_shift_left_click(self, event: tk.Event) -> None:
        """@brief Handle shift+click event to add point in canvas coordinates"""
        canvas = self.main_view.canvas_view.canvas
        canvas_x = canvas.canvasx(event.x)
        canvas_y = canvas.canvasy(event.y)
        lx, ly = self.main_view.canvas_view.canvas_to_logic(canvas_x, canvas_y)
        self.polygon_controller.add_point(lx, ly)