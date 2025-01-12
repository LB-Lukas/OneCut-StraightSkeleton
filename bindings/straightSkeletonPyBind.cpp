#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <memory>
#include "../include/StraightSkeleton/straightSkeleton.h"
#include "../include/StraightSkeleton/folding.h"
#include "../include/StraightSkeleton/perpendicularGraph.h"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Surface_mesh.h>

namespace py = pybind11;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Line_2 Line;
typedef CGAL::Surface_mesh<Point> PlanarGraph;

namespace geometry {

    PYBIND11_MODULE(geometry, m) {
        //py::class_<Geometry::StraightSkeleton, std::shared_ptr<Geometry::StraightSkeleton>>(m, "StraightSkeleton")
        //    .def(py::init<const std::vector<Point>&>(), py::arg("vertices"));


        py::enum_<Geometry::FoldType>(m, "FoldType")
            .value("Convex", Geometry::FoldType::Convex)
            .value("Reflex", Geometry::FoldType::Reflex)
            .export_values();

        // Expose the Folding class to Python
        py::class_<Geometry::Folding>(m, "Folding")
            .def(py::init<>())  // Expose the default constructor
            // Add additional methods or members to expose if necessary
            .def("getVertices", &Geometry::Folding::getVertices)
            .def("getMountains", &Geometry::Folding::getMountains)
            .def("getValleys", &Geometry::Folding::getValleys)
            .def("getFolding", Geometry::Folding::getFolding);

        // Expose the Point_2 type to Python
        py::class_<Point>(m, "Point")
            .def(py::init<double, double>())
            .def("x", [](const Point& p) { return CGAL::to_double(p.x()); })
            .def("y", [](const Point& p) { return CGAL::to_double(p.y()); });

         py::class_<PlanarGraph>(m, "PlanarGraph")
        .def(py::init<>())
        .def("add_vertex", [](PlanarGraph& graph, const Point& p) {
            auto vi = graph.add_vertex(p);
            // cast the vertex index to size_t to return as a Python integer
            return static_cast<size_t>(vi);
        })
        // accept a list of integer vertex indices for add_face
        .def("add_face", [](PlanarGraph& graph, size_t v0, size_t v1, size_t v2, size_t v3) {
        auto f = graph.add_face(PlanarGraph::Vertex_index(v0),
                                PlanarGraph::Vertex_index(v1),
                                PlanarGraph::Vertex_index(v2),
                                PlanarGraph::Vertex_index(v3));
        return static_cast<size_t>(f);  // Return an integer
        })
        .def("add_edge", [](PlanarGraph& graph, size_t v0, size_t v1) {
            auto e = graph.add_edge(PlanarGraph::Vertex_index(v0), PlanarGraph::Vertex_index(v1));
            return static_cast<size_t>(e);
        })
        .def("point", [](const PlanarGraph& graph, size_t v) {
            return graph.point(PlanarGraph::Vertex_index(v));
        })
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
        // Add parallel methods for add_edge, etc. if needed


    py::class_<Geometry::PerpendicularGraph>(m, "PerpendicularGraph")
        .def(py::init<>())
        .def(py::init<const PlanarGraph&, const PlanarGraph&>())
        .def("addPerpendiculars", &Geometry::PerpendicularGraph::addPerpendiculars)
        .def("getPerpendicularGraph", &Geometry::PerpendicularGraph::getPerpendicularGraph)
        .def("getCutGraph", &Geometry::PerpendicularGraph::getCutGraph)
        .def("getStraightSkeletonGraph", &Geometry::PerpendicularGraph::getStraightSkeletonGraph);
            
    }
}