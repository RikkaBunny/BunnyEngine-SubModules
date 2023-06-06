#include "BEpch.h"
#include "MeshLoad.h"

#include <fstream>
#include <yaml-cpp/yaml.h>


namespace BE {

	std::vector<MeshSource> MeshLoad::loadModel(std::string const& path)
	{

        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_OptimizeGraph| aiProcess_OptimizeMeshes);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            //cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            BE_CORE_ASSERT("ERROR::ASSIMP:: ", importer.GetErrorString());
        }

        std::vector<MeshSource> meshes;
        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene, &meshes);

		return meshes;
	}

    YAML::Emitter& operator<<(YAML::Emitter& out, const Vertex& v) {
        out << v.Position[0] << v.Position[1] << v.Position[2]
            << v.Normal[0] << v.Normal[1] << v.Normal[2]
            << v.TexCoords[0] << v.TexCoords[1]
            << v.Tangent[0] << v.Tangent[1] << v.Tangent[2];
        return out;
    }

    void MeshLoad::SerializeMesh(const std::string& filepath)
    {
        std::vector<MeshSource> meshes = loadModel(filepath);
        MeshSource meshSource = meshes.at(0);
        
        
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Type";
        out << YAML::Value << "MeshSource";

        out << YAML::Key << "Vertices" << YAML::Flow << YAML::BeginSeq;

        for (size_t i = 0; i < meshSource.m_Vertices.size(); i++) {
            out << meshSource.m_Vertices.at(i);
        }

        out << YAML::EndSeq;

        out << YAML::Key << "Indices" << YAML::Flow << YAML::BeginSeq;

        for (size_t i = 0; i < meshSource.m_Indices.size(); i++) {
            out << meshSource.m_Indices.at(i);
        }

        out << YAML::EndSeq;

        out << YAML::EndMap;

        std::ofstream fout("F:\\GitHub\\BunnyEngine\\BunnyEngineEditor\\Assets\\box.BunnyMesh");
        fout << out.c_str();
    }

    void MeshLoad::processNode(aiNode* node, const aiScene* scene, std::vector<MeshSource>* meshes)
    {
        
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes->push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene, meshes);
        }
        
    }

    MeshSource MeshLoad::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        /*  vector<Vertex> vertices;
          vector<unsigned int> indices;
          vector<Texture> textures;*/

          // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            float vector[3]; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vertex.Position[0] = mesh->mVertices[i].x;
            vertex.Position[1] = mesh->mVertices[i].y;
            vertex.Position[2] = mesh->mVertices[i].z;

            // normals
            if (mesh->HasNormals())
            {
                vertex.Normal[0] = mesh->mNormals[i].x;
                vertex.Normal[1] = mesh->mNormals[i].y;
                vertex.Normal[2] = mesh->mNormals[i].z;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vertex.TexCoords[0] = mesh->mTextureCoords[0][i].x;
                vertex.TexCoords[1] = mesh->mTextureCoords[0][i].y;
                // tangent
                vertex.Tangent[0] = mesh->mTangents[i].x;
                vertex.Tangent[1] = mesh->mTangents[i].y;
                vertex.Tangent[2] = mesh->mTangents[i].z;
            }
            else {
                vertex.TexCoords[0] = 0.0f;
                vertex.TexCoords[1] = 0.0f;
            }
            if (mesh->HasTangentsAndBitangents()) {
                // tangent
                vertex.Tangent[0] = mesh->mTangents[i].x;
                vertex.Tangent[1] = mesh->mTangents[i].y;
                vertex.Tangent[2] = mesh->mTangents[i].z;
            }


            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        return MeshSource(vertices, indices);
    }

}