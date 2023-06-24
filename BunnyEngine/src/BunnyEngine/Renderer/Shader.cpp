#include "BEpch.h"
#include "Shader.h"

#include <filesystem>

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace BE {
	extern const std::filesystem::path g_AssetsPath;
	
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


	std::unordered_map<std::string, Ref<Shader>> ShaderLibray::s_Shaders;

	void ShaderLibray::Add(const std::string& name, const Ref<Shader>& shader)
	{
		BE_CORE_ASSERT(!Exists(name), "Shader already exists!");
		ShaderLibray::s_Shaders[name] = shader;
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

	Ref<Shader> ShaderLibray::Get(const std::string& name, const std::string& filepath)
	{
		if(Exists(name)) 
			return Get(name);
		
		return Load(filepath);
	}

	Ref<Shader> ShaderLibray::Get(const std::string& name)
	{
		BE_CORE_ASSERT(Exists(name), "Shader not found!");
		return ShaderLibray::s_Shaders[name];
	}

	void ShaderLibray::UpdateShaderLibray()
	{
		ShaderLibray::FileIterator(g_AssetsPath);
	}

	bool ShaderLibray::Exists(const std::string& name) {
		return ShaderLibray::s_Shaders.find(name) != ShaderLibray::s_Shaders.end();
	}

	void ShaderLibray::FileIterator(std::filesystem::path filePath)
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(filePath)) {
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetsPath);
			std::string filenameString = relativePath.filename().string();

			if (directoryEntry.is_directory()) {
				FileIterator(path);

			}
			else {

				std::string filename = filenameString.substr(filenameString.size() - 4, 4);
				if (filename == "glsl") {
					//Extract name from filepath
					auto lastSlash = filenameString.find_last_of("/\\");
					lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
					auto lastDot = filenameString.rfind('.');
					auto count = lastDot == std::string::npos ? filenameString.size() - lastSlash : lastDot - lastSlash;
					std::string name = filenameString.substr(lastSlash, count);
					Get(name,path.string());
				}
			}
		}
	}
}
