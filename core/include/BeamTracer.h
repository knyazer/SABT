//
// Created by knyaz on 6/4/2022.
//

#ifndef SABT_BEAM_TRACER_H
#define SABT_BEAM_TRACER_H

#include <memory>

#include <Octree.h>
#include <ConnectedStack.h>
#include <Beam.h>

class BeamTracer {
    std::unique_ptr<BeamTracer> children[4];
    ConnectedStack<std::weak_ptr<Octree>> stack;

    Beam beam;
};

#endif //SABT_BEAM_TRACER_H
