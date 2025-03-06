#include "straight_skeleton/Event.h"

namespace straight_skeleton {

Event::Event() : type(UndefinedEvent), time(0.0) {}

Event::Event(const EventType& type, const double& time, const TrianglePtr& triangle, const bool& valid)
    : type(type), time(time), triangle(triangle), valid(valid) {}

Event::Event(const EventType& type, const double& time, const RayPtr& ray, const bool& valid)
    : type(type), time(time), ray(ray), valid(valid) {}

EventType Event::getType() const {
    return type;
}

double Event::getTime() const {
    return time;
}

// TODO: maybe add a check for nullptr or throw an exception
TrianglePtr Event::getTriangle() const {
    return triangle;
}

// TODO: maybe add a check for nullptr or throw an exception
RayPtr Event::getRay() const {
    return ray;
}

bool Event::isValid() const {
    return valid;
}

void Event::setValid(bool valid) {
    this->valid = valid;
}

bool Event::operator==(const Event& other) const {
    return type == other.type && time == other.time && triangle == other.triangle && ray == other.ray ||
           type == other.type && time == other.time && triangle.get() == other.triangle.get() &&
               ray.get() == other.ray.get();
}

bool Event::operator>(const Event& other) const {
    if (time > other.time) {
        return true;
    } else if (time == other.time) {
        return type > other.type;
    } else {
        return false;
    }
}

bool Event::operator<(const Event& other) const {
    if (time < other.time) {
        return true;
    } else if (time == other.time) {
        return type < other.type;
    } else {
        return false;
    }
}

std::ostream& Event::print(std::ostream& os) const {
    std::map<EventType, std::string> eventTypeToString = {{EdgeEvent, "EdgeEvent"},
                                                          {SplitEvent, "SplitEvent"},
                                                          {UndefinedEvent, "UndefinedEvent"},
                                                          {VertexEvent, "VertexEvent"}};
    auto it = eventTypeToString.find(type);
    std::string typeStr = (it != eventTypeToString.end()) ? it->second : "Error";
    os << "{type: " << typeStr << ", time: " << time << ", triangle: " << triangle << ", ray: " << ray << "}";
    return os;
}

}  // namespace straight_skeleton