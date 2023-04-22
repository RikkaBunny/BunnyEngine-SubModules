#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"
#include "BunnyEngine/Renderer/Camera.h"
#include "BunnyEngine/Renderer/EditorCamera.h"
#include <BunnyEngine/Scene/Components.h>
namespace BE {

	class Renderer2D
	{
	public:
		static void Init();
		static void InitQuad();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const glm::mat4 viewProjectionMatrix);
		static void EndScene();

		//Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f), const glm::vec2& TexTiling = glm::vec2(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f), const glm::vec2& TexTiling = glm::vec2(1.0f));

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
																			   
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f), const glm::vec2& TexTiling = glm::vec2(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f), const glm::vec2& TexTiling = glm::vec2(1.0f));

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f), const glm::vec2& TexTiling = glm::vec2(1.0f));

		static void DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& component, const int entityID = -1);

		static void DrawPBRQuad(const glm::mat4& transform, const QuadRendererComponent& component, const int entityID = -1);

	};

}


