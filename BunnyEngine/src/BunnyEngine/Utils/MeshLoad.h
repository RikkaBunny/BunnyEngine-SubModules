#pragma once
#include "BunnyEngine/Renderer/Mesh.h"

#include "../assimp/include/assimp/Importer.hpp"
#include "../assimp/include/assimp/scene.h"
#include "../assimp/include/assimp/postprocess.h"


namespace BE {
    class MeshSource;
    class MeshLoad{
    public:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        static std::vector<MeshSource> loadModel(std::string const& path);
        static std::vector<MeshSource> loadDynamicMesh(std::string const& path);
        static MeshSource loadStaticMesh(std::string const& path);

       /* static void SerializeMesh(const std::string& filepath);
        static MeshSource DeserializeMesh(const std::string& filepath);*/
        
        //bool Deserialize(const std::string& filepath);
    private:

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        static void processNode(aiNode* node, const aiScene* scene, std::vector<MeshSource>* meshes);
        static void processNode(aiNode* node, const aiScene* scene, std::vector<float>* verticesDatas, std::vector<uint32_t>* indicesDatas);

        static MeshSource processMesh(aiMesh* mesh, const aiScene* scene);
        static void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<float>* verticesDatas, std::vector<uint32_t>* indicesDatas);

    };
}