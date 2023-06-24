#pragma once
#include "BunnyEngine/Renderer/RendererAPI.h"
#include "BunnyEngine/Renderer/Renderer.h"

namespace BE {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void BeginScene() override; 
		virtual void DepthLESS() override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void Clear(const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

		virtual void Postprocess() override;
	};

}


