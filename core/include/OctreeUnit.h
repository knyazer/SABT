//
// Created by knyaz on 20/07/22.
//

#ifndef SABT_OCTREE_UNIT_H
#define SABT_OCTREE_UNIT_H

#include "OctreeBase.h"
#include "Color.h"

#include <cstdint>

class OctreeUnit : public OctreeBase {
protected:
    bool exist;

public:
    Color color;

    OctreeUnit();

    bool isEmpty() override;

    bool isFull() override;

    bool isSemi() override;

    void fill(Color color) override;

    void clear() override;

    Color getColor(int faceIndex) override;
};


#endif //SABT_OCTREE_UNIT_H
