#pragma once
#include "entt.hpp"

#include "BunnyEngine/Renderer/EditorCamera.h"
#include "BunnyEngine/Renderer/IBL.h"

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

		void SetCurrentScneneIBL(const Ref<IBL>& scneneIBL) { m_CurrentScneneIBL = scneneIBL; }
		Ref<IBL> GetCurrentScneneIBL() { return m_CurrentScneneIBL; }

	private:
		void LightDataManage();

	private:
		entt::registry m_Registry;

		ScneneStates m_CurrentScneneStates = ScneneStates::Ediotr;
		
		Ref<IBL> m_CurrentScneneIBL = IBL::Create("Assets/textures/kloofendal_48d_partly_cloudy_puresky_4k.hdr");

		uint32_t m_ViewportWidth;
		uint32_t m_ViewportHeight;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}
