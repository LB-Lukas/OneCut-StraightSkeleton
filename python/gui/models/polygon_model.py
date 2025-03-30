import one_cut
from one_cut import Point, FoldManager, FoldType, Origin

class PolygonModel:
    """ 
    @brief Represents a polygon model with points and crease pattern generation capabilities.
    """
    def __init__(self, points=None):
        """ 
        @brief Initializes a PolygonModel with optional initial points.
        @param points Optional list of (x,y) tuples to initialize polygon (default None).
        """
        self.points = points.copy() if points else []
        self.skeleton_line_ids = []  # List of dicts with key "coords"
        self.perpendicular_line_ids = []
        self.mountain_line_ids = []
        self.valley_line_ids = []
    
        
    def clear_creases(self):
        """ 
        @brief Clears all stored crease patterns (skeleton, perpendiculars, and fold lines).
        """
        self.mountain_line_ids = []
        self.valley_line_ids = []
        self.skeleton_line_ids = []
        self.perpendicular_line_ids = []


    def add_point(self, point: tuple[float, float]):
        """ 
        @brief Appends a new point to the polygon.
        @param point Tuple containing (x,y) coordinates of new point.
        """
        self.points.append(point)


    def insert_point(self, index: int, point: tuple[float, float]):
        """ 
        @brief Inserts a point at specified position in polygon.
        @param index Position to insert new point.
        @param point Tuple containing (x,y) coordinates of new point.
        """
        self.points.insert(index, point)


    def delete_point(self, index: int) -> tuple[float, float]:
        """ 
        @brief Removes and returns a point from the polygon.
        @param index Position of point to remove.
        @return Tuple containing (x,y) coordinates of deleted point.
        @raises ValueError If polygon would have fewer than 3 points after deletion.
        """
        if len(self.points) <= 3:
            raise ValueError("A polygon must have at least 3 points.")
        return self.points.pop(index)


    def move_point(self, index: int, new_point: tuple[float, float]):
        """ 
        @brief Updates coordinates of an existing polygon point.
        @param index Position of point to modify.
        @param new_point Tuple containing new (x,y) coordinates.
        """
        self.points[index] = new_point
        
        
    def generate_creases(self):
        """ 
        @brief Generates crease patterns using FoldManager algorithm.
        @raises RuntimeError If crease generation fails.
        @return True if creases generated successfully.
        """
        try:
            points_obj = [Point(x,y) for x,y in self.points]
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
        """ 
        @brief Regenerates creases if mountain/valley lines already exist.
        """
        if self.valley_line_ids and self.mountain_line_ids:
            self.generate_creases()


    @staticmethod
    def point_in_polygon(x: float, y: float, points: list[tuple[float, float]]) -> bool:
        """ 
        @brief Determines if a point is inside a polygon using ray-casting algorithm.
        @param x X-coordinate of point to check.
        @param y Y-coordinate of point to check.
        @param points List of (x,y) tuples defining polygon vertices.
        @return True if point is inside polygon, False otherwise.
        """
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