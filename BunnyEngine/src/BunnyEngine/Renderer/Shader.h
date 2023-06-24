#pragma once

#include <string>
#include <glm/glm.hpp>

namespace BE {
	enum class ShaderInputType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Bool, Texture2D
	};
	struct ShaderParameter
	{
		ShaderInputType InputType;
		std::string ParameterName;
		std::string ParameterVlaue;
		
	}; 

	class Shader {
	public:
		
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const std::string& GetName() const = 0;
		virtual const std::vector<ShaderParameter>& GetShaderParameter() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
					
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
					
		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;

		virtual void SetUniformBuffer(const std::string& name) = 0;

		static Ref<Shader> Create(const std::string& filePath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

	};

	class ShaderLibray {
	public:
		static void Add(const std::string& name, const Ref<Shader>& shader);
		static void Add(const Ref<Shader>& shader);
		static Ref<Shader> Load(const std::string& filepath);

		static Ref<Shader> Get(const std::string& name, const std::string& filepath);
		static Ref<Shader> Get(const std::string& name);

		static void UpdateShaderLibray();
		
		static bool Exists(const std::string& name);
		static const std::unordered_map<std::string, Ref<Shader>> GetShaderLibray() { return s_Shaders; }
	private:
		static void FileIterator(std::filesystem::path filePath);
	private:
		static std::unordered_map<std::string, Ref<Shader>> s_Shaders ;
	};
}
