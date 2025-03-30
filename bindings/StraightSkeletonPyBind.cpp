#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Line_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Surface_mesh.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <memory>

#include "../include/OneCut/Crease.h"
#include "../include/OneCut/FoldManager.h"
#include "../include/OneCut/PerpendicularFinder.h"
#include "../include/OneCut/SkeletonBuilder.h"
#include "../include/OneCut/StraightSkeleton.h"
#include "../include/OneCut/StraightSkeletonTypes.h"

namespace py = pybind11;

namespace OneCut {

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

    py::class_<OneCut::PerpendicularFinder>(m, "PerpendicularFinder")
        .def(py::init<OneCut::StraightSkeleton&>(), py::arg("skeleton"))
        .def("find_perpendiculars", [](OneCut::PerpendicularFinder& pf) {
            std::vector<std::pair<OneCut::Point, OneCut::Point>> edges;
            std::vector<PerpChain> chains = pf.findPerpendiculars();
            for (const auto& chain : chains) {
                for (const auto& seg : chain) {
                    edges.emplace_back(seg.start, seg.end);
                }
            }
            return edges;
        });

    py::enum_<OneCut::Origin>(m, "Origin")
        .value("POLYGON", OneCut::Origin::POLYGON)
        .value("SKELETON", OneCut::Origin::SKELETON)
        .value("PERPENDICULAR", OneCut::Origin::PERPENDICULAR)
        .export_values();

    py::enum_<OneCut::FoldType>(m, "FoldType")
        .value("MOUNTAIN", OneCut::FoldType::MOUNTAIN)
        .value("VALLEY", OneCut::FoldType::VALLEY)
        .value("UNFOLDED", OneCut::FoldType::UNFOLDED)
        .export_values();

    py::class_<OneCut::Crease>(m, "Crease")
        .def(py::init<>())
        .def_readonly("edge", &OneCut::Crease::edge)
        .def_readonly("foldType", &OneCut::Crease::foldType)
        .def_readonly("origin", &OneCut::Crease::origin)
        .def_readonly("faceIndex", &OneCut::Crease::faceIndex)
        .def_readonly("edgeIndex", &OneCut::Crease::edgeIndex)
        .def_readonly("isBoundaryEdge", &OneCut::Crease::isBoundaryEdge);

    py::class_<OneCut::FoldManager>(m, "FoldManager")
        .def(py::init<const std::vector<TestSkeleton::Point>&>(), py::arg("vertices"))
        .def("get_creases", &OneCut::FoldManager::getCreases);
}

}  // namespace OneCut