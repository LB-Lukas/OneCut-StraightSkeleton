import tkinter as tk
import math

# Constants for logic dimensions.
CANVAS_WIDTH = 600
CANVAS_HEIGHT = 600

class CanvasView(tk.Frame):
    def __init__(self, parent: tk.Frame, app):
        super().__init__(parent)
        self.app = app

        self.camera_scale = 1.0
        self.camera_offset_x = 0
        self.camera_offset_y = 0

        self.canvas = tk.Canvas(self, width=CANVAS_WIDTH, height=CANVAS_HEIGHT, bg="white")
        self.canvas.configure(scrollregion=(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT))

        self.horizontal_bar = tk.Scrollbar(self, orient=tk.HORIZONTAL, command=self.canvas.xview)
        self.vertical_bar = tk.Scrollbar(self, orient=tk.VERTICAL, command=self.canvas.yview)
        self.canvas.configure(xscrollcommand=self.horizontal_bar.set, yscrollcommand=self.vertical_bar.set)

        self.canvas.grid(row=0, column=0, sticky="nsew")
        self.vertical_bar.grid(row=0, column=1, sticky="ns")
        self.horizontal_bar.grid(row=1, column=0, sticky="ew")
        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)

        # Panning parameters.
        self._pan_last_x = 0
        self._pan_last_y = 0

        # Vertex dragging state.
        self._dragging_vertex = False

        # Bind events for scrolling, zooming, panning, and vertex dragging.
        self.canvas.bind("<MouseWheel>", self.on_canvas_mousewheel)
        self.canvas.bind("<Button-4>", self.on_canvas_mousewheel)
        self.canvas.bind("<Button-5>", self.on_canvas_mousewheel)

        self.canvas.bind("<ButtonPress-2>", self.on_pan_start)
        self.canvas.bind("<B2-Motion>", self.on_pan_drag)

        self.canvas.bind("<ButtonPress-1>", self.on_left_button_down)
        self.canvas.bind("<B1-Motion>", self.on_left_button_drag)
        self.canvas.bind("<ButtonRelease-1>", self.on_left_button_up)

        self._redraw()


    def logic_to_canvas(self, lx: float, ly: float) -> tuple:
        cx = lx * self.camera_scale + self.camera_offset_x
        cy = ly * self.camera_scale + self.camera_offset_y
        return (cx, cy)


    def canvas_to_logic(self, cx: float, cy: float) -> tuple:
        lx = (cx - self.camera_offset_x) / self.camera_scale
        ly = (cy - self.camera_offset_y) / self.camera_scale
        return (lx, ly)
    
    
    def clip_line(self, x1, y1, x2, y2, x_min=0, y_min=0, x_max=600, y_max=600):
        dx = x2 - x1
        dy = y2 - y1
        t0, t1 = 0.0, 1.0
        
        for p, q in ((-dx, x1 - x_min), (dx, x_max - x1), (-dy, y1 - y_min), (dy, y_max - y1)):
            if p == 0:
                if q < 0:
                    return None
            else:
                t = q / p
                if p < 0:
                    if t > t1:
                        return None
                    if t > t0:
                        t0 = t
                else:
                    if t < t0:
                        return None
                    if t < t1:
                        t1 = t
                        
        if t0 > t1:
            return None
        
        return (x1 + t0 * dx, y1 + t0 * dy, x1 + t1 * dx, y1 + t1 * dy)


    def redraw_all(self, polygons, in_progress_points):
        self.canvas.delete("all")
        # Draw border lines in logic coordinates.
        bx0, by0 = self.logic_to_canvas(0, 0)
        bx1, by1 = self.logic_to_canvas(CANVAS_WIDTH, CANVAS_HEIGHT)
        self.canvas.create_line(bx0, by0, bx1, by0, fill="gray", width=2)
        self.canvas.create_line(bx0, by0, bx0, by1, fill="gray", width=2)
        self.canvas.create_line(bx1, by0, bx1, by1, fill="gray", width=2)
        self.canvas.create_line(bx0, by1, bx1, by1, fill="gray", width=2)

        # Scale factor for drawing vertices.
        BASE_RADIUS = 7
        point_scale_factor = max(0.5, min(2.0, math.sqrt(self.camera_scale)))

        selected_vertex = self.app.polygon_controller.selected_vertex
        
        # Draw grid lines if enabled
        if self.app.polygon_controller.grid_enabled:
            grid_spacing = self.app.polygon_controller.GRID_SPACING
            for x in range(0, CANVAS_WIDTH +1, grid_spacing):
                start_x, start_y = self.logic_to_canvas(x, 0)
                end_x, end_y = self.logic_to_canvas(x, CANVAS_HEIGHT)
                self.canvas.create_line(start_x, start_y, end_x, end_y, fill="lightgray", dash=(2, 2))
            for y in range(0, CANVAS_HEIGHT +1, grid_spacing):
                start_x, start_y = self.logic_to_canvas(0, y)
                end_x, end_y = self.logic_to_canvas(CANVAS_WIDTH, y)
                self.canvas.create_line(start_x, start_y, end_x, end_y, fill="lightgray", dash=(2, 2))
        

        # Draw completed polygons.
        for i, poly in enumerate(polygons):  # poly is now a PolygonModel instance.
            pts = poly.points
            n = len(pts)
            # Draw polygon edges.
            for j in range(n):
                x1, y1 = self.logic_to_canvas(*pts[j])
                x2, y2 = self.logic_to_canvas(*pts[(j + 1) % n])
                is_selected = (self.app.polygon_controller.selected_edge is not None and self.app.polygon_controller.selected_edge[0] == i and self.app.polygon_controller.selected_edge[1] == j)
                line_color = "gray" if is_selected else "black"
                self.canvas.create_line(x1, y1, x2, y2, fill=line_color, width=5)
            # Draw polygon vertices.
            for j, (lx, ly) in enumerate(pts):
                cx, cy = self.logic_to_canvas(lx, ly)
                r = BASE_RADIUS * point_scale_factor
                color = "gray" if selected_vertex == (i, j) else "black"
                self.canvas.create_oval(cx - r, cy - r, cx + r, cy + r, fill=color)
            # Draw skeleton/perpendicular/mountain/valley lines
            if self.app.polygon_controller.show_skeleton:
                for sk in poly.skeleton_line_ids:
                    sx1, sy1, sx2, sy2 = sk["coords"]
                    clipped = self.clip_line(sx1, sy1, sx2, sy2)
                    if clipped:
                        cax1, cay1 = self.logic_to_canvas(clipped[0], clipped[1])
                        cax2, cay2 = self.logic_to_canvas(clipped[2], clipped[3])
                        self.canvas.create_line(cax1, cay1, cax2, cay2, fill="green", width=3)
            if self.app.polygon_controller.show_perpendiculars:
                 for per in poly.perpendicular_line_ids:
                    px1, py1, px2, py2 = per["coords"]
                    clipped = self.clip_line(px1, py1, px2, py2)
                    if clipped:
                        cpx1, cpy1 = self.logic_to_canvas(clipped[0], clipped[1])
                        cpx2, cpy2 = self.logic_to_canvas(clipped[2], clipped[3])
                        self.canvas.create_line(cpx1, cpy1, cpx2, cpy2, fill="purple", width=3)
            if self.app.polygon_controller.show_crease_pattern:
                for mountain in poly.mountain_line_ids:
                    mx1, my1, mx2, my2 = mountain["coords"]
                    clipped = self.clip_line(mx1, my1, mx2, my2)
                    if clipped:
                        cmx1, cmy1 = self.logic_to_canvas(clipped[0], clipped[1])
                        cmx2, cmy2 = self.logic_to_canvas(clipped[2], clipped[3])
                        self.canvas.create_line(cmx1, cmy1, cmx2, cmy2, fill="red", width=3)
                for valley in poly.valley_line_ids:
                    vx1, vy1, vx2, vy2 = valley["coords"]
                    clipped = self.clip_line(vx1, vy1, vx2, vy2)
                    if clipped:
                        cvx1, cvy1 = self.logic_to_canvas(clipped[0], clipped[1])
                        cvx2, cvy2 = self.logic_to_canvas(clipped[2], clipped[3])
                        self.canvas.create_line(cvx1, cvy1, cvx2, cvy2, fill="blue", width=3)    
                
                

        # Draw in-progress polygon edges.
        if len(in_progress_points) > 1:
            for i in range(len(in_progress_points) - 1):
                x1, y1 = self.logic_to_canvas(*in_progress_points[i])
                x2, y2 = self.logic_to_canvas(*in_progress_points[i + 1])
                self.canvas.create_line(x1, y1, x2, y2, fill="black", width=5)
        # Draw in-progress polygon vertices.
        for (lx, ly) in in_progress_points:
            cx, cy = self.logic_to_canvas(lx, ly)
            r = BASE_RADIUS * point_scale_factor
            self.canvas.create_oval(cx - r, cy - r, cx + r, cy + r, fill="black")
                        
        

        # Update scroll region.
        self.canvas.config(scrollregion=self.canvas.bbox("all"))


    def _redraw(self):
        polygons = self.app.polygon_controller.polygons
        in_progress = self.app.polygon_controller.current_points
        self.redraw_all(polygons, in_progress)


    def on_canvas_mousewheel(self, event: tk.Event):
        ctrl_pressed = (event.state & 0x4) != 0  # platform dependent ???
        if ctrl_pressed:
            self.on_zoom(event)
        else:
            if hasattr(event, "delta") and event.delta:
                self.canvas.yview_scroll(-int(event.delta / 120), "units")
            elif event.num == 4:
                self.canvas.yview_scroll(-1, "units")
            elif event.num == 5:
                self.canvas.yview_scroll(1, "units")


    def on_zoom(self, event: tk.Event):
        factor = 1.1 if (hasattr(event, "delta") and event.delta > 0) or (hasattr(event, "num") and event.num == 4) else 0.9
        # Zoom relative to the mouse pointer.
        canvas_x = self.canvas.canvasx(event.x)
        canvas_y = self.canvas.canvasy(event.y)
        lx_before, ly_before = self.canvas_to_logic(canvas_x, canvas_y)

        self.camera_scale *= factor

        # Adjust offset so that (lx_before, ly_before) remains under the cursor.
        cx_after, cy_after = self.logic_to_canvas(lx_before, ly_before)
        dx = canvas_x - cx_after
        dy = canvas_y - cy_after
        self.camera_offset_x += dx
        self.camera_offset_y += dy

        self._redraw()


    def on_pan_start(self, event: tk.Event):
        self._pan_last_x = event.x
        self._pan_last_y = event.y


    def on_pan_drag(self, event: tk.Event):
        dx = event.x - self._pan_last_x
        dy = event.y - self._pan_last_y
        self.camera_offset_x += dx
        self.camera_offset_y += dy
        self._pan_last_x = event.x
        self._pan_last_y = event.y
        self._redraw()


    def center_canvas(self, event: tk.Event = None):
        # Center the view based on all polygon points and in-progress points.
        all_points = []
        for poly in self.app.polygon_controller.polygons:
            all_points.extend(poly.points)
        all_points.extend(self.app.polygon_controller.current_points)
        if not all_points:
            return
        xs = [p[0] for p in all_points]
        ys = [p[1] for p in all_points]
        cx_logic = (min(xs) + max(xs)) / 2
        cy_logic = (min(ys) + max(ys)) / 2

        canvas_w = self.canvas.winfo_width()
        canvas_h = self.canvas.winfo_height()
        desired_cx = canvas_w / 2
        desired_cy = canvas_h / 2

        curr_cx, curr_cy = self.logic_to_canvas(cx_logic, cy_logic)
        self.camera_offset_x += desired_cx - curr_cx
        self.camera_offset_y += desired_cy - curr_cy

        self._redraw()


    def on_left_button_down(self, event: tk.Event):
        # Skip dragging if Shift is pressed (used for adding points).
        if (event.state & 0x0001) != 0:
            return
        canvas_x = self.canvas.canvasx(event.x)
        canvas_y = self.canvas.canvasy(event.y)
        lx, ly = self.canvas_to_logic(canvas_x, canvas_y)
        # 1) Attempt to select the nearest vertex.
        vertex_selected = self.app.polygon_controller.select_vertex(lx, ly)
        edge_selected = False

        # 2) If no vertex selected, check for edge
        if not vertex_selected:
            edge_selected = self.app.polygon_controller.select_edge(lx, ly)

        # Clear the other selection type
        if vertex_selected:
            self.app.polygon_controller.selected_edge = None
        elif edge_selected:
            self.app.polygon_controller.selected_vertex = None

        # Redraw to show the updated selection highlight.
        self._redraw()

        # 3) If a vertex was found, record the move vertex action.
        if vertex_selected:
            self.app.polygon_controller.begin_move_vertex(lx, ly)
            self._dragging_vertex = True
        else:
            self._dragging_vertex = False


    def on_left_button_drag(self, event: tk.Event):
        if not self._dragging_vertex:
            return
        canvas_x = self.canvas.canvasx(event.x)
        canvas_y = self.canvas.canvasy(event.y)
        lx, ly = self.canvas_to_logic(canvas_x, canvas_y)
        self.app.polygon_controller.drag_vertex(lx, ly)


    def on_left_button_up(self, event: tk.Event):
        if self._dragging_vertex:
            self.app.polygon_controller.end_move_vertex()
        self._dragging_vertex = False
