//
// Created by knyaz on 27/07/22.
//

#include "include/BeamTracer.h"

void BeamTracer::attach(BeamTracer &other) {
    stack.connectToEnd(&other.stack);
    params = other.params;
}

void BeamTracer::attach(BeamTracer *other) {
    stack.connectToEnd(&other->stack);
    params = other->params;
}

TracingResult BeamTracer::trace(double desiredSize) {
    TracingResult result;

    // make array for sorting
    using S_T = std::pair<std::pair<OctreeBase *, Cube>, double>;
    S_T *sorted = new S_T[8];
    int g_cnt = 0;
    while (!stack.parentEmpty()) {
        result.iterations++;

        auto rawNode = stack.front();

        if (rawNode.node == nullptr)
            throw std::runtime_error("Pointer to BaseOctree in call stack is nullptr");

        // ignore if the node is empty
        if (rawNode.node->isEmpty()) {
            stack.pop();
            continue;
        }

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
                std::cout << *rawNode.node << " is full; done\n";

            break;
        }

        // If node is not full, it means it is not the OctreeUnit, so we can cast the Octree*.
        Octree *node = dynamic_cast<Octree *>(rawNode.node);

        // quick adequacy test
        if (!node->hasChildren()) {
            throw std::runtime_error("Cannot have no children while being SEMI.");
        }

        // if node is semi - continue iteration, push all the children sorted by the distance to node
        // for each node check that it intersects with current beam (thick ray), and if so - push it to stack
        if (params == nullptr)
            throw std::runtime_error(
                    "Params cannot be nullptr, they should have been transferred from parent BeamTracer");

        const Cube &rootCube = rawNode.cube;

        // check whether the cube is LE than desired, if so - finish
        double distanceToCubeSq = (rootCube.getCenter() - origin).sqsize();
        if (static_cast<double>(sq(rootCube.size)) / distanceToCubeSq < sq(desiredSize)) { // ~~ tan(alpha) ~~ alpha
            result.fill = true;
            result.color = node->getColor(0);

            break;
        }

        stack.pop();

        long sortedSize = 0;
        for (size_t i = 0; i < 8; i++) {
            OctreeBase *child = node->getChild(i);

            if (child->isEmpty()) {
                if (verbose)
                    std::cout << *child << " is empty, so ignore. It was at " << Triplet(i) << "\n";

                continue;
            }

            Cube cube = Octree::getCubeForChild(rootCube, i);
            double distance = (Vec3f(cube.getCenter()) - origin).sqsize();

            sorted[sortedSize] = {{child, cube}, distance};
            sortedSize++;

            if (verbose)
                std::cout << *child << " is not empty, so add to the dist arr. It was at " << Triplet(i) \
 << ", distance from origin: " << distance << "\n";
        }

        if (verbose)
            std::cout << "Distance array size is " << sortedSize << "\n";

        std::sort(sorted, sorted + sortedSize, [](const S_T &A, const S_T &B) {
            return A.second > B.second;
        });


        // TODO: Check if all the points lie strictly inside the beam, if so - skip all the intersection checks for all future nodes

        for (size_t i = 0; i < sortedSize; i++) {
            auto *child = sorted[i].first.first;
            Cube cube = sorted[i].first.second;

            stack.push({child, cube});
        }
    }

    if (verbose) {
        if (stack.parentEmpty())
            std::cout << "Stack is empty; done\n\n\n";
        else
            std::cout << "Final stack size is " << stack.size() << "\n\n\n";
    }

    // Cleanup
    for (size_t i = 0; i < stack.size(); i++)
        stack.at(i).clearFromTempData();
    delete[] sorted;

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
    Vec2f *beamVertices = rect.getVertices();
    Vec3f restored[4];

    for (size_t j = 0; j < 4; j++)
        restored[j] = params->camera->restore(beamVertices[j]);

    set(params->camera->getPosition(), restored);

    if (children != nullptr)
        for (size_t i = 0; i < 4; i++)
            children[i].update();

    delete beamVertices;
}

void BeamTracer::construct(const AlignedRect &newRect) {
    this->rect = newRect;

    update();
}

BeamTracer::~BeamTracer() {
    delete[] children;
}

