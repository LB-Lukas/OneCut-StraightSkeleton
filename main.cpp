#include "include/StraightSkeleton/doublyConnectedEdgeList.h"

int main() {
    std::vector<std::shared_ptr<Geometry::Vertex>> vertices;

    for(int i = 0; i < 10; i++) {
        auto vertex = std::make_shared<Geometry::Vertex>(i, 2 * i);
        vertices.push_back(vertex);
    }

    auto dcel = Geometry::DCEL(vertices);
    dcel.printDCEL();

    std::cout << "test";

    return 0;
}
