#pragma once
#include "VertexArray.h"

namespace BE {

    struct Vertex {
        // position
        float Position[3];
        // normal
        float Normal[3];
        // texCoords
		float TexCoords[2];
		//float TexCoords[2*8];
        // tangent
        float Tangent[3];
        // bitangent
        //float Bitangent[3];
        
    };

	class MeshSource
	{
	
	public:
		MeshSource();
		MeshSource(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
		Ref<VertexArray> GetVertexArray() { return m_VertexArray; }
	private:
		void SetMeshSource();

	public:
		// mesh Data
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		uint32_t m_Index;

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

	};

}