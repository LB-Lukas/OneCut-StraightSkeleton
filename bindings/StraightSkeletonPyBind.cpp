#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Line_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Surface_mesh.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <memory>

#include "../include/straight_skeleton/Crease.h"
#include "../include/straight_skeleton/FoldManager.h"
#include "../include/straight_skeleton/PerpendicularFinder.h"
#include "../include/straight_skeleton/SkeletonBuilder.h"
#include "../include/straight_skeleton/StraightSkeleton.h"
#include "../include/straight_skeleton/StraightSkeletonTypes.h"

namespace py = pybind11;

namespace straight_skeleton {

PYBIND11_MODULE(geometry, m) {
    // Expose the Point_2 type to Python
    py::class_<Point>(m, "Point")
        .def(py::init<double, double>())
        .def("x", [](const Point& p) { return CGAL::to_double(p.x()); })
        .def("y", [](const Point& p) { return CGAL::to_double(p.y()); });

    py::class_<TestSkeleton::Point>(m, "TestPoint")
        .def(py::init<double, double>())
        .def("x", [](const TestSkeleton::Point& p) { return CGAL::to_double(p.x()); })
        .def("y", [](const TestSkeleton::Point& p) { return CGAL::to_double(p.y()); });

    py::class_<PlanarGraph>(m, "PlanarGraph")
        .def(py::init<>())
        .def("add_vertex",
             [](PlanarGraph& graph, const Point& p) {
                 auto vi = graph.add_vertex(p);
                 // cast the vertex index to size_t to return as a Python integer
                 return static_cast<size_t>(vi);
             })
        // accept a list of integer vertex indices for add_face
        .def("add_face",
             [](PlanarGraph& graph, size_t v0, size_t v1, size_t v2, size_t v3) {
                 auto f = graph.add_face(PlanarGraph::Vertex_index(v0), PlanarGraph::Vertex_index(v1),
                                         PlanarGraph::Vertex_index(v2), PlanarGraph::Vertex_index(v3));
                 return static_cast<size_t>(f);  // Return an integer
             })
        .def("add_edge",
             [](PlanarGraph& graph, size_t v0, size_t v1) {
                 auto e = graph.add_edge(PlanarGraph::Vertex_index(v0), PlanarGraph::Vertex_index(v1));
                 return static_cast<size_t>(e);
             })
        .def("point", [](const PlanarGraph& graph, size_t v) { return graph.point(PlanarGraph::Vertex_index(v)); })
        .def("edges", [](const PlanarGraph& graph) {
            std::vector<std::pair<Point, Point>> edges;
            for (auto e : graph.edges()) {
                auto h = graph.halfedge(e);
                auto src = graph.point(graph.source(h));
                auto tgt = graph.point(graph.target(h));
                edges.emplace_back(src, tgt);
            }
            return edges;
        });

    py::class_<TestSkeleton::SkeletonBuilder>(m, "SkeletonBuilder")
        .def(py::init<const std::vector<TestSkeleton::Point>&>(), py::arg("vertices"));

    py::class_<straight_skeleton::PerpendicularFinder>(m, "PerpendicularFinder")
        .def(py::init<straight_skeleton::StraightSkeleton&>(), py::arg("skeleton"))
        .def("find_perpendiculars", [](straight_skeleton::PerpendicularFinder& pf) {
            std::vector<std::pair<straight_skeleton::Point, straight_skeleton::Point>> edges;
            std::vector<PerpChain> chains = pf.findPerpendiculars();
            for (const auto& chain : chains) {
                for (const auto& seg : chain) {
                    edges.emplace_back(seg.start, seg.end);
                }
            }
            return edges;
        });

    py::enum_<straight_skeleton::Origin>(m, "Origin")
        .value("POLYGON", straight_skeleton::Origin::POLYGON)
        .value("SKELETON", straight_skeleton::Origin::SKELETON)
        .value("PERPENDICULAR", straight_skeleton::Origin::PERPENDICULAR)
        .export_values();

    py::enum_<straight_skeleton::FoldType>(m, "FoldType")
        .value("MOUNTAIN", straight_skeleton::FoldType::MOUNTAIN)
        .value("VALLEY", straight_skeleton::FoldType::VALLEY)
        .value("UNFOLDED", straight_skeleton::FoldType::UNFOLDED)
        .export_values();

    py::class_<straight_skeleton::Crease>(m, "Crease")
        .def(py::init<>())
        .def_readonly("edge", &straight_skeleton::Crease::edge)
        .def_readonly("foldType", &straight_skeleton::Crease::foldType)
        .def_readonly("origin", &straight_skeleton::Crease::origin)
        .def_readonly("faceIndex", &straight_skeleton::Crease::faceIndex)
        .def_readonly("edgeIndex", &straight_skeleton::Crease::edgeIndex)
        .def_readonly("isBoundaryEdge", &straight_skeleton::Crease::isBoundaryEdge);

    py::class_<straight_skeleton::FoldManager>(m, "FoldManager")
        .def(py::init<const std::vector<TestSkeleton::Point>&>(), py::arg("vertices"))
        .def("get_creases", &straight_skeleton::FoldManager::getCreases);
}

}  // namespace straight_skeleton