#include "straight_skeleton/TestStraightSkeleton.h"

namespace TestSkeleton {

struct PointComparator {
    bool operator()(const Point& a, const Point& b) const {
        if (a.x() != b.x()) return a.x() < b.x();
        return a.y() < b.y();
    }
};

// A helper to form a normalized edge key from two points.
std::pair<Point, Point> make_normalized_edge(const Point& a, const Point& b) {
    if (PointComparator()(a, b))
        return {a, b};
    else
        return {b, a};
}

TestStraightSkeleton::TestStraightSkeleton(const std::vector<Point>& polygon_points)
    : originalPolygonPoints(polygon_points) {
    // Construct the polygon from the input points
    Polygon_2 polygon;
    for (const auto& p : polygon_points) {
        std::cout << "Adding point: " << p << " to polygon." << std::endl;
        polygon.push_back(p);
    }

    if (polygon.is_clockwise_oriented()) {
        polygon.reverse_orientation();
    }

    // Compute the straight skeleton
    std::cout << "Computing straight skeleton..." << std::endl;
    iss_ = CGAL::create_interior_straight_skeleton_2(polygon.vertices_begin(), polygon.vertices_end());
    std::cout << "Straight skeleton computed." << std::endl;
    std::cout << "Computing exterior skeleton" << std::endl;
    oss_ = CGAL::create_exterior_straight_skeleton_2(1000, polygon.vertices_begin(), polygon.vertices_end());
    std::cout << "Exterior skeleton computed." << std::endl;

    if (!iss_) {
        std::cerr << "Failed to create straight skeleton (possibly invalid polygon)" << std::endl;
        return;
    }
    CGAL::Straight_skeletons_2::IO::print_straight_skeleton(*iss_);

    if (!iss_) {
        std::cerr << "Failed to create exterior skeleton" << std::endl;
        return;
    }
    CGAL::Straight_skeletons_2::IO::print_straight_skeleton(*oss_);

    auto polygonBoundaryEdges = computePolygonBoundaryEdges();



    // TODO: this approach is not completely correct. Many faces are adjacent to faces with index -2 ???
    std::vector<straight_skeleton::SkeletonFace> innerFaces = skeletonToFaces(iss_, polygonBoundaryEdges);
    std::vector<straight_skeleton::SkeletonFace> outerFaces = skeletonToFaces(oss_, polygonBoundaryEdges);

    std::cout << std::endl << "INNER FACES: " << std::endl;
    for (const straight_skeleton::SkeletonFace& face : innerFaces) {
        std::cout << face << std::endl;
    }
    std::cout << std::endl << "OUTER FACES: " << std::endl;
    for (const straight_skeleton::SkeletonFace& face : outerFaces) {
        std::cout << face << std::endl;
    }




    std::vector<straight_skeleton::SkeletonFace> filteredOuterFaces;
    for (const auto& face : outerFaces) {
        bool incident = false;
        for (int adj : face.adjacentFaces) {
            if (adj == -2) {  // pending marker means this edge lies on the polygon boundary.
                incident = true;
                break;
            }
        }
        if (incident) {
            filteredOuterFaces.push_back(face);
        }
    }

    // TODO insert only outer faces that are incident to the polygon boundary
    std::vector<straight_skeleton::SkeletonFace> allFaces;
    allFaces.insert(allFaces.end(), innerFaces.begin(), innerFaces.end());
    allFaces.insert(allFaces.end(), filteredOuterFaces.begin(), filteredOuterFaces.end());


    std::cout << std::endl << "ALL FACES: " << std::endl;
    for (const straight_skeleton::SkeletonFace& face : allFaces) {
        std::cout << face << std::endl;
    }

    // Fix the adjacent face indices for polygon boundary edges.
    matchPolygonEdges(allFaces, polygonBoundaryEdges);

    // Now store the merged faces for later processing.
    faces = allFaces;

    // Map to store graph vertices corresponding to each skeleton point.
    std::map<Point, SurfaceMesh::Vertex_index> vertex_map;

    // Iterate over the skeleton’s halfedges.
    // (Each edge appears twice, so process one representative from each pair.)
    for (auto he = oss_->halfedges_begin(); he != oss_->halfedges_end(); ++he) {  // Outer skeleotn
        std::cout << "Processing halfedge: " << &(*he) << std::endl;
        // Use a simple trick to avoid duplicates:
        if (he < he->opposite()) {
            Point src = he->vertex()->point();
            Point tgt = he->opposite()->vertex()->point();
            std::cout << "Halfedge passes duplicate check. Source: " << src << ", Target: " << tgt << std::endl;

            // Add source vertex if not already added
            if (vertex_map.find(src) == vertex_map.end()) {
                SurfaceMesh::Vertex_index v = graph.add_vertex();
                graph.point(v) = src;
                vertex_map[src] = v;
                std::cout << "Added source vertex: " << src << std::endl;
            } else {
                std::cout << "Source vertex already exists: " << src << std::endl;
            }

            // Add target vertex if not already added
            if (vertex_map.find(tgt) == vertex_map.end()) {
                SurfaceMesh::Vertex_index v = graph.add_vertex();
                graph.point(v) = tgt;
                vertex_map[tgt] = v;
                std::cout << "Added target vertex: " << tgt << std::endl;
            } else {
                std::cout << "Target vertex already exists: " << tgt << std::endl;
            }

            // Add an edge connecting the two vertices in the graph.
            graph.add_edge(vertex_map[src], vertex_map[tgt]);
            std::cout << "Added edge connecting source: " << src << " and target: " << tgt << std::endl;
        }
    }

    for (auto he = iss_->halfedges_begin(); he != iss_->halfedges_end(); ++he) {  // Inner skeleton
        std::cout << "Processing halfedge: " << &(*he) << std::endl;
        // Use a simple trick to avoid duplicates:
        if (he < he->opposite()) {
            Point src = he->vertex()->point();
            Point tgt = he->opposite()->vertex()->point();
            std::cout << "Halfedge passes duplicate check. Source: " << src << ", Target: " << tgt << std::endl;

            // Add source vertex if not already added
            if (vertex_map.find(src) == vertex_map.end()) {
                SurfaceMesh::Vertex_index v = graph.add_vertex();
                graph.point(v) = src;
                vertex_map[src] = v;
                std::cout << "Added source vertex: " << src << std::endl;
            } else {
                std::cout << "Source vertex already exists: " << src << std::endl;
            }

            // Add target vertex if not already added
            if (vertex_map.find(tgt) == vertex_map.end()) {
                SurfaceMesh::Vertex_index v = graph.add_vertex();
                graph.point(v) = tgt;
                vertex_map[tgt] = v;
                std::cout << "Added target vertex: " << tgt << std::endl;
            } else {
                std::cout << "Target vertex already exists: " << tgt << std::endl;
            }

            // Add an edge connecting the two vertices in the graph.
            graph.add_edge(vertex_map[src], vertex_map[tgt]);
            std::cout << "Added edge connecting source: " << src << " and target: " << tgt << std::endl;
        }
    }
}

std::vector<std::pair<Point, Point>> TestStraightSkeleton::getEdges() const {
    std::vector<std::pair<Point, Point>> edges;
    for (auto e : graph.edges()) {
        auto h = graph.halfedge(e);
        auto src = graph.point(graph.source(h));
        auto tgt = graph.point(graph.target(h));
        edges.emplace_back(src, tgt);
    }
    return edges;
}

size_t TestStraightSkeleton::faceCount() const {
    return faces.size();
}

const straight_skeleton::SkeletonFace& TestStraightSkeleton::face(size_t i) const {
    return faces[i];
}

std::vector<straight_skeleton::SkeletonFace> TestStraightSkeleton::skeletonToFaces(
    SsPtr skeleton, const std::set<std::pair<Point, Point>, std::less<>>& polygonBoundaryEdges) const {
    std::vector<straight_skeleton::SkeletonFace> faces;
    faces.reserve(this->originalPolygonPoints.size());

    std::map<Ss::Face_handle, int> faceIndexMap;

    // iterate through all faces of the skeleton
    int counter = 0;
    for (auto face = skeleton->faces_begin(); face != skeleton->faces_end(); face++) {
        faceIndexMap.emplace(face, counter);
        counter++;
    }
    std::cout << "Assigned " << counter << " face indices." << std::endl;
    std::cout << "Map Size: " << faceIndexMap.size() << std::endl;

    // for each face, collect its vertices and adjacent face indices
    // for border edges (opposite face == nullptr) that lie on the polygon boundary,
    // push -2 to mark that we want to later update it
    std::vector<std::pair<int, straight_skeleton::SkeletonFace>> indexedFaces;

    // get the adjacent faces of the face for every face in the map
    for (const auto& facePair : faceIndexMap) {
        Ss::Face_handle face = facePair.first;
        int currentFaceIndex = facePair.second;
        Ss::Halfedge_handle start = face->halfedge();
        Ss::Halfedge_handle he = start;

        std::vector<Point> points;
        std::vector<int> adjacentFaces;

        std::cout << "Face with Index: " << currentFaceIndex << std::endl;

        do {
            Point startPoint = he->prev()->vertex()->point();
            Point endPoint = he->vertex()->point();
            // Compute the normalized edge key.
            auto edgeKey = make_normalized_edge(startPoint, endPoint);

            Ss::Face_handle oppFace = he->opposite()->face();
            if (oppFace == nullptr) {
                // This is a border halfedge.
                // If the edge is part of the original polygon boundary then mark it as pending (-2)
                if (polygonBoundaryEdges.find(edgeKey) != polygonBoundaryEdges.end()) {
                    adjacentFaces.push_back(-2);  // pending update (should be replaced by matching face index)
                } else {
                    adjacentFaces.push_back(-1);  // truly outer
                }
                points.push_back(startPoint);
            } else if (oppFace == face) {
                // Should not happen; skip if the opposite face is the same
                he = he->next();
                continue;
            } else {
                // Regular interior edge.
                int oppIndex = faceIndexMap.at(oppFace);
                adjacentFaces.push_back(oppIndex);
                points.push_back(startPoint);
            }
            he = he->next();
        } while (he != start);

        std::vector<straight_skeleton::Point> convertedPoints;
        convertedPoints.reserve(points.size());
        for (const Point& p : points) {
            convertedPoints.emplace_back(convertPoint(p));
        }
        straight_skeleton::SkeletonFace sFace(convertedPoints, adjacentFaces);
        indexedFaces.emplace_back(currentFaceIndex, sFace);
    }

    std::sort(indexedFaces.begin(), indexedFaces.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

    for (const auto& pair : indexedFaces) {
        faces.push_back(pair.second);
    }

    std::cout << std::endl << "FACES: " << std::endl;
    for (const straight_skeleton::SkeletonFace& face : faces) {
        std::cout << face << std::endl;
    }

    return faces;
}

std::set<std::pair<Point, Point>, std::less<>> TestStraightSkeleton::computePolygonBoundaryEdges() const {
    std::set<std::pair<Point, Point>, std::less<>> boundaryEdges;
    size_t n = originalPolygonPoints.size();
    for (size_t i = 0; i < n; i++) {
        const Point& a = originalPolygonPoints[i];
        const Point& b = originalPolygonPoints[(i + 1) % n];
        boundaryEdges.insert(make_normalized_edge(a, b));
    }
    return boundaryEdges;
}

void TestStraightSkeleton::matchPolygonEdges(
    std::vector<straight_skeleton::SkeletonFace>& faces,
    const std::set<std::pair<Point, Point>, std::less<>>& polygonBoundaryEdges) const {
    // Map from normalized edge to vector of (faceIndex, localEdgeIndex)
    std::map<std::pair<Point, Point>, std::vector<std::pair<int, int>>> edgeMatches;

    // Iterate over all faces.
    for (size_t i = 0; i < faces.size(); i++) {
        const straight_skeleton::SkeletonFace& face = faces[i];
        const std::vector<straight_skeleton::Point>& pts = face.vertices;
        size_t m = pts.size();
        for (size_t j = 0; j < m; j++) {
            // If the adjacent face marker is pending (-2)
            if (face.adjacentFaces[j] == -2) {
                // Get the edge from pts[j] to pts[(j+1)%m]
                // (Assume you can convert back to Point if needed; if not, store raw Points too)
                // For this example we assume straight_skeleton::Point can be compared with Point.
                // Otherwise, you might want to store the original Points as well.
                Point p1(CGAL::to_double(pts[j].x()), CGAL::to_double(pts[j].y()));
                Point p2(CGAL::to_double(pts[(j + 1) % m].x()), CGAL::to_double(pts[(j + 1) % m].y()));
                auto key = make_normalized_edge(p1, p2);
                // Only process if this edge is actually on the polygon boundary.
                if (polygonBoundaryEdges.find(key) != polygonBoundaryEdges.end()) {
                    edgeMatches[key].push_back({static_cast<int>(i), static_cast<int>(j)});
                }
            }
        }
    }

    // Now go through each edge in edgeMatches.
    for (const auto& entry : edgeMatches) {
        const auto& occurrences = entry.second;
        if (occurrences.size() == 2) {
            // Exactly two faces share this polygon boundary edge.
            int f1 = occurrences[0].first;
            int e1 = occurrences[0].second;
            int f2 = occurrences[1].first;
            int e2 = occurrences[1].second;
            faces[f1].adjacentFaces[e1] = f2;
            faces[f2].adjacentFaces[e2] = f1;
        } else {
            // If only one face has this boundary edge, leave it as outer (-1)
            int f1 = occurrences[0].first;
            int e1 = occurrences[0].second;
            faces[f1].adjacentFaces[e1] = -1;
        }
    }
}

bool TestStraightSkeleton::isOuterFace(const Ss::Face_handle& face) const {
    Ss::Halfedge_handle halfedgeIterator = face->halfedge();
    Ss::Halfedge_handle start = halfedgeIterator;

    do {
        if (halfedgeIterator->is_border()) {
            return true;
        }
        halfedgeIterator = halfedgeIterator->next();
    } while (halfedgeIterator != start);
    return false;
}

straight_skeleton::Point TestStraightSkeleton::convertPoint(const Point& point) const {
    return straight_skeleton::Point(point.x(), point.y());
}

}  // namespace TestSkeleton