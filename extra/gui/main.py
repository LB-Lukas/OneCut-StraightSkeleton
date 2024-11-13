from tkinter import Tk
from polygon_drawer import PolygonDrawer

def main():
    root = Tk()
    root.title("One Cut Polygon")
    app = PolygonDrawer(root)
    root.mainloop()

if __name__ == "__main__":
    main()