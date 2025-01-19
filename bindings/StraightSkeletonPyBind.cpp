#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <memory>
#include "../include/straight_skeleton/StraightSkeleton.h"
#include "../include/straight_skeleton/Folding.h"
#include "../include/straight_skeleton/StraightSkeletonTypes.h"

namespace py = pybind11;

namespace straight_skeleton
{
    PYBIND11_MODULE(geometry, m)
    {
        // Expose FoldType enum to Python
        py::enum_<FoldType>(m, "FoldType")
            .value("Convex", FoldType::Convex)
            .value("Reflex", FoldType::Reflex)
            .export_values();

        // Expose the Folding class to Python
        py::class_<Folding>(m, "Folding")
            .def(py::init<>()) // Default constructor
            .def("getVertices", &Folding::getVertices)
            .def("getMountains", &Folding::getMountains)
            .def("getValleys", &Folding::getValleys)
            .def("getFolding", [](const std::vector<Point> &points)
                 {
                try {
                    return Folding::getFolding(points);
                } catch (const std::exception& e) {
                    PyErr_SetString(PyExc_RuntimeError, e.what());
                    throw py::error_already_set();
                } });

        // Expose the Point type to Python
        py::class_<Point>(m, "Point")
            .def(py::init<double, double>())
            .def("x", [](const Point &p)
                 { return CGAL::to_double(p.x()); })
            .def("y", [](const Point &p)
                 { return CGAL::to_double(p.y()); });
    }
}
