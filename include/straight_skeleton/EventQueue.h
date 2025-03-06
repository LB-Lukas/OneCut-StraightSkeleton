#pragma once

#include <iostream>
#include <queue>

#include "Event.h"
#include "IDataStructure.h"

namespace straight_skeleton {

struct EventCompare {
    bool operator()(const std::shared_ptr<Event>& a, const std::shared_ptr<Event>& b) const {
        // Compare the underlying Event by time, type, etc.
        return (*a > *b);
    }
};

class EventQueue : public IDataStructure {
   public:
    EventQueue();

    virtual void push(const std::shared_ptr<Event>& event);

    virtual std::shared_ptr<Event> pop();

    virtual std::shared_ptr<Event> top() const;

    bool isEmpty() const override;

    size_t size() const override;

    std::ostream& print(std::ostream& os) const;

   private:
    std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EventCompare> events;
};

inline std::ostream& operator<<(std::ostream& os, const EventQueue& queue) {
    return queue.print(os);
}
}  // namespace straight_skeleton