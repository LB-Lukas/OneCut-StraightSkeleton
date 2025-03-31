#include "OneCut/SkeletonBuilder.h"

namespace SkeletonConstruction {

struct PointComparator {
    bool operator()(const Point& a, const Point& b) const {
        if (a.x() != b.x()) return a.x() < b.x();
        return a.y() < b.y();
    }
};

const double TOLERANCE = 1e-6;

Point normalized_point(const Point& p, double tol = TOLERANCE) {
    double new_x = std::round(p.x() / tol) * tol;
    double new_y = std::round(p.y() / tol) * tol;
    return Point(new_x, new_y);
}

// A helper to form a normalized edge key from two points.
std::pair<Point, Point> make_normalized_edge(const Point& a, const Point& b, double tol = TOLERANCE) {
    Point norm_a = normalized_point(a, tol);
    Point norm_b = normalized_point(b, tol);
    if (PointComparator()(norm_a, norm_b))
        return {norm_a, norm_b};
    else
        return {norm_b, norm_a};
}

SkeletonBuilder::SkeletonBuilder(const std::vector<Point>& polygon_points) : originalPolygonPoints(polygon_points) {
    // Construct the polygon from the input points
    Polygon_2 polygon;
    for (const auto& p : polygon_points) {
        polygon.push_back(p);
    }

    if (polygon.is_clockwise_oriented()) {
        polygon.reverse_orientation();
    }

    // Compute the straight skeleton
    iss_ = CGAL::create_interior_straight_skeleton_2(polygon.vertices_begin(), polygon.vertices_end());
    oss_ = CGAL::create_exterior_straight_skeleton_2(1000, polygon.vertices_begin(), polygon.vertices_end());

    if (!iss_) {
        std::cerr << "Failed to create straight skeleton" << std::endl;
        return;
    }
    CGAL::Straight_skeletons_2::IO::print_straight_skeleton(*iss_);

    if (!iss_) {
        std::cerr << "Failed to create exterior skeleton" << std::endl;
        return;
    }
    CGAL::Straight_skeletons_2::IO::print_straight_skeleton(*oss_);

    facesInner = innerSkeletonToFaces(iss_, 0);
    facesOuter = outerSkeletonToFaces(oss_, facesInner.size());

    // Combine inner and outer faces into a single vector
    faces.reserve(facesInner.size() + facesOuter.size());
    faces.insert(faces.end(), facesInner.begin(), facesInner.end());
    faces.insert(faces.end(), facesOuter.begin(), facesOuter.end());
}

OneCut::StraightSkeleton SkeletonBuilder::buildSkeleton() {
    return OneCut::StraightSkeleton(faces);
}

std::vector<OneCut::SkeletonFace> SkeletonBuilder::innerSkeletonToFaces(SsPtr skeleton, int offset) {
    std::vector<OneCut::SkeletonFace> faces;
    faces.reserve(this->originalPolygonPoints.size());

    std::map<Ss::Face_handle, int> faceIndexMap;
    std::vector<std::pair<int, OneCut::SkeletonFace>> indexedFaces;

    // iterate through all faces of the skeleton
    int counter = 0;
    for (auto face = skeleton->faces_begin(); face != skeleton->faces_end(); face++) {
        faceIndexMap.emplace(face, counter);
        counter++;
    }

    // get the adjacent faces of the face for every face in the map
    for (const auto& faceIndexPair : faceIndexMap) {
        Ss::Face_handle face = faceIndexPair.first;

        // go through the face halfedges in a circle
        Ss::Halfedge_handle start = face->halfedge();
        Ss::Halfedge_handle halfedgeIterator = start;

        std::vector<Point> points;
        std::vector<int> adjacentFaces;

        do {
            // for each halfedge get first point and second point and get the opposite face
            Point startPoint = halfedgeIterator->prev()->vertex()->point();
            Point endPoint = halfedgeIterator->vertex()->point();

            Ss::Face_handle oppositeFace = halfedgeIterator->opposite()->face();
            if (oppositeFace == nullptr) {
                // Opposite face is null <=> the halfedge is a border edge
                // map the edge of the polygon to this face
                polyEdgeToFaceIndexMap.emplace(make_normalized_edge(startPoint, endPoint), faceIndexPair.second);

                points.push_back(startPoint);
                adjacentFaces.push_back(-1);
                halfedgeIterator = halfedgeIterator->next();
                continue;
            } else if (oppositeFace == face) {
                halfedgeIterator = halfedgeIterator->next();
                continue;
            } else {
                // use the opposite face to lookup the face index in the map
                int oppositeFaceIndex = faceIndexMap[oppositeFace];
                points.push_back(startPoint);
                adjacentFaces.push_back(oppositeFaceIndex);
                halfedgeIterator = halfedgeIterator->next();
            }
        } while (halfedgeIterator != start);

        std::vector<OneCut::Point> epeckPoints;
        epeckPoints.reserve(points.size());
        for (const Point& point : points) {
            epeckPoints.emplace_back(convertPoint(point));
        }

        OneCut::SkeletonFace sFace(epeckPoints, adjacentFaces);
        sFace.isOuter = false;
        indexedFaces.emplace_back(faceIndexPair.second, sFace);
    }
    std::sort(indexedFaces.begin(), indexedFaces.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

    for (const auto& pair : indexedFaces) {
        faces.push_back(pair.second);
    }

    std::cout << std::endl << "FACES: " << std::endl;
    for (const OneCut::SkeletonFace& face : faces) {
        std::cout << face << std::endl;
    }

    return faces;
}

std::vector<OneCut::SkeletonFace> SkeletonBuilder::outerSkeletonToFaces(SsPtr skeleton, int offset) {
    std::vector<OneCut::SkeletonFace> faces;
    faces.reserve(this->originalPolygonPoints.size());

    std::map<Ss::Face_handle, int> faceIndexMap;
    std::vector<std::pair<int, OneCut::SkeletonFace>> indexedFaces;

    // iterate through all faces of the skeleton
    int counter = 0 + offset;
    for (auto face = skeleton->faces_begin(); face != skeleton->faces_end(); face++) {
        faceIndexMap.emplace(face, counter);
        counter++;
    }
    std::cout << "Counter: " << counter << std::endl;
    std::cout << "Map Size: " << faceIndexMap.size() << std::endl;

    // get the adjacent faces of the face for every face in the map
    for (const auto& faceIndexPair : faceIndexMap) {
        std::cout << "Face with Index: " << faceIndexPair.second << std::endl;
        // get the face
        Ss::Face_handle face = faceIndexPair.first;

        // through the face halfedges in a circle
        Ss::Halfedge_handle start = face->halfedge();
        Ss::Halfedge_handle halfedgeIterator = start;

        std::vector<Point> points;
        std::vector<int> adjacentFaces;

        do {
            // for each halfedge get first point and second point and get the opposite face
            Point startPoint = halfedgeIterator->prev()->vertex()->point();
            Point endPoint = halfedgeIterator->vertex()->point();
            std::cout << "startPoint: " << startPoint << std::endl;
            std::cout << "endPoint " << endPoint << std::endl;

            Ss::Face_handle oppositeFace = halfedgeIterator->opposite()->face();
            if (oppositeFace == nullptr) {
                // Opposite face is null <=> the halfedge is a border edge
                // figure out if the edge is similar to the edge of the polygon
                auto it = polyEdgeToFaceIndexMap.find(make_normalized_edge(startPoint, endPoint));
                if (it != polyEdgeToFaceIndexMap.end()) {
                    // add the face index to the adjacent faces
                    adjacentFaces.push_back(it->second);

                    OneCut::SkeletonFace& innerFace = facesInner[it->second];
                    std::replace(innerFace.adjacentFaces.begin(), innerFace.adjacentFaces.end(), -1,
                                 faceIndexPair.second);

                } else {
                    std::cout << "Edge not found in map: " << startPoint << " " << endPoint << std::endl;
                    adjacentFaces.push_back(-1);
                }

                points.push_back(startPoint);
                halfedgeIterator = halfedgeIterator->next();
                continue;
            } else if (oppositeFace == face) {
                halfedgeIterator = halfedgeIterator->next();
                continue;
            } else {
                // use the opposite face to lookup the face index in the map
                int oppositeFaceIndex = faceIndexMap[oppositeFace];
                points.push_back(startPoint);
                adjacentFaces.push_back(oppositeFaceIndex);
                halfedgeIterator = halfedgeIterator->next();
            }
        } while (halfedgeIterator != start);

        std::vector<OneCut::Point> epeckPoints;
        epeckPoints.reserve(points.size());
        for (const Point& point : points) {
            epeckPoints.emplace_back(convertPoint(point));
        }

        OneCut::SkeletonFace sFace(epeckPoints, adjacentFaces);
        sFace.isOuter = true;
        indexedFaces.emplace_back(faceIndexPair.second, sFace);
    }
    std::sort(indexedFaces.begin(), indexedFaces.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

    for (const auto& pair : indexedFaces) {
        faces.push_back(pair.second);
    }

    std::cout << std::endl << "FACES: " << std::endl;
    for (const OneCut::SkeletonFace& face : faces) {
        std::cout << face << std::endl;
    }

    return faces;
}

OneCut::Point SkeletonBuilder::convertPoint(const Point& point) const {
    return OneCut::Point(point.x(), point.y());
}

}  // namespace SkeletonConstruction