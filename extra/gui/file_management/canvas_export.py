from reportlab.lib.pagesizes import A4
from reportlab.pdfgen import canvas as pdf_canvas
from PIL import Image
from tkinter import Canvas
import os

class CanvasExport:
    """
    A class to export a Tkinter Canvas as an image or PDF.
    Attributes:
    -----------
    canvas : Canvas
        The Tkinter Canvas to be exported.
    Methods:
    --------
    __init__(tk_canvas: Canvas):
        Initializes the CanvasExport with a Tkinter Canvas.
    export_as_image(filepath: str):
        Exports the Tkinter Canvas as an image file.
    export_as_pdf(filepath: str):
        Exports the Tkinter Canvas as a PDF file.
    """
    def __init__(self, tk_canvas: Canvas):
        self.canvas = tk_canvas

    def export_as_image(self, filepath: str):
        eps_filepath = filepath + ".eps"
        self.canvas.postscript(file=eps_filepath)
        img = Image.open(eps_filepath)
        img.save(filepath)
        os.remove(eps_filepath)  # Delete the EPS file
        print(f"Image saved as {filepath}")

    def export_as_pdf(self, filepath: str):
        # Export the canvas as an image
        image_filepath = filepath + ".png"
        self.export_as_image(image_filepath)

        # Create an A4 PDF and embed the image
        pdf = pdf_canvas.Canvas(filepath, pagesize=A4)
        a4_width, a4_height = A4

        # Load the image to get its dimensions
        img = Image.open(image_filepath)
        img_width, img_height = img.size

        # Scale the image to fit within A4 while maintaining the aspect ratio
        aspect_ratio = img_width / img_height
        max_width = a4_width - 50  # Leave some margin (optional)
        max_height = a4_height - 50

        if img_width > max_width or img_height > max_height:
            if img_width / max_width > img_height / max_height:
                img_width = max_width
                img_height = max_width / aspect_ratio
            else:
                img_height = max_height
                img_width = max_height * aspect_ratio

        # Center the image on the page
        x_offset = (a4_width - img_width) / 2
        y_offset = (a4_height - img_height) / 2

        pdf.drawImage(image_filepath, x_offset, y_offset, width=img_width, height=img_height)
        pdf.save()
        os.remove(image_filepath)  # Delete the PNG file
        print(f"PDF saved as {filepath}")