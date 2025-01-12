from tkinter import Tk, filedialog

from folding_manager import FoldingManager
from polygon_drawer import PolygonDrawer

MAX_POLYGONS: int = 1
SOFTWARE_NAME: str = "Straight-Skeleton Tool"
SOFTWARE_VERSION: str = "0.1.0"


def main():
    root = Tk()
    root.title(f"{SOFTWARE_NAME} v{SOFTWARE_VERSION}")
    #root.iconbitmap("python/gui/icon.ico") # no icon added yet
    app = PolygonDrawer(root, max_polygons=MAX_POLYGONS)
    foldingManager = FoldingManager([(100, 140), (300, 60), (400, 200), (250, 400), (60, 300)])
    app.draw_folding(foldingManager)
    root.mainloop()

if __name__ == "__main__":
    main()
