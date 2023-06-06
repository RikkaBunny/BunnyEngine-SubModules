#pragma once
#include "BunnyEngine/Renderer/MeshSource.h"

#include "../assimp/include/assimp/Importer.hpp"
#include "../assimp/include/assimp/scene.h"
#include "../assimp/include/assimp/postprocess.h"


namespace BE {

    class MeshLoad{
    public:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        static std::vector<MeshSource> loadModel(std::string const& path);

        static void SerializeMesh(const std::string& filepath);

        //bool Deserialize(const std::string& filepath);
    private:

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        static void processNode(aiNode* node, const aiScene* scene, std::vector<MeshSource>* meshes);

        static MeshSource processMesh(aiMesh* mesh, const aiScene* scene);

    };
}