//
// Created by knyaz on 12/08/22.
//

#ifndef SABT_MESH_H
#define SABT_MESH_H

#include "ObjLoader.h"
#include "Color.h"
#include "Triangle.h"
#include "BasicMath.h"
#include "CImg.h"

#include <utility>
#include <string>
#include <vector>

using cimg_library::CImg;
using std::vector;

/**
 * Represents a 3D model in .obj format. Used by OctreeRoot to load models into octree format.
 * All the models are "shallow", though it usually does not affect visualisation.
 */
class Mesh {
protected:
    vector<Triangle> data;

    vector<CImg<unsigned char> *> textures;

public:
    Mesh();

    /// Constructs the vector of colored triangles from the obj-mtl files and corresponding textures
    explicit Mesh(const std::string &filename);

    /// Returns the number of triangles in the mesh
    [[nodiscard]] unsigned long long size() const;

    /// Returns a particular triangle in the mesh
    [[nodiscard]] Triangle get(unsigned long long index) const;
};


#endif //SABT_MESH_H
