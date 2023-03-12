#pragma once

#include "BunnyEngine.h"

namespace BE {
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttcah() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;
	private:

		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;
		Ref<Texture2D> m_Texture;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;

		Ref<Framebuffer> m_Framebuffer;

		OrthographicCameraController m_CameraController;

		glm::vec4 m_SquareColor = glm::vec4(1, 1, 1, 1);

		glm::vec2 m_ViewportSize = { 0.0f,0.0f };

		bool m_ViewportFocused = false, m_ViewportHovered = false;

	};
}

