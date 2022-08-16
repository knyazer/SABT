//
// Created by knyaz on 15/08/22.
//

#ifndef SABT_TRIANGLE_H
#define SABT_TRIANGLE_H

#include "Vec3f.h"
#include "Vec2f.h"
#include "Shape3d.h"
#include "Color.h"
#include "CImg.h"

#include <functional>

using cimg_library::CImg;

/** Represents a 3 dimensional triangle with texture applied.
 *
 */

struct Triangle : public Shape3d {

    /// Vertices coordinates
    Vec3f v1, v2, v3;

    /// Texture coordinates corresponding to each vertex
    Vec2f t1, t2, t3;

    /// Pointer to the texture
    CImg<unsigned char> *texture;

    Triangle(Vec3f firstVertex, Vec3f secondVertex, Vec3f thirdVertex);

    /// Apply given function to all the vertices
    void transform(const std::function<Vec3f(const Vec3f &)> &f);

    /// Setter for pointer to the texture of the triangle
    void setTexture(CImg<unsigned char> *texturePtr);

    /// Setter for texture coordinates
    void setTextureCoordinates(Vec2f first, Vec2f second, Vec2f third);

    /// Finds the pixel corresponding to provided point and returns its color
    [[nodiscard]] Color getColor(Vec3f point) const;

    [[nodiscard]] Vec3f getFarthestPointInDirection(Vec3f direction) const override;

    [[nodiscard]] Vec3f getCenter() const override;
};


#endif //SABT_TRIANGLE_H
