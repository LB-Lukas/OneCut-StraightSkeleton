#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <vector>

namespace StraightSkeleton {
    using K      = CGAL::Exact_predicates_exact_constructions_kernel;
    using Point2 = K::Point_2;

    /// Simple structure to hold polygon data or utility methods
    struct PolygonData {
        std::vector<Point2> points;

        // convenience: check orientation, etc.
        bool isConvex() const;
        bool isSimple() const;
    };
}
