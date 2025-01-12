# import geometry module provided via pybind11
# try:
#     import geometry
# except ImportError as e:
#     print(e)
    
# from geometry import Folding, Point

#from geometry import Folding, Point
import geometry as gm
    

class FoldingManager():
    """
    A class to manage the folding operations for a given set of points.
    Methods
    -------
    __init__(points)
        Initializes the FoldingManager with a set of points and generates the folding.
    __generate_folding(points)
        Generates the folding based on the given points.
    get_vertices()
        Returns a list of all vertices in the folding.
    get_mountains()
        Returns a list of all mountain folds in the folding.
    get_valleys()
        Returns a list of all valley folds in the folding.
    """
    def __init__(self, points):
        self.folding = gm.Folding() # A folding has the methods getFolding(points[]), getVertices(), getMountains(), getValleys()
        self.__generate_folding(points)
    
    def __generate_folding(self, points):
        polygon = []
        for point in points:
            _point = gm.Point(point[0], point[1])
            polygon.append(_point)
        self.folding = gm.Folding.getFolding(polygon)
    
    # function to get all the vertices
    def get_vertices(self):
        print("FoldingManger.get_vertices() - start")
        vertices = []
        for vertex in self.folding.getVertices():
            vertices.append((vertex.x(), vertex.y()))
        print("FoldingManger.get_vertices() - end")
        return vertices
        
    # fuction to get all the mountains
    def get_mountains(self):
        print("FoldingManger.get_mountains() - start")
        mountains = []
        for mountain in self.folding.getMountains():
            start = mountain[0]
            end = mountain[1]
            mountains.append(((start.x(), start.y()), (end.x(), end.y())))
        print("FoldingManger.get_mountains() - end")
        return mountains
    
    # function to get all the valleys
    def get_valleys(self):
        print("FoldingManger.get_valleys() - start")
        valleys = []
        for valley in self.folding.getValleys():
            start = valley[0]
            end = valley[1]
            valleys.append(((start.x(), start.y()), (end.x(), end.y())))
        print("FoldingManger.get_valleys() - end")
        return valleys