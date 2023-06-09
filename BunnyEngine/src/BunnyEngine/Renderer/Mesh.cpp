#include "BEpch.h"
#include "Mesh.h"


#include "../assimp/include/assimp/Importer.hpp"
#include "../assimp/include/assimp/scene.h"
#include "../assimp/include/assimp/postprocess.h"

#include "BunnyEngine/Utils/PlatformUtils.h"

namespace BE {

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

	void Mesh::SetMeshSource()
	{

		std::string filepath = FileDialogs::OpenFile("Mesh Files (*.obj;*.fbx)\0*.fbx;*.obj\0*");
		if (!filepath.empty()) {
			//m_MeshSource = MeshLoad::DeserializeMesh(filepath);
			m_MeshSource = MeshLoad::loadStaticMesh(filepath);
			//m_MeshSource = MeshLoad::loadDynamicMesh(filepath).at(0);
		}


	}

	
}