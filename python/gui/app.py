import tkinter as tk
from controllers.polygon_controller import PolygonController
from controllers.file_controller import FileController
from views.main_view import MainView


class StraightSkeletonApp:
    def __init__(self, root: tk.Tk, max_polygons: int = 1):
        self.root = root
        self.max_polygons = max_polygons
        
        # Create the main view
        self.main_view = MainView(root)
        self.main_view.pack(fill=tk.BOTH, expand=True)
        
        # Create controllers
        self.polygon_controller = PolygonController(self.main_view.canvas_view.canvas, max_polygons)
        self.file_controller = FileController()
        
        # Bind buttonss
        self._setup_button_commands()
        
        
    def _setup_button_commands(self):
        #self.main_view.toggle_grid_button.config(command=self.main_view.canvas_view.toggle_grid)
        self.main_view.open_file_button.config(command=self.file_controller.open_file)
        self.main_view.finish_polygon_button.config(command=self.polygon_controller.finish_polygon)
        self.main_view.export_pdf_button.config(command=self.file_controller.export_to_pdf)
        self.main_view.export_png_button.config(command=self.file_controller.export_to_png)
        self.main_view.generate_skeleton_button.config(command=self.polygon_controller.generate_skeleton)
        
        self.main_view.canvas_view.canvas.bind("<Shift-Button-1>", self.polygon_controller.add_point)
        self.main_view.canvas_view.canvas.bind("<Button-3>", self.polygon_controller.finish_polygon)
        self.main_view.canvas_view.canvas.bind_all("<Control-z>", self.polygon_controller.undo_last_action)
        # Drag and drop
        self.main_view.canvas_view.canvas.tag_bind("vertex", "<ButtonPress-1>", self.polygon_controller.move_start)
        self.main_view.canvas_view.canvas.tag_bind("vertex", "<ButtonRelease-1>", self.polygon_controller.move_stop)
        self.main_view.canvas_view.canvas.tag_bind("vertex", "<B1-Motion>", self.polygon_controller.move)
        self.main_view.canvas_view.canvas.tag_bind("vertex", "<Enter>", self.polygon_controller.on_enter)
        self.main_view.canvas_view.canvas.tag_bind("vertex", "<Leave>", self.polygon_controller.on_leave)
        
        # Bind Mousewheel events
        self.main_view.canvas_view.canvas.bind("<MouseWheel>", self.main_view.canvas_view.on_canvas_mousewheel)
        self.main_view.canvas_view.canvas.bind("Control-MouseWheel", self.main_view.canvas_view.on_canvas_mousewheel)
        self.main_view.canvas_view.canvas.bind("<Button-4>", self.main_view.canvas_view.on_canvas_mousewheel)
        self.main_view.canvas_view.canvas.bind("<Button-5>", self.main_view.canvas_view.on_canvas_mousewheel)
        self.main_view.canvas_view.canvas.bind("<Control-Button-4>", self.main_view.canvas_view.on_canvas_mousewheel)
        self.main_view.canvas_view.canvas.bind("<Control-Button-5>", self.main_view.canvas_view.on_canvas_mousewheel)
        
        for widget in (self.main_view.canvas_view.vertical_bar, self.main_view.canvas_view.horizontal_bar):
            widget.bind("<MouseWheel>", self.main_view.canvas_view.on_zoom)
            widget.bind("<Button-4>", self.main_view.canvas_view.on_zoom)
            widget.bind("<Button-5>", self.main_view.canvas_view.on_zoom)
            
        self.main_view.canvas_view.canvas.bind("<ButtonPress-2>", self.main_view.canvas_view.on_pan_start)
        self.main_view.canvas_view.canvas.bind("<B2-Motion>", self.main_view.canvas_view.on_pan_drag)
        
        self.main_view.canvas_view.canvas.bind("<Return>", self.main_view.canvas_view.center_canvas)