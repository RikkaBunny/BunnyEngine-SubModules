#include "BEpch.h"
#include "Mesh.h"

#include "../assimp/include/assimp/Importer.hpp"
#include "../assimp/include/assimp/scene.h"
#include "../assimp/include/assimp/postprocess.h"

namespace BE {



	void Mesh::SetMeshSource()
	{

		//m_MeshSource = MeshLoad::loadModel("Resources/Mesh/box.obj");
		//m_MeshSource = MeshLoad::loadModel("Resources/Mesh/pighead.obj");
		m_MeshSource = MeshLoad::loadModel("Resources/Mesh/pighead.fbx");
		//m_MeshSource = MeshLoad::loadModel("Resources/Mesh/nanosuit/nanosuit.obj");

		/*m_VertexArray = VertexArray::Create();

		float vertices[4 * 11] = {
			-0.5f,-0.5f,0.0f, 0.0f,0.0f,1.0f, 0.0f,0.0f, 0.707107f,0.707107f,0.0f,
			0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f, 1.0f,0.0f, -0.707107f,0.707107f,0.0f,
			0.5f, 0.5f, 0.0f, 0.0f,0.0f,1.0f, 1.0f,1.0f, -0.707107f,-0.707107f,0.0f,
			-0.5f, 0.5f, 0.0f, 0.0f,0.0f,1.0f, 0.0f,1.0f, 0.707107f,-0.707107f,0.0f
		};

		Ref<VertexBuffer> m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float3, "a_Normal"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float3, "a_Tangent"}
		};

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<IndexBuffer> m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);*/

	}

	
}