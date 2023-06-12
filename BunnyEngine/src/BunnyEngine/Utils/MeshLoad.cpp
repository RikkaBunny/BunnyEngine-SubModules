#include "BEpch.h"
#include "MeshLoad.h"

#include <fstream>
#include <yaml-cpp/yaml.h>


namespace BE {

	std::vector<MeshSource> MeshLoad::loadModel(std::string const& path)
	{

        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
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

    std::vector<MeshSource> MeshLoad::loadDynamicMesh(std::string const& path)
    {
        return loadModel(path);
    }

    MeshSource MeshLoad::loadStaticMesh(std::string const& path)
    {
        std::vector<float> verticesDatas;
        std::vector<uint32_t> indicesDatas;

        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
        //const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);
        scene = importer.ApplyPostProcessing(aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            //cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            BE_CORE_ASSERT("ERROR::ASSIMP:: ", importer.GetErrorString());
        }

 
        processNode(scene->mRootNode, scene, &verticesDatas, &indicesDatas);

        auto startPos = path.find_last_of('\\');
        auto endPos = path.find_last_of('.');
        std::string name;
        if (startPos == std::string::npos || endPos == std::string::npos) {
            name = "UntitledMesh";
        }
        else {
            name = path.substr(startPos + 1, endPos - startPos - 1);
        }

        std::vector<BufferElement> elements = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float3, "a_Tangent"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float2, "a_TexCoord0"},

        };
        float* verticesData = new float[verticesDatas.size()];
        if (!verticesDatas.empty())
        {
            memcpy(verticesData, &verticesDatas[0], verticesDatas.size() * sizeof(float));
        }
        uint32_t* indicesData = new uint32_t[indicesDatas.size()];
        if (!verticesDatas.empty())
        {
            memcpy(indicesData, &indicesDatas[0], indicesDatas.size() * sizeof(uint32_t));
        }

