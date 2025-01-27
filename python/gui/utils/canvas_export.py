from reportlab.lib.pagesizes import A4
from reportlab.pdfgen import canvas as pdf_canvas
from PIL import Image
import os


class CanvasExport:
    def __init__(self, tk_canvas):
        self.canvas = tk_canvas


    def export_as_image(self, filepath: str):
        eps_filepath = filepath + ".eps"
        self.canvas.postscript(file=eps_filepath)
        img = Image.open(eps_filepath)
        img.save(filepath)
        os.remove(eps_filepath)
        print(f"Image saved as {filepath}")


    def export_as_pdf(self, filepath: str):
        # Export the canvas as image first
        image_filepath = filepath + ".png"
        self.export_as_image(image_filepath)

        # Build PDF
        pdf = pdf_canvas.Canvas(filepath, pagesize=A4)
        a4_width, a4_height = A4
        img = Image.open(image_filepath)
        img_width, img_height = img.size

        # Fit inside A4
        margin = 50
        max_width = a4_width - margin
        max_height = a4_height - margin
        aspect = img_width / img_height

        if img_width > max_width or img_height > max_height:
            if (img_width / max_width) > (img_height / max_height):
                img_width = max_width
                img_height = max_width / aspect
            else:
                img_height = max_height
                img_width = max_height * aspect

        x_offset = (a4_width - img_width) / 2
        y_offset = (a4_height - img_height) / 2

        pdf.drawImage(image_filepath, x_offset, y_offset, width=img_width, height=img_height)
        pdf.save()
        os.remove(image_filepath)
        print(f"PDF saved as {filepath}")
