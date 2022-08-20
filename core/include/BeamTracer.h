//
// Created by knyaz on 27/07/22.
//

#ifndef SABT_BEAM_TRACER_H
#define SABT_BEAM_TRACER_H

#include "Beam.h"
#include "ConnectedStack.h"
#include "Cube.h"
#include "OctreeBase.h"
#include "OctreeRoot.h"
#include "Color.h"
#include "Vec3f.h"
#include "WorldParams.h"
#include "AlignedRect.h"
#include "Biplet.h"
#include "include/Ray.h"

#include <cmath>

constexpr double SQRT3 = 1.73205080757;

/**
 * The type of the element contained in stack, currently made up from the pointer to the octree and its cube.
 */
struct ID {
    OctreeBase *node;
    Cube cube;

    ID() {
        node = nullptr;
    }

    ID(OctreeBase* _node, Cube _cube) : node(_node), cube(_cube) {};

    ID(OctreeBase* _node, Cube _cube, bool _certainIntersection) : node(_node), cube(_cube) {};

    void clearFromTempData() {

    }
};

/**
 * Result of the trace, returns a bunch of useful and not so information.
 */
struct TracingResult {
    /// State of the final node
    Color color = Color::BLACK;

    /// Is final node empty or not ?
    bool fill = false;

    /// Number of iterations taken to get to the latest node
    long long iterations = 0;
};

/**
 * Beam tracer is a beam which has the ability to search the octree for intersection by itself.
 */
class BeamTracer : public Beam {
public:
    /// The main stack
    ConnectedStack<ID> stack;

    /// Pointer to the children of the beam tracer
    BeamTracer *children{};

    /// Generates the 4 children and attaches them to itself
    void makeChildren();

    /// BeamTracer tracing rect - location of beam in camera coordinate system
    AlignedRect rect;

    /// Pointer to the data saved at root of the beam tree
    WorldParams *params{};

    /// Distance to the closest cube, computed from bottom to top
    double distanceToTheClosestCube{-1};

    /// Set this flag to true to debug this BeamTracer
    bool verbose{false};

    /// Construct beam rays from the 2-dimensional camera space newRect
    void construct(const AlignedRect& newRect);

    /// Recursively calculates distance to the closest cube
    /// Yeah, recursion is bad, but I do not want to add parent pointers
    /// So the only reasonable way to do this is DFS
    [[nodiscard]] double calculateMinDistance();

    /// Connects current stack to the stack of other beam tracer,
    /// syncs the params of the root
    void attach(BeamTracer &other);

    /// Same as top one, but with pointer to the parent
    void attach(BeamTracer *other);

    /// Finds the node in the beam which is the closest to the origin, and sufficiently small
    /// to satisfy the desiredSize condition (angular size of the node is approximately
    /// less than the parameter passed, desiredSize)
    [[nodiscard]] TracingResult trace(double desiredSize);

    /// Casts a ray through the point using this beam stack
    [[nodiscard]] TracingResult castRay(Vec2f point);

    void update();

    ~BeamTracer();
};


#endif //SABT_BEAM_TRACER_H
