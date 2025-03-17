import tkinter as tk
import webbrowser
from views.canvas_view import CanvasView


class MainView(tk.Frame):
    def __init__(self, root, app):
        super().__init__(root)
        self.app = app
        self.grid(sticky="nsew")
        self._create_menu(root)
        self._create_widgets()
        self._configure_grid()


    def _create_menu(self, root: tk.Tk):
        self.menu = tk.Menu(root)
        root.config(menu=self.menu)

        file_menu = tk.Menu(self.menu, tearoff=0)
        self.menu.add_cascade(label="File", menu=file_menu)
        file_menu.add_command(label="Save", command=self.save_file)
        file_menu.add_command(label="Save As", command=self.save_file_as)
        file_menu.add_command(label="Open", command=self.open_file)
        file_menu.add_separator()
        file_menu.add_command(label="Import")
        
        
        export_menu = tk.Menu(file_menu, tearoff=0)
        file_menu.add_cascade(label="Export", menu=export_menu)
        # In MainView's _create_menu method
        export_menu.add_command(label="Export as PDF",command=lambda: self.app.file_controller.export_to_pdf(self.canvas_view))
        export_menu.add_command(label="Export as PNG",command=lambda: self.app.file_controller.export_to_png(self.canvas_view))
        
        
        
        
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=root.quit)

        edit_menu = tk.Menu(self.menu, tearoff=0)
        self.menu.add_cascade(label="Edit", menu=edit_menu)
        edit_menu.add_command(label="Undo", command=self.app.polygon_controller.undo_last_action)

        help_menu = tk.Menu(self.menu, tearoff=0)
        self.menu.add_cascade(label="Help", menu=help_menu)
        help_menu.add_command(label="About", command=lambda: webbrowser.open("https://github.com/LB-Lukas/OneCut-StraightSkeleton"))


    def _create_widgets(self):
        self.container = tk.Frame(self) # Container for side pane and canvas
        self.container.grid(row=0, column=0, sticky="nsew")
        self._create_side_pane(self.container)
        self._create_canvas_area(self.container)


    def _create_side_pane(self, parent: tk.Frame):
        self.side_pane = tk.Frame(parent, width=200, bg="lightgray")
        self.side_pane.grid(row=0, column=0, sticky="ns")
        
        self.polygon_editor_label = tk.Label(self.side_pane, text="Polygon Tools:")
        self.polygon_editor_label.pack(pady=(10,0), padx=5, fill="x")
        
        # Create and pack buttons for the side pane
        self.toggle_grid_button = tk.Button(self.side_pane, wraplength=100, text="Toggle Grid")
        self.toggle_grid_button.pack(pady=5, padx=5, fill="x")
        
        self.finish_polygon_button = tk.Button(self.side_pane, wraplength=100, text="Finish Polygon")
        self.finish_polygon_button.pack(pady=5, padx=5, fill="x")
        
        self.delete_vertex_button = tk.Button(self.side_pane, wraplength=100, text="Delete Point")
        self.delete_vertex_button.pack(pady=5, padx=5, fill="x")
        
        self.add_vertex_button = tk.Button(self.side_pane, wraplength=100, text="Add Point")
        self.add_vertex_button.pack(pady=5, padx=5, fill="x")
        
        self.delete_polygon_button = tk.Button(self.side_pane, wraplength=100, text="Delete Polygon")
        self.delete_polygon_button.pack(pady=5, padx=5, fill="x")

        # Drop Down menu for different crease drawing modes
        self.drawing_mode_label = tk.Label(self.side_pane, text="Draw Creases:")
        self.drawing_mode_label.pack(pady=(10,0), padx=5, fill="x")
        
        self.drawing_mode = tk.StringVar(self.side_pane)
        self.drawing_mode.set("None") # default
        self.drawing_mode_options = [
            "None",
            "Foldpattern",
            "Skeleton",
            "Perpendiculars",
            "Skeleton and Perpendiculars"
        ]
        max_text_length = max(len(option) for option in self.drawing_mode_options)
        self.drawing_mode_menu = tk.OptionMenu(self.side_pane, self.drawing_mode, *self.drawing_mode_options, command=self.app.polygon_controller.update_drawing_mode)
        self.drawing_mode_menu.config(width=max_text_length)
        self.drawing_mode_menu.pack(pady=5, padx=5, fill="x")


    def _create_canvas_area(self, parent: tk.Frame):
        # Create the canvas view in the right column.
        self.canvas_view = CanvasView(parent, self.app)
        self.canvas_view.grid(row=0, column=1, sticky="nsew")


    def _configure_grid(self):
        # Allow the main frame to expand.
        self.rowconfigure(0, weight=1)
        self.columnconfigure(0, weight=1)
        
        # Side pane should not expand, canvas should expand
        self.container.columnconfigure(0, weight=0)
        self.container.columnconfigure(1, weight=1)
        self.container.rowconfigure(0, weight=1)
        
    def save_file(self):
        self.app.file_controller.save_fold(self.canvas_view)

    def save_file_as(self):
        self.app.file_controller.save_fold_as(self.canvas_view)
        
    def open_file(self):
        self.app.file_controller.open_file(self.canvas_view)
