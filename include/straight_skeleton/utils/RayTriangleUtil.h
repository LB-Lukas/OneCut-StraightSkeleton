#pragma once

#include "../StraightSkeletonTypes.h"
#include "../Ray.h"
#include "../Triangle.h"

namespace straight_skeleton {

    struct IntersectionResult {
        bool hasIntersection{false};
        Ray intersectedRay;
        Triangle intersectedTriangle;
        Point3D intersectionPoint;
        double parameterT;
    
        IntersectionResult() = default;
    };

class RayTriangleUtil {
   public:

   static Triangle3D getTriangle(const Triangle& triangle);

   static Ray3D getRay(const Ray& ray);

   /**
    * @brief 
    * 
    * @param ray 
    * @param triangle 
    * @return std::optional<std::pair<double, Point3D>> Returns a pair of the parameter t and the intersection point
    */
   static std::optional<std::pair<double, Point3D>> intersectRayTriangle(const Ray3D& ray, const Triangle3D& triangle);
};

}  // namespace straight_skeleton