//
// Created by knyaz on 12/08/22.
//

#include "include/Mesh.h"

Mesh::Mesh(const std::string& rootPath) {
    data.clear();

    std::string objName = rootPath.substr(rootPath.find_last_of('/') + 1);
    std::string objFilePath = rootPath + "/" + objName + ".obj";

    objl::Loader loader;
    loader.LoadFile(objFilePath);

    std::cout << std::endl;

    textures.resize(loader.LoadedMeshes.size());

    // for each mesh
    for (size_t i = 0; i < loader.LoadedMeshes.size(); i++) {
        std::cout << ".";

        auto mesh = loader.LoadedMeshes[i];

        // load texture
        std::string texturePath = mesh.MeshMaterial.map_Ka;

        // ignore bump texture, because why not?

        if (texturePath.empty()) {
            std::cout << "\n\n[WARNING] ignore mesh (size " << mesh.Indices.size() / 3 << "), failed to determine texture name\n\n";
            continue;
        }

        // Save the texture
        textures[i] = new CImg<unsigned char>((rootPath + "/" + texturePath).c_str());

        // For each triangle extract colors and push complete triangle to the vector
        for (size_t j = 0; j < mesh.Indices.size(); j += 3) {
            auto v1 = mesh.Vertices[mesh.Indices[j]],
                 v2 = mesh.Vertices[mesh.Indices[j + 1]],
                 v3 = mesh.Vertices[mesh.Indices[j + 2]];

            Triangle triangle({v1.Position.X, v1.Position.Y, v1.Position.Z},
                              {v2.Position.X, v2.Position.Y, v2.Position.Z},
                              {v3.Position.X, v3.Position.Y, v3.Position.Z});
            std::cout << textures[i] << std::endl;
            triangle.setTexture(textures[i]);
            triangle.setTextureCoordinates({v1.TextureCoordinate.X, v1.TextureCoordinate.Y},
                                         {v2.TextureCoordinate.X, v2.TextureCoordinate.Y},
                                         {v2.TextureCoordinate.X, v2.TextureCoordinate.Y});

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
