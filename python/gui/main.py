import tkinter as tk
from app import StraightSkeletonApp


SOFTWARE_NAME: str = "Straight-Skeleton Tool"
SOFTWARE_VERSION: str = "0.2.0"
MAX_POLYGONS: int = 1


def main():
    root = tk.Tk()
    root.title(f"{SOFTWARE_NAME} v{SOFTWARE_VERSION}")
    app = StraightSkeletonApp(root)
    root.mainloop()


if __name__ == "__main__":
    main()