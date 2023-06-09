#pragma once
#include "VertexArray.h"
#include "../Utils/MeshLoad.h"

namespace BE {

	class MeshSource
	{

	public:
		MeshSource();
		MeshSource(std::string meshName, std::vector<BufferElement> elements, float* vertices, uint32_t size, uint32_t* indices, uint32_t count);

		Ref<VertexArray> GetVertexArray() { return m_VertexArray; }
	private:

		void SetMeshSource(std::vector<BufferElement> elements, float* vertices, uint32_t size, uint32_t* indices, uint32_t count);
	public:
		std::vector<BufferElement> m_Elements;
		std::string m_MeshName;
	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

	};

	class Mesh
	{
	public:
		//Mesh();
		void SetMeshSource();
		Ref<VertexArray> GetMeshSource() { return m_MeshSource.GetVertexArray(); }

	private:
		Ref<VertexArray> m_VertexArray;
		MeshSource m_MeshSource;
	};
}