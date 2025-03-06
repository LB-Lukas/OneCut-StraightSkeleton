#include <gtest/gtest.h>

#include <iostream>
#include <memory>
#include <sstream>

#include "straight_skeleton/EventQueue.h"
#include "straight_skeleton/Triangle.h"

namespace straight_skeleton {

// TODO FIXME 
// TEST(EventQueueTest, InitiallyEmpty) {
//     EventQueue queue;
//     EXPECT_TRUE(queue.isEmpty());
//     EXPECT_EQ(queue.size(), 0);
// }

// TEST(EventQueueTest, PushAndPop) {
//     EventQueue queue;
//     std::shared_ptr<straight_skeleton::Event> event1 =
//         std::make_shared<Event>(EventType::EdgeEvent, 1.0, std::shared_ptr<Triangle>(nullptr), true);
//     std::shared_ptr<straight_skeleton::Event> event2 =
//         std::make_shared<Event>(EventType::SplitEvent, 2.0, std::shared_ptr<Ray>(nullptr), true);
//     queue.push(event1);
//     queue.push(event2);
//     ASSERT_FALSE(queue.isEmpty());
//     EXPECT_EQ(queue.size(), 2);
//     EXPECT_EQ(queue.pop(), event1);
//     EXPECT_EQ(queue.pop(), event2);
//     EXPECT_TRUE(queue.isEmpty());
// }

// TEST(EventQueueTest, Top) {
//     EventQueue queue;
//     std::shared_ptr<straight_skeleton::Event> event1 =
//         std::make_shared<Event>(EventType::EdgeEvent, 1.0, std::shared_ptr<Triangle>(nullptr), true);
//     std::shared_ptr<straight_skeleton::Event> event2 =
//         std::make_shared<Event>(EventType::SplitEvent, 2.0, std::shared_ptr<Ray>(nullptr), true);
//     queue.push(event1);
//     queue.push(event2);
//     EXPECT_EQ(queue.top(), event1);
// }

// TEST(EventQueueTest, Print) {
//     EventQueue queue;
//     TrianglePtr ptr = std::make_shared<Triangle>(Point3D(0, 0, 0), Point3D(0, 2, 0), Point3D(1, 1, 0));
//     std::shared_ptr<straight_skeleton::Event> event1 = std::make_shared<Event>(EventType::EdgeEvent, 1.0, ptr, true);
//     std::shared_ptr<straight_skeleton::Event> event2 =
//         std::make_shared<Event>(EventType::SplitEvent, 2.0, std::shared_ptr<Ray>(nullptr));
//     queue.push(event1);
//     queue.push(event2);
//     std::cout << queue << std::endl;
//     std::ostringstream oss;
//     oss << queue;
//     EXPECT_FALSE(oss.str().empty());
// }

}  // namespace straight_skeleton