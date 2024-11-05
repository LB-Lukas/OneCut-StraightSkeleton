import tkinter as tk
from tkinter import messagebox
from enum import Enum

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
        
        self.points = []  # List for points
        self.lines = []  # List to store lines
        self.polygons = []  # List to store polygons
        
        self.grid_visible = False  # status if
        self.last_action = []

        # Button to toggle grid
        self.toggle_grid_button = tk.Button(root, text="Toggle Grid", command=self.toggle_grid)
        self.toggle_grid_button.pack()

        # Event handlers for mouse clicks
        self.canvas.bind('<Button-1>', self.add_point)
        self.canvas.bind('<Button-3>', self.finish_polygon)  # Right-click to close the polygon
        self.canvas.bind_all('<Control-z>', self.undo_last_action)
        # self.canvas.bind_all('<Control-y>', self.redo_last_action)

    def add_point(self, event):
        x, y = event.x, event.y
        
        # TODO: Check for interstion with existing polygons

        self.points.append((x, y))
        
        # Draw point on canvas
        point = self.canvas.create_oval(x-3, y-3, x+3, y+3, fill='black')
        
        # Draw line to previous point
        if len(self.points) > 1:
            line = self.canvas.create_line(self.points[-2], self.points[-1], fill='black')
            self.lines.append(line)
        
        # Save the last drawn objects for undo
        self.lines.append(point)
        self.last_action.append(LastAction.ADD_POINT)

    def finish_polygon(self, event):
        if len(self.points) > 2:
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
            self.canvas.delete(self.lines.pop())
            self.points.pop()
            if self.lines:
                self.canvas.delete(self.lines.pop())
        elif action == LastAction.FINISH_POLYGON:
            polygon = self.polygons.pop()
            self.points, self.lines = polygon
            if self.lines:
                self.canvas.delete(self.lines.pop())
                
    def redo_last_action(self, event):
        pass
    
    # # Ray casting algorithm to check if a point is inside a polygon
    # def is_point_in_polygon(self, point, polygon):
    #     x, y = point
    #     n = len(polygon)
    #     inside = False

    #     # Iterate through each edge of the polygon
    #     for i in range(n):
    #         x1, y1 = polygon[i]
    #         x2, y2 = polygon[(i + 1) % n]

    #         # Check if the point is between the y-bounds of the edge
    #         if (y1 > y) != (y2 > y):
    #             # Calculate the x-coordinate of the intersection of the edge with the ray
    #             intersect_x = x1 + (y - y1) * (x2 - x1) / (y2 - y1)
                
    #             # If the point is to the left of this intersection, toggle the inside flag
    #             if x < intersect_x:
    #                 inside = not inside

    #     return inside

if __name__ == "__main__":
    root = tk.Tk()
    root.title("Polygon Drawer with Grid and Undo/Redo")
    app = PolygonDrawer(root)
    root.mainloop()
