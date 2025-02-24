#include <cairo/cairo-pdf.h>
#include <cairo/cairo.h>
#include <gtest/gtest.h>

#include <vector>

#include "straight_skeleton/PerpendicularFinder.h"
#include "straight_skeleton/PlaceholderSkeleton.h"
#include "straight_skeleton/StraightSkeletonTypes.h"

namespace straight_skeleton {

class PerpendicularTest : public ::testing::Test {
protected:
    std::vector<SkeletonFace> recFaces;
    std::vector<SkeletonFace> triFaces;
    // Cairo PDF surface, managed via unique_ptr with custom deleter
    std::unique_ptr<cairo_surface_t, decltype(&cairo_surface_destroy)> surface{
        nullptr, &cairo_surface_destroy
    };

    void SetUp() override {

    // Setup Skeleton for rectangle    
    // Define Points for a larger, centered rectangle
    Point2D p0(0, 200);
    Point2D p1(400, 200);
    Point2D p2(400, 0);
    Point2D p3(0, 0);
    Point2D s0(100, 100);
    Point2D s1(300, 100);

    // Define Skeleton Faces
    std::vector<Point2D> f0 = {p0, p1, s1, s0};
    std::vector<Point2D> f1 = {p1, p2, s1};
    std::vector<Point2D> f2 = {p2, p3, s0, s1};
    std::vector<Point2D> f3 = {p3, p0, s0};

    // Define Adjacent Faces
    SkeletonFace face0(f0, {-1, 1, 2, 3});
    SkeletonFace face1(f1, {-1, 2, 0});
    SkeletonFace face2(f2, {-1, 3, 0, 1});
    SkeletonFace face3(f3, {-1, 0, 2});

    recFaces = {face0, face1, face2, face3};


    //Setup Skeleton for triangle
    Point2D a(0, 0);
    Point2D b(200, 0);
    Point2D c(100, sqrt(3) * 100);
    Point2D ts(100, sqrt(3)/3 *100);

    std::vector<Point2D> tf0 = {a, b, ts};
    std::vector<Point2D> tf1 = {b, c, ts};
    std::vector<Point2D> tf2 = {c, a, ts};

    SkeletonFace tface0(tf0, {-1, 1, 2});
    SkeletonFace tface1(tf1, {-1, 2, 0});
    SkeletonFace tface2(tf2, {-1, 0, 1});

    triFaces = {tface0, tface1, tface2};
}


    void TearDown() override {
    }

    void draw_polygon(const std::vector<Point2D>& points, cairo_t* cr, double r, double g, double b) {
        if (points.size() < 2) return;

        cairo_set_source_rgb(cr, r, g, b);
        cairo_set_line_width(cr, 1.0);

        for (size_t i = 0; i < points.size(); ++i) {
            auto source = points[i];
            auto target = points[(i + 1) % points.size()];

            cairo_move_to(cr, CGAL::to_double(source.x()), CGAL::to_double(source.y()));
            cairo_line_to(cr, CGAL::to_double(target.x()), CGAL::to_double(target.y()));
        }

        cairo_stroke(cr);
    }

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
};

TEST_F(PerpendicularTest, PerpendicularFinderRectangle) {
    const std::string rectangle_filename = "/home/lukas/Documents/workspace/OneCut-StraightSkeleton/test-output/PerpendicularTest_Rectangle.pdf";
    std::unique_ptr<cairo_surface_t, decltype(&cairo_surface_destroy)> surface{
        cairo_pdf_surface_create(rectangle_filename.c_str(), 500, 500), &cairo_surface_destroy
    };
    std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cr{
        cairo_create(surface.get()), &cairo_destroy
    };

    PlaceholderSkeleton skeleton(recFaces);
    PerpendicularFinder finder(skeleton);
    std::vector<PerpChain> chains = finder.findPerpendiculars();

    ASSERT_FALSE(chains.empty());
    EXPECT_EQ(chains.size(), 6);

    // Draw original polygons
    for (const auto& face : recFaces) {
        draw_polygon(face.getVertices(), cr.get(), 0.0, 0.0, 1.0); // Blue for polygons
    }

    // Draw perpendiculars
    draw_perpendiculars(chains, cr.get(), 1.0, 0.0, 0.0); // Red for perpendiculars

    std::cout << "Rectangle PDF written to: " << rectangle_filename << std::endl;
}

TEST_F(PerpendicularTest, PerpendicularFinderTriangle) {
    const std::string triangle_filename = "/home/lukas/Documents/workspace/OneCut-StraightSkeleton/test-output/PerpendicularTest_Triangle.pdf";
    std::unique_ptr<cairo_surface_t, decltype(&cairo_surface_destroy)> surface{
        cairo_pdf_surface_create(triangle_filename.c_str(), 500, 500), &cairo_surface_destroy
    };
    std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cr{
        cairo_create(surface.get()), &cairo_destroy
    };

    PlaceholderSkeleton skeleton(triFaces);
    PerpendicularFinder finder(skeleton);
    std::vector<PerpChain> chains = finder.findPerpendiculars();

    ASSERT_FALSE(chains.empty());
    EXPECT_EQ(chains.size(), 3);

    // Draw original polygons
    for (const auto& face : triFaces) {
        draw_polygon(face.getVertices(), cr.get(), 0.0, 0.0, 1.0); // Blue for polygons
    }

    // Draw perpendiculars
    draw_perpendiculars(chains, cr.get(), 1.0, 0.0, 0.0); // Red for perpendiculars

    std::cout << "Triangle PDF written to: " << triangle_filename << std::endl;
}


}  // namespace straight_skeleton
