#pragma once

#include <iostream>
#include <memory>
#include <utility>

#include "straight_skeleton/Event.h"
#include "straight_skeleton/StraightSkeletonTypes.h"

namespace straight_skeleton {

class Triangle {
   public:
    Triangle();
    Triangle(const Point3D& a, const Point3D& b, const Point3D& c);
    Triangle(const Point3D& a, const Point3D& b, const Point3D& c, const bool& valid);

    Point3D getBaseA() const;
    Point3D getBaseB() const;
    Point3D getApex() const;

    std::pair<Point3D, Point3D> getBase() const;

    bool isValid() const;
    void setValid(const bool& isValid);

    TrianglePtr getClockwiseNeighbour() const;
    TrianglePtr getCounterclockwiseNeighbour() const;

    void setClockwiseNeighbour(const TrianglePtr& newNeighbour);
    void setCounterclockwiseNeighbour(const TrianglePtr& newNeighbour);

    bool operator==(const Triangle& other) const;
    std::ostream& print(std::ostream& os) const;

   private:
    Point3D baseA;
    Point3D baseB;
    Point3D apex;
    bool valid;

    TrianglePtr clockwiseNeighbour;
    TrianglePtr counterclockwiseNeighbour;
};

inline std::ostream& operator<<(std::ostream& os, const Triangle& triangle) {
    return triangle.print(os);
}
}  // namespace straight_skeleton