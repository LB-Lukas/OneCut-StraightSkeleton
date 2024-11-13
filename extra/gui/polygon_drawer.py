from last_action import LastAction
import tkinter as tk
from tkinter import messagebox
from intersection_helper import IntersectionHelper

class PolygonDrawer:
    def __init__(self, root, max_polygons=1):

        self.root = root
        self.max_polygons = max_polygons
        self.canvas = tk.Canvas(root, width=800, height=600, bg='white')
        self.canvas.pack()
        
        self.points = []  # List for points
        self.lines = []  # List to store lines
        self.polygons = []  # List to store polygons
        
        self.grid_visible = False  # status if
        self.last_action = []
        
        self.intersection_helper = IntersectionHelper()

        # Button to toggle grid
        self.toggle_grid_button = tk.Button(root, text="Toggle Grid", command=self.toggle_grid)
        self.toggle_grid_button.pack()

        # Event handlers for mouse clicks
        self.canvas.bind('<Button-1>', self.add_point)
        self.canvas.bind('<Button-3>', self.finish_polygon)  # Right-click to close the polygon
        self.canvas.bind_all('<Control-z>', self.undo_last_action)

    def add_point(self, event):
        if len(self.polygons) >= self.max_polygons:
            messagebox.showerror("Error", f"Maximum number of polygons is {self.max_polygons}.")
            return
        x, y = event.x, event.y
        
        if self.intersection_helper.check_intersection(x, y, self.polygons, self.lines, self.canvas, self.points):
            messagebox.showerror("Error", "The new line intersects with an existing line.")
            return

        self.points.append((x, y))
        
        # Draw point on canvas
        point = self.canvas.create_oval(x-1, y-1, x+1, y+1, fill='black')
        
        # Draw line to previous point
        if len(self.points) > 1:
            line = self.canvas.create_line(self.points[-2], self.points[-1], fill='black')
            self.lines.append(line)
        
        # Save the last drawn objects for undo
        self.lines.append(point)
        self.last_action.append(LastAction.ADD_POINT)

    def finish_polygon(self, event):
        if len(self.points) <= 2:
            return 
        if self.intersection_helper.check_intersection(self.points[0][0], self.points[0][1], self.polygons, self.lines, self.canvas, self.points):
            messagebox.showerror("Error", "The new line intersects with an existing line.")
            return
        
        # Draw line from last point to first point to close the polygon
        line = self.canvas.create_line(self.points[-1], self.points[0], fill='black')
        self.lines.append(line)
        self.polygons.append((self.points, self.lines))
        self.points = []
        self.lines = []
        self.last_action.append(LastAction.FINISH_POLYGON)


    def toggle_grid(self):
        if self.grid_visible:
            # Delete grid
            self.canvas.delete('grid_line')
        else:
            # Draw grid
            grid_size = 20
            for i in range(0, self.canvas.winfo_width(), grid_size):
                self.canvas.create_line(i, 0, i, self.canvas.winfo_height(), tag='grid_line', fill='lightgray')
            for j in range(0, self.canvas.winfo_height(), grid_size):
                self.canvas.create_line(0, j, self.canvas.winfo_width(), j, tag='grid_line', fill='lightgray')
        
        self.grid_visible = not self.grid_visible
        
    def undo_last_action(self, event):
        if not self.last_action:
            return
        action = self.last_action.pop()
        if action == LastAction.ADD_POINT:
            line = self.lines.pop()
            self.canvas.delete(line)
            self.points.pop()
            if self.lines:
                self.canvas.delete(self.lines.pop())
        elif action == LastAction.FINISH_POLYGON:
            polygon = self.polygons.pop()
            self.points, self.lines = polygon
            if self.lines:
                self.canvas.delete(self.lines.pop())
