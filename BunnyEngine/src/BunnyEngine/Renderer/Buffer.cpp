#include "BEpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "BunnyEngine/Renderer/Light.h"
#include "BunnyEngine/Scene/Scene.h"
#include "glm/glm.hpp"


namespace BE {
	OutBuffer::VisibleBuffer OutBuffer::m_OutBuffer = OutBuffer::VisibleBuffer::FinalBuffer;
	//OpenGLUniformBuffer UniformBuffer::*openGLUniformBuffer ;

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		BE_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		BE_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}



}


