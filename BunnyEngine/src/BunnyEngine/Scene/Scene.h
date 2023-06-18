#pragma once
#include "entt.hpp"

#include "BunnyEngine/Renderer/EditorCamera.h"

namespace BE {

	enum class ScneneStates
	{
		Ediotr = 0,
		Runtime = 1
	};

	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		entt::registry& Reg() { return m_Registry; }
		Entity CreateEntity(const std::string& name = "NewGameEntity");
		void DestroyEntity(Entity entity);
		void OnUpdateEditor(EditorCamera& camera);
		void OnUpdateEditorSelect(EditorCamera& camera);
		void OnUpdateRuntime();
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetMainCameraEntity();

		ScneneStates GetCurrentScneneStates() { return m_CurrentScneneStates; }
		void SetCurrentScneneStates(ScneneStates scneneState) { m_CurrentScneneStates = scneneState; }

	private:
		void LightDataManage();

	private:
		entt::registry m_Registry;

		ScneneStates m_CurrentScneneStates = ScneneStates::Ediotr;

		uint32_t m_ViewportWidth;
		uint32_t m_ViewportHeight;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}