TracingResult BeamTracer::castRay(Vec2f point) {
    TracingResult result;
    Ray ray(params->camera->getPosition(), params->camera->restore(point));

    // in case parent stack is empty, exit immediately
    if (stack.parentEmpty())
        return {Color::BLACK, false, 0};

    // check the very first node if it is complete, if so - return it without all stack business
    auto firstNode = stack.front();
    if (firstNode.node->isFull() && ray.hasIntersection(firstNode.cube)) {
        result.color = firstNode.node->getColor(0);
        result.fill = true;

        if (verbose)
            std::cout << "First-step exit confirmed; " << *dynamic_cast<Octree *>(firstNode.node) << " is full; done\n";

        return result;
    }

    // make & setup the stack
    ConnectedStack<ID> rayStack;
    rayStack.connectToEnd(&stack);


    using S_T = std::pair<std::pair<OctreeBase *, Cube>, double>;
    S_T *sorted = new S_T[8];

    while (!rayStack.parentEmpty()) {
        result.iterations++;

        ID rawNode = rayStack.front();

        if (rawNode.node == nullptr)
            throw std::runtime_error("Pointer to BaseOctree in call stack is nullptr");

        // ignore if the node is empty
        if (rawNode.node->isEmpty())
            continue;

        // Check for intersection
        if (!ray.hasIntersection(rawNode.cube)) {
            rayStack.pop();
            continue;
        }

        // if node is full - finish, returning this node
        if (rawNode.node->isFull()) {
            result.color = rawNode.node->getColor(0);
            result.fill = true;

            if (verbose)
                std::cout << *rawNode.node << " is full; done\n";

            break;
        }

        rayStack.pop();

        // If node is not full, it means it is not the OctreeUnit, so we can cast the Octree*.
        auto *node = dynamic_cast<Octree *>(rawNode.node);

        // quick adequacy test
        if (!node->hasChildren()) {
            throw std::runtime_error("Cannot have no children while being SEMI.");
        }

        // if node is semi - continue iteration, push all the children sorted by the distance to node
        // for each node check that it intersects with current beam (thick ray), and if so - push it to stack
        if (params == nullptr)
            throw std::runtime_error(
                    "Params cannot be nullptr, they should have been transferred from parent BeamTracer");

        const Cube &rootCube = rawNode.cube;

        long sortedSize = 0;
        for (size_t i = 0; i < 8; i++) {
            OctreeBase *child = node->getChild(i);

            if (child->isEmpty()) {
                if (verbose)
                    std::cout << *child << " is empty, so ignore. It was at " << Triplet(i) << "\n";

                continue;
            }

            Cube cube = Octree::getCubeForChild(rootCube, i);

            double distance = Vec3f::dot(ray.getDirection(), Vec3f(cube.pos));

            sorted[sortedSize] = {{child, cube}, distance};
            sortedSize++;

            if (verbose)
                std::cout << *child << " is not empty, so add to the dist arr. It was at " << Triplet(i) \
 << ", distance from origin: " << distance << "\n";
        }

        if (verbose)
            std::cout << "Distance array size is " << sortedSize << "\n";

        std::sort(sorted, sorted + sortedSize, [](const S_T &A, const S_T &B) {
            return A.second > B.second;
        });

        for (size_t i = 0; i < sortedSize; i++) {
            auto *child = sorted[i].first.first;
            Cube cube = sorted[i].first.second;

            rayStack.push({child, cube});
        }
    }

    if (verbose) {
        if (rayStack.parentEmpty())
            std::cout << "Stack is empty; done\n\n\n";
        else
            std::cout << "Final stack size is " << rayStack.size() << "\n\n\n";
    }

    delete[] sorted;

    return result;
}

double BeamTracer::calculateMinDistance() {
    if (children == nullptr) {
        distanceToTheClosestCube = max2((stack.front().cube.getCenter() - origin).size() - (stack.front().cube.size / 2) * SQRT3, 0);
    }
    else {
        double minDistance = children[0].calculateMinDistance();
        for (size_t i = 1; i < 4; i++)
            minDistance = min2(minDistance, children[i].calculateMinDistance());

        distanceToTheClosestCube = minDistance;
    }

    return distanceToTheClosestCube;
}
