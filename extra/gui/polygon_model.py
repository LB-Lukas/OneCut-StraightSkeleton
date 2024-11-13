import numpy as np

class PolygonModel:
    def __init__(self):
        self.points = []
        self.lines = []
        self.polygons = []

    def add_point(self, x, y):
        self.points.append((x, y))

    def finish_polygon(self):
        self.polygons.append((self.points, self.lines))
        self.points = []
        self.lines = []

    def check_intersection(self, x, y):
        if len(self.points) > 0:
            for polygon in self.polygons:
                for line in polygon[1]:
                    coords = self.canvas.coords(line)
                    if len(coords) == 4:
                        if self.is_intersecting(((coords[0], coords[1]), (coords[2], coords[3])), ((self.points[-1][0], self.points[-1][1]), (x, y))):
                            return True

        for line in self.lines:
            coords = self.canvas.coords(line)
            if len(coords) == 4:
                if self.is_intersecting(((coords[0], coords[1]), (coords[2], coords[3])), ((self.points[-1][0], self.points[-1][1]), (x, y))):
                    return True
        return False

    def is_intersecting(self, line1, line2):
        A, B = np.array(line1[0]), np.array(line1[1])
        C, D = np.array(line2[0]), np.array(line2[1])
        AB = B - A
        CD = D - C
        def det(v, w):
            return v[0] * w[1] - v[1] * w[0]
        AC = C - A
        denominator = det(AB, CD)
        if denominator == 0:
            return False
        t = det(AC, CD) / denominator
        u = det(AC, AB) / denominator
        return 0 < t < 1 and 0 < u < 1