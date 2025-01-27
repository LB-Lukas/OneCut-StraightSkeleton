#include "straight_skeleton/PlaceholderSkeleton.h"

namespace straight_skeleton {

PlaceholderSkeleton::PlaceholderSkeleton(const std::vector<SkeletonFace>& faces) : faces(faces) {}

size_t PlaceholderSkeleton::faceCount() const {
    return faces.size();
}

const SkeletonFace& PlaceholderSkeleton::face(size_t i) const {
    return faces[i];
}


}