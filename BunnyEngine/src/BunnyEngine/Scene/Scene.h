#pragma once
#include "entt.hpp"


namespace BE {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		entt::registry& Reg() { return m_Registry; }
		Entity CreateEntity(const std::string& name = "NewGameEntity");
		void OnUpdate();


	private:
		entt::registry m_Registry;

		friend class Entity;
	};

}
