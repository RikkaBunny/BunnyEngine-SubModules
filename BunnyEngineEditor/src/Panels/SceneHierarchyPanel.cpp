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



		ImGui::Begin("Properties");
		if (m_SelectionContext) {
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
		
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

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
	
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100) {
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		float lineHeight = ImGui::GetFontSize() + GImGui->Style.FramePadding.y*2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.75f,0.15f,0.1f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f,0.2f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f,0.15f,0.1f,1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.15f,0.75f,0.1f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.9f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15f,0.75f,0.1f,1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.15f,0.1f,0.75f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.2f,0.9f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15f,0.1f,0.75f,1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<NameComponent>()) {
			auto& name = entity.GetComponent<NameComponent>().Name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), name.c_str());
			if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {
				name = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("Add Component");

		if (ImGui::BeginPopup("Add Component")) {
			if (ImGui::MenuItem("Camera")) {
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Sprite Renderer")) {
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Quad Renderer")) {
				m_SelectionContext.AddComponent<QuadRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Direction Light")) {
				m_SelectionContext.AddComponent<DirctionLightComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Material")) {
				m_SelectionContext.AddComponent<MaterialComponent>();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale);
			});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
			auto& camera = component.Camera;

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			ImGui::Checkbox("Main Camera", &component.Main);
			ImGui::Checkbox("Fixed AspectRatio", &component.FixedAspectRatio);

			if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
				float perspectiveFOV = glm::degrees(camera.GetPerspectiveFOV());
				if (ImGui::DragFloat("FOV", &perspectiveFOV))
					camera.SetPerspectiveFOV(glm::radians(perspectiveFOV));

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					camera.SetPerspectiveNearClip(perspectiveNear);

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					camera.SetPerspectiveFarClip(perspectiveFar);
			}
			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetOrthographicFarClip(orthoFar);
			}
			});

		DrawComponent<SpriteRendererComponent>("SpriteRenderer", entity, [](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

			ImGui::Button("Texture", ImVec2(100.0f, 100.0f));
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					const wchar_t* path = (const wchar_t*)payload->Data;
					auto texturePath = std::filesystem::path(g_AssetsPath) / path;
					component.Texture = Texture2D::Create(texturePath.string());
				}
				ImGui::EndDragDropTarget();

			}

			ImGui::DragFloat("Tiling", &component.Tiling, 0.1f, 0.0f, 10.0f);
			});

		DrawComponent<QuadRendererComponent>("QuadRenderer", entity, [&](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

			//ImGui::Button("AlbedoTexture", ImVec2(100.0f, 100.0f));
			
			ImGui::ImageButton((ImTextureID)component.u_AlbedoTexture->GetRendererID(), { m_ThumbnailSize, m_ThumbnailSize }, { 0, 1 }, { 1, 0 });
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					const wchar_t* path = (const wchar_t*)payload->Data;
					auto texturePath = std::filesystem::path(g_AssetsPath) / path;
					component.u_AlbedoTexture = Texture2D::Create(texturePath.string());
				}
				ImGui::EndDragDropTarget();

			}
			ImGui::SameLine();
			ImGui::Text("AlbedoTexture");

			//ImGui::Button("NormalTexture", ImVec2(100.0f, 100.0f));
			ImGui::ImageButton((ImTextureID)component.u_NormalTexture->GetRendererID(), { m_ThumbnailSize, m_ThumbnailSize }, { 0, 1 }, { 1, 0 });
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					const wchar_t* path = (const wchar_t*)payload->Data;
					auto texturePath = std::filesystem::path(g_AssetsPath) / path;
					component.u_NormalTexture = Texture2D::Create(texturePath.string());
				}
				ImGui::EndDragDropTarget();

			}
			ImGui::SameLine();
			ImGui::Text("NormalTexture");

			ImGui::DragFloat("Metallic", &component.Metallic, 0.1f, 0.0f, 10.0f);
			//ImGui::Button("MetallicTexture", ImVec2(100.0f, 100.0f));
			ImGui::ImageButton((ImTextureID)component.u_MetallicTexture->GetRendererID(), { m_ThumbnailSize, m_ThumbnailSize }, { 0, 1 }, { 1, 0 });
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					const wchar_t* path = (const wchar_t*)payload->Data;
					auto texturePath = std::filesystem::path(g_AssetsPath) / path;
					component.u_MetallicTexture = Texture2D::Create(texturePath.string());
				}
				ImGui::EndDragDropTarget();

			}
			ImGui::SameLine();
			ImGui::Text("MetallicTexture");

			ImGui::DragFloat("Roughness", &component.Roughness, 0.1f, 0.0f, 10.0f);
			//ImGui::Button("RoughnessTexture", ImVec2(100.0f, 100.0f));
			ImGui::ImageButton((ImTextureID)component.u_RoughnessTexture->GetRendererID(), { m_ThumbnailSize, m_ThumbnailSize }, { 0, 1 }, { 1, 0 });
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					const wchar_t* path = (const wchar_t*)payload->Data;
					auto texturePath = std::filesystem::path(g_AssetsPath) / path;
					component.u_RoughnessTexture = Texture2D::Create(texturePath.string());
				}
				ImGui::EndDragDropTarget();

			}
			ImGui::SameLine();
			ImGui::Text("RoughnessTexture");

			//ImGui::Button("AoTexture", ImVec2(100.0f, 100.0f));
			ImGui::ImageButton((ImTextureID)component.u_AoTexture->GetRendererID(), { m_ThumbnailSize, m_ThumbnailSize }, { 0, 1 }, { 1, 0 });
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					const wchar_t* path = (const wchar_t*)payload->Data;
					auto texturePath = std::filesystem::path(g_AssetsPath) / path;
					component.u_AoTexture = Texture2D::Create(texturePath.string());
				}
				ImGui::EndDragDropTarget();

			}
			ImGui::SameLine();
			ImGui::Text("AoTexture");

			ImGui::DragFloat("Emissive", &component.Emissive, 0.1f, 0.0f, 10.0f);

			ImGui::DragFloat("Tiling", &component.Tiling, 0.1f, 0.0f, 10.0f);
			});

		DrawComponent<DirctionLightComponent>("DirctionLight", entity, [](auto& component) {
			ImGui::ColorEdit3("DirctionColor", glm::value_ptr(component.DirctionColor));

			ImGui::DragFloat("DirctionIntersity", &component.DirctionIntersity, 0.1f, 0.0f, 10.0f);
			});

		DrawComponent<MaterialComponent>("Material", entity, [](auto& component) {

			std::string curShader = component.Mat.GetShader() ? component.Mat.GetShader()->GetName() : "UnShader";
			if (ImGui::BeginMenu(curShader.c_str()))
			{
				for (auto kv : component.Mat.GetShaderLibray()) {
					std::string shaderName = kv.first;
					if (ImGui::MenuItem(shaderName.c_str()) && curShader != shaderName) {
						component.Mat.SetShader(kv.second);
					}
				}

				ImGui::EndMenu();
			}
			if (!component.Mat.GetShader())
				return;

			auto m_ShaderParameters = component.Mat.GetShader()->GetShaderParameter();
			for (size_t  i = 0; i < m_ShaderParameters.size(); i++) {
				ShaderParameter m_ShaderParameter = m_ShaderParameters.at(i);
				switch (m_ShaderParameter.InputType)
				{
				case BE::ShaderInputType::None:
					break;
				case BE::ShaderInputType::Float: {
					std::string name = m_ShaderParameter.ParameterName;
					float value = std::stof(m_ShaderParameter.ParameterVlaue);
					ImGui::DragFloat(name.c_str(), &std::get<float>(component.Mat.m_ShaderParameters.at(i)), 0.1f);
					break;
				}
				case BE::ShaderInputType::Float2: {
					std::string name = m_ShaderParameter.ParameterName;
					glm::vec2* value = &std::get<glm::vec2>(component.Mat.m_ShaderParameters[i]);
					ImGui::DragFloat2(name.c_str(), glm::value_ptr(*value), 0.1f);
					break;
				}
				case BE::ShaderInputType::Float3: {
					std::string name = m_ShaderParameter.ParameterName;
					glm::vec3* value = &std::get<glm::vec3>(component.Mat.m_ShaderParameters[i]);
					ImGui::DragFloat3(name.c_str(), glm::value_ptr(*value), 0.1f);
					break;
				}
				case BE::ShaderInputType::Float4: {
					std::string name = m_ShaderParameter.ParameterName;
					glm::vec4* value = &std::get<glm::vec4>(component.Mat.m_ShaderParameters[i]);
					ImGui::DragFloat4(name.c_str(), glm::value_ptr(*value), 0.1f);
					break;
				}
				case BE::ShaderInputType::Mat3: {
					std::string name = m_ShaderParameter.ParameterName;
					glm::mat3* value = &std::get<glm::mat3>(component.Mat.m_ShaderParameters[i]);
					ImGui::DragScalarN(name.c_str(), ImGuiDataType_Float, glm::value_ptr(*value), 9, 0.1f);
					break;
				}
				case BE::ShaderInputType::Mat4: {
					std::string name = m_ShaderParameter.ParameterName;
					glm::mat4* value = &std::get<glm::mat4>(component.Mat.m_ShaderParameters[i]);
					ImGui::DragScalarN(name.c_str(), ImGuiDataType_Float, glm::value_ptr(*value), 16, 0.1f);
					break;
				}
				case BE::ShaderInputType::Int: {
					std::string name = m_ShaderParameter.ParameterName;
					//int* value = &std::get<int>(component.Mat.m_ShaderParameters[i]);
					ImGui::DragInt(name.c_str(), &std::get<int>(component.Mat.m_ShaderParameters[i]), 0.1f);
					break;
				}
				case BE::ShaderInputType::Bool: {
					std::string name = m_ShaderParameter.ParameterName;
					//bool* value = &std::get<bool>(component.Mat.m_ShaderParameters[i]);
					ImGui::Checkbox(name.c_str(), &std::get<bool>(component.Mat.m_ShaderParameters[i]));
					break;
				}
				case BE::ShaderInputType::Texture2D: {
					std::string name = m_ShaderParameter.ParameterName;
					Ref<Texture2D> value = std::get<Ref<Texture2D>>(component.Mat.m_ShaderParameters[i]);
					float m_ThumbnailSize = 80.0f;
					ImGui::ImageButton((ImTextureID)value->GetRendererID(), { m_ThumbnailSize, m_ThumbnailSize }, { 0, 1 }, { 1, 0 });
					if (ImGui::BeginDragDropTarget()) {
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
							const wchar_t* path = (const wchar_t*)payload->Data;
							auto texturePath = std::filesystem::path(g_AssetsPath) / path;
							component.Mat.m_ShaderParameters[i] = Texture2D::Create(texturePath.string());
						}
						ImGui::EndDragDropTarget();

					}
					ImGui::SameLine();
					ImGui::Text(name.c_str());
					//ImGui::DragFloat3(name.c_str(), glm::value_ptr(*value), 0.1f);
					break;
				}
				//default:
				//	break;
				}
			}
			/*m_ShaderParameter->pop_back()
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale);*/
			});
	}

}