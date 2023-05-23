#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace BE {

	class Renderer {
	public:
		enum class RenderPipeline
		{
			ForwardRendering = 0,
			DeferredRendering = 1
		};
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene(OrthographicCamera& camera);

		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& worldTransform = glm::mat4(1.0f));

		static void Postprocess();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		inline static RenderPipeline GetRenderPipeline() { return s_RenderPipeline; }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;

		static RenderPipeline s_RenderPipeline;
	};

}


