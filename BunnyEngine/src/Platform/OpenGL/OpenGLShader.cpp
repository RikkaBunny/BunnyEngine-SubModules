#include "BEpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>


namespace BE {
	
	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex" || type == "Vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "Fragment" || type == "pixel" || type == "Pixel")
			return GL_FRAGMENT_SHADER;

		BE_CORE_ASSERT(false, "Unknown shader type! '{0}'", type);
		return 0;
	}
	static void trim(std::string& s)
	{
		int index = 0;
		if (!s.empty())
		{
			while ((index = s.find(' ', index)) != std::string::npos)
			{
				s.erase(index, 1);
			}
		}
	}

	static ShaderParameter ShaderInputTypeFromString(const std::string& source, const char* floatToken, size_t pos) {
		ShaderParameter parameter;
		
		size_t tokenLength = strlen(floatToken);
		size_t begin = pos + tokenLength + 1;
		//size_t begin = source.find(floatToken, tokenbegin);
		size_t equal = source.find('=', begin);
		size_t end = source.find(';', begin);
		std::string name = source.substr(begin, equal- begin);
		std::string value = source.substr(equal + 1, end- (equal + 1));
		trim(name);
		trim(value);
		parameter.ParameterName = name;
		parameter.ParameterVlaue = value;

		return parameter;
	}
	static void FindParameter(const std::string& source, const char* token, std::vector<ShaderParameter>* shaderParameter, ShaderInputType inputType, size_t parameterStartPos, size_t parameterEndPos) {
		size_t pos = parameterStartPos;
		size_t curPos = parameterStartPos;
		bool parameterChunk = true;
		while (parameterChunk)
		{
			parameterChunk = false;
			pos = source.find(token, curPos);
			if (pos != std::string::npos && pos < parameterEndPos) {
				ShaderParameter parameter = ShaderInputTypeFromString(source, token, pos);
				parameter.InputType = inputType;
				curPos = source.find_first_of("\r\n", pos);
				shaderParameter->push_back(parameter);
				parameterChunk = true;
			}
		}
	} 

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		std::string source = ReadFile(filePath);
		auto shaderSources = PreProcess(source);
		m_ShaderParameter = PreProcessParameter(source);

		Compile(shaderSources);

		//Extract name from filepath
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);

	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		UploadUniformMat3(name, matrix);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		UploadUniformMat4(name, matrix);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				BE_CORE_ERROR("Could not read from file '{0}'", filePath);
			}
		}
		else
		{
			BE_CORE_ERROR("Could not open file '{0}'", filePath);
		}

		return result;

	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			BE_CORE_ASSERT(eol != std::string::npos, "Syntax Error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			BE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = 
				source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		
		return shaderSources;
	}


	std::vector<ShaderParameter> OpenGLShader::PreProcessParameter(const std::string& source) {
		std::vector<ShaderParameter> shaderParameter;

		const char* parameterToken = "#parameter";
		//parameter chunk
		size_t parameterTokenLength = strlen(parameterToken);
		size_t parameterPos = source.find(parameterToken, 0);
		if (parameterPos == std::string::npos)
		{
			return shaderParameter;
		}
		size_t parameterBegin = parameterPos + parameterTokenLength + 1;
		size_t parameterStartPos = source.find('{', parameterBegin);
		size_t parameterEndPos = source.find('}', parameterBegin);
		std::string parameterType;
		//
		const char* floatToken = "float";
		const char* vec2Token = "vec2";
		const char* vec3Token = "vec3";
		const char* vec4Token = "vec4";
		const char* intToken = "int";
		const char* boolToken = "bool";
		const char* mat3Token = "mat3";
		const char* mat4Token = "mat4";
		const char* sampler2DToken = "sampler2D";
		// current parameter line
		size_t pos = parameterStartPos;
		size_t curPos = parameterStartPos;
		FindParameter(source, floatToken, &shaderParameter, ShaderInputType::Float, parameterStartPos,  parameterEndPos);
		FindParameter(source, vec2Token, &shaderParameter, ShaderInputType::Float2, parameterStartPos, parameterEndPos);
		FindParameter(source, vec3Token, &shaderParameter, ShaderInputType::Float3, parameterStartPos, parameterEndPos);
		FindParameter(source, vec4Token, &shaderParameter, ShaderInputType::Float4, parameterStartPos, parameterEndPos);
		FindParameter(source, intToken, &shaderParameter, ShaderInputType::Int, parameterStartPos, parameterEndPos);
		FindParameter(source, boolToken, &shaderParameter, ShaderInputType::Bool, parameterStartPos, parameterEndPos);
		FindParameter(source, mat3Token, &shaderParameter, ShaderInputType::Mat3, parameterStartPos, parameterEndPos);
		FindParameter(source, mat4Token, &shaderParameter, ShaderInputType::Mat4, parameterStartPos, parameterEndPos);
		FindParameter(source, sampler2DToken, &shaderParameter, ShaderInputType::Texture2D, parameterStartPos, parameterEndPos);

		
		return shaderParameter;
		
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		// Get a program object.
		GLuint program = glCreateProgram();
		BE_CORE_ASSERT(shaderSources <= 2, "We only support 2 shaders for now!");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;

		for (auto& kv : shaderSources) {
			GLenum type = kv.first;
			const std::string& source = kv.second;
			GLuint shader = glCreateShader(type);

			// Send the  shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				BE_CORE_ERROR("{0}", infoLog.data());
				BE_CORE_ERROR("`{0}` Shader Compilation Failure!", type);
	/*			BE_CORE_ASSERT(false, "Shader Compilation Failure!");*/
				return;
			}
			// Create an empty  shader handle
			// Attach our shaders to our program
			glAttachShader(program, shader);

			glShaderIDs[glShaderIDIndex++] = shader;
		}
		

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for(auto id : glShaderIDs)
				glDeleteShader(id);
			

			BE_CORE_ERROR("{0}", infoLog.data());
			BE_CORE_ASSERT(false, "Shader Link Failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
			glDeleteShader(id);

		m_RendererID = program;
	}


}


