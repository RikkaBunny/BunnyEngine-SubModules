#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "BunnyEngine/Scene/SceneCamera.h"
#include "BunnyEngine/Scene/ScriptableEntity.h"
#include "BunnyEngine/Renderer/Texture.h"

namespace BE {

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f,0.0f,0.0f };
		glm::vec3 Rotation = { 0.0f,0.0f,0.0f };
		glm::vec3 Scale = { 1.0f,1.0f,1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const {
			glm::mat4 rotaion = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotaion
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct NameComponent
	{
		std::string Name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string name)
			: Name(name) {}

	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f };
		Ref<Texture2D> Texture;
		float Tiling = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}

	};

	struct QuadRendererComponent
	{
		glm::vec4 Color{ 1.0f };
		Ref<Texture2D> u_AlbedoTexture;
		Ref<Texture2D> u_NormalTexture;
		float Metallic = 0.0f;
		Ref<Texture2D> u_MetallicTexture;
		float Roughness = 0.0f;
		Ref<Texture2D> u_RoughnessTexture;
		Ref<Texture2D> u_AoTexture;
		float Emissive = 0.0f;
		float Tiling = 1.0f;


		QuadRendererComponent() = default;
		QuadRendererComponent(const QuadRendererComponent&) = default;
		QuadRendererComponent(const glm::vec4& color, float metallic, float roughness, float emissive)
			: Color(color), Metallic(metallic), Roughness(roughness), Emissive(emissive) {}

	};

	struct CameraComponent {
		SceneCamera Camera;
		bool Main = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct ScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(ScriptComponent*);

		template<typename T>
		void Bind() {
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](ScriptComponent* scriptComponent) {delete scriptComponent->Instance; scriptComponent->Instance = nullptr; };

		}
	};
}
