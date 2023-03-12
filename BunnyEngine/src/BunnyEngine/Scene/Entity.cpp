#include "BEpch.h"
#include "Entity.h"

namespace BE {

	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle), m_Scene(scene)
	{

	}

}