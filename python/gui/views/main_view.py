import tkinter as tk
from views.polygon_canvas import CanvasView
import webbrowser


class MainView(tk.Frame):
    def __init__(self, root):
        super().__init__(root)
        self.grid(sticky="nsew")
        self.menu = tk.Menu(root)
        root.config(menu=self.menu)
        
        file_menu = tk.Menu(self.menu)
        self.menu.add_cascade(label="File", menu=file_menu)
        file_menu.add_command(label="Import")
        file_menu.add_command(label="Export")
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=root.quit)
        
        
        edit_menu = tk.Menu(self.menu)
        self.menu.add_cascade(label="Edit", menu=edit_menu)
        edit_menu.add_command(label="Undo")
        
        help_menu = tk.Menu(self.menu)
        self.menu.add_cascade(label="Help", menu=help_menu)
        help_menu.add_command(label="About",  command=lambda: webbrowser.open("https://github.com/OsterfeldTom/OneStraightCut"))
        
        self._create_widgets()
        self._configure_grid()


    def _create_widgets(self):
        """Initialize and place all widgets in the view."""
        self._create_button_frame()
        self._create_canvas_area()
        

    def _create_button_frame(self):
        """Create the button frame and add all control buttons."""
        self.button_frame = tk.Frame(self)
        self.button_frame.grid(row=0, column=0, columnspan=2, sticky="ew", padx=5, pady=5)

        # Create all the buttons with a helper method
        self.toggle_grid_button = self._create_button(self.button_frame, "Toggle Grid")
        self.open_file_button = self._create_button(self.button_frame, "Open File")
        self.finish_polygon_button = self._create_button(self.button_frame, "Finish Polygon")
        self.export_pdf_button = self._create_button(self.button_frame, "Export to PDF")
        self.export_png_button = self._create_button(self.button_frame, "Export to PNG")
        self.generate_skeleton_button = self._create_button(self.button_frame, "Generate Skeleton")


    def _create_button(self, parent, text):
        """Helper method to create and pack a button."""
        button = tk.Button(parent, text=text)
        button.pack(side=tk.LEFT, padx=5)
        return button


    def _create_canvas_area(self):
        """Create the canvas view along with its scrollbars."""
        # Create the canvas view
        self.canvas_view = CanvasView(self)
        self.canvas_view.grid(row=1, column=0, sticky="nsew")


    def _configure_grid(self):
        """Set up grid weights for proper resizing behavior."""
        self.rowconfigure(1, weight=1)
        self.columnconfigure(0, weight=1)


    def on_zoom(self, event: tk.Event):
        """Handle zooming of the canvas based on mouse wheel event."""
        factor = 1.1 if event.delta > 0 else 0.9
        self.canvas_view.zoom(factor)
