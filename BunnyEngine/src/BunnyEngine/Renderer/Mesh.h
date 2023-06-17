#pragma once
#include "VertexArray.h"
#include "../Utils/MeshLoad.h"
#include<filesystem>

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
		void SetMeshSource(std::string filepath);
		Ref<VertexArray> GetMeshSource() { return m_MeshSource.GetVertexArray(); }

	private:
		Ref<VertexArray> m_VertexArray;
		MeshSource m_MeshSource;
	};

	class MeshLibray{
	public:
		//static void Add(const std::string& name, const std::string& meshPath);

		static MeshSource Get(const std::string& name);

		static const std::vector<std::string> GetMeshLibray() { return s_MeshPaths; }
		static const std::vector<const char*> GetMeshNames() { return s_MeshNames; }
		static void UpdateMeshLibray();
		static void UpdateMeshNames();
	private:
		static void FileIterator(std::filesystem::path filePath);
	private:
		static std::vector<std::string> s_MeshPaths;
		static std::vector<const char*> s_MeshNames;
	};
}