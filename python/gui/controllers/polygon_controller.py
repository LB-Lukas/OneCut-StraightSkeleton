import tkinter as tk
import geometry
from tkinter import messagebox
from utils.last_action import LastAction
from utils.intersection_helper import IntersectionHelper

# TODO undo last action not working properly, 
class PolygonController:
    def __init__(self, canvas: tk.Canvas, max_polygons=1):
        self.canvas = canvas
        self.max_polygons = max_polygons

        self.points = []
        self.point_ids = []
        self.line_ids = []
        self.polygons = []
        self.last_actions = []
        self.move_data = {"object": None, "x": 0, "y": 0, "orig_x": 0, "orig_y": 0}

        self.intersection_helper = IntersectionHelper()


    def add_point(self, event):
        if len(self.polygons) >= self.max_polygons:
            messagebox.showinfo("Error", f"Maximum number of polygons is {self.max_polygons}")
            return

        x = self.canvas.canvasx(event.x)
        y = self.canvas.canvasy(event.y)

        if self.intersection_helper.check_intersection(x, y, self.polygons, self.line_ids, self.canvas, self.points):
            messagebox.showerror("Error", "The new line intersects with an existing line.")
            return

        self.points.append((x, y))
        point_id = self.canvas.create_oval(x-7, y-7, x+7, y+7, fill="black", tags="vertex")
        self.point_ids.append(point_id)

        # connect point wiht previous point
        if len(self.points) > 1:
            line = self.canvas.create_line(self.points[-2], self.points[-1], fill="black", width=5, tags="polygon_line")
            self.line_ids.append(line)

        self.last_actions.append((LastAction.ADD_POINT, {"line_id": line, "point_id": point_id}))


    def finish_polygon(self, event=None):
        if len(self.points) < 3:
            messagebox.showinfo("Info", "A polygon must have at least 3 points.")
            return

        first_x, first_y = self.points[0]
        if self.intersection_helper.check_intersection(first_x, first_y, self.polygons, self.line_ids, self.canvas, self.points):
            messagebox.showerror("Error", "Closing line intersects with an existing line.")
            return

        last_point = self.points[-1]
        closing_line = self.canvas.create_line(last_point[0], last_point[1], first_x, first_y, fill='black', width=5, tags="polygon_line")
        self.line_ids.append(closing_line)

        polygon = {
            'points': self.points.copy(),
            'point_ids': self.point_ids.copy(),
            'line_ids': self.line_ids.copy(),
            'skeleton_line_ids': []
        }
        self.polygons.append(polygon)
        self.points.clear()
        self.point_ids.clear()
        self.line_ids.clear()

        self.last_actions.append((LastAction.FINISH_POLYGON, {"polygon": polygon})) # record last action


    def generate_skeleton(self, event=None):
        if not self.polygons:
            messagebox.showinfo("Info", "No polygon available to generate a skeleton.")
            return

        polygon = self.polygons[0]

        # Remove existing skeleton lines
        if polygon['skeleton_line_ids']:
            for sk_line in polygon['skeleton_line_ids']:
                line_id = sk_line['line_id']
                self.canvas.delete(line_id)
            self.last_actions.append((LastAction.REMOVE_SKELETON, {"skeleton_line_ids": polygon['skeleton_line_ids'].copy()})) # record last action
            polygon['skeleton_line_ids'].clear()

        polygon_vertices = [geometry.Point(p[0], p[1]) for p in polygon['points']]

        try:
            skeleton = geometry.StraightSkeleton(polygon_vertices)
            print(skeleton) 
            skeleton_edges = skeleton.get_edges()
            print(f"Number of skeleton edges: {len(skeleton_edges)}") 

            if not skeleton_edges:
                messagebox.showinfo("Info", "No skeleton edges were generated.")
                return

            # Draw the skeleton lines
            for edge in skeleton_edges:
                src, tgt = edge 
                line_id = self.canvas.create_line(src.x(), src.y(), tgt.x(), tgt.y(), fill='green', width=3, tags="skeleton")
                polygon['skeleton_line_ids'].append({"line_id": line_id, "coords": (src.x(), src.y(), tgt.x(), tgt.y())})

            self.canvas.tag_lower("skeleton") # Adjust stacking order
            self.canvas.tag_raise("polygon_line")
            self.canvas.tag_raise("vertex")

            self.last_actions.append((LastAction.GENERATE_SKELETON, {"skeleton_line_ids": polygon['skeleton_line_ids'].copy()})) # record last actoin
            #messagebox.showinfo("Success", "Straight skeleton generated and drawn successfully.")

        except Exception as e:
            messagebox.showerror("Error", f"An error occurred while generating the skeleton:\n{e}")


    # FIXME undo last action not working properly
    def undo_last_action(self, event=None):
        if not self.last_actions:
            messagebox.showinfo("Info", "No actions to undo.")
            return

        action, data = self.last_actions.pop()

        if action == LastAction.ADD_POINT:
            # Remove the point and the line leading to it
            line_id = data.get("line_id")
            point_id = data.get("point_id")

            if line_id:
                try:
                    self.line_ids.remove(line_id)
                    self.canvas.delete(line_id)
                except ValueError:
                    pass  # Line might have been already removed

            if point_id:
                try:
                    self.point_ids.remove(point_id)
                    self.canvas.delete(point_id)
                except ValueError:
                    pass  # Point might have been already removed

            if self.points:
                self.points.pop()

        elif action == LastAction.FINISH_POLYGON:
            # Remove the last polygon
            polygon = data.get("polygon")
            if polygon:
                # Remove all lines of the polygon
                for line_id in polygon['line_ids']:
                    self.canvas.delete(line_id)
                # Remove all points of the polygon
                for point_id in polygon['point_ids']:
                    self.canvas.delete(point_id)
                # Remove any skeleton lines if they exist
                for sk_line_id in polygon['skeleton_line_ids']:
                    self.canvas.delete(sk_line_id)
                # Remove the polygon from the list
                self.polygons.pop()

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

            # Regenerate the skeleton after moving a point
            self.generate_skeleton()

        elif action == LastAction.GENERATE_SKELETON:
            # Remove all skeleton lines
            skeleton_line_ids = data.get('skeleton_line_ids', [])
            for line_id in skeleton_line_ids:
                self.canvas.delete(line_id)
            # Clear the skeleton_line_ids from the polygon
            if self.polygons:
                polygon = self.polygons[0]
                polygon['skeleton_line_ids'].clear()

        elif action == LastAction.REMOVE_SKELETON:
            # TODO implement undo for removing skeleton lines
            pass


    def move_start(self, event):
        obj = self.canvas.find_withtag("current")
        if not obj:
            return
        object_id = obj[0]
        self.move_data["object"] = object_id
        canvas_x = self.canvas.canvasx(event.x)
        canvas_y = self.canvas.canvasy(event.y)
        self.move_data["x"] = canvas_x
        self.move_data["y"] = canvas_y

        coords = self.canvas.coords(object_id)
        orig_x = (coords[0] + coords[2]) / 2
        orig_y = (coords[1] + coords[3]) / 2
        self.move_data["orig_x"] = orig_x
        self.move_data["orig_y"] = orig_y

        self.canvas.tag_raise(object_id)


    def move_stop(self, event):
        object_id = self.move_data["object"]
        if object_id is not None:
            coords = self.canvas.coords(object_id)
            new_x = (coords[0] + coords[2]) / 2
            new_y = (coords[1] + coords[3]) / 2

            action_data = {
                'object_id': object_id,
                'old_x': self.move_data["orig_x"],
                'old_y': self.move_data["orig_y"]
            }
            self.last_actions.append((LastAction.MOVE_POINT, action_data))

            self.move_data = {"object": None, "x": 0, "y": 0, "orig_x": 0, "orig_y": 0}

            self.generate_skeleton()


    def move(self, event):
        if self.move_data["object"] is not None:
            
            canvas_x = self.canvas.canvasx(event.x)
            canvas_y = self.canvas.canvasy(event.y)
            
            object_id = self.move_data["object"]
            dx = canvas_x - self.move_data["x"]
            dy = canvas_y - self.move_data["y"]

            self.canvas.move(object_id, dx, dy)

            coords = self.canvas.coords(object_id)
            new_x = (coords[0] + coords[2]) / 2
            new_y = (coords[1] + coords[3]) / 2

            self.update_connected_line_ids(object_id, new_x, new_y)

            self.move_data["x"] = canvas_x
            self.move_data["y"] = canvas_y
            self.generate_skeleton()


    def update_connected_line_ids(self, point_id, new_x, new_y):
        for polygon in self.polygons:
            if point_id in polygon['point_ids']:
                index = polygon['point_ids'].index(point_id)
                num_points = len(polygon['points'])

                polygon['points'][index] = (new_x, new_y)

                prev_index = (index - 1) % num_points
                prev_line_id = polygon['line_ids'][prev_index]
                prev_point = polygon['points'][prev_index]
                self.canvas.coords(prev_line_id, prev_point[0], prev_point[1], new_x, new_y)

                next_index = (index + 1) % num_points
                next_line_id = polygon['line_ids'][index]
                next_point = polygon['points'][next_index]
                self.canvas.coords(next_line_id, new_x, new_y, next_point[0], next_point[1])
                
                
    def on_enter(self, evnet):
        self.canvas.itemconfig("current", fill="gray")
        
    def on_leave(self, event):
        self.canvas.itemconfig("current", fill="black")
