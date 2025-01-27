import tkinter as tk
from views.polygon_canvas import CanvasView


class MainView(tk.Frame):
    def __init__(self, root):
        super().__init__(root)
        self.button_frame = tk.Frame(self)
        self.button_frame.pack(side=tk.TOP, fill=tk.X)
    
        # Create all the buttons
        self.toggle_grid_button = tk.Button(self.button_frame, text="Toggle Grid")
        self.toggle_grid_button.pack(side=tk.LEFT, padx=5)
        
        self.open_file_button = tk.Button(self.button_frame, text="Open File")
        self.open_file_button.pack(side=tk.LEFT, padx=5)
        
        self.finish_polygon_button = tk.Button(self.button_frame, text="Finish Polygon")
        self.finish_polygon_button.pack(side=tk.LEFT, padx=5)

        self.export_pdf_button = tk.Button(self.button_frame, text="Export to PDF")
        self.export_pdf_button.pack(side=tk.LEFT, padx=5)

        self.export_png_button = tk.Button(self.button_frame, text="Export to PNG")
        self.export_png_button.pack(side=tk.LEFT, padx=5)
        
        self.generate_skeleton_button = tk.Button(self.button_frame, text="Generate Skeleton")
        self.generate_skeleton_button.pack(side=tk.LEFT, padx=5)
        
        # Create canvas area
        self.canvas_view = CanvasView(self)
        self.canvas_view.pack(side=tk.BOTTOM, expand=True)