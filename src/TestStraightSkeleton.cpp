#include "straight_skeleton/TestStraightSkeleton.h"

namespace TestSkeleton {

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
    oss_ = CGAL::create_exterior_straight_skeleton_2(600, polygon.vertices_begin(), polygon.vertices_end());
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


    // TODO Change this logic
    faces = skeletonToFaces(iss_);


    // Map to store graph vertices corresponding to each skeleton point.
    std::map<Point, SurfaceMesh::Vertex_index> vertex_map;

    // Iterate over the skeleton’s halfedges.
    // (Each edge appears twice, so process one representative from each pair.)
    for (auto he = oss_->halfedges_begin(); he != oss_->halfedges_end(); ++he) {
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

        for (auto he = iss_->halfedges_begin(); he != iss_->halfedges_end(); ++he) {
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

std::vector<straight_skeleton::SkeletonFace> TestStraightSkeleton::skeletonToFaces(SsPtr skeleton) const {
    std::vector<straight_skeleton::SkeletonFace> faces;
    faces.reserve(this->originalPolygonPoints.size());

    std::map<Ss::Face_handle, int> faceIndexMap;
    std::vector<std::pair<int, straight_skeleton::SkeletonFace>> indexedFaces;

    // iterate through all faces of the skeleton
    int counter = 0;
    for (auto face = skeleton->faces_begin(); face != skeleton->faces_end(); face++) {
        // TODO: check if the face is an outer face
        if (false) {
            faceIndexMap.emplace(face, -1);
        } else {
            faceIndexMap.emplace(face, counter);
            counter++;
        }
    }
    std::cout << "Counter: " << counter << std::endl;
    std::cout << "Map Size: " << faceIndexMap.size() << std::endl;

    // get the adjacent faces of the face for every face in the map
    for (const auto& faceIndexPair : faceIndexMap) {
        std::cout << "Face with Index: " << faceIndexPair.second << std::endl;
        // 1. get the face
        Ss::Face_handle face = faceIndexPair.first;

        // 2. go through the face halfedges in a circle
        Ss::Halfedge_handle start = face->halfedge();
        Ss::Halfedge_handle halfedgeIterator = start;

        std::vector<Point> points;
        std::vector<int> adjacentFaces;

        do {
            // 3. for each halfedge get first point and second point and get the opposite face
            Point startPoint = halfedgeIterator->prev()->vertex()->point();
            Point endPoint = halfedgeIterator->vertex()->point();
            std::cout << "startPoint: " << startPoint << std::endl;
            std::cout << "endPoint " << endPoint << std::endl;

            Ss::Face_handle oppositeFace = halfedgeIterator->opposite()->face();
            if (oppositeFace == nullptr) {
                // Opposite face is null <=> the halfedge is a border edge
                std::cout << std::endl << "SKIP: oppositeFace == nullptr" << std::endl;
                points.push_back(startPoint);
                adjacentFaces.push_back(-1);
                halfedgeIterator = halfedgeIterator->next();
                continue;
            } else if (oppositeFace == face) {
                std::cout << std::endl << "SKIP: oppositeFace == face" << std::endl;
                halfedgeIterator = halfedgeIterator->next();
                continue;
            } else {
                std::cout << std::endl << "NO SKIP: default face" << std::endl;
                // 4. use the opposite face to lookup the face index in the map
                int oppositeFaceIndex = faceIndexMap[oppositeFace];
                // 5. insert the points of the face in a vector
                points.push_back(startPoint);
                // 6. insert the index of the adjacent face in a vector (for points (0,1) the element 0 in the vector
                // should be the face that is adjacent regarding that edge)
                adjacentFaces.push_back(oppositeFaceIndex);
                halfedgeIterator = halfedgeIterator->next();
            }
        } while (halfedgeIterator != start);

        std::vector<straight_skeleton::Point> epeckPoints;
        epeckPoints.reserve(points.size());
        for (const Point& point : points) {
            epeckPoints.emplace_back(convertPoint(point));
        }

        // faces.emplace_back(epeckPoints, adjacentFaces);
        straight_skeleton::SkeletonFace sFace(epeckPoints, adjacentFaces);
        indexedFaces.emplace_back(faceIndexPair.second, sFace);
        // 3. for each halfedge get first point and second point and get the opposite face
        // 4. use the opposite face to lookup the face index in the map
        // 5. insert the points of the face in a vector
        // 6. insert the index of the adjacent face in a vector (for points (0,1) the element 0 in the vector should be
        // the face that is adjacent regarding that edge)
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