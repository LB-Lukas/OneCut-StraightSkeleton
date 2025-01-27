import tkinter as tk
from tkinter import filedialog, messagebox
from utils.canvas_export import CanvasExport
from utils.fold_file_manager import FoldFileManager


class FileController:
    def __intit__(self, canvas: tk.Canvas, polygon_controller):
        self.canvas = canvas
        self.polygon_controller = polygon_controller
        
        
    def open_file(self):
        file_path = filedialog.askopenfilename(filetypes=[("Fold File", "*.fold")])
        if file_path:
            fold_data = FoldFileManager.read_fold_file(file_path)
            self._draw_crease_pattern(fold_data)
            
    
    def _draw_crease_pattern(self, fold_data):
        # TODO implement this method
        pass
            
            
    def _translate_vertices_to_center(self, vertices):
        # TODO implement this method
        pass
    
    
    def export_to_pdf(self):
        filename = filedialog.asksaveasfilename(defaultextension=".pdf", filetypes=[("PDF files", "*.pdf")])
        
        if not filename:
            return
        
        exporter = CanvasExport(self.canvas)
        exporter.export_as_pdf(filename)


    def export_to_png(self):
        filename = filedialog.asksaveasfilename(defaultextension=".png", filetypes=[("PNG files", "*.png")])
        
        if not filename:
            return
        
        exporter = CanvasExport(self.canvas)
        exporter.export_as_image(filename)
        
    
    