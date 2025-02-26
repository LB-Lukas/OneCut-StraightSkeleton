#include "straight_skeleton/Ray.h"

namespace straight_skeleton {

Ray::Ray() : origin(Point3D(0,0,0)), direction(Direction3D(1,0,0)), tStart(0.0) {}

Ray::Ray(const Point3D& origin, const Direction3D& direction) : origin(origin), direction(direction), tStart(0.0) {}

Ray::Ray(const Point3D& origin, const Direction3D& direction, double tStart) : origin(origin), direction(direction), tStart(tStart) {}

Point3D Ray::getOrigin() const {
    return origin;
}

Direction3D Ray::getDirection() const {
    return direction;
}

double Ray::getTStart() const {
    return tStart;
}

Point3D Ray::positionAt(double t) const {
    double dt = t - tStart;
    Vector3D directionVector = direction.vector();
    Point3D position = origin + directionVector * dt;
    return position;
}

bool Ray::operator==(const Ray& other) const {
    return origin == other.origin && direction == other.direction && tStart == other.tStart;
}

}  // namespace straight_skeleton