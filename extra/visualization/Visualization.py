import matplotlib.pyplot as plt
import numpy as np
import random

import geometry


class Polygon:
    def __init__(self, vertices):
        if len(vertices) < 3:
            raise ValueError("Polygon must have at least 3 vertices")
        self.vertices = vertices

    @staticmethod
    def random_simple_polygon(number_of_vertices, radius=1):  # not self-intersecting polygon.h

        vertices = [(random.uniform(0, 1), random.uniform(0, 1)) for _ in range(number_of_vertices)]

        # Step 2: Calculate the centroid of these points
        centroid = (sum(x for x, y in vertices) / number_of_vertices,
                    sum(y for x, y in vertices) / number_of_vertices)

        # Step 3: Sort points by angle with respect to the centroid
        vertices.sort(key=lambda point: np.arctan2(point[1] - centroid[1], point[0] - centroid[0]))

        return Polygon(vertices)

    def area(self):
        x = [p[0] for p in self.vertices]
        y = [p[1] for p in self.vertices]
        return 0.5 * abs(sum(x[i] * y[i + 1] - x[i + 1] * y[i] for i in range(-1, len(self.vertices) - 1)))

    def perimeter(self):
        def distance(p1, p2):
            return np.sqrt((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2)
        return sum(distance(self.vertices[i], self.vertices[(i + 1) % len(self.vertices)]) for i in range(len(self.vertices)))

    def draw(self, ax):
        polygon = plt.Polygon(self.vertices, fill=False, edgecolor='purple', linewidth=2)
        ax.add_patch(polygon)
        ax.set_aspect('equal')



# Visualization function
def visualize_shapes(shapes):
    fig, ax = plt.subplots()
    for shape in shapes:
        shape.draw(ax)
    plt.xlim(0, 1)
    plt.ylim(0, 1)
    plt.grid(True)
    plt.show()


# Example Usage
if __name__ == "__main__":
    fold_type_convex = geometry.FoldType.Convex

    if fold_type_convex == geometry.FoldType.Convex:
        print("false")

    n = 1
    shapes = [Polygon.random_simple_polygon(3, 5) for _ in range(n)]

    visualize_shapes(shapes)

    # Print area and perimeter of each shape
    for shape in shapes:
        print(f"{shape.__class__.__name__}: Area = {shape.area():.2f}, Perimeter = {shape.perimeter():.2f}")