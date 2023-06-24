#pragma once

#include "../imgui/imgui.h"
#include<filesystem>
#include "BunnyEngine.h"
#include <BunnyEngine/Scene/SceneSerializer.h>
#include "BunnyEngine/Utils/PlatformUtils.h"

#include "SceneHierarchyPanel.h"
#include "ContentBrowserPanel.h"
#include "ViewportPanel.h"
#include "PropertiesPanel.h"

namespace BE {

	class DockSpace
	{
	public:
		DockSpace(){};
		//DockSpace( SceneHierarchyPanel* sceneHierarchyPanel,  ContentBrowserPanel* contentBrowserPanel);
		//void SetContext( SceneHierarchyPanel* sceneHierarchyPanel,  ContentBrowserPanel* contentBrowserPanel);
		void OnAttcah() ;

		void SetContext(SceneHierarchyPanel* sceneHierarchyPanel, PropertiesPanel* propertiesPanel, ContentBrowserPanel* contentBrowserPanel, ViewportPanel* viewportPanel_0);

		void OnDetach() ;
		void OnUpdate() ;
		void OnImGuiRender() ;

		void OnEvent(Event& event);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();

		Ref<Scene> GetActiveScene() { return this->m_ActiveScene; }
		SceneHierarchyPanel* GetSceneHierarchyPanel() { return this->m_SceneHierarchyPanel; }
		PropertiesPanel* GetPropertiesPanel() { return this->m_PropertiesPanel; }
		ContentBrowserPanel* GetContentBrowserPanel() { return this->m_ContentBrowserPanel; }
		ViewportPanel* GetViewportPanel(int viewportPanelIndex = 0) { return this->m_ViewportPanel_0; }

	private:
		bool OnKeyPressed(KeyPressedEvent& event);

	private:
		Ref<Scene> m_ActiveScene = std::make_shared<Scene>();

		SceneHierarchyPanel* m_SceneHierarchyPanel;
		PropertiesPanel* m_PropertiesPanel;
		ContentBrowserPanel* m_ContentBrowserPanel;
		ViewportPanel* m_ViewportPanel_0;

		ImVec2 m_WindowPos = ImVec2(100, 100);
		ImVec2 m_WindowSize = ImVec2(800, 600);

		ImVec2 m_CurWindowPos = ImVec2(100, 100);
		ImVec2 m_CurWindowSize = ImVec2(800, 600);

		bool m_Drag = false;

		float m_MenuBarHeight = 8.5f;

		Ref<Texture2D> m_BEIcon = Texture2D::Create("Resources/Icons/Common/BunnyIcon.png");
		Ref<Texture2D> m_MinIcon = Texture2D::Create("Resources/Icons/Common/min.png");
		Ref<Texture2D> m_MaxIcon = Texture2D::Create("Resources/Icons/Common/max.png");
		Ref<Texture2D> m_CloseIcon = Texture2D::Create("Resources/Icons/Common/close.png");

		WindowState m_WindowState = WindowState::NORMAL;
	};

}