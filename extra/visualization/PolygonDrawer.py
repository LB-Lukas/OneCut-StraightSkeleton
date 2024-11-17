import numpy as np
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
        # self.canvas.bind_all('<Control-y>', self.redo_last_action) maybe implement later

    def add_point(self, event):
        x, y = event.x, event.y
        
        if self.check_intersection(x, y):
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

    def check_intersection(self, x, y):
        if len(self.points) > 0:
            for polygon in self.polygons:
                for line in polygon[1]:
                    coords = self.canvas.coords(line)
                    if len(coords) == 4:
                        if self.is_intersecting(((coords[0], coords[1]), (coords[2], coords[3])), ((self.points[-1][0], self.points[-1][1]), (x, y))):
                            return True

        for line in self.lines:
            coords = self.canvas.coords(line)
            if len(coords) == 4:
                if self.is_intersecting(((coords[0], coords[1]), (coords[2], coords[3])), ((self.points[-1][0], self.points[-1][1]), (x, y))):
                    return True
        return False

    def finish_polygon(self, event):
        if len(self.points) <= 2:
            return 
        if self.check_intersection(self.points[0][0], self.points[0][1]):
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
                
    def redo_last_action(self, event):
        # Maybe implement later
        pass
    
    def is_intersecting(self, line1, line2):
        # Start- und Endpunkte der Linien
        A, B = np.array(line1[0]), np.array(line1[1])
        C, D = np.array(line2[0]), np.array(line2[1])
        
        # Richtungsvektoren der Linien
        AB = B - A
        CD = D - C
        
        # Helper-Funktion zur Berechnung des Determinanten von zwei Vektoren
        def det(v, w):
            return v[0] * w[1] - v[1] * w[0]
        
        # Differenz der Startpunkte der beiden Linien
        AC = C - A
        
        # Berechne den Determinanten für Parallelität
        denominator = det(AB, CD)
        if denominator == 0:
            # Die Linien sind parallel und schneiden sich nicht
            return False
        
        # Parameter t und u berechnen
        t = det(AC, CD) / denominator
        u = det(AC, AB) / denominator
        
        # Prüfen, ob der Schnittpunkt innerhalb beider Liniensegmente liegt
        return 0 < t < 1 and 0 < u < 1

if __name__ == "__main__":
    root = tk.Tk()
    root.title("Polygon Drawer")
    app = PolygonDrawer(root)
    root.mainloop()
