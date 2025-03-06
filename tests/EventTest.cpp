#include <gtest/gtest.h>

#include "straight_skeleton/Event.h"
#include "straight_skeleton/Ray.h"
#include "straight_skeleton/Triangle.h"

namespace straight_skeleton {

TEST(EventTest, DefaultConstructor) {
    Event event;
    EXPECT_EQ(event.getType(), UndefinedEvent);
    EXPECT_EQ(event.getTime(), 0);
    EXPECT_EQ(event.getTriangle(), nullptr);
    EXPECT_EQ(event.getRay(), nullptr);
}

TEST(EventTest, ConstructorWithTriangle) {
    Triangle triangle(Point3D(1, 0, 0), Point3D(0, 1, 0), Point3D(0, 0, 1));
    Triangle clockwiseNeighbour(Point3D(0, 1, 0), Point3D(1, 2, 0), Point3D(0, 0, 1));
    Triangle counterclockwiseNeighbour(Point3D(1, 2, 0), Point3D(1, 0, 0), Point3D(1, 0, 0));
    triangle.setClockwiseNeighbour(std::make_shared<Triangle>(clockwiseNeighbour));
    triangle.setCounterclockwiseNeighbour(std::make_shared<Triangle>(counterclockwiseNeighbour));
    Event event(EdgeEvent, 1.0, std::make_shared<Triangle>(triangle), true);

    EXPECT_EQ(event.getType(), EdgeEvent);
    EXPECT_EQ(event.getTime(), 1.0);
    EXPECT_NE(event.getTriangle(), nullptr);
    EXPECT_EQ(event.getRay(), nullptr);
}

TEST(EventTest, ConstructorWithRay) {
    Ray ray(Point3D(1, 0, 0), Direction3D(0, 1, 0));
    Event event(SplitEvent, 2.0, std::make_shared<Ray>(ray), true);

    EXPECT_EQ(event.getType(), SplitEvent);
    EXPECT_EQ(event.getTime(), 2.0);
    EXPECT_EQ(event.getTriangle(), nullptr);
    EXPECT_NE(event.getRay(), nullptr);
}

TEST(EventTest, EqualityOperator) {
    Triangle triangle(Point3D(1, 0, 0), Point3D(0, 1, 0), Point3D(0, 0, 1));
    TrianglePtr trianglePtr = std::make_shared<Triangle>(triangle);
    TrianglePtr otherPointer = trianglePtr;
    Ray ray(Point3D(1, 0, 0), Direction3D(0, 1, 0));
    Event event1(EdgeEvent, 1.0, trianglePtr, true);
    Event event2(EdgeEvent, 1.0, trianglePtr, true);
    Event event3(EdgeEvent, 1.0, std::make_shared<Ray>(ray), true);
    Event event4(EdgeEvent, 1.0, otherPointer, true);

    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 == event3);
    EXPECT_FALSE(event2 == event3);
    EXPECT_FALSE(event1 == Event());
    EXPECT_FALSE(event2 != event1);
    EXPECT_TRUE(event1 == event4);
}

TEST(EventTest, LessThanOperator) {
    Triangle triangle(Point3D(1, 0, 0), Point3D(0, 1, 0), Point3D(0, 0, 1));
    Event event1(EdgeEvent, 1.0, std::make_shared<Triangle>(triangle), true);
    Event event2(EdgeEvent, 2.0, std::make_shared<Triangle>(triangle), true);

    EXPECT_TRUE(event1 < event2);
    EXPECT_FALSE(event2 < event1);

    Event event3(EdgeEvent, 1.0, std::make_shared<Triangle>(triangle), true);
    EXPECT_FALSE(event1 < event3);
}

TEST(EventTest, PrintEvent) {
    Event event(EdgeEvent, 1.0, std::shared_ptr<Triangle>(nullptr), true);
    std::cout << event << std::endl;
    std::ostringstream oss;
    event.print(oss);
    EXPECT_EQ(oss.str(), "{type: EdgeEvent, time: 1, triangle: 0, ray: 0}");
}

}  // namespace straightc_skeleton
