#include "OneCut/StraightSkeleton.h"

namespace OneCut {

StraightSkeleton::StraightSkeleton(const std::vector<SkeletonFace>& faces) : faces(faces) {}

size_t StraightSkeleton::faceCount() const {
    return faces.size();
}

const SkeletonFace& StraightSkeleton::face(size_t i) const {
    return faces[i];
}

std::vector<SkeletonFace> StraightSkeleton::getFaces() const {
    return faces;
}

}  // namespace OneCut
