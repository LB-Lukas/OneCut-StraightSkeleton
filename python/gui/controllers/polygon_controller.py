import tkinter as tk
from tkinter import messagebox
from utils.last_action import LastAction
from utils.intersection_helper import IntersectionHelper

# TODO make last action consistent
class PolygonController:
    def __init__(self, canvas: tk.Canvas, max_polygons=1):
        self.canvas = canvas
        self.max_polygons = max_polygons
        
        # Data
        self.points = []
        self.point_ids = []
        self.line_ids = []
        self.polygons = []
        self.last_actions = []
        self.move_data = {"object": None, "x": 0, "y": 0, "orig_x": 0, "orig_y": 0}
        
        # Utility
        self.intersection_helper = IntersectionHelper()
       
        
    def add_point(self, event):
        if len(self.polygons) >= self.max_polygons:
            messagebox.showinfo("Error", f"Maximum number of polygons is {self.max_polygons}")
            return

        x, y = event.x, event.y

        if self.intersection_helper.check_intersection(x, y, self.polygons, self.line_ids, self.canvas, self.points):
            messagebox.showerror("Error", "The new line intersects with an existing line.")
            return

        self.points.append((x, y))
        
        # Create the point (oval)
        point_id = self.canvas.create_oval(x-5, y-5, x+5, y+5, fill="black", tags="vertex")

        # IMPORTANT: keep track of the point's ID in self.point_ids
        self.point_ids.append(point_id)

        # If at least one other point exists, connect the last two points with a line
        if len(self.points) > 1:
            line = self.canvas.create_line(self.points[-2], self.points[-1], fill="black", width=3)
            self.line_ids.append(line)

        self.last_actions.append((LastAction.ADD_POINT, line))  
        
        
    def finish_polygon(self, event=None):
        if len(self.points) < 3:
            return

        first_x, first_y = self.points[0]
        if self.intersection_helper.check_intersection(first_x, first_y, self.polygons, self.line_ids, self.canvas, self.points):
            messagebox.showerror("Error", "Closing line intersects with an existing line.")
            return

        last_point = self.points[-1]
        closing_line = self.canvas.create_line(last_point[0], last_point[1], first_x, first_y, fill='black', width=3)
        self.line_ids.append(closing_line)

        polygon = {
            'points': self.points.copy(),
            'point_ids': self.point_ids.copy(),
            'line_ids': self.line_ids.copy()
        }
        self.polygons.append(polygon)

        # Clear current polygon-in-progress data
        self.points.clear()
        self.point_ids.clear()
        self.line_ids.clear()

        # Record the action with polygon data
        self.last_actions.append((LastAction.FINISH_POLYGON, polygon))

        
    # FIXME TypeError: cannot unpack non-iterable LastAction object
    # FIXME IndexError: pop from empty list last_id = self.line_ids.pop()
    def undo_last_action(self, event=None):
        if not self.last_actions:
            return

        action, data = self.last_actions.pop()

        if action == LastAction.ADD_POINT:
            # Remove the point and the line leading to it (if any)
            # line/point is stored in self.line_ids in reverse order
            last_id = self.line_ids.pop()
            self.canvas.delete(last_id)

            self.points.pop()

            if len(self.points) >= 1 and self.line_ids:
                # Remove the line that connected the last 2 points
                last_line_id = self.line_ids.pop()
                self.canvas.delete(last_line_id)

        elif action == LastAction.FINISH_POLYGON:
            # Retrieve the last polygon
            if self.polygons:
                points, line_ids = self.polygons.pop()
                # Remove the closing line
                if line_ids:
                    self.canvas.delete(line_ids[-1])
                # Revert to that "in-progress" polygon
                self.points = points
                self.line_ids = line_ids[:-1]  # exclude the closing line
                
        elif action == LastAction.MOVE_POINT:
            object_id = data['object_id']
            old_x = data['old_x']
            old_y = data['old_y']
            
            # Get current position
            coords = self.canvas.coords(object_id)
            current_x = (coords[0] + coords[2]) / 2
            current_y = (coords[1] + coords[3]) / 2
            
            # Calculate delta to move back to original position
            dx = old_x - current_x
            dy = old_y - current_y
            
            # Move the point back
            self.canvas.move(object_id, dx, dy)
            
            # Update the internal points list
            if object_id in self.point_ids:
                index = self.point_ids.index(object_id)
                self.points[index] = (old_x, old_y)
            
            # Update connected line_ids
            self.update_connected_line_ids(object_id, old_x, old_y)
                
                
    def move_start(self, event):
        print(type(event))
        object_id = self.canvas.find_closest(event.x, event.y)[0]
        self.move_data["object"] = object_id
        self.move_data["x"] = event.x
        self.move_data["y"] = event.y
        
        # Get original position
        coords = self.canvas.coords(object_id)
        orig_x = (coords[0] + coords[2]) / 2
        orig_y = (coords[1] + coords[3]) / 2
        self.move_data["orig_x"] = orig_x
        self.move_data["orig_y"] = orig_y
        
        self.canvas.tag_raise(object_id)
    
    
    def move_stop(self, event):
        object_id = self.move_data["object"]
        if object_id is not None:
            # Get new position
            coords = self.canvas.coords(object_id)
            new_x = (coords[0] + coords[2]) / 2
            new_y = (coords[1] + coords[3]) / 2
            
            # Record the move with original position for undo
            action_data = {
                'object_id': object_id,
                'old_x': self.move_data["orig_x"],
                'old_y': self.move_data["orig_y"]
            }
            self.last_actions.append((LastAction.MOVE_POINT, action_data))
            
            # Reset move data
            self.move_data = {"object": None, "x": 0, "y": 0, "orig_x": 0, "orig_y": 0}
            
    
    def move(self, event):
        if self.move_data["object"] is not None:
            object_id = self.move_data["object"]
            dx = event.x - self.move_data["x"]
            dy = event.y - self.move_data["y"]

            # Move the point
            self.canvas.move(object_id, dx, dy)

            # Update the stored new position
            coords = self.canvas.coords(object_id)
            new_x = (coords[0] + coords[2]) / 2
            new_y = (coords[1] + coords[3]) / 2
            
            # Update connected line_ids
            self.update_connected_line_ids(object_id, new_x, new_y)
            
            # Update move data
            self.move_data["x"] = event.x
            self.move_data["y"] = event.y


    def update_connected_line_ids(self, point_id, new_x, new_y):
        for polygon in self.polygons:
            # Check if point_id belongs to this polygon
            if point_id in polygon['point_ids']:
                index = polygon['point_ids'].index(point_id)
                num_points = len(polygon['points'])

                # Update the point's coordinates in polygon['points']
                polygon['points'][index] = (new_x, new_y)

                # The line that connects this point with the previous point
                prev_index = (index - 1) % num_points
                prev_line_id = polygon['line_ids'][prev_index]
                prev_point = polygon['points'][prev_index]
                self.canvas.coords(prev_line_id, prev_point[0], prev_point[1], new_x, new_y)

                # The line that connects this point with the next point
                next_index = (index + 1) % num_points
                next_line_id = polygon['line_ids'][index]
                next_point = polygon['points'][next_index]
                self.canvas.coords(next_line_id, new_x, new_y, next_point[0], next_point[1])


        