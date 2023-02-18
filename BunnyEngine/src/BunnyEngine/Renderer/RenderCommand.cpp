#include "BEpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace BE {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}