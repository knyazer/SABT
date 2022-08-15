//
// Created by knyaz on 12/08/22.
//

#include "include/Mesh.h"

Mesh::Mesh(const std::string& rootPath) {
    data.clear();

    std::string objName = rootPath.substr(rootPath.find_last_of('/') + 1);
    std::string objFilePath = rootPath + "/" + objName + ".obj";

    // Lambda for getting vertex color
    auto getVertexColor = [](CImg<unsigned char> &img, objl::Vertex &v){
        int x = round(double(v.TextureCoordinate.X) * double(img.width())),
            y = round(double(v.TextureCoordinate.Y) * double(img.height()));

        while (x < 0 || x >= img.width())
            x -= sign(x) * img.width();

        while (y < 0 || y >= img.height())
            y -= sign(y) * img.height();

        return Color((int)(img(x, y, 0, 0)), (int)(img(x, y, 0, 1)), (int)(img(x, y, 0, 2)));
    };

    objl::Loader loader;
    loader.LoadFile(objFilePath);

    std::cout << std::endl;
    // for each mesh
    for (auto mesh : loader.LoadedMeshes) {
        std::cout << ".";

        // load texture
        std::string texturePath = mesh.MeshMaterial.map_Ka;

        // ignore bump texture, because why not?

        if (texturePath.empty()) {
            std::cout << "\n\n[WARNING] ignore mesh (size " << mesh.Indices.size() / 3 << "), failed to determine texture name\n\n";
            continue;
        }

        CImg<unsigned char> texture((rootPath + "/" + texturePath).c_str());

        // For each triangle extract colors and push complete triangle to the vector
        for (size_t i = 0; i < mesh.Indices.size(); i += 3) {
            auto v1 = mesh.Vertices[mesh.Indices[i]],
                 v2 = mesh.Vertices[mesh.Indices[i + 1]],
                 v3 = mesh.Vertices[mesh.Indices[i + 2]];

            Triangle triangle({v1.Position.X, v1.Position.Y, v1.Position.Z},
                              {v2.Position.X, v2.Position.Y, v2.Position.Z},
                              {v3.Position.X, v3.Position.Y, v3.Position.Z});

            // TODO: better colors for triangles; not interpolation but exact
            triangle.setColor(getVertexColor(texture, v1), getVertexColor(texture, v2), getVertexColor(texture, v3));

            data.push_back(triangle);
        }
    }

    std::cout << " done loading. Triangle count: " << size() << std::endl;
}

Mesh::Mesh() {
    data.clear();
}

unsigned long long Mesh::size() const {
    return data.size();
}

Triangle Mesh::get(unsigned long long int index) const {
    if (index >= size())
        throw std::runtime_error("Attempt to access triangle outside of mesh bounds (provided index >= vector size), exiting.");

    return data[index];
}
