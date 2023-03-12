#include "BEpch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace BE {
	Ref<Framebuffer> BE::Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLFramebuffer>(spec);
		}

		BE_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}
}

