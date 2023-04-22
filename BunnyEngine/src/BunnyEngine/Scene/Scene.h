#pragma once
#include "entt.hpp"

#include "BunnyEngine/Renderer/EditorCamera.h"

namespace BE {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		entt::registry& Reg() { return m_Registry; }
		Entity CreateEntity(const std::string& name = "NewGameEntity");
		void DestroyEntity(Entity entity);
		void OnUpdateEditor(EditorCamera& camera);
		void OnUpdateRuntime();
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetMainCameraEntity();

		

	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth;
		uint32_t m_ViewportHeight;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}
