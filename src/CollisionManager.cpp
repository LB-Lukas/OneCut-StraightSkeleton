// #include "straight_skeleton/CollisionManager.h"

// namespace straight_skeleton {

// CollisionManager::CollisionManager(std::unique_ptr<ILowestIntersectionQuery> lowestIntersector,
//                                    std::unique_ptr<IRayShootingQuery> rayShooter)
//     : lowestIntersector(std::move(lowestIntersector)), rayShooter(std::move(rayShooter)) {}

// void CollisionManager::addRay(const Ray& ray) {
//     lowestIntersector->addRay(ray);
// }

// void CollisionManager::removeRay(const Ray& ray) {
//     lowestIntersector->removeRay(ray);
// }

// void CollisionManager::addTriangle(const Triangle& triangle) {
//     rayShooter->addTriangle(triangle);
// }

// void CollisionManager::removeTriangle(const Triangle& triangle) {
//     rayShooter->removeTriangle(triangle);
// }

// // TODO: STOPPED HERE ON 28-02-2025
// CollisionEvent CollisionManager::findEarliestCollision() const {
//     CollisionEvent earliestEvent;
//     earliestEvent.time = std::numeric_limits<double>::infinity();
//     for (const auto& ray : activeRays) {
//         IntersectionResult result = rayShooter->findRayIntersection(ray);
//         if (result.hasIntersection && result.parameterT < earliestEvent.time) {
//             earliestEvent.time = result.parameterT;
//             earliestEvent.ray = ray;
//             earliestEvent.triangle = result.intersectedTriangle;
//         }
//     }
//     for (const auto& triangle : activeTriangles) {
//         IntersectionResult result = lowestIntersector->findLowestIntersection(triangle);
//         if (result.hasIntersection && result.parameterT < earliestEvent.time) {
//             earliestEvent.time = result.parameterT;
//             earliestEvent.ray = result.intersectedRay;
//             earliestEvent.triangle = triangle;
//         }
//     }
//     return earliestEvent;
// }


// }  // namespace straight_skeleton