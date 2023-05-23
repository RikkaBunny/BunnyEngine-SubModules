#include "BEpch.h"
#include "Scene.h"

#include <glm/glm.hpp>

#include "BunnyEngine/Renderer/Renderer2D.h"
#include "BunnyEngine/Renderer/RenderCommand.h"
#include "Entity.h"
#include "Components.h"

namespace BE {

	static void DoMath(const glm::mat4& transform) {

	}

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity) {

	}

	Scene::Scene()
	{
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

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(EditorCamera& camera)
	{
		Renderer2D::InitQuad();
		Renderer2D::BeginScene(camera);
		{
			auto group1 = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group1) {
				auto [transform, Sprite] = group1.get<TransformComponent, SpriteRendererComponent>(entity);
				//Renderer2D::DrawQuad(transform.GetTransform(), Sprite.Color);
				Renderer2D::DrawSprite(transform.GetTransform(), Sprite, (int)entity);
			}
		}
		{
			auto view = m_Registry.view<TransformComponent, QuadRendererComponent>();
			for (auto entity1 : view) {
				auto [transform, Quad] = view.get<TransformComponent, QuadRendererComponent>(entity1);

				Renderer2D::DrawPBRQuad(transform.GetTransform(), Quad, (int)entity1);
			}
			//auto group2 = m_Registry.group<TransformComponent>(entt::get<QuadRendererComponent>);
			//for (auto entity1 : group2) {
			//	auto [transform, Quad] = group2.get<TransformComponent, QuadRendererComponent>(entity1);
			//	//Renderer2D::DrawQuad(transform.GetTransform(), Sprite.Color);
			//	Renderer2D::DrawPBRQuad(transform.GetTransform(), Quad, (int)entity1);
			//}
		}
		Renderer2D::EndScene();
		
	}



	void Scene::OnUpdateRuntime()
	{
		//Update scripts
		{
			m_Registry.view<ScriptComponent>().each([=](auto entity, auto& scriptComponent) {
				//TODO: Move to Scene::OnScenePlay
				if (!scriptComponent.Instance) {
					scriptComponent.Instance = scriptComponent.InstantiateScript();
					scriptComponent.Instance->m_Entity = Entity{ entity, this };
					scriptComponent.Instance->OnCreate();
				}				
				scriptComponent.Instance->OnUpdate();
				});
		}

		//Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 mainCamerTransform ;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view) {
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Main) {
					mainCamera = &camera.Camera;
					mainCamerTransform = transform.GetTransform();
					break;
				}
			}
		}
		if (mainCamera) {
			Renderer2D::BeginScene(*mainCamera, mainCamerTransform);

			auto group1 = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group1) {
				auto [transform, Sprite] = group1.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform.GetTransform(), Sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::GetMainCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Main)
				return Entity{ entity, this };
		}
		return {};
	}


}