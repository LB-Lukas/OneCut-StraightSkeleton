from tkinter import Tk, filedialog

from extra.gui.folding_manager import FoldingManager
from polygon_drawer import PolygonDrawer

MAX_POLYGONS: int = 3
SOFTWARE_NAME: str = "Straight-Skeleton Tool"
SOFTWARE_VERSION: str = "0.1.0"


def main():
    root = Tk()
    root.title(f"{SOFTWARE_NAME} v{SOFTWARE_VERSION}")
    #root.iconbitmap("extra/gui/icon.ico") # no icon added yet
    app = PolygonDrawer(root, max_polygons=MAX_POLYGONS)
    foldingManager = FoldingManager([(1, 30), (100, 30), (100, 130), (50, 230), (120, 50), (140, 75), (180, 90), (160, 120), (200, 150), (220, 170), (210, 200), (180, 210), (160, 240), (140, 230), (110, 180), (90, 160), (80, 130), (70, 90), (60, 70), (50, 50)])
    app.draw_folding(foldingManager)
    root.mainloop()

if __name__ == "__main__":
    main()