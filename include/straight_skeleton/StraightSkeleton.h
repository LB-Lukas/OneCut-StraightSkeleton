#pragma once

#include <unordered_map>
#include <vector>

#include "IStraightSkeleton.h"
#include "SkeletonFace.h"
#include "StraightSkeletonTypes.h"

namespace straight_skeleton {

class StraightSkeleton : public IStraightSkeleton {
   public:
    StraightSkeleton(const std::vector<SkeletonFace>& faces);
    size_t faceCount() const override;
    const SkeletonFace& face(size_t i) const override;
    std::vector<SkeletonFace> getFaces() const;

   private:
    std::vector<SkeletonFace> faces;
};

}  // namespace straight_skeleton
