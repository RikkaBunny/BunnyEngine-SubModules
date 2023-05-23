#include "BEpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace BE {
	Ref<Shader> Shader::Create(const std::string& filePath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			auto shader = std::make_shared<OpenGLShader>(filePath);
			ShaderLibray::Add(shader);
			return shader;
		}

		BE_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			auto shader = std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
			ShaderLibray::Add(shader);
			return shader;
		}

		BE_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}


	std::unordered_map<std::string, Ref<Shader>> ShaderLibray::m_Shaders;

	void ShaderLibray::Add(const std::string& name, const Ref<Shader>& shader)
	{
		BE_CORE_ASSERT(!Exists(name), "Shader already exists!");
		ShaderLibray::m_Shaders[name] = shader;
	}

	void ShaderLibray::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibray::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibray::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name,shader);
		return shader;
	}

	Ref<Shader> ShaderLibray::Get(const std::string& name)
	{
		BE_CORE_ASSERT(Exists(name), "Shader not found!");
		return ShaderLibray::m_Shaders[name];
	}

	bool ShaderLibray::Exists(const std::string& name) {
		return ShaderLibray::m_Shaders.find(name) != ShaderLibray::m_Shaders.end();
	}

}