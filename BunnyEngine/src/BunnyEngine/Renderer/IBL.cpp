#include "BEpch.h"
#include "IBL.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLIBL.h"

namespace BE {



	Ref<IBL> IBL::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIBL>(path);
		}

		BE_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

}
