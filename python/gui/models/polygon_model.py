import geometry
from geometry import TestStraightSkeleton, TestPoint, PerpendicularFinder, Crease,  FoldManager, FoldType, Origin

class PolygonModel:
    def __init__(self, points=None):
        self.points = points.copy() if points else []
        self.skeleton_line_ids = []  # List of dicts with key "coords"
        self.perpendicular_line_ids = []
        self.mountain_line_ids = []
        self.valley_line_ids = []


    def add_point(self, point: tuple[float, float]):
        self.points.append(point)


    def insert_point(self, index: int, point: tuple[float, float]):
        self.points.insert(index, point)


    def delete_point(self, index: int) -> tuple[float, float]:
        if len(self.points) <= 3:
            raise ValueError("A polygon must have at least 3 points.")
        return self.points.pop(index)


    def move_point(self, index: int, new_point: tuple[float, float]):
        self.points[index] = new_point
        
        
    def generate_creases(self):
        try:
            points_obj = [TestPoint(x,y) for x,y in self.points]
            fold_manager = FoldManager(points_obj)
            creases = fold_manager.get_creases()
            self.skeleton_line_ids = []
            self.perpendicular_line_ids = []
            self.mountain_line_ids = []
            self.valley_line_ids = []
            if creases:
                for crease in creases:
                    src, tgt = crease.edge
                    if crease.foldType == FoldType.MOUNTAIN:
                        self.mountain_line_ids.append({"coords": (src.x(), src.y(), tgt.x(), tgt.y())})
                    elif crease.foldType == FoldType.VALLEY:
                        self.valley_line_ids.append({"coords": (src.x(), src.y(), tgt.x(), tgt.y())})
                    if crease.origin == Origin.SKELETON:
                        self.skeleton_line_ids.append({"coords": (src.x(), src.y(), tgt.x(), tgt.y())})
                    elif crease.origin == Origin.PERPENDICULAR:
                         self.perpendicular_line_ids.append({"coords": (src.x(), src.y(), tgt.x(), tgt.y())})
            return True
        except Exception as e:
            raise RuntimeError(f"Error retrieving creases: {e}")
        
        
    def update_creases(self):
        if self.valley_line_ids and self.mountain_line_ids:
            self.generate_creases()


    @staticmethod
    def point_in_polygon(x: float, y: float, points: list[tuple[float, float]]) -> bool:
        inside = False
        n = len(points)
        if n < 3:
            return False
        p1x, p1y = points[0]
        for i in range(1, n + 1):
            p2x, p2y = points[i % n]
            if y > min(p1y, p2y):
                if y <= max(p1y, p2y):
                    if x <= max(p1x, p2x):
                        if p1y != p2y:
                            xinters = (y - p1y) * (p2x - p1x) / (p2y - p1y) + p1x
                        else:
                            xinters = p1x
                        if p1x == p2x or x <= xinters:
                            inside = not inside
            p1x, p1y = p2x, p2y
        return inside
