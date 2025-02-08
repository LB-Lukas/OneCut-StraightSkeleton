import numpy as np


class IntersectionHelper:
    def __init__(self):
        pass


    def check_intersection(self, x, y, polygons, lines, canvas, points):
        if not points:
            return False

        new_edge = (points[-1], (x, y))

        # Check against the edges of the in-progress polygon.
        if len(points) >= 2:
            # Compare new_edge with each existing edge in the in-progress polygon (except adjacent segments)
            for i in range(len(points) - 2):
                current_edge = (points[i], points[i+1])
                if self.is_intersecting(current_edge, new_edge):
                    return True

        # Check against the edges of all finished polygons.
        for polygon in polygons:
            poly_points = polygon.points
            n = len(poly_points)
            if n >= 3:
                # Loop through each edge (including the closing edge from the last to the first point)
                for i in range(n):
                    edge = (poly_points[i], poly_points[(i + 1) % n])
                    if self.is_intersecting(edge, new_edge):
                        return True

        # You might still have checks against other lines if needed.
        for line in lines:
            coords = canvas.coords(line)
            if len(coords) == 4:
                line_edge = ((coords[0], coords[1]), (coords[2], coords[3]))
                if self.is_intersecting(line_edge, new_edge):
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
