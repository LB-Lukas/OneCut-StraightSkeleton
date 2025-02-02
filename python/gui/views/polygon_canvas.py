import tkinter as tk
import math


class CanvasView(tk.Frame):
    def __init__(self, parent):
        super().__init__(parent)
        self.zoom: float = 1.0
        
        self.canvas = tk.Canvas(self, width=600, height=600, bg="white")
        self.canvas.configure(scrollregion=(0, 0, 600, 600))
        
        self.horizontal_bar: tk.Scrollbar = tk.Scrollbar(self, orient=tk.HORIZONTAL, command=self.canvas.xview)
        self.vertical_bar: tk.Scrollbar = tk.Scrollbar(self, orient=tk.VERTICAL, command=self.canvas.yview)
        self.canvas.configure(xscrollcommand=self.horizontal_bar.set, yscrollcommand=self.vertical_bar.set)
        
        # Layout canvas and scrollbars
        self.canvas.grid(row=0, column=0, sticky="nsew")
        self.vertical_bar.grid(row=0, column=1, sticky="ns")
        self.horizontal_bar.grid(row=1, column=0, sticky="ew")
        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)
        
        # Canvas borders
        self.canvas.create_line(0, 0, 600, 0, fill="lightgray", tags="border")
        self.canvas.create_line(0, 0, 0, 600, fill="lightgray", tags="border")
        self.canvas.create_line(600, 0, 600, 600, fill="lightgray", tags="border")
        self.canvas.create_line(0, 600, 600, 600, fill="lightgray", tags="border")
        
        self.grid_visible = False
        
        
    # def toggle_grid(self):
    #     if self.grid_visible:
    #         self.canvas.delete("grid_line")
    #     else:
    #         grid_size = 20
    #         width = self.canvas.winfo_width()
    #         height = self.canvas.winfo_height()
            
    #         for i in range(0, width, grid_size):
    #             self.canvas.create_line(i, 0, i, height, tags="grid_line", fill="lightgray")
    #         for j in range(0, height, grid_size):
    #             self.canvas.create_line(0, j, width, j, tags="grid_line", fill="lightgray")
                
    #     self.grid_visible = not self.grid_visible
        
        
    def on_canvas_mousewheel(self, event: tk.Event) -> None:
        ctrl_pressed: bool = (event.state & 0x4) != 0
        if ctrl_pressed:
            self.on_zoom(event)
        else:
            bbox = self.canvas.bbox("all")
            if bbox:
                content_width: int = bbox[2] - bbox[0]
                content_height: int = bbox[3] - bbox[1]
            else:
                content_width = 0
                content_height = 0
                
            canvas_width: int = self.canvas.winfo_width()
            canvas_height: int = self.canvas.winfo_height()
            
            if content_height > canvas_height:
                if hasattr(event, "delta") and event.delta:
                    self.canvas.yview_scroll(-int(event.delta /120), "units")
                elif event.num == 4:
                    self.canvas.yview_scroll(-1, "units")
                elif event.num == 5:
                    self.canvas.yview_scroll(1, "units")
                    
    
    def on_zoom(self, event: tk.Event) -> None:
        if hasattr(event, "delta") and event.delta:
            factor: float = math.pow(1.1, event.delta / 120)
        elif event.num == 4:
            factor = 1.1
        elif event.num == 5:
            factor = 0.9
        else: 
            factor = 1.0
            
        self.zoom = self.zoom * factor
        self.canvas.scale("all", 0, 0, factor, factor)
        
        bbox = self.canvas.bbox("all")
        if bbox:
            self.canvas.configure(scrollregion=bbox)
            
            
    def on_pan_start(self, event: tk.Event) -> None:
        self.canvas.scan_mark(event.x, event.y)
        
    def on_pan_drag(self, event: tk.Event) -> None:
        self.canvas.scan_dragto(event.x, event.y, gain=1)
        
    def center_canvas(self, event: tk.Event = None) -> None:
        bbox = self.canvas.bbox("all")
        if not bbox:
            return
        
        x0, y0, x1, y1 = bbox
        region_width = x1 - x0
        region_height = y1 - y0
        
        canvas_width = self.canvas.winfo_width()
        canvas_height = self.canvas.winfo_height()
        
        center_x: float = (x0 + x1) / 2
        center_y: float = (y0 + y1) / 2
        
        fraction_x: float =(center_x - canvas_width / 2 - x0) / region_width
        fraction_y: float = (center_y - canvas_height / 2 - y0) / region_height

        fraction_x = max(0, min(1, fraction_x))
        fraction_y = max(0, min(1, fraction_y))
        
        self.canvas.xview_moveto(fraction_x)
        self.canvas.yview_moveto(fraction_y)