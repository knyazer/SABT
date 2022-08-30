//
// Created by knyaz on 12/08/22.
//

#define TINYOBJLOADER_IMPLEMENTATION
#include "third-party/ObjLoader.h"
#include "third-party/CImg.h"

#include "include/Mesh.h"

Mesh::Mesh(const std::string& rootPath) {
    data.clear();


    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string objName = rootPath.substr(rootPath.find_last_of('/') + 1);
    std::string objFilePath = rootPath + "/" + objName + ".obj";

    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFilePath.c_str(),
                                rootPath.c_str(), true);
    std::cout << "meow" << std::endl;

    // for each mesh
    for (size_t i = 0; i < shapes.size(); i++) {
        std::cout << ".";

        const auto& mesh = shapes[i].mesh;

        // load texture
        std::string texturePath = materials[mesh.material_ids[i]].diffuse_texname.c_str();

        // ignore bump texture, because why not

        if (texturePath.empty()) {
            std::cout << "Fuck, empty texture. Continue anyways" << std::endl;
            continue;
        }

        // Save the texture
        textures.push_back(new CImg<unsigned char>((rootPath + "/" + texturePath).c_str()));

        // For each triangle extract colors and push complete triangle to the vector
        for (size_t j = 0; j < mesh.indices.size(); j += 3) {
            Vec3f v1 = {attrib.vertices[mesh.indices[j].vertex_index * 3], attrib.vertices[mesh.indices[j].vertex_index * 3 + 1], attrib.vertices[mesh.indices[j].vertex_index * 3 + 2]};
            Vec3f v2 = {attrib.vertices[mesh.indices[j + 1].vertex_index * 3], attrib.vertices[mesh.indices[j + 1].vertex_index * 3 + 1], attrib.vertices[mesh.indices[j + 1].vertex_index * 3 + 2]};
            Vec3f v3 = {attrib.vertices[mesh.indices[j + 2].vertex_index * 3], attrib.vertices[mesh.indices[j + 2].vertex_index * 3 + 1], attrib.vertices[mesh.indices[j + 2].vertex_index * 3 + 2]};

            Triangle triangle(v1, v2, v3);

            triangle.setTexture(textures[textures.size() - 1]);

            triangle.setTextureCoordinates({attrib.texcoords[mesh.indices[j].texcoord_index * 2], attrib.texcoords[mesh.indices[j].texcoord_index * 2 + 1]},
                                           {attrib.texcoords[mesh.indices[j + 1].texcoord_index * 2], attrib.texcoords[mesh.indices[j + 1].texcoord_index * 2 + 1]},
                                           {attrib.texcoords[mesh.indices[j + 2].texcoord_index * 2], attrib.texcoords[mesh.indices[j + 2].texcoord_index * 2 + 1]});
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
