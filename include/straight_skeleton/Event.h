#pragma once

#include <memory>
#include <string>
#include <map>

namespace straight_skeleton {

/**
 * @enum EventType
 * @brief Enumeration classifying types of events during straight skeleton computation.
 *
 * Events occur during the inward offsetting of polygon edges and vertices.
 */
enum EventType { EdgeEvent, SplitEvent, VertexEvent, UndefinedEvent };

// Forward declarations (used internally in the computation)
class Triangle;
class Ray;

typedef std::shared_ptr<Triangle> TrianglePtr;
typedef std::shared_ptr<Ray> RayPtr;

/**
 * @class Event
 * @brief Represents a discrete event during straight skeleton computation.
 *
 * Events are prioritized by their `time` member, which determines the order of processing.
 * The type of event (EdgeEvent, SplitEvent, VertexEvent) dictates how the skeleton topology changes.
 */
class Event {
   public:
    /**
     * @brief Default constructor (creates an UndefinedEvent at time 0).
     */
    Event();

    /**
     * @brief Construct a new Event object.
     * 
     * @param type Type of the event (EdgeEvent, SplitEvent, VertexEvent) @see EventType.
     * @param time Simulation time at which the event occurs.
     * @param triangle Associated triangle in the skeleton mesh (if applicable).
     */
    Event(const EventType& type, const double& time, const TrianglePtr& triangle);


    /**
     * @brief Construct a new Event object.
     * 
     * @param type Type of the event (EdgeEvent, SplitEvent, VertexEvent) @see EventType.
     * @param time Simulation time at which the event occurs.
     * @param ray Associated ray causing the event (if applicable).
     */
    Event(const EventType& type, const double& time, const RayPtr& ray);

    /**
     * @brief Get the event type.
     * @return EventType enum value (e.g., EdgeEvent, SplitEvent).
     */
    EventType getType() const;

    /**
     * @brief Get the event time.
     * @return Simulation time when the event occurs (lower values are processed first).
     */
    double getTime() const;

    /**
     * @brief Get the associated triangle.
     * @return Shared pointer to the Triangle involved in the event (may be nullptr).
     */
    TrianglePtr getTriangle() const;

    /**
     * @brief Get the associated ray.
     * @return Shared pointer to the Ray causing the event (may be nullptr).
     */
    RayPtr getRay() const;

    /**
     * @brief Check if two event are equal.
     * Two events are considered equal if they have the same type and time and point to the same triangle and ray.
     * 
     * @param other The other event to compare with.
     * @return true if the events are equal.
     * @return false if the events are not equal.
     */
    bool operator==(const Event& other) const;


    /**
     * @brief Check if this event is greater than another event.
     * Events are compared first by time, then by type.
     * 
     * @param other The other event to compare with.
     * @return true if this event is greater than the other event.
     * @return false otherwise.
     */
    bool operator>(const Event& other) const;

    /**
     * @brief Check if this event is less than another event.
     * Events are compared first by time, then by type.
     * 
     * @param other The other event to compare with.
     * @return true if this event is less than the other event.
     * @return false otherwise.
     */
    bool operator<(const Event& other) const;

    /**
     * @brief Print the event to an output stream.
     * 
     * @param os The output stream to print to.
     * @return The output stream after printing the event.
     */
    std::ostream& print(std::ostream& os) const;


   private:
    EventType type;
    double time;
    TrianglePtr triangle;
    RayPtr ray;
};

inline std::ostream& operator<<(std::ostream& os, const Event& event) {
    return event.print(os);
}

}  // namespace straight_skeleton