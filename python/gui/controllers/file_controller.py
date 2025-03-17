import tkinter as tk
from tkinter import filedialog, messagebox
from utils.fold_file_manager import FoldFileManager
from utils.fold_format import FOLDFormat
from models.polygon_model import PolygonModel
from utils.canvas_export import CanvasExport


class FileController:
    def __init__(self):
        self.current_file = None  # Track current file path
        
    def export_to_pdf(self, canvas_view):
        file_path = filedialog.asksaveasfilename(
        defaultextension=".pdf",
        filetypes=[("PDF files", "*.pdf")]
        )
        if file_path:
            CanvasExport(canvas_view).export_as_pdf(filepath=file_path, creator=canvas_view.app.root.title())


    def export_to_png(self, canvas_view):  # Changed parameter
        file_path = filedialog.asksaveasfilename(
            defaultextension=".png",
            filetypes=[("PNG files", "*.png")]
        )
        if file_path:
            CanvasExport(canvas_view).export_as_image(file_path)    
        
    
    def open_file(self, canvas_view):
        file_path = filedialog.askopenfilename(filetypes=[("FOLD files", "*.fold")])
        if not file_path:
            return
        
        try:
            fold_data = FoldFileManager.read_fold_file(file_path)
            self._import_fold_data(fold_data, canvas_view)
            messagebox.showinfo("Success", "File imported successfully")
        except Exception as e:
            messagebox.showerror("Import Error", str(e))
            print(f"Error importing FOLD file: {e}")

    def _import_fold_data(self, fold_data: dict, canvas_view):
        # Clear existing data
        canvas_view.app.polygon_controller.polygons = []
        canvas_view.app.polygon_controller.current_points = []

        # Get first frame data (or root frame)
        frame = fold_data
        if "file_frames" in fold_data and len(fold_data["file_frames"]) > 0:
            frame = fold_data["file_frames"][0]

        # Extract vertices (convert 3D to 2D if needed)
        vertices = frame.get("vertices_coords", [])
        if vertices and len(vertices[0]) > 2:  # Check if vertices are 3D
            vertices_2d = [[v[0], v[1]] for v in vertices]
        else:
            vertices_2d = vertices
        
        # Get first face (polygon)
        faces = frame.get("faces_vertices", [])
        if not faces:
            raise ValueError("No faces found in FOLD file")
        
        face_vertex_indices = faces[0]
        polygon_points = [vertices_2d[i] for i in face_vertex_indices]
        
        # Create and validate polygon
        polygon_model = PolygonModel(polygon_points)
        if len(polygon_model.points) < 3:
            raise ValueError("Imported polygon has less than 3 vertices")

        # Add creases
        edges = frame.get("edges_vertices", [])
        assignments = frame.get("edges_assignment", [])
        
        for edge, assign in zip(edges, assignments):
            if assign in ("M", "V"):
                try:
                    # Validate indices before access
                    if edge[0] >= len(vertices_2d) or edge[1] >= len(vertices_2d):
                        raise IndexError(f"Edge {edge} references non-existent vertices")
                        
                    v1 = vertices_2d[edge[0]]
                    v2 = vertices_2d[edge[1]]
                except (IndexError, TypeError) as e:
                    print(f"Skipping invalid edge {edge}: {str(e)}")
                    continue
                
                crease_data = {
                    "coords": (v1[0], v1[1], v2[0], v2[1])
                }
                
                if assign == "M":
                    polygon_model.mountain_line_ids.append(crease_data)
                elif assign == "V":
                    polygon_model.valley_line_ids.append(crease_data)

        # Add to application
        canvas_view.app.polygon_controller.polygons = [polygon_model]
        canvas_view.app.polygon_controller.show_crease_pattern = True
        canvas_view._redraw()    
        

    def save_fold(self, canvas_view) -> None:
        if self.current_file:
            self._save_fold_data(canvas_view, self.current_file)
        else:
            self.save_fold_as(canvas_view)

    def save_fold_as(self, canvas_view) -> None:
        filename = filedialog.asksaveasfilename(
            defaultextension=".fold",
            filetypes=[("FOLD files", "*.fold")]
        )
        if filename:
            self.current_file = filename
            self._save_fold_data(canvas_view, filename)

    def _save_fold_data(self, canvas_view, filename: str) -> None:
        fold_data = self._generate_fold_data(canvas_view)
        if fold_data:
            FoldFileManager.write_fold_file(filename, fold_data)
            messagebox.showinfo("Success", "File saved successfully.")
        else:
            messagebox.showerror("Error", "No polygon to save.")

    def _generate_fold_data(self, canvas_view) -> dict:
        fold = FOLDFormat()
        polygon_controller = canvas_view.app.polygon_controller
        if not polygon_controller.polygons:
            return None

        polygon = polygon_controller.polygons[0]
        vertices = polygon.points.copy()
        vertex_set = {(x, y) for x, y in vertices}

        # Collect crease points
        crease_points = []
        for mountain in polygon.mountain_line_ids:
            x1, y1, x2, y2 = mountain["coords"]
            crease_points.extend([(x1, y1), (x2, y2)])
        for valley in polygon.valley_line_ids:
            x1, y1, x2, y2 = valley["coords"]
            crease_points.extend([(x1, y1), (x2, y2)])

        # Add unique crease points to vertices
        for point in crease_points:
            if point not in vertex_set:
                vertices.append(point)
                vertex_set.add(point)

        # Populate FOLD data
        fold.file_author = "User"  # Customize as needed
        fold.file_classes = ["foldedForm"]
        fold.vertices_coords = [[x, y] for x, y in vertices]
        fold.faces_vertices = [list(range(len(polygon.points)))]

        # Add edges (boundary and creases)
        edges = []
        edges_assignment = []
        n = len(polygon.points)
        for i in range(n):
            edges.append([i, (i+1) % n])
            edges_assignment.append("B")

        # Add creases
        def add_edges(creases, assign):
            for crease in creases:
                x1, y1, x2, y2 = crease["coords"]
                try:
                    idx1 = vertices.index((x1, y1))
                    idx2 = vertices.index((x2, y2))
                except ValueError:
                    continue
                edges.append([idx1, idx2])
                edges_assignment.append(assign)

        add_edges(polygon.mountain_line_ids, "M")
        add_edges(polygon.valley_line_ids, "V")

        fold.edges_vertices = edges
        fold.edges_assignment = edges_assignment

        return fold.to_dict()