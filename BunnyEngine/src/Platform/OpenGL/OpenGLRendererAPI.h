#pragma once
#include "BunnyEngine/Renderer/RendererAPI.h"

namespace BE {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void Clear(const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	};

}


