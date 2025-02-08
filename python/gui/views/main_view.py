import tkinter as tk
import webbrowser
from views.polygon_canvas import CanvasView


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
        file_menu.add_command(label="Save")
        file_menu.add_command(label="Save As")
        file_menu.add_separator()
        file_menu.add_command(label="Import")
        file_menu.add_command(label="Export")
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=root.quit)

        edit_menu = tk.Menu(self.menu, tearoff=0)
        self.menu.add_cascade(label="Edit", menu=edit_menu)
        edit_menu.add_command(label="Undo", command=self.app.polygon_controller.undo_last_action)

        help_menu = tk.Menu(self.menu, tearoff=0)
        self.menu.add_cascade(label="Help", menu=help_menu)
        help_menu.add_command(label="About", command=lambda: webbrowser.open("https://github.com/OsterfeldTom/OneStraightCut"))


    def _create_widgets(self):
        self.container = tk.Frame(self) # Container for side pane and canvas
        self.container.grid(row=0, column=0, sticky="nsew")
        self._create_side_pane(self.container)
        self._create_canvas_area(self.container)


    def _create_side_pane(self, parent: tk.Frame):
        self.side_pane = tk.Frame(parent, width=200, bg="lightgray")
        self.side_pane.grid(row=0, column=0, sticky="ns")
        
        # Create and pack buttons for the side pane
        self.toggle_grid_button = tk.Button(self.side_pane, wraplength=100, text="Toggle Grid")
        self.toggle_grid_button.pack(pady=5, padx=5, fill="x")
        
        self.finish_polygon_button = tk.Button(self.side_pane, wraplength=100, text="Finish Polygon")
        self.finish_polygon_button.pack(pady=5, padx=5, fill="x")
        
        self.toggle_skeleton_button = tk.Button(self.side_pane, wraplength=100, text="Toggle Skeleton")
        self.toggle_skeleton_button.pack(pady=5, padx=5, fill="x")
        
        self.toggle_perpendiculars_button = tk.Button(self.side_pane, wraplength=100, text="Toggle Perpendiculars")
        self.toggle_perpendiculars_button.pack(pady=5, padx=5, fill="x")
        
        self.delete_vertex_button = tk.Button(self.side_pane, wraplength=100, text="Delete Point")
        self.delete_vertex_button.pack(pady=5, padx=5, fill="x")
        
        self.add_vertex_button = tk.Button(self.side_pane, wraplength=100, text="Add Point")
        self.add_vertex_button.pack(pady=5, padx=5, fill="x")
        
        self.delete_polygon_button = tk.Button(self.side_pane, wraplength=100, text="Delete Polygon")
        self.delete_polygon_button.pack(pady=5, padx=5, fill="x")


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
