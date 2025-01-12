#########################
#                       #
#   ONLY FOR TESTING    #
#                       #
#########################

import tkinter as tk
import geometry as gm

def test_perpendicular_graph_same_as_cpp():
    # CREATE CUT GRAPH (SQUARE IN MIDDLE)
    cut_graph = gm.PlanarGraph()
    v0 = cut_graph.add_vertex(gm.Point(250, 150))
    v1 = cut_graph.add_vertex(gm.Point(350, 150))
    v2 = cut_graph.add_vertex(gm.Point(350, 250))
    v3 = cut_graph.add_vertex(gm.Point(250, 250))
    cut_graph.add_face(v0, v1, v2, v3)

    # CREATE SKELETON GRAPH
    skeleton_graph = gm.PlanarGraph()
    center = skeleton_graph.add_vertex(gm.Point(300, 200))
    v5 = skeleton_graph.add_vertex(gm.Point(200, 100))
    v6 = skeleton_graph.add_vertex(gm.Point(200, 300))
    v7 = skeleton_graph.add_vertex(gm.Point(400, 100))
    v8 = skeleton_graph.add_vertex(gm.Point(400, 300))
    skeleton_graph.add_edge(center, v5)
    skeleton_graph.add_edge(center, v6)
    skeleton_graph.add_edge(center, v7)
    skeleton_graph.add_edge(center, v8)

    # CREATE PerpendicularGraph AND ADD PERPENDICULARS
    perpendicular_graph_obj = gm.PerpendicularGraph(skeleton_graph, cut_graph)
    perpendicular_graph_obj.addPerpendiculars()
    perpendicular_graph = perpendicular_graph_obj.getPerpendicularGraph()

    # Extract edges from cut_graph (gray)
    cut_edges = []
    for (src, tgt) in cut_graph.edges():
        cut_edges.append(((float(src.x()), float(src.y())),
                          (float(tgt.x()), float(tgt.y()))))

    # Extract edges from skeleton_graph (green)
    skeleton_edges = []
    for (src, tgt) in skeleton_graph.edges():
        skeleton_edges.append(((float(src.x()), float(src.y())),
                               (float(tgt.x()), float(tgt.y()))))

    # Extract edges from perpendicular_graph (blue)
    perp_edges = []
    for (src, tgt) in perpendicular_graph.edges():
        perp_edges.append(((float(src.x()), float(src.y())),
                           (float(tgt.x()), float(tgt.y()))))

    # DRAW EDGES
    root = tk.Tk()
    canvas = tk.Canvas(root, width=600, height=400, bg="white")
    canvas.pack()

    # Cut graph edges
    for (x1, y1), (x2, y2) in cut_edges:
        canvas.create_line(x1, y1, x2, y2, fill="gray", width=2)

    # Skeleton graph edges
    for (x1, y1), (x2, y2) in skeleton_edges:
        canvas.create_line(x1, y1, x2, y2, fill="green", width=2)

    # Perpendicular graph edges
    for (x1, y1), (x2, y2) in perp_edges:
        canvas.create_line(x1, y1, x2, y2, fill="blue", dash=(4, 2))

    root.mainloop()

# ONLY FOR TESTING
if __name__ == "__main__":
    test_perpendicular_graph_same_as_cpp()