        MeshSource meshSource(name, elements, verticesData, (verticesDatas.size() * sizeof(float)), indicesData, indicesDatas.size());
        return meshSource;
    }

  /*  YAML::Emitter& operator<<(YAML::Emitter& out, const Vertex& v) {
        out << v.Position[0] << v.Position[1] << v.Position[2]
            << v.Normal[0] << v.Normal[1] << v.Normal[2]
            << v.TexCoords[0] << v.TexCoords[1]
            << v.Tangent[0] << v.Tangent[1] << v.Tangent[2];
        return out;
    }*/
    /*
    void MeshLoad::SerializeMesh(const std::string& filepath)
    {
        std::vector<MeshSource> meshes = loadModel(filepath);
        MeshSource meshSource = meshes.at(0);

        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Type";
        out << YAML::Value << "MeshSource";
        out << YAML::Key << "MeshName";
        auto startPos = filepath.find_last_of('\\');
        auto endPos = filepath.find_last_of('.');
        std::string name;
        if (startPos == std::string::npos || endPos == std::string::npos) {
            name = "UntitledMesh";
        }
        else {
            name = filepath.substr(startPos+1, endPos - startPos-1);
        }
        out << YAML::Value << name;
        out << YAML::Key << "Vertices" << YAML::Flow << YAML::BeginSeq;

        for each (auto meshSource in meshes)
        {
            for (size_t i = 0; i < meshSource.m_Vertices.size(); i++) {
                out << meshSource.m_Vertices.at(i);
            }
        }

        out << YAML::EndSeq;

        out << YAML::Key << "Indices" << YAML::Flow << YAML::BeginSeq;

        for each (auto meshSource in meshes)
        {
            for (size_t i = 0; i < meshSource.m_Indices.size(); i++) {
                out << meshSource.m_Indices.at(i);
            }
        }

        out << YAML::EndSeq;

        out << YAML::EndMap;

        std::ofstream fout("F:\\GitHub\\BunnyEngine\\BunnyEngineEditor\\Assets\\box.BunnyMesh");
        fout << out.c_str();
    }
    */
    /*
    MeshSource MeshLoad::DeserializeMesh(const std::string& filepath)
    {
        MeshSource nullMesh;

        std::ifstream stream(filepath);
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());
        if (!data["MeshName"])
            return nullMesh;

        std::string meshName = data["MeshName"].as<std::string>();
        BE_CORE_TRACE("Deserializing Scene `{0}`", meshName);
        auto vertices = data["Vertices"];
        auto indices = data["Indices"];

        size_t verticesNum = vertices.size();
        float* verticesData = (float*)malloc(verticesNum * sizeof(float));
        for (size_t i = 0; i < verticesNum; i++) {
            std::fill(verticesData + i, verticesData + (i + 1), vertices[i].as<float>());
            float* data1 = verticesData + i;
        }

        size_t indicesNum = indices.size();
        uint32_t* indicesData = (uint32_t*)malloc(indicesNum * sizeof(uint32_t));
        for (size_t i = 0; i < indicesNum; i++) {
            std::fill(indicesData + i, indicesData + (i + 1), indices[i].as<uint32_t>());
            uint32_t* data1 = indicesData + i;
        }

        MeshSource meshSource(verticesData, verticesNum * sizeof(float), indicesData, indicesNum);

        return meshSource;
    }
    */
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
    
    /*
    MeshSource MeshLoad::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        //vector<Vertex> vertices;
        //vector<unsigned int> indices;
        //vector<Texture> textures;

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
    */
    MeshSource MeshLoad::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<BufferElement> elements = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float3, "a_Tangent"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float2, "a_TexCoord0"},

        };
        
        size_t verticesInfoNum = 15;
        //elements.push_back({ ShaderDataType::Float3, "a_Position" });
        bool hasNormals = mesh->HasNormals();
        //if (hasNormals) {
        //    elements.push_back({ ShaderDataType::Float3, "a_Normal" });
        //    verticesInfoNum += 3;
        //}
        bool hasTexCoord0 = mesh->HasTextureCoords(0);
        //if (hasTexCoord) {
        //    elements.push_back({ ShaderDataType::Float3, "a_TexCoord" });
        //    verticesInfoNum += 2;
        //}
        bool hasTangent = mesh->HasTangentsAndBitangents();
        //if (hasTangent) {
        //    elements.push_back({ ShaderDataType::Float3, "a_Tangent" });
        //    verticesInfoNum += 3;
        //}
        bool hasColor = mesh->HasVertexColors(0);

        std::string meshName = mesh->mName.C_Str();
        size_t numVertices = (size_t)mesh->mNumVertices;

        float* verticesData = (float*)malloc(verticesInfoNum * numVertices * sizeof(float));
        float* currentData = verticesData;

        for (size_t i = 0; i < numVertices; i++) {
            //////////Position
            std::fill(currentData, currentData + 1, mesh->mVertices[i].x);
            currentData++;
            std::fill(currentData, currentData + 1, mesh->mVertices[i].y);
            currentData++;
            std::fill(currentData, currentData + 1, mesh->mVertices[i].z);
            currentData++;
            //////////Normal
            if (hasNormals) {
                std::fill(currentData, currentData + 1, mesh->mNormals[i].x);
                currentData++;
                std::fill(currentData, currentData + 1, mesh->mNormals[i].y);
                currentData++;
                std::fill(currentData, currentData + 1, mesh->mNormals[i].z);
                currentData++;
            }
            else {
                std::fill(currentData, currentData + 1, 0);
                currentData++;
                std::fill(currentData, currentData + 1, 0);
                currentData++;
                std::fill(currentData, currentData + 1, 0);
                currentData++;
            }
            //////////Tangent
            if (hasTangent) {
                std::fill(currentData, currentData + 1, mesh->mTangents[i].x);
                currentData++;
                std::fill(currentData, currentData + 1, mesh->mTangents[i].y);
                currentData++;
                std::fill(currentData, currentData + 1, mesh->mTangents[i].z);
                currentData++;
            }
            else {
                std::fill(currentData, currentData + 1, 0);
                currentData++;
                std::fill(currentData, currentData + 1, 0);
                currentData++;
                std::fill(currentData, currentData + 1, 0);
                currentData++;
            }
            //////////VertexColor
            if (hasColor) {
                std::fill(currentData, currentData + 1, mesh->mColors[0][i].r);
                currentData++;
                std::fill(currentData, currentData + 1, mesh->mColors[0][i].g);
                currentData++;
                std::fill(currentData, currentData + 1, mesh->mColors[0][i].b);
                currentData++;
                std::fill(currentData, currentData + 1, mesh->mColors[0][i].a);
                currentData++;
            }
            else {
                std::fill(currentData, currentData + 1, 1);
                currentData++;
                std::fill(currentData, currentData + 1, 1);
                currentData++;
                std::fill(currentData, currentData + 1, 1);
                currentData++;
                std::fill(currentData, currentData + 1, 1);
                currentData++;
            }
            //////////TexCoord0
            if (hasTexCoord0) {
                std::fill(currentData, currentData + 1, mesh->mTextureCoords[0][i].x);
                currentData++;
                std::fill(currentData, currentData + 1, mesh->mTextureCoords[0][i].y);
                currentData++;
            }
            else {
                std::fill(currentData, currentData + 1, 0);
                currentData++;
                std::fill(currentData, currentData + 1, 0);
                currentData++;
            }
            float* data1 = verticesData + i;
        }

        size_t facesNum = (size_t)mesh->mNumFaces;
        uint32_t* indicesData = (uint32_t*)malloc(facesNum* 3 * sizeof(uint32_t));
        uint32_t* currentIndexData = indicesData;

        for (size_t i = 0; i < facesNum; i++) {
            aiFace face = mesh->mFaces[i];
            std::fill(currentIndexData, currentIndexData + 1, (uint32_t)face.mIndices[0]);
            currentIndexData++;
            std::fill(currentIndexData, currentIndexData + 1, (uint32_t)face.mIndices[1]);
            currentIndexData++;
            std::fill(currentIndexData, currentIndexData + 1, (uint32_t)face.mIndices[2]);
            currentIndexData++;
            uint32_t* data1 = indicesData + i;
        }
          
        MeshSource meshSource(meshName, elements,verticesData, (verticesInfoNum * numVertices * sizeof(float)), indicesData, facesNum * 3);
        return meshSource;
    }

    /// <summary>
    /// ////////////////////////////////////  Load Static Mesh; Only One Mesh;
    /// </summary>
    /// <param name="node"></param>
    /// <param name="scene"></param>
    /// <param name="meshe"></param>

    void MeshLoad::processNode(aiNode* node, const aiScene* scene, std::vector<float>* verticesDatas, std::vector<uint32_t>* indicesDatas)
    {
        
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene, verticesDatas, indicesDatas);
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene, verticesDatas, indicesDatas);
        }
    }

    void MeshLoad::processMesh(aiMesh* mesh, const aiScene* scene, std::vector<float>* verticesDatas, std::vector<uint32_t>* indicesDatas)
    {
        size_t verticesInfoNum = 15;
        uint32_t vertexNum = uint32_t((verticesDatas->size())/ verticesInfoNum);

        bool hasNormals = mesh->HasNormals();

        bool hasTexCoord0 = mesh->HasTextureCoords(0);

        bool hasTangent = mesh->HasTangentsAndBitangents();

        bool hasColor = mesh->HasVertexColors(0);

        std::string meshName = mesh->mName.C_Str();
        size_t numVertices = (size_t)mesh->mNumVertices;

        for (size_t i = 0; i < numVertices; i++) {
            //////////Position
            verticesDatas->push_back(mesh->mVertices[i].x);
            verticesDatas->push_back(mesh->mVertices[i].y);
            verticesDatas->push_back(mesh->mVertices[i].z);

            //////////Normal
            if (hasNormals) {
                verticesDatas->push_back(mesh->mNormals[i].x);
                verticesDatas->push_back(mesh->mNormals[i].y);
                verticesDatas->push_back(mesh->mNormals[i].z);
            }
            else {
                verticesDatas->push_back(0);
                verticesDatas->push_back(0);
                verticesDatas->push_back(1);
            }
            //////////Tangent
            if (hasTangent) {
                verticesDatas->push_back(mesh->mTangents[i].x);
                verticesDatas->push_back(mesh->mTangents[i].y);
                verticesDatas->push_back(mesh->mTangents[i].z);
            }
            else {
                verticesDatas->push_back(0);
                verticesDatas->push_back(1);
                verticesDatas->push_back(0);
            }
            //////////VertexColor
            if (hasColor) {
                verticesDatas->push_back(mesh->mColors[0][i].r);
                verticesDatas->push_back(mesh->mColors[0][i].g);
                verticesDatas->push_back(mesh->mColors[0][i].b);
                verticesDatas->push_back(mesh->mColors[0][i].a);
            }
            else {
                verticesDatas->push_back(1);
                verticesDatas->push_back(1);
                verticesDatas->push_back(1);
                verticesDatas->push_back(1);
            }
            //////////TexCoord0
            if (hasTexCoord0) {
                verticesDatas->push_back(mesh->mTextureCoords[0][i].x);
                verticesDatas->push_back(mesh->mTextureCoords[0][i].y);
            }
            else {
                verticesDatas->push_back(0);
                verticesDatas->push_back(0);
            }
        }

        size_t facesNum = (size_t)mesh->mNumFaces;

        for (size_t i = 0; i < facesNum; i++) {
            aiFace face = mesh->mFaces[i];
            indicesDatas->push_back((uint32_t)face.mIndices[0]+ vertexNum);
            indicesDatas->push_back((uint32_t)face.mIndices[1]+ vertexNum);
            indicesDatas->push_back((uint32_t)face.mIndices[2]+ vertexNum);
        }
        
    }

}