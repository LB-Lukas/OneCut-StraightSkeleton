#include <CGAL/convex_hull_2.h>
#include <cairo/cairo-pdf.h>
#include <cairo/cairo.h>
#include <gtest/gtest.h>

#include <memory>
#include <random>
#include <vector>

#include "straight_skeleton/TestStraightSkeleton.h"
#include "straight_skeleton/PlaceholderSkeleton.h"
#include "straight_skeleton/PerpendicularFinder.h"

namespace straight_skeleton {

// Base fixture providing PDF drawing and utility functions.
class SkeletonTestBase : public ::testing::Test {
protected:
    std::unique_ptr<cairo_surface_t, decltype(&cairo_surface_destroy)> surface{nullptr, &cairo_surface_destroy};

    // Set up the PDF surface with a given filename and dimensions.
    void setSurface(const std::string& filename, int width = 500, int height = 500) {
        surface.reset(cairo_pdf_surface_create(filename.c_str(), width, height));
        auto cr = std::unique_ptr<cairo_t, decltype(&cairo_destroy)>(cairo_create(surface.get()), &cairo_destroy);
        cairo_set_source_rgb(cr.get(), 1.0, 1.0, 1.0);
        cairo_paint(cr.get());
    }

    // Draws a polygon given its points.
    void draw_polygon(const std::vector<TestSkeleton::Point2D>& points, double r, double g, double b) {
        auto cr = std::unique_ptr<cairo_t, decltype(&cairo_destroy)>(cairo_create(surface.get()), &cairo_destroy);
        cairo_set_source_rgb(cr.get(), r, g, b);
        cairo_set_line_width(cr.get(), 1.0);

        for (size_t i = 0; i < points.size(); ++i) {
            auto source = points[i];
            auto target = points[(i + 1) % points.size()];
            cairo_move_to(cr.get(), CGAL::to_double(source.x()), CGAL::to_double(source.y()));
            cairo_line_to(cr.get(), CGAL::to_double(target.x()), CGAL::to_double(target.y()));
        }
        cairo_stroke(cr.get());
    }

    // Draws the skeleton edges.
    void draw_skeleton(const std::vector<std::pair<TestSkeleton::Point2D, TestSkeleton::Point2D>>& edges, double r, double g, double b) {
        auto cr = std::unique_ptr<cairo_t, decltype(&cairo_destroy)>(cairo_create(surface.get()), &cairo_destroy);
        cairo_set_source_rgb(cr.get(), r, g, b);
        cairo_set_line_width(cr.get(), 1.0);
        for (const auto& edge : edges) {
            auto source = edge.first;
            auto target = edge.second;
            cairo_move_to(cr.get(), CGAL::to_double(source.x()), CGAL::to_double(source.y()));
            cairo_line_to(cr.get(), CGAL::to_double(target.x()), CGAL::to_double(target.y()));
        }
        cairo_stroke(cr.get());
    }

    // Draws the perpendicular chains.
    void draw_perpendiculars(const std::vector<PerpChain>& chains, cairo_t* cr, double r, double g, double b) {
        cairo_set_source_rgb(cr, r, g, b);
        cairo_set_line_width(cr, 1.0);
        for (const auto& chain : chains) {
            for (const auto& segment : chain) {
                auto source = segment.start;
                auto target = segment.end;
                cairo_move_to(cr, CGAL::to_double(source.x()), CGAL::to_double(source.y()));
                cairo_line_to(cr, CGAL::to_double(target.x()), CGAL::to_double(target.y()));
            }
        }
        cairo_stroke(cr);
    }

    // Utility function to generate a random convex polygon.
    std::vector<TestSkeleton::Point2D> generate_random_convex_polygon(int num_points) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(0.0, 500.0);

        std::vector<TestSkeleton::Point2D> random_points;
        random_points.reserve(num_points);
        for (int i = 0; i < num_points; ++i) {
            double x = dist(gen);
            double y = dist(gen);
            random_points.push_back(TestSkeleton::Point2D(x, y));
        }

        std::vector<TestSkeleton::Point2D> hull;
        hull.reserve(num_points);
        CGAL::convex_hull_2(random_points.begin(), random_points.end(), std::back_inserter(hull));
        return hull;
    }
};

//Fixture for tests with random convex polygons.
class RandomConvexSkeletonTest : public SkeletonTestBase {
protected:
    const std::string filename = "/home/lukas/Documents/workspace/OneCut-StraightSkeleton/test-output/BasicSkeletonPerpendicularTest_RandomConvex.pdf";
    std::vector<TestSkeleton::TestStraightSkeleton> straightSkeletons;

    void SetUp() override {
        setSurface(filename, 500, 500);
        const int num_polygons = 1;
        for (int i = 0; i < num_polygons; ++i) {
            auto polygon_points = generate_random_convex_polygon(5 + i);
            TestSkeleton::TestStraightSkeleton skeleton(polygon_points);
            PerpendicularFinder finder(skeleton);
            std::vector<PerpChain> chains = finder.findPerpendiculars();

            draw_polygon(polygon_points, 1.0, 0.0, 0.0);
            draw_skeleton(skeleton.getEdges(), 0.0, 1.0, 0.0);

            auto cr = cairo_create(surface.get());
            draw_perpendiculars(chains, cr, 0.0, 0.0, 1.0);
            cairo_destroy(cr);

            straightSkeletons.push_back(skeleton);
        }
    }

    void TearDown() override {
        surface.reset();
        std::cout << "PDF written to: " << filename << std::endl;
    }
};

TEST_F(RandomConvexSkeletonTest, RandomConvex) {
    EXPECT_FALSE(straightSkeletons.empty()) << "No straight skeletons were generated.";
}

// Fixture for tests using a rectangle.
class RectangleSkeletonTest : public SkeletonTestBase {
protected:
    const std::string rectangle_filename = "/home/lukas/Documents/workspace/OneCut-StraightSkeleton/test-output/BasicSkeletonPerpendicularTest_Rectangle.pdf";

    void SetUp() override {
        setSurface(rectangle_filename, 500, 500);
    }

    void TearDown() override {
        surface.reset();
        std::cout << "PDF written to: " << rectangle_filename << std::endl;
    }
};

TEST_F(RectangleSkeletonTest, Rectangle) {
    auto cr = std::unique_ptr<cairo_t, decltype(&cairo_destroy)>(cairo_create(surface.get()), &cairo_destroy);

    TestSkeleton::Point2D p0(0, 200);
    TestSkeleton::Point2D p1(400, 200);
    TestSkeleton::Point2D p2(400, 0);
    TestSkeleton::Point2D p3(0, 0);
    std::vector<TestSkeleton::Point2D> rectangle = {p0, p1, p2, p3};

    TestSkeleton::TestStraightSkeleton t_skeleton(rectangle);
    EXPECT_FALSE(t_skeleton.getEdges().empty()) << "No edges in the skeleton.";

    PerpendicularFinder finder(t_skeleton);
    std::vector<PerpChain> chains = finder.findPerpendiculars();
    EXPECT_FALSE(chains.empty()) << "No perpendiculars were found.";

    //draw_polygon(rectangle, 1.0, 0.0, 0.0);
    draw_skeleton(t_skeleton.getEdges(), 0.0, 1.0, 0.0);
    draw_perpendiculars(chains, cr.get(), 0.0, 0.0, 1.0);
}

} // namespace straight_skeleton
