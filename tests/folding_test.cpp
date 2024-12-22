#include <gtest/gtest.h>
#include "StraightSkeleton/folding.h"
#include "StraightSkeleton/straightSkeleton.h"

bool are_lines_equal(const Line& line1, const Line& line2) {
    const double TOLERANCE = 1e-10;

    // Check if lines are parallel: direction vectors should be proportional
    Vector dir1 = line1.direction().vector();
    Vector dir2 = line2.direction().vector();

    // Normalize the directions to ensure comparison is based on direction, not length
    dir1 *= 1.0 / std::sqrt(CGAL::to_double(dir1.squared_length())); // Normalize dir1
    dir2 *= 1.0 / std::sqrt(CGAL::to_double(dir2.squared_length())); // Normalize dir2

    // Check if the directions are the same (parallel lines)
    if (std::sqrt(CGAL::to_double((dir1 - dir2).squared_length())) < TOLERANCE
        && std::sqrt(CGAL::to_double((dir1 + dir2).squared_length())) < TOLERANCE) {
        return false; // Lines are not parallel
    }

    // Check if a point on line1 lies on line2 (or vice versa)
    Point point_on_line1 = line1.point(0);  // Any point on line1 (can use point(0) as it’s a point on the line)

    if (std::sqrt(CGAL::to_double(CGAL::squared_distance(point_on_line1, line2))) < TOLERANCE) {
        return true; // Lines are parallel and coincident
    }

    return false; // Lines are parallel but not coincident
}

void scalePoint(Point& point, const int& scale_factor) {
    Vector vector_from_origin = point - Point(0, 0);

    // Scale the vector by multiplying by the scale factor
    Vector scaled_vector = vector_from_origin * scale_factor;

    // Convert the scaled vector back to a point (relative to the origin)
    point = Point(0, 0) + scaled_vector;
}

TEST (FoldingTest, Test1) {
    Geometry::Folding folding;
    ASSERT_EQ(folding.getVertices().size(), 0);
}

void testIntersectionCornerCases(const Point& l_point, const Point& ll_point, const Point& r_point,
    const Point& rr_point, const Point& exp_point_l, Point& exp_point_r) {

    // Call the method
    Line result = Geometry::StraightSkeleton::computeRay(l_point, ll_point, r_point, rr_point);
    Line exp_line = Line(exp_point_l, exp_point_r);

    //ASSERT_EQ(result, exp_line);
    ASSERT_TRUE(are_lines_equal(result, exp_line));
}

// Test for computeRay
TEST(ComputeRayTest, IntersectionTest) {
    Point l_point = Point(0 , 0), ll_point = Point(0, 1), r_point = Point(0, 0), rr_point = Point(1, 0);
    Point exp_point_l = Point(0, 0), exp_point_2 = Point(1, 1);

    // test some cornerCases
    testIntersectionCornerCases(l_point, ll_point, r_point, rr_point, exp_point_l, exp_point_2);

    Vector x_vect = Vector(1, 0);
    Vector y_vect = Vector(0, 1);

    // translate points
    l_point += y_vect;
    ll_point += y_vect;
    r_point += x_vect;
    rr_point += x_vect;

    testIntersectionCornerCases(l_point, ll_point, r_point, rr_point, exp_point_l, exp_point_2);

    // scale points
    scalePoint(l_point, 2);
    scalePoint(ll_point, 2);
    scalePoint(r_point, 3);
    scalePoint(rr_point, 3);

    testIntersectionCornerCases(l_point, ll_point, r_point, rr_point, exp_point_l, exp_point_2);

}
