#pragma once

#include <memory>
#include <limits>
#include <optional>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "ILowestIntersectionQuery.h"
#include "IRayShootingQuery.h"
#include "Ray.h"
#include "StraightSkeletonTypes.h"
#include "Triangle.h"

namespace straight_skeleton {

struct CollisionEvent {
    double time;
    Ray ray;
    Triangle triangle;
};

class CollisionManager {
   public:
    CollisionManager(std::unique_ptr<ILowestIntersectionQuery> lowestIntersector,
                     std::unique_ptr<IRayShootingQuery> rayShooter);

    void addRay(const Ray& ray);
    void removeRay(const Ray& ray);

    void addTriangle(const Triangle& triangle);
    void removeTriangle(const Triangle& triangle);

    CollisionEvent findEarliestCollision() const;

   private:
    std::unique_ptr<ILowestIntersectionQuery> lowestIntersector;
    std::unique_ptr<IRayShootingQuery> rayShooter;
};
}  // namespace straight_skeleton