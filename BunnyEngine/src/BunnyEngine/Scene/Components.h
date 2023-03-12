#pragma once
#include "glm/glm.hpp"

namespace BE {

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct NameComponent
	{
		std::string Name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string name)
			: Name(name) {}

	};

	struct SpriteRenderComponent
	{
		glm::vec4 Color{ 1.0f };

		SpriteRenderComponent() = default;
		SpriteRenderComponent(const SpriteRenderComponent&) = default;
		SpriteRenderComponent(const glm::vec4& color)
			: Color(color) {}

	};


}
