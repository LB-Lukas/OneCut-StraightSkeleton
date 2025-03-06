import geometry
from geometry import StraightSkeleton, Point2D, TestStraightSkeleton, TestPoint, PerpendicularFinder

class PolygonModel:
    def __init__(self, points=None):
        self.points = points.copy() if points else []
        self.skeleton_line_ids = []  # List of dicts with key "coords"
        self.perpendicular_line_ids = []


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


    def generate_skeleton(self):
        try:
            points_obj = [Point2D(x, y) for x, y in self.points]
            skeleton = StraightSkeleton(points_obj)
            # points_obj = [TestPoint(x, y) for x, y in self.points]
            # skeleton = TestStraightSkeleton(points_obj)
            edges = skeleton.get_edges()
            self.skeleton_line_ids = []
            if edges:
                for edge in edges:
                    src, tgt = edge
                    self.skeleton_line_ids.append({
                        "coords": (src.x(), src.y(), tgt.x(), tgt.y())
                    })
            return True
        except Exception as e:
            raise RuntimeError(f"Error generating skeleton: {e}")


    def update_skeleton(self):
        if self.skeleton_line_ids:
            self.generate_skeleton()
            
            
    def generate_perpendiculars(self):
        try:
            points_obj = [TestPoint(x, y) for x, y in self.points]
            skeleton = TestStraightSkeleton(points_obj)
            finder = PerpendicularFinder(skeleton)
            edges = finder.find_perpendiculars()
            self.perpendicular_line_ids = []
            if edges:
                for edge in edges:
                    src, tgt = edge
                    self.perpendicular_line_ids.append({
                        "coords": (src.x(), src.y(), tgt.x(), tgt.y())
                    })
            return True
        except Exception as e:
            raise RuntimeError(f"Error generating perpendiculars: {e}")
        
    def update_perpendiculars(self):
        if self.perpendicular_line_ids:
            self.generate_perpendiculars()


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
