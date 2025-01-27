import tkinter as tk


class CanvasView(tk.Frame):
    def __init__(self, parent):
        super().__init__(parent)
        self.canvas = tk.Canvas(self, width=800, height=600, bg="white")
        self.canvas.pack(fill=tk.BOTH, expand=True)
        
        self.grid_visible = False
        
        
    def toggle_grid(self):
        if self.grid_visible:
            self.canvas.delete("grid_line")
        else:
            grid_size = 20
            width = self.canvas.winfo_width()
            height = self.canvas.winfo_height()
            
            for i in range(0, width, grid_size):
                self.canvas.create_line(i, 0, i, height, tags="grid_line", fill="lightgray")
            for j in range(0, height, grid_size):
                self.canvas.create_line(0, j, width, j, tags="grid_line", fill="lightgray")
                
        self.grid_visible = not self.grid_visible