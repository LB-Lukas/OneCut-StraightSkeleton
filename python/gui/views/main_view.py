import tkinter as tk
from views.polygon_canvas import CanvasView
import webbrowser

class MainView(tk.Frame):
    def __init__(self, root: tk.Tk, app):
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
        self._create_button_frame()
        self._create_canvas_area()

    def _create_button_frame(self):
        self.button_frame = tk.Frame(self)
        self.button_frame.grid(row=0, column=0, columnspan=2, sticky="ew", padx=5, pady=5)

        self.toggle_grid_button = self._create_button(self.button_frame, "Toggle Grid")
        self.open_file_button = self._create_button(self.button_frame, "Open File")
        self.finish_polygon_button = self._create_button(self.button_frame, "Finish Polygon")
        self.export_pdf_button = self._create_button(self.button_frame, "Export to PDF")
        self.export_png_button = self._create_button(self.button_frame, "Export to PNG")
        self.generate_skeleton_button = self._create_button(self.button_frame, "Toggle Skeleton")

    def _create_button(self, parent: tk.Frame, text: str) -> tk.Button:
        btn = tk.Button(parent, text=text)
        btn.pack(side=tk.LEFT, padx=5)
        return btn

    def _create_canvas_area(self):
        self.canvas_view = CanvasView(self, self.app)
        self.canvas_view.grid(row=1, column=0, sticky="nsew")

    def _configure_grid(self):
        self.rowconfigure(1, weight=1)
        self.columnconfigure(0, weight=1)
