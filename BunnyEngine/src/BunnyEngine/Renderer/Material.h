#pragma once
#include "Shader.h"
#include "Texture.h"
#include <variant>

namespace BE {
	using ShaderInputParameter = std::variant<int, float, bool, std::string, glm::vec2, glm::vec3, glm::vec4, glm::mat3, glm::mat4, Ref<Texture2D>>;
	

	class Material
	{
	public:
		Material() {};

		void SetShader(Ref<Shader> shader);
		Ref<Shader> GetShader() { return m_Shader; }
		auto GetShaderLibray() { return ShaderLibray::GetShaderLibray(); }
		void BindShader();
		void BindShader(glm::mat4 transform);

	private:
		void SetParameterMap();
		
	public:
		Ref<Shader> m_Shader;
		std::vector<ShaderParameter>* m_ShaderParameter;

		std::vector<ShaderInputParameter> m_ShaderParameters;

	};
}
