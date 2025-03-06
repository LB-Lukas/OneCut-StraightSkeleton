#include "straight_skeleton/Triangle.h"

namespace straight_skeleton {

Triangle::Triangle() : baseA(Point3D()), baseB(Point3D()), apex(Point3D()) {}

Triangle::Triangle(const Point3D& a, const Point3D& b, const Point3D& c) : baseA(a), baseB(b), apex(c) {}

Triangle::Triangle(const Point3D& a, const Point3D& b, const Point3D& c, const bool& valid)
    : baseA(a), baseB(b), apex(c), valid(valid) {}

Point3D Triangle::getBaseA() const {
    return baseA;
}

Point3D Triangle::getBaseB() const {
    return baseB;
}

Point3D Triangle::getApex() const {
    return apex;
}

bool Triangle::isValid() const {
    return valid;
}

void Triangle::setValid(const bool& isValid) {
    valid = isValid;
}

std::pair<Point3D, Point3D> Triangle::getBase() const {
    return std::make_pair(baseA, baseB);
}

TrianglePtr Triangle::getClockwiseNeighbour() const {
    return clockwiseNeighbour;
}

TrianglePtr Triangle::getCounterclockwiseNeighbour() const {
    return counterclockwiseNeighbour;
}

void Triangle::setClockwiseNeighbour(const TrianglePtr& newNeighbour) {
    // Allow setting to nullptr
    if (newNeighbour == nullptr) {
        clockwiseNeighbour = nullptr;
        return;
    }
    // check that baseB is the same as the new neighbour's baseA
    if (baseB == newNeighbour->baseA) {
        clockwiseNeighbour = newNeighbour;
    } else {
        throw std::runtime_error("Error: baseB does not match new neighbour's baseA");
    }
}

void Triangle::setCounterclockwiseNeighbour(const TrianglePtr& newNeighbour) {
    // Allow setting to nullptr
    if (newNeighbour == nullptr) {
        counterclockwiseNeighbour = nullptr;
        return;
    }
    // check that baseA is the same as the new neighbour's baseB
    if (baseA == newNeighbour->baseB) {
        counterclockwiseNeighbour = newNeighbour;
    } else {
        throw std::runtime_error("Error: baseA does not match new neighbour's baseB");
    }
}

bool Triangle::operator==(const Triangle& other) const {
    if (this->baseA == other.baseA && this->baseB == other.baseB && this->apex == other.apex &&
        this->clockwiseNeighbour == other.clockwiseNeighbour &&
        this->counterclockwiseNeighbour == other.counterclockwiseNeighbour) {
        return true;
    }
    return false;
}

std::ostream& Triangle::print(std::ostream& os) const {
    os << "Triangle: BaseA " << baseA << ", BaseB " << baseB << ", Apex: " << apex;
    return os;
}

}  // namespace straight_skeleton