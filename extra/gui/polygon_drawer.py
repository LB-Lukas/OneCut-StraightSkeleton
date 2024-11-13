import numpy as np
import tkinter as tk
from tkinter import filedialog, messagebox
from enum import Enum
from polygon_model import PolygonModel

class LastAction(Enum):
    ADD_POINT = 1
    FINISH_POLYGON = 2
    UNDO = 3
    REDO = 4
    NO_ACTION = 5

class PolygonDrawer:
    def __init__(self, root):
        self.root = root
        self.canvas = tk.Canvas(root, width=800, height=600, bg='white')
        self.canvas.pack()
        
        self.model = PolygonModel()
        self.grid_visible = False
        self.last_action = []

        self.toggle_grid_button = tk.Button(root, text="Toggle Grid", command=self.toggle_grid)
        self.toggle_grid_button.pack()

        self.open_fold_file_button = tk.Button(root, text="Open Fold File", command=self.open_fold_file)
        self.open_fold_file_button.pack()

        self.canvas.bind('<Button-1>', self.add_point)
        self.canvas.bind('<Button-3>', self.finish_polygon)
        self.canvas.bind_all('<Control-z>', self.undo_last_action)

    def add_point(self, event):
        x, y = event.x, event.y
        if self.model.check_intersection(x, y):
            messagebox.showerror("Error", "The new line intersects with an existing line.")
            return
        
        self.model.add_point(x, y)
        self.draw_point(x, y)
        
        if len(self.model.points) > 1:
            self.draw_line(self.model.points[-2], self.model.points[-1])
        self.last_action.append(LastAction.ADD_POINT)

    def finish_polygon(self, event):
        if len(self.model.points) <= 2:
            return
        if self.model.check_intersection(self.model.points[0][0], self.model.points[0][1]):
            messagebox.showerror("Error", "The new line intersects with an existing line.")
            return
        
        self.draw_line(self.model.points[-1], self.model.points[0])
        self.model.finish_polygon()
        self.last_action.append(LastAction.FINISH_POLYGON)

    def undo_last_action(self, event):
        if not self.last_action:
            return
        action = self.last_action.pop()
        if action == LastAction.ADD_POINT:
            self.canvas.delete(self.model.lines.pop())
            self.model.points.pop()
            if self.model.lines:
                self.canvas.delete(self.model.lines.pop())
        elif action == LastAction.FINISH_POLYGON:
            polygon_points, polygon_lines = self.model.polygons.pop()
            for line in polygon_lines:
                self.canvas.delete(line)

    def toggle_grid(self):
        if self.grid_visible:
            self.canvas.delete('grid_line')
        else:
            grid_size = 20
            for i in range(0, self.canvas.winfo_width(), grid_size):
                self.canvas.create_line(i, 0, i, self.canvas.winfo_height(), tag='grid_line', fill='lightgray')
            for j in range(0, self.canvas.winfo_height(), grid_size):
                self.canvas.create_line(0, j, self.canvas.winfo_width(), tag='grid_line', fill='lightgray')
        self.grid_visible = not self.grid_visible

    def draw_point(self, x, y):
        self.canvas.create_oval(x-1, y-1, x+1, y+1, fill='black')

    def draw_line(self, start, end, color='black'):
        self.model.lines.append(self.canvas.create_line(start[0], start[1], end[0], end[1], fill=color))
        
    def open_fold_file():
        pass
    # TODO implement open_fold_file