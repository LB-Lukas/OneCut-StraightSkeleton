#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <memory>
#include "../include/StraightSkeleton/straightSkeleton.h"
#include "../include/StraightSkeleton/folding.h"

namespace py = pybind11;

PYBIND11_MODULE(geometry, m) {
    py::class_<Geometry::StraightSkeleton, std::shared_ptr<Geometry::StraightSkeleton>>(m, "StraightSkeleton")
        .def(py::init<const std::vector<std::shared_ptr<Point>>&>(), py::arg("vertices"));


    py::enum_<Geometry::FoldType>(m, "FoldType")
        .value("Convex", Geometry::FoldType::Convex)
        .value("Reflex", Geometry::FoldType::Reflex)
        .export_values();

    // Expose the Folding class to Python
    py::class_<Geometry::Folding>(m, "Folding")
        .def(py::init<>())  // Expose the default constructor
        // Add additional methods or members to expose if necessary
        .def("getVertices", Geometry::Folding::getVertices)
        .def("getMountains", Geometry::Folding::getMountains)
        .def("getValleys", Geometry::Folding::getValleys);

    // Expose the Point_2 type to Python
    py::class_<Point>(m, "Point")
        .def(py::init<double, double>())
        .def("x", &Point::x)
        .def("y", &Point::y);
}
