#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <memory>
#include "../include/straight_skeleton/StraightSkeleton.h"
#include "../include/straight_skeleton/Folding.h"

namespace py = pybind11;

namespace straight_skeleton {
    PYBIND11_MODULE(geometry, m) {
        //py::class_<Geometry::straight_skeleton, std::shared_ptr<Geometry::straight_skeleton>>(m, "straight_skeleton")
        //    .def(py::init<const std::vector<Point>&>(), py::arg("vertices"));


        py::enum_<FoldType>(m, "FoldType")
                .value("Convex", FoldType::Convex)
                .value("Reflex", FoldType::Reflex)
                .export_values();

        // Expose the Folding class to Python
        py::class_<Folding>(m, "Folding")
                .def(py::init<>()) // Expose the default constructor
                // Add additional methods or members to expose if necessary
                .def("getVertices", &Folding::getVertices)
                .def("getMountains", &Folding::getMountains)
                .def("getValleys", &Folding::getValleys)
                .def("getFolding",
                     [](const std::vector<Point> &points) {
                         try {
                             return Folding::getFolding(points);
                         } catch (const std::exception &e) {
                             // Construct a detailed error message with traceback information
                             std::ostringstream oss;
                             oss << "C++ Exception in getFolding: " << e.what();
                             std::string msg = oss.str();
                             PyErr_SetString(PyExc_RuntimeError, msg.c_str());
                             throw py::error_already_set();
                         }
                     },
                     "Get the folding representation of a polygon.");


        // Expose the Point_2 type to Python
        py::class_<Point>(m, "Point")
                .def(py::init<double, double>())
                .def("x", [](const Point &p) { return CGAL::to_double(p.x()); })
                .def("y", [](const Point &p) { return CGAL::to_double(p.y()); });
    }
}
