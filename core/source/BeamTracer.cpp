//
// Created by knyaz on 27/07/22.
//

#include "include/BeamTracer.h"

void BeamTracer::attach(const BeamTracer &other) {
    stack.connectToEnd(std::make_shared<ConnectedStack<ID>>(other.stack));
    params = other.params;
}

void BeamTracer::attach(BeamTracer *other) {
    stack.connectToEnd(std::make_shared<ConnectedStack<ID>>(other->stack));
    params = other->params;
}

TracingResult BeamTracer::trace(double desiredSize) {
    TracingResult result;

    while (!stack.parentEmpty()) {
        result.iterations ++;

        auto rawNode = stack.front();

        if (rawNode.node == nullptr)
            throw std::runtime_error("Pointer to BaseOctree in call stack is nullptr");

        // ignore if the node is empty
        if (rawNode.node->isEmpty())
            continue;

        // Check for intersection
        if (!Shape3d::hasIntersection(this, &rawNode.cube)) {
            stack.pop();
            continue;
        }

        // if node is full - finish, returning this node
        if (rawNode.node->isFull()) {
            result.color = rawNode.node->getColor(0);
            result.fill = true;

            if (verbose)
                std::cout << *dynamic_cast<Octree*>(rawNode.node) << " is full; done\n";

            break;
        }

        stack.pop();

        // If node is not full, it means it is not the OctreeUnit derived, so we can cast the Octree*.
        Octree *node = dynamic_cast<Octree*>(rawNode.node);

        // quick adequacy test
        if (!node->hasChildren()) {
            throw std::runtime_error("Cannot have no children while being SEMI.");
        }

        // if node is semi - continue iteration, push all the children sorted by the distance to node
        // for each node check that it intersects with current beam (thick ray), and if so - push it to stack
        if (params == nullptr)
            throw std::runtime_error("Params cannot be nullptr, they should have been transferred from parent BeamTracer");

        Cube rootCube = params->root->getCubeFor(node);

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

            if (child->isEmpty()) {
                if (verbose)
                    std::cout << *child << " is empty, so ignore. It was at " << Triplet(i) << "\n";

                continue;
            }

            Cube cube = Octree::getCubeForChild(rootCube, i);
            double distance = (Vec3f(cube.pos) - origin).size();

            sorted.push_back({{child, cube}, distance});


            if (verbose)
                std::cout << *child << " is not empty, so add to the dist arr. It was at " << Triplet(i) \
                    << ", distance from origin: " << distance << "\n";
        }

        if (verbose)
            std::cout << "Distance array size is " << sorted.size()  << "\n";

        std::sort(sorted.begin(), sorted.end(), [](const S_T& A, const S_T &B) {
            return A.second > B.second;
        });

        for (const auto& zipped : sorted) {
            auto *child = zipped.first.first;
            Cube cube = zipped.first.second;

            stack.push({child, cube});
        }
    }

    if (verbose) {
        if (stack.parentEmpty())
            std::cout << "Stack is empty; done\n\n\n";
        else
            std::cout << "Final stack size is " << stack.size() << "\n\n\n";
    }

    return result;
}

void BeamTracer::makeChildren() {
    children = new BeamTracer[4];
    Vec2f center = rect.mid();

    for (size_t i = 0; i < 4; i++) {
        AlignedRect childRect(rect[Biplet(i)], center);

        children[i].attach(this);
        children[i].construct(childRect);
    }
}

void BeamTracer::update() {
    Vec2f* beamVertices = rect.getVertices();
    Vec3f restored[4];

    for (size_t j = 0; j < 4; j++)
        restored[j] = params->camera->restore(beamVertices[j]);

    set(params->camera->getPosition(), restored);

    if (children != nullptr)
        for (size_t i = 0; i < 4; i++)
            children[i].update();
}

void BeamTracer::construct(const AlignedRect& newRect) {
    this->rect = newRect;

    update();
}
