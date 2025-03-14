from reportlab.lib.pagesizes import A4
from reportlab.pdfgen import canvas as pdf_canvas
from PIL import Image, ImageDraw
import os
import tempfile

SCALE_FACTOR = 5  # 600x600 * 5 = 3000x3000 
LINE_WIDTH_MULTIPLIER = 2  # Scale line widths for better visibility

class CanvasExport:
    def __init__(self, canvas_view):
        self.canvas_view = canvas_view
        self.canvas = canvas_view.canvas

    def export_as_image(self, filepath: str):
        # Create high-resolution image
        img = Image.new("RGB", 
                      (600 * SCALE_FACTOR, 600 * SCALE_FACTOR), 
                      "white")
        draw = ImageDraw.Draw(img)

        # Draw all lines with original colors and scaled coordinates
        for item in self.canvas.find_all():
            if self.canvas.type(item) == "line":
                # Get line properties from canvas
                coords = self._get_scaled_coords(item)
                color = self.canvas.itemcget(item, "fill")
                width = max(1, int(float(self.canvas.itemcget(item, "width")) * LINE_WIDTH_MULTIPLIER))

                # Draw scaled line
                draw.line(coords, fill=color, width=width)

        # Save high-quality PNG
        img.save(filepath, dpi=(400, 400))
        
        print(f"High-res image saved to {filepath}")
    

    def export_as_pdf(self, filepath: str, title: str = "OneCut PDF", creator: str = "OneCut App"):
        temp_image_path = None
        try:
            # Create a temporary image file
            with tempfile.NamedTemporaryFile(suffix='.png', delete=False) as tmpfile:
                temp_image_path = tmpfile.name
                self.export_as_image(temp_image_path)
            
            # Create PDF canvas
            pdf = pdf_canvas.Canvas(filepath, pagesize=A4)
            page_width, page_height = A4
            
            pdf.setTitle(title)
            pdf.setCreator(creator)
            pdf.setAuthor(creator)
            pdf.setSubject("OneCut Export")
            
            
            # Open the temporary image to get dimensions and DPI
            with Image.open(temp_image_path) as img:
                dpi = img.info.get('dpi', (400, 400))  # Default to 400 DPI if not found
                img_width_pt = (img.width / dpi[0]) * 72  # Convert inches to points
                img_height_pt = (img.height / dpi[1]) * 72
                
                margin = 10  # Narrow margin of 20 points
                available_width = page_width - 2 * margin
                
                # Calculate scaling factor if image is wider than available space
                scale_factor = 1.0
                if img_width_pt > available_width:
                    scale_factor = available_width / img_width_pt
                    img_width_pt *= scale_factor
                    img_height_pt *= scale_factor
                
                # Calculate position to center horizontally and place at top with margin
                x = (page_width - img_width_pt) / 2
                y = page_height - margin - img_height_pt
                
                # Draw the image on the PDF
                pdf.drawImage(temp_image_path, x, y, width=img_width_pt, height=img_height_pt)
            
            # Save the PDF
            pdf.save()
            print(f"PDF saved to {filepath}")
        finally:
            # Clean up the temporary image file
            if temp_image_path and os.path.exists(temp_image_path):
                os.remove(temp_image_path)
                
                
    def _get_scaled_coords(self, item):
        """Convert canvas coords to logical coords and scale up"""
        raw_coords = self.canvas.coords(item)
        scaled_coords = []
        
        # Process coordinates in pairs (x1,y1,x2,y2)
        for i in range(0, len(raw_coords), 2):
            # Convert to logical coordinates
            lx, ly = self.canvas_view.canvas_to_logic(raw_coords[i], 
                                                    raw_coords[i+1])
            # Scale for high-res output
            scaled_coords.extend([
                lx * SCALE_FACTOR,
                ly * SCALE_FACTOR
            ])
        return scaled_coords
