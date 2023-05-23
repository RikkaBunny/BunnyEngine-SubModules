#pragma once
# include <../imgui/imgui.h>
# include <../imgui/imgui_internal.h>
# include "../imgui-node-editor/imgui_node_editor.h"

namespace ax::NodeEditor::BE {
	class NodeEditorPanel
	{
	public:
		NodeEditorPanel() {};
		//DockSpace( SceneHierarchyPanel* sceneHierarchyPanel,  ContentBrowserPanel* contentBrowserPanel);
		//void SetContext( SceneHierarchyPanel* sceneHierarchyPanel,  ContentBrowserPanel* contentBrowserPanel);
		void OnAttcah();

		//void SetContext(SceneHierarchyPanel* sceneHierarchyPanel, ContentBrowserPanel* contentBrowserPanel, ViewportPanel* viewportPanel_0);

		void OnDetach();
		void OnUpdate();
		void OnImGuiRender();

		//void OnEvent(Event& event);

		struct LinkInfo
		{
			LinkId Id;
			PinId  InputId;
			PinId  OutputId;
		};

		EditorContext* m_Context = nullptr;

		ImVector<LinkInfo>   m_Links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
		int                  m_NextLinkId = 100;     // Counter to help generate link ids. In real application this

	};
}
