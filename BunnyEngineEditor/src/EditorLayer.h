#pragma once

#include "BunnyEngine.h"
#include "Panels/DockSpace.h"
#include "Panels/NodeEditorPanel.h"
# include "../imgui-node-editor/imgui_node_editor.h"

namespace ed = ax::NodeEditor::BE;
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
		void EditorSelectEntity();
	private:

		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;
		Ref<Texture2D> m_Texture;

		
		Entity m_SquareEntity;
		Entity m_CameraEntity;

		OrthographicCameraController m_CameraController;


		//Panels 
		DockSpace m_DockSpace;
		ViewportPanel m_ViewportPanel;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		PropertiesPanel m_PropertiesPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
		ed::NodeEditorPanel m_NodeEditorPanel;
	};
}

