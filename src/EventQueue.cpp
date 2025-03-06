#include "straight_skeleton/EventQueue.h"

namespace straight_skeleton {

    EventQueue::EventQueue() {}

    void EventQueue::push(const std::shared_ptr<Event>& event) {
        events.push(event);
    }

    std::shared_ptr<Event> EventQueue::pop() {
        std::shared_ptr<Event> event = events.top();
        events.pop();
        return event;
    }

    std::shared_ptr<Event> EventQueue::top() const {
        return events.top();
    }

    bool EventQueue::isEmpty() const {
        return events.empty();
    }

    size_t EventQueue::size() const {
        return events.size();
    }

    std::ostream& EventQueue::print(std::ostream& os) const {
        std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EventCompare> tempQueue = events;
        while (!tempQueue.empty()) {
            const std::shared_ptr<Event>& event = tempQueue.top();
            os << "Priority/Time: " << event->getTime() << " | " << *event << "\n";
            tempQueue.pop();
        }
        return os;
    }

}