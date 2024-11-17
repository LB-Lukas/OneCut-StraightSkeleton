from tkinter import Tk, filedialog
from polygon_drawer import PolygonDrawer

MAX_POLYGONS: int = 3
SOFTWARE_NAME: str = "Straight-Skeleton Tool"
SOFTWARE_VERSION: str = "0.1.0"


def main():
    root = Tk()
    root.title(f"{SOFTWARE_NAME} v{SOFTWARE_VERSION}")
    #root.iconbitmap("extra/gui/icon.ico") # no icon added yet
    app = PolygonDrawer(root, max_polygons=MAX_POLYGONS)
    root.mainloop()

if __name__ == "__main__":
    main()