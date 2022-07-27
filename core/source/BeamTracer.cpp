//
// Created by knyaz on 27/07/22.
//

#include "include/BeamTracer.h"

void BeamTracer::attach(const BeamTracer &other) {
    stack.connect(std::make_shared<ConnectedStack<ID>>(other.stack), other.stack.size() - 1);
    root = other.root;
    origin = other.origin;
}

TracingResult BeamTracer::trace(double desiredSize) {
    TracingResult result;

    while (!stack.parentEmpty()) {
        auto rawNode = stack.front();
        stack.pop();

        if (rawNode == nullptr)
            throw std::runtime_error("Pointer to BaseOctree in call stack is nullptr");

        // if node is full - finish, returning this node
        if (rawNode->isFull()) {
            result.color = rawNode->getColor(0);
            result.fill = true;
            break;
        }

        // If node is not full, it means it is not the OctreeUnit derived, so we can cast the Octree*.
        auto node = dynamic_cast<Octree*>(rawNode);

        // ignore if the node is empty
        if (node->isEmpty())
            continue;

        // quick adequacy test
        if (!node->hasChildren()) {
            throw std::runtime_error("Cannot have no children while being SEMI.");
        }

        // if node is semi - continue iteration, push all the children sorted by the distance to node
        // for each node check that it intersects with current beam (thick ray), and if so - push it to stack
        Cube rootCube = root->getCubeFor(node);

        // check whether the cube is LE than desired, if so - finish
        double distanceToCube = (rootCube.getCenter() - origin).size();
        if (static_cast<double>(rootCube.size) / distanceToCube < desiredSize) { // ~~ tan(alpha) ~~ alpha
            result.fill = true;
            result.color = node->getColor(0);

            break;
        }

        using S_T = std::pair<std::pair<OctreeBase*, Cube>, double>;

        std::vector<S_T> sorted;
        for (size_t i = 0; i < 8; i++) {
            OctreeBase* child = node->getChild(i);
            if (child->isEmpty())
                continue;

            Cube cube = Octree::getCubeForChild(rootCube, i);
            double distance = (Vec3f(cube.pos) - origin).size();

            sorted.push_back({{child, cube}, distance});
        }

        std::sort(sorted.begin(), sorted.end(), [](const S_T& A, const S_T &B) {
            return A.second < B.second;
        });

        for (const auto& zipped : sorted) {
            auto *child = zipped.first.first;
            Cube cube = zipped.first.second;

            if (Shape3d::hasIntersection(this, &cube)) {
                stack.push(child);
                break;
            }
        }
    }

    return result;
}