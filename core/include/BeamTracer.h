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

struct ID {
    OctreeBase *node;
    Cube cube;
};

struct TracingResult {
    Color color = BLACK;
    bool fill = false;
    long long iterations = 0;
};

class BeamTracer : public Beam {
public:
    ConnectedStack<ID> stack;

    OctreeRoot *root{};

    bool verbose{false};

    void attach(const BeamTracer &other);

    [[nodiscard]] TracingResult trace(double desiredSize);
};


#endif //SABT_BEAM_TRACER_H
