#include <gtest/gtest.h>
#include "straight_skeleton/StraightSkeletonTypes.h"
#include "straight_skeleton/Folding.h"
#include "straight_skeleton/StraightSkeleton.h"

#include <CGAL/convex_hull_2.h>
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>

#include <random>
#include <memory>
#include <iostream>

namespace straight_skeleton {

class StraightSkeletonTest : public ::testing::Test {
protected:
    // Output PDF
    const std::string filename = "/home/lukas/Documents/workspace/OneCut-StraightSkeleton/test-output/StraightSkeletonTest_RandomConvex.pdf";

    // We store multiple skeletons in this vector
    std::vector<StraightSkeleton> straightSkeletons;

    // Cairo PDF surface, managed via unique_ptr with custom deleter
    std::unique_ptr<cairo_surface_t, decltype(&cairo_surface_destroy)> surface{
        nullptr, &cairo_surface_destroy
    };

    // -- GoogleTest Hooks --
    void SetUp() override {
        // Create the PDF surface once (500x500)
        surface.reset(cairo_pdf_surface_create(filename.c_str(), 500, 500));

        // Clear the background once
        std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cr{
            cairo_create(surface.get()), &cairo_destroy
        };
        cairo_set_source_rgb(cr.get(), 1.0, 1.0, 1.0);  // White background
        cairo_paint(cr.get());

        // Generate N random convex polygons
        const int num_polygons = 1;  // Adjust as needed
        for(int i = 0; i < num_polygons; ++i) {
            auto polygon_points = generate_random_convex_polygon(6 + i);
            // Example: polygons of size 6, 7, 8, ...

            // Build a StraightSkeleton from these points
            StraightSkeleton skeleton(polygon_points);

            // Draw the polygon in red
            draw_polygon(polygon_points, 1.0, 0.0, 0.0);

            // Draw the skeleton in green
            draw_skeleton(skeleton.graph, 0.0, 1.0, 0.0);

            // Store it for potential validation
            straightSkeletons.push_back(skeleton);
        }
    }

    void TearDown() override {
        // Surface is automatically destroyed by unique_ptr
        // Finish writing the PDF
        surface.reset();
        std::cout << "PDF written to: " << filename << std::endl;
    }

    // -- Helper Functions --
    /// Generate a random convex polygon within [0, 500] × [0, 500]
    /// `num_points` is the number of random points to place, from which we build a convex hull.
    std::vector<Point> generate_random_convex_polygon(int num_points) {
        // 1. Generate uniform random points
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(0.0, 500.0);

        std::vector<Point> random_points;
        random_points.reserve(num_points);
        for(int i = 0; i < num_points; ++i) {
            double x = dist(gen);
            double y = dist(gen);
            random_points.push_back(Point(x, y));
        }

        // 2. Compute the convex hull
        std::vector<Point> hull;
        hull.reserve(num_points);
        CGAL::convex_hull_2(random_points.begin(), random_points.end(), std::back_inserter(hull));

        // hull now holds the vertices of the convex polygon in CCW order
        return hull;
    }

    /// Draw a polygon with the given color on the PDF surface
    void draw_polygon(const std::vector<Point>& points, double r, double g, double b) {
        if (points.size() < 2) return;

        // Create a new cairo context for each draw call
        std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cr{
            cairo_create(surface.get()), &cairo_destroy
        };

        cairo_set_source_rgb(cr.get(), r, g, b);
        cairo_set_line_width(cr.get(), 1.0);

        // Draw edges
        for (size_t i = 0; i < points.size(); ++i) {
            auto source = points[i];
            auto target = points[(i + 1) % points.size()];

            cairo_move_to(cr.get(), CGAL::to_double(source.x()), CGAL::to_double(source.y()));
            cairo_line_to(cr.get(), CGAL::to_double(target.x()), CGAL::to_double(target.y()));
        }

        cairo_stroke(cr.get());
    }

    /// Draw a skeleton with the given color on the PDF surface
    void draw_skeleton(const PlanarGraph& graph, double r, double g, double b) {
        std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cr{
            cairo_create(surface.get()), &cairo_destroy
        };

        cairo_set_source_rgb(cr.get(), r, g, b);
        cairo_set_line_width(cr.get(), 1.0);

        for (auto edge : graph.halfedges()) {
            auto source = graph.point(graph.source(edge));
            auto target = graph.point(graph.target(edge));

            cairo_move_to(cr.get(), CGAL::to_double(source.x()), CGAL::to_double(source.y()));
            cairo_line_to(cr.get(), CGAL::to_double(target.x()), CGAL::to_double(target.y()));
        }

        cairo_stroke(cr.get());
    }
};

// -- Test Cases --
// We already do the random generation/drawing in SetUp(), so this test
// just verifies that we have a non-empty collection of skeletons.
TEST_F(StraightSkeletonTest, RandomConvexPolygons) {
    EXPECT_FALSE(straightSkeletons.empty())
        << "No straight skeletons were generated!";
    // Additional checks can be made here, e.g., verifying certain properties
    // of the skeleton, number of edges, etc.
}

} // namespace straight_skeleton
