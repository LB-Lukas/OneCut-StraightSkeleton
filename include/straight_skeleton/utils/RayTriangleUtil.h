#pragma once

#include "../StraightSkeletonTypes.h"
#include "../Ray.h"
#include "../Triangle.h"

namespace straight_skeleton {

class RayTriangleUtil {
   public:

   static Triangle3D getTriangle(const Triangle& triangle);

   static Ray3D getRay(const Ray& ray);

   /**
    * @brief 
    * @see <a href="https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm">Intersection Line Triangle</a>
    * 
    * @param ray 
    * @param triangle 
    * @return std::optional<std::pair<double, Point3D>> 
    */
   static std::optional<std::pair<double, Point3D>> intersectRayTriangle(const Ray3D& ray, const Triangle3D& triangle);
};

}  // namespace straight_skeleton