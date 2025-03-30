#pragma once

#include "StraightSkeletonTypes.h"
#include "SkeletonFace.h"
#include "IStraightSkeleton.h"
#include <vector>
#include <stddef.h>

namespace OneCut {

class PlaceholderSkeleton : public IStraightSkeleton {
public:
    PlaceholderSkeleton(const std::vector<SkeletonFace>& faces);

    size_t faceCount() const override;
    const SkeletonFace& face(size_t i) const override;

private:
    const std::vector<SkeletonFace>& faces;


};

}