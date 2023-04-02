#pragma once

#include "BunnyEngine.h"
#include "Panels/SceneHierarchyPanel.h"
#include "BunnyEngine/Renderer/EditorCamera.h"

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
		bool OnKeyPressed(KeyPressedEvent& event);
		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:

		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;
		Ref<Texture2D> m_Texture;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;

		Ref<Framebuffer> m_Framebuffer;

		OrthographicCameraController m_CameraController;

		glm::vec4 m_SquareColor = glm::vec4(1, 1, 1, 1);

		glm::vec2 m_ViewportSize = { 0.0f,0.0f };

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		int m_GizmoType = -1;
		//Panels 
		SceneHierarchyPanel m_SceneHierarchyPanel;

		EditorCamera m_EditorCamera;
	};
}

