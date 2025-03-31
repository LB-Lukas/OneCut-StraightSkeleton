import tkinter as tk
from app import StraightSkeletonApp


SOFTWARE_NAME: str = "One-Cut Tool"
SOFTWARE_VERSION: str = "1.0.0"
MAX_POLYGONS: int = 1


def main():
    root = tk.Tk()
    root.title(f"{SOFTWARE_NAME} v{SOFTWARE_VERSION}")
    root.minsize(1000, 800)
    app = StraightSkeletonApp(root, MAX_POLYGONS)
    icon = tk.PhotoImage(file="icon.png")
    root.iconphoto(True, icon)
    root.mainloop()


if __name__ == "__main__":
    main()