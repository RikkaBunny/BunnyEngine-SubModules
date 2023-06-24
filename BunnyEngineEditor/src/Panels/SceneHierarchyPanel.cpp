#include "SceneHierarchyPanel.h"

#include <../imgui/imgui.h>
#include <../imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include "DockSpace.h"


namespace BE {
	extern const std::filesystem::path g_AssetsPath;

	//SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	//{
	//	SetContext(context);
	//}

	//void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	//{
	//	m_Context = context;
	//	m_SelectionContext = {};
	//}

	void SceneHierarchyPanel::SetContext(DockSpace* context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRenderer()
	{
		ImGui::Begin("Scene Hierarchy");
		m_Context->GetActiveScene()->m_Registry.each([&](auto& entityID) {
			Entity entity{ entityID, m_Context->GetActiveScene().get() };
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// Right - Click On blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false)) {
			if (ImGui::MenuItem("Create New Entity")) {
				m_Context->GetActiveScene()->CreateEntity();
			}
			ImGui::EndPopup();
		}

		ImGui::End();



		//ImGui::Begin("Properties");
		//if (m_SelectionContext) {
		//	DrawComponents(m_SelectionContext);
		//}
		//ImGui::End();
		
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& name = entity.GetComponent<NameComponent>().Name;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());

		if (ImGui::IsItemClicked()) {
			m_SelectionContext = entity;

		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity")) {
				entityDeleted = true; 
			}
			ImGui::EndPopup();
		}

		if (opened) {
			ImGui::TreePop();
		}

		if (entityDeleted) {
			if (m_SelectionContext == entity)
				m_SelectionContext = {};

			m_Context->GetActiveScene()->DestroyEntity(entity);
		}

	}

	

}