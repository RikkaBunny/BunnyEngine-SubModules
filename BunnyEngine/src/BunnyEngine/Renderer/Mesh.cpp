#include "BEpch.h"
#include "Mesh.h"


#include "../assimp/include/assimp/Importer.hpp"
#include "../assimp/include/assimp/scene.h"
#include "../assimp/include/assimp/postprocess.h"

#include "BunnyEngine/Utils/PlatformUtils.h"

namespace BE {
	extern const std::filesystem::path g_AssetsPath;

#pragma region MeshSource

	MeshSource::MeshSource()
	{
	}

	MeshSource::MeshSource(std::string meshName, std::vector<BufferElement> elements, float* vertices, uint32_t size, uint32_t* indices, uint32_t count)
		:m_MeshName(meshName), m_Elements(elements)
	{
		SetMeshSource(elements, vertices, size, indices, count);
	}

	void MeshSource::SetMeshSource(std::vector<BufferElement> elements, float* vertices, uint32_t size, uint32_t* indices, uint32_t count)
	{
		m_VertexArray = VertexArray::Create();

		m_VertexBuffer = VertexBuffer::Create(vertices, size);

		BufferLayout layout(elements);
		//layout.AddElement({ ShaderDataType::Float2, "a_TexCoord" });

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);


		m_IndexBuffer = IndexBuffer::Create(indices, count);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}
#pragma endregion

	void Mesh::SetMeshSource(std::string filepath)
	{
		//MeshLibray::UpdateMeshNames();
		//std::string filepath = FileDialogs::OpenFile("Mesh Files (*.obj;*.fbx)\0*.fbx;*.obj\0*");
		//if (!filepath.empty()) {
		//	//m_MeshSource = MeshLoad::DeserializeMesh(filepath);
		//	m_MeshSource = MeshLoad::loadStaticMesh(filepath);
		//	//m_MeshSource = MeshLoad::loadDynamicMesh(filepath).at(0);
		//}
		m_MeshSource = MeshLoad::loadStaticMesh(filepath);


	}

	std::vector<std::string> MeshLibray::s_MeshPaths;
	std::vector<const char*> MeshLibray::s_MeshNames;

	MeshSource MeshLibray::Get(const std::string& name)
	{
		return MeshSource();
	}

	void MeshLibray::UpdateMeshLibray()
	{
		s_MeshPaths.clear();
		MeshLibray::FileIterator(g_AssetsPath);
	}
	void MeshLibray::UpdateMeshNames()
	{
		UpdateMeshLibray();
		s_MeshNames.clear();
		s_MeshNames.reserve(s_MeshPaths.size());
		
		for (const auto& str : s_MeshPaths) {
			s_MeshNames.push_back(str.c_str());
		}
	}

	void MeshLibray::FileIterator(std::filesystem::path filePath)
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(filePath)) {
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetsPath);
			std::string filenameString = relativePath.filename().string();

			if (directoryEntry.is_directory()) {
				FileIterator(path);

			}
			else {

				std::string name = filenameString.substr(filenameString.size() - 3, 3);
				if (name == "fbx" || name == "obj" || name == "FBX" || name == "OBJ") {
					s_MeshPaths.push_back(path.string());
				}
			}
		}
	}


}


