#pragma once

#include <iostream>
#include <queue>

#include "Event.h"
#include "IDataStructure.h"

namespace straight_skeleton {

class EventQueue : public IDataStructure {
   public:
    EventQueue();

    virtual void push(const Event& event);

    virtual Event pop();

    virtual Event top() const;

    bool isEmpty() const override;

    size_t size() const override;

    std::ostream& print(std::ostream& os) const;

   private:
    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> events;
};

inline std::ostream& operator<<(std::ostream& os, const EventQueue& queue) {
    return queue.print(os);
}
}  // namespace straight_skeleton