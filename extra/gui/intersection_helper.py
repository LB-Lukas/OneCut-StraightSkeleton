class IntersectionHelper:
    import numpy as np
    
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
        # Start- und Endpunkte der Linien
        A, B = self.np.array(line1[0]), self.np.array(line1[1])
        C, D = self.np.array(line2[0]), self.np.array(line2[1])
        
        # Richtungsvektoren der Linien
        AB = B - A
        CD = D - C
        
        # Helper-Funktion zur Berechnung des Determinanten von zwei Vektoren
        def det(v, w):
            return v[0] * w[1] - v[1] * w[0]
        
        # Differenz der Startpunkte der beiden Linien
        AC = C - A
        
        # Berechne den Determinanten für Parallelität
        denominator = det(AB, CD)
        if denominator == 0:
            # Die Linien sind parallel und schneiden sich nicht
            return False
        
        # Parameter t und u berechnen
        t = det(AC, CD) / denominator
        u = det(AC, AB) / denominator
        
        # Prüfen, ob der Schnittpunkt innerhalb beider Liniensegmente liegt
        return 0 < t < 1 and 0 < u < 1
