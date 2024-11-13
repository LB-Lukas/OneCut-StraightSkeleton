from tkinter import Tk
from polygon_drawer import PolygonDrawer

MAX_POLYGONS = 1

def main():
    root = Tk()
    root.title("One Cut Polygon")
    app = PolygonDrawer(root, max_polygons=MAX_POLYGONS)
    root.mainloop()

if __name__ == "__main__":
    main()