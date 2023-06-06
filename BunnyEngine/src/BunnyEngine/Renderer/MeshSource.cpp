#include "BEpch.h"
#include "MeshSource.h"

namespace BE {
	MeshSource::MeshSource()
	{
	}
	MeshSource::MeshSource(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
		:m_Vertices(vertices), m_Indices(indices)
	{
		SetMeshSource();
	}
	void MeshSource::SetMeshSource()
	{
		m_VertexArray = VertexArray::Create();
		size_t num = m_Vertices.size();
		float* verticesData = (float*)malloc(num * 11 * sizeof(float));
		for (size_t i = 0; i < num; i++) {
			std::fill(verticesData + (i * 11 + 0), verticesData + (i * 11 + 1), m_Vertices.at(i).Position[0]);
			std::fill(verticesData + (i * 11 + 1), verticesData + (i * 11 + 2), m_Vertices.at(i).Position[1]);
			std::fill(verticesData + (i * 11 + 2), verticesData + (i * 11 + 3), m_Vertices.at(i).Position[2]);
			std::fill(verticesData + (i * 11 + 3), verticesData + (i * 11 + 4), m_Vertices.at(i).Normal[0]);
			std::fill(verticesData + (i * 11 + 4), verticesData + (i * 11 + 5), m_Vertices.at(i).Normal[1]);
			std::fill(verticesData + (i * 11 + 5), verticesData + (i * 11 + 6), m_Vertices.at(i).Normal[2]);
			std::fill(verticesData + (i * 11 + 6), verticesData + (i * 11 + 7), m_Vertices.at(i).TexCoords[0]);
			std::fill(verticesData + (i * 11 + 7), verticesData + (i * 11 + 8), m_Vertices.at(i).TexCoords[1]);
			std::fill(verticesData + (i * 11 + 8), verticesData + (i * 11 + 9), m_Vertices.at(i).Tangent[0]);
			std::fill(verticesData + (i * 11 + 9), verticesData + (i * 11 + 10), m_Vertices.at(i).Tangent[1]);
			std::fill(verticesData + (i * 11 + 10), verticesData + (i * 11 + 11), m_Vertices.at(i).Tangent[2]);
			float* data = verticesData + (i * 11 + 0);
		}

		m_VertexBuffer = VertexBuffer::Create(verticesData, (num * 11 * sizeof(float)));

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float3, "a_Normal"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float3, "a_Tangent"}
		};
		//layout.AddElement({ ShaderDataType::Float2, "a_TexCoord" });

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		size_t indicesNum = m_Indices.size();
		uint32_t* indicesData = (uint32_t*)malloc(indicesNum * sizeof(uint32_t));
		for (size_t i = 0; i < indicesNum; i++) {
			std::fill(indicesData + i, indicesData + (i + 1), m_Indices.at(i));
			uint32_t* data = indicesData + i;
		}
		m_IndexBuffer = IndexBuffer::Create(indicesData, indicesNum);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}
	/*void SetMeshSource()
	{
		m_VertexArray = VertexArray::Create();

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
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


	}*/
}
