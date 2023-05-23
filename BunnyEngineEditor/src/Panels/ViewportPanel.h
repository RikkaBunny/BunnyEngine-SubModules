#pragma once
#include "BunnyEngine.h"

namespace BE {

	class DockSpace;
	class ViewportPanel
	{
	public:

		ViewportPanel();
		//ViewportPanel();
		//ViewportPanel(const FramebufferSpecification& fbSpec, Ref<Scene>* activeScene, SceneHierarchyPanel* sceneHierarchyPanel, EditorCamera editorCamera =EditorCamera(45.0f, 1.778f, 0.1f, 10000.0f));
		//void SetContext(const FramebufferSpecification& fbSpec, Ref<Scene>* activeScene, SceneHierarchyPanel* sceneHierarchyPanel, EditorCamera editorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 10000.0f));
		void SetContext(DockSpace* context, const FramebufferSpecification& fbSpec, EditorCamera editorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 10000.0f));
		void OnImGuiRenderer();

		void OnUpdate();

		void UI_Toolbar();
		
		bool IsViewportFocused() { return m_ViewportFocused; }
		bool IsViewportHovered() { return m_ViewportHovered; }

		Ref<Framebuffer> GetFramebuffer(int fbnum = 0) { if (fbnum==0) { return m_Framebuffer; } else { return m_Framebuffer1; } }

		glm::vec2 GetViewportSize() { return m_ViewportSize; }
		glm::vec2 GetViewportBoundsMin() { return m_ViewportBounds[0]; }
		glm::vec2 GetViewportBoundsMax() { return m_ViewportBounds[1]; }

		EditorCamera GetEditorCamera() { return m_EditorCamera; }

		Entity GetHoveredEntity() { return m_HoveredEntity; }
		float GetPixelData() { return m_PixelData; }

		void OnEvent(Event& event);

		DockSpace* GetContext() { return m_Context; }
	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

	private:

		DockSpace* m_Context;

		FramebufferSpecification fbSpec;

		Ref<Framebuffer> m_Framebuffer;
		Ref<Framebuffer> m_Framebuffer1;

		EditorCamera m_EditorCamera;

		Entity m_HoveredEntity;
		float m_PixelData;

		int m_GizmoType = -1;

		glm::vec2 m_ViewportSize = { 0.0f,0.0f };
		glm::vec2 m_ViewportBounds[2];

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		Ref<Texture2D> m_PauseIcon = Texture2D::Create("Resources/Icons/ViewportPanel/pause.png");
		Ref<Texture2D> m_PlayIcon = Texture2D::Create("Resources/Icons/ViewportPanel/play.png");

	};

}