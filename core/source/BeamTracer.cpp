//
// Created by knyaz on 27/07/22.
//

#include "include/BeamTracer.h"

void BeamTracer::attach(BeamTracer &other) {
    if (!other.quadStack.parentEmpty())
        quadStack.connectToEnd(&other.quadStack);

    parent = &other;

    params = other.params;
}

void BeamTracer::attach(BeamTracer *other) {
    if (!other->quadStack.parentEmpty())
        quadStack.connectToEnd(&other->quadStack);

    parent = other;

    params = other->params;
}

void BeamTracer::attachStack(BeamTracer *other) {
    stack.connectToEnd(&other->stack);
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

            result.fullNodeReturns++;

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
        double approximateCubeAngularSizeSq = static_cast<double>(sq(rootCube.size)) / distanceToCubeSq;
        if (approximateCubeAngularSizeSq < sq(desiredSize)) {
            result.fill = true;
            result.color = node->getColor(0);
            result.smallNodeReturns++;

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
    delete[] children;
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
        restored[j] = params->camera.restore(beamVertices[j]);

    set(params->camera.getPosition(), restored);

    if (children != nullptr)
        for (size_t i = 0; i < 4; i++)
            children[i].update();

    delete[] beamVertices;
}

void BeamTracer::construct(const AlignedRect &newRect) {
    rect = newRect;
    rect.squeeze();

    update();
}

BeamTracer::~BeamTracer() {
    delete[] children;
}

TracingResult BeamTracer::castRay(Vec2f point) {
    TracingResult result;
    Ray ray(params->camera.getPosition(), params->camera.restore(point));

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
    ConnectedStack<OctreeAndCubePair> rayStack;
    rayStack.connectToEnd(&stack);


    using S_T = std::pair<std::pair<OctreeBase *, Cube>, double>;
    S_T *sorted = new S_T[8];

    while (!rayStack.parentEmpty()) {
        result.iterations++;

        OctreeAndCubePair rawNode = rayStack.front();

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

void BeamTracer::calculateMinMaxDistances() {
    if (children == nullptr) {
        if (stack.parentEmpty())
            return;

        const Cube& cube = stack.front().cube;
        minCubeDistance = max2((cube.getCenter() - origin).size() - double(cube.size / 2) * SQRT3, 0);
        maxCubeDistance = max2((cube.getCenter() - origin).size() + double(cube.size / 2) * SQRT3, 0);
    }
    else {
        for (size_t i = 0; i < 4; i++) {
            if (children[i].stack.parentEmpty())
                continue;

            children[i].calculateMinMaxDistances();

            if (minCubeDistance == -1)
                minCubeDistance = children[i].minCubeDistance;
            if (maxCubeDistance == -1)
                maxCubeDistance = children[i].maxCubeDistance;

            minCubeDistance = min2(minCubeDistance, children[i].minCubeDistance);
            maxCubeDistance = max2(maxCubeDistance, children[i].maxCubeDistance);
        }
    }
}

TracingResult BeamTracer::prepare(BeamTracer* baseTracer, double desiredSize, bool lastLevel) {
    TracingResult res;

    // make array for sorting
    using S_T = std::pair<BeamTracer*, double>;
    S_T *sorted = new S_T[4];

    Vec2f beamOriginProjectedToOldBeamPlane = baseTracer->params->camera.project(origin);

    marked = false;
    while (!quadStack.parentEmpty()) {
        res.iterations++;

        BeamTracer *node = quadStack.front();

        if (node == nullptr)
            throw std::runtime_error("Pointer to BeamTracer in quad stack is nullptr");

        // ignore if the node is empty
        if (node->stack.parentEmpty()) {
            quadStack.pop();
            continue;
        }

        // Check for intersection // TODO: order of this and next check for the last node
        Polytope boundingVolume = node->makeBoundingVolume();
        if (!Shape3d::hasIntersection(this, &boundingVolume)) {
            quadStack.pop();
            continue;
        }

        if (params == nullptr)
            throw std::runtime_error(
                    "Params cannot be nullptr, they should have been transferred from parent BeamTracer");

        // If node has no children while stack is not empty, then this node is the bottom-most leaf, which means we should return
        // the final stack node as the result for the tracing (kinda)
        if (node->children == nullptr) {

            Cube cube = node->stack.front().cube;
            if (lastLevel && !Shape3d::hasIntersection(this, &cube)) {
                //quadStack.printAll([](auto x){return x->rect;});
                break;
            }

            marked = true;
            res.fill = true;
            res.color = node->stack.front().node->getColor(0);
            break;
        }

        quadStack.pop();

        long sortedSize = 0;
        for (size_t i = 0; i < 4; i++) {
            BeamTracer *child = &node->children[i];

            if (child->stack.parentEmpty())
                continue;

            //Vec2f projected = params->camera.project(child->origin + child->params->camera.restore(child->rect.mid()));
            Vec2f projected = params->camera.project(child->stack.front().cube.getCenter());
            double distance = (projected - rect.mid()).sizesq();

            sorted[sortedSize] = {child, distance};
            sortedSize++;
        }

        if (verbose)
            std::cout << "Distance array size is " << sortedSize << "\n";

        std::sort(sorted, sorted + sortedSize, [](const S_T &A, const S_T &B) {
            return A.second > B.second;
        });

        //std::cout << sorted[0].second << " " <<sorted[1].second << std::endl;

        // TODO: Check if all the points lie strictly inside the beam, if so - skip all the intersection checks for all future nodes

        for (size_t i = 0; i < sortedSize; i++)
            quadStack.push(sorted[i].first);
    }

    if (marked == false) {
        res.fill = false;
    }

    // Cleanup
    delete[] sorted;

    return res;
}

Polytope BeamTracer::makeBoundingVolume() const {
    if (minCubeDistance < 0 || maxCubeDistance < 0)
        throw std::runtime_error("Distance to either closest or farthest cube is negative, probably forgot to initiliaze it. Committing suicide.");

    std::vector<Vec3f> vertices(8);

    for (size_t i = 0; i < 4; i++) {
        Vec3f dir = (rays[i] - origin).norm();
        vertices[i * 2] = origin + dir * minCubeDistance;
        vertices[i * 2 + 1] = rays[i]; //origin + dir * maxCubeDistance;
    }

    return vertices;
}

void BeamTracer::updatePrecomputed() {
    if (children != nullptr) {
        precomputed = true;
        for (size_t i = 0; i < 4; i++) {
            children[i].updatePrecomputed();
            if (!children[i].precomputed)
                precomputed = false;
        }
    }
}
