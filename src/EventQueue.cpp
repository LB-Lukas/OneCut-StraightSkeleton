#include "straight_skeleton/EventQueue.h"

namespace straight_skeleton {

    EventQueue::EventQueue() {}

    void EventQueue::push(const Event& event) {
        events.push(event);
    }

    Event EventQueue::pop() {
        Event event = events.top();
        events.pop();
        return event;
    }

    Event EventQueue::top() const {
        return events.top();
    }

    bool EventQueue::isEmpty() const {
        return events.empty();
    }

    size_t EventQueue::size() const {
        return events.size();
    }

    std::ostream& EventQueue::print(std::ostream& os) const {
        std::priority_queue<Event, std::vector<Event>, std::greater<Event>> tempQueue = events;
        while (!tempQueue.empty()) {
            const Event& event = tempQueue.top();
            os << "Priority/Time: " << event.getTime() << " | " << event << "\n";
            tempQueue.pop();
        }
        return os;
    }

}