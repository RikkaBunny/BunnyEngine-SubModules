#include "BEpch.h"
#include "Scene.h"

#include <glm/glm.hpp>

#include "BunnyEngine/Renderer/Renderer2D.h"
#include "Entity.h"
#include "Components.h"

namespace BE {

	static void DoMath(const glm::mat4& transform) {

	}

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity) {

	}

	Scene::Scene()
	{
		/*struct MeshCompent{
			float value;
			MeshCompent() = default;
		};
		struct TransformComponent
		{
			glm::mat4 Transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4& transform)
				: Transform(transform){}

			operator glm::mat4& () { return Transform; }
			operator const glm::mat4& () const{ return Transform; }
		};

		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

		if (m_Registry.try_get<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view) {
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshCompent>);
		for (auto entity : group) {
			auto&[transform, mesh] = group.get<TransformComponent, MeshCompent>(entity);
		}*/

	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& entityName = entity.AddComponent<NameComponent>();
		entityName.Name = name;

		return entity;
	}

	void Scene::OnUpdate()
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRenderComponent>);
		for (auto entity : group) {
			auto& [transform, Sprite] = group.get<TransformComponent, SpriteRenderComponent>(entity);
			Renderer2D::DrawQuad(transform.Transform, Sprite.Color);
		}
	}

}