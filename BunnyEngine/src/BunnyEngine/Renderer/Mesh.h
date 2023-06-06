#pragma once
#include "VertexArray.h"
#include "../Utils/MeshLoad.h"

namespace BE {
	class Mesh
	{
	public:
		//Mesh();
		void SetMeshSource();
		Ref<VertexArray> GetMeshSource() { return m_MeshSource.at(0).GetVertexArray(); }

	private:
		Ref<VertexArray> m_VertexArray;
		std::vector<MeshSource> m_MeshSource;
	};
}