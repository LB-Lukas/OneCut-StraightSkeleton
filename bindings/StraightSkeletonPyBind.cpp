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

/**
 * @defgroup pythonBindings Python Bindings
 * @brief Python interface for OneCut origami computation library
 * 
 * This module provides Python bindings for the core OneCut functionality using pybind11.
 * It exposes geometric types, skeleton builders, and fold management components.
 * It is best to use FoldManager as an interface to the origami library.
 */

/**
 * @brief Python module initialization for OneCut
 * @ingroup pythonBindings
 */
PYBIND11_MODULE(one_cut, m) {
    // Expose the Point_2 type to Python
    /**
     * @class ExactKernelPoint
     * @brief Python-exposed CGAL exact point type with double coordinate access
     * @ingroup pythonBindings
     */
    py::class_<Point>(m, "ExactKernelPoint")
        .def(py::init<double, double>())
        .def("x", [](const Point& p) { return CGAL::to_double(p.x()); }, 
             "Get X coordinate converted to double")
        .def("y", [](const Point& p) { return CGAL::to_double(p.y()); }, 
             "Get Y coordinate converted to double");

    /**
     * @class Point
     * @brief Python-exposed point type for test skeletons
     * @ingroup pythonBindings
     */
    py::class_<SkeletonConstruction::Point>(m, "Point")
        .def(py::init<double, double>())
        .def("x", [](const SkeletonConstruction::Point& p) { return CGAL::to_double(p.x()); }, 
             "Get X coordinate converted to double")
        .def("y", [](const SkeletonConstruction::Point& p) { return CGAL::to_double(p.y()); }, 
             "Get Y coordinate converted to double");

    /**
     * @class SkeletonBuilder
     * @brief Python interface for building straight skeletons
     * @ingroup pythonBindings
     */
    py::class_<SkeletonConstruction::SkeletonBuilder>(m, "SkeletonBuilder")
        .def(py::init<const std::vector<SkeletonConstruction::Point>&>(), 
             py::arg("vertices"), 
             "Construct from polygon vertices");

    /**
     * @class PerpendicularFinder
     * @brief Python interface for finding perpendicular folds
     * @ingroup pythonBindings
     */
    py::class_<OneCut::PerpendicularFinder>(m, "PerpendicularFinder")
        .def(py::init<OneCut::StraightSkeleton&>(), 
             py::arg("skeleton"), 
             "Construct with computed straight skeleton")
        .def("find_perpendiculars", [](OneCut::PerpendicularFinder& pf) {
            std::vector<std::pair<OneCut::Point, OneCut::Point>> edges;
            std::vector<PerpChain> chains = pf.findPerpendiculars();
            for (const auto& chain : chains) {
                for (const auto& seg : chain) {
                    edges.emplace_back(seg.start, seg.end);
                }
            }
            return edges;
        }, "Find all perpendicular fold chains as flattened edge list");

    /**
     * @enum Origin
     * @brief Origin classification for creases
     * @ingroup pythonBindings
     */
    py::enum_<OneCut::Origin>(m, "Origin")
        .value("POLYGON", OneCut::Origin::POLYGON, "Original polygon boundary")
        .value("SKELETON", OneCut::Origin::SKELETON, "Straight skeleton edge")
        .value("PERPENDICULAR", OneCut::Origin::PERPENDICULAR, "Perpendicular fold")
        .export_values();

    /**
     * @enum FoldType
     * @brief Classification of fold directions
     * @ingroup pythonBindings
     */
    py::enum_<OneCut::FoldType>(m, "FoldType")
        .value("MOUNTAIN", OneCut::FoldType::MOUNTAIN, "Mountain fold")
        .value("VALLEY", OneCut::FoldType::VALLEY, "Valley fold")
        .value("UNFOLDED", OneCut::FoldType::UNFOLDED, "Unfolded crease")
        .export_values();

    /**
     * @class Crease
     * @brief Python interface for fold crease information
     * @ingroup pythonBindings
     */
    py::class_<OneCut::Crease>(m, "Crease")
        .def(py::init<>())
        .def_readonly("edge", &OneCut::Crease::edge, "Edge endpoints")
        .def_readonly("foldType", &OneCut::Crease::foldType, "Fold direction type")
        .def_readonly("origin", &OneCut::Crease::origin, "Crease origin classification")
        .def_readonly("faceIndex", &OneCut::Crease::faceIndex, "Associated face index")
        .def_readonly("edgeIndex", &OneCut::Crease::edgeIndex, "Edge index in face")
        .def_readonly("isBoundaryEdge", &OneCut::Crease::isBoundaryEdge, 
                     "True if polygon boundary edge");

    /**
     * @class FoldManager
     * @brief Main entry point for Python fold computation
     * @ingroup pythonBindings
     */
    py::class_<OneCut::FoldManager>(m, "FoldManager")
        .def(py::init<const std::vector<SkeletonConstruction::Point>&>(), 
             py::arg("vertices"), 
             "Initialize with polygon vertices")
        .def("get_creases", &OneCut::FoldManager::getCreases, 
             "Retrieve all computed creases");
}

}  // namespace OneCut