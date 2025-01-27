import numpy as np


class IntersectionHelper:
    def __init__(self):
        pass


    def check_intersection(self, x, y, polygons, lines, canvas, points):
        if not points:
            return False

        for polygon in polygons:
            for line in polygon[1]:
                coords = canvas.coords(line)
                if len(coords) == 4:
                    if self.is_intersecting(((coords[0], coords[1]), (coords[2], coords[3])), ((points[-1][0], points[-1][1]), (x, y))):
                        return True
                    
        for line in lines:
            coords = canvas.coords(line)
            if len(coords) == 4:
                if self.is_intersecting(((coords[0], coords[1]), (coords[2], coords[3])), ((points[-1][0], points[-1][1]), (x, y))):
                    return True

        return False


    def is_intersecting(self, line1, line2):
        # line1: (A, B), line2: (C, D)
        A, B = np.array(line1[0]), np.array(line1[1])
        C, D = np.array(line2[0]), np.array(line2[1])

        AB = B - A
        CD = D - C

        # Determinant (AB x CD)
        def det(v, w):
            return v[0]*w[1] - v[1]*w[0]

        AC = C - A
        denom = det(AB, CD)
        if denom == 0:
            # Parallel lines => no intersection
            return False

        t = det(AC, CD) / denom
        u = det(AC, AB) / denom

        return 0 < t < 1 and 0 < u < 1
