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

/**
 * The type of the element contained in stack, currently made up from the pointer to the octree and its cube.
 */
struct ID {
    OctreeBase *node;
    Cube cube;
};

/**
 * Result of the trace, returns a bunch of useful and not so information.
 */
struct TracingResult {
    /// State of the final node
    Color color = BLACK;

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

    /// Pointer to the root of the octree
    OctreeRoot *root{};

    /// Set this flag to true to debug this BeamTracer
    bool verbose{false};

    /// Connects current stack to the stack of other beam tracer,
    /// and sets current parent to the parent of the other
    void attach(const BeamTracer &other);

    /// Finds the node in the beam which is the closest to the origin, and sufficiently small
    /// to satisfy the desiredSize condition (angular size of the node is approximately
    /// less than the parameter passed, desiredSize)
    [[nodiscard]] TracingResult trace(double desiredSize);
};


#endif //SABT_BEAM_TRACER_H
