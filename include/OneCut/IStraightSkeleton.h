#pragma once

#include "StraightSkeletonTypes.h"
#include "ISkeletonFace.h"

namespace OneCut {

class IStraightSkeleton {
public:

    virtual ~IStraightSkeleton() = default;

    virtual size_t faceCount() const = 0;

    virtual const ISkeletonFace& face(size_t i) const = 0;

};

}