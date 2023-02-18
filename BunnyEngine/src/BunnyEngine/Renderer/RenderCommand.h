#pragma once

#include "RendererAPI.h"

namespace BE {

	class RenderCommand {
	public:
		inline static void Init() {
			s_RendererAPI->Init();
		}

		inline static void Clear(const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)) {
			s_RendererAPI->Clear(color);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
			s_RendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}
