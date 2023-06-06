#include "BEpch.h"
#include "Material.h"


namespace BE {
	static std::vector<float> StringToMulitFloatValue(const std::string& valueString) {
		std::vector<float> floatValues;
		size_t startPos = valueString.find("(");
		size_t endPos = valueString.find(")");
		size_t pos = valueString.find(",");

		size_t curPos = startPos;
		bool parameterChunk = true;
		while (parameterChunk)
		{
			parameterChunk = false;
			curPos += 1;
			pos = valueString.find(",", curPos);
			if (pos != std::string::npos && pos < endPos) {

				std::string value = valueString.substr(curPos, pos - curPos);
				floatValues.push_back(std::stof(value));
				curPos = pos;
				parameterChunk = true;
			}
		}
		std::string value = valueString.substr(curPos, endPos - curPos);
		floatValues.push_back(std::stof(value));
		return floatValues;
	}

	void Material::SetShader(Ref<Shader> shader)
	{
		m_Shader = shader;
		SetParameterMap();
	}

	void Material::BindShader()
	{
		if (!m_Shader)
			return;

		m_Shader->Bind();
		int textureSlot = 0;
		auto shaderParameters = m_Shader->GetShaderParameter();
		for (size_t i = 0; i < shaderParameters.size(); i++) {
			ShaderParameter m_ShaderParameter = shaderParameters.at(i);
			switch (m_ShaderParameter.InputType)
			{
			case BE::ShaderInputType::None:
				break;
			case BE::ShaderInputType::Float: {
				std::string name = m_ShaderParameter.ParameterName;
				float value = std::get<float>(m_ShaderParameters[i]);
				m_Shader->SetFloat(name, value);
				break;
			}
			case BE::ShaderInputType::Float2: {
				std::string name = m_ShaderParameter.ParameterName;
				glm::vec2 value = std::get<glm::vec2>(m_ShaderParameters[i]);				
				m_Shader->SetFloat2(name, value);
				
				break;
			}
			case BE::ShaderInputType::Float3:
			{
				std::string name = m_ShaderParameter.ParameterName;
				glm::vec3 value = std::get<glm::vec3>(m_ShaderParameters[i]);
				m_Shader->SetFloat3(name, value);
				
				break;
			}
			case BE::ShaderInputType::Float4:
			{
				std::string name = m_ShaderParameter.ParameterName;
				glm::vec4 value = std::get<glm::vec4>(m_ShaderParameters[i]);
				m_Shader->SetFloat4(name, value);

				break;
			}
			case BE::ShaderInputType::Mat3:
			{
				std::string name = m_ShaderParameter.ParameterName;
				glm::mat3 value = std::get<glm::mat3>(m_ShaderParameters[i]);
				m_Shader->SetMat3(name, value);
				break;
			}
			case BE::ShaderInputType::Mat4:
			{
				std::string name = m_ShaderParameter.ParameterName;
				glm::mat4 value = std::get<glm::mat4>(m_ShaderParameters[i]);
				m_Shader->SetMat3(name, value);
				break;
			}
			case BE::ShaderInputType::Int:
			{
				std::string name = m_ShaderParameter.ParameterName;
				int value = std::get<int>(m_ShaderParameters[i]);
				m_Shader->SetInt(name, value);
				break;
			}
			case BE::ShaderInputType::Bool:
			{
				std::string name = m_ShaderParameter.ParameterName;
				bool value = std::get<bool>(m_ShaderParameters[i]);
				//m_Shader->SetInt(name, value);
				break;
			}
			case BE::ShaderInputType::Texture2D:
			{
				std::string name = m_ShaderParameter.ParameterName;
				Ref<Texture2D> value = std::get<Ref<Texture2D>>(m_ShaderParameters[i]);
				value->Bind(textureSlot);
				textureSlot++;
				break;
			}
			}
		}
	}

	void Material::BindShader(glm::mat4 transform)
	{
		BindShader();
		m_Shader->SetMat4("u_WorldTransform", transform);
	}

	void Material::SetParameterMap()
	{
		m_ShaderParameters.clear();
		m_ShaderParameters.shrink_to_fit();
		auto shaderParameters = m_Shader->GetShaderParameter();
		for (size_t i = 0; i < shaderParameters.size(); i++) {
			ShaderParameter m_ShaderParameter = shaderParameters.at(i);
			switch (m_ShaderParameter.InputType)
			{
			case BE::ShaderInputType::None:
				break;
			case BE::ShaderInputType::Float: {
				std::string name = m_ShaderParameter.ParameterName;
				float value = std::stof(m_ShaderParameter.ParameterVlaue);
				m_ShaderParameters.push_back(value);
				break;
			}
			case BE::ShaderInputType::Float2: {
				std::string name = m_ShaderParameter.ParameterName;
				std::vector<float> floatValues = StringToMulitFloatValue(m_ShaderParameter.ParameterVlaue);
				if (floatValues.size() > 1) {
					float f1 = floatValues.at(0);
					float f2 = floatValues.at(1);
					glm::vec2 value = glm::vec2(f1, f2);
					m_ShaderParameters.push_back(value);
				}
				else if(floatValues.size() == 1){
					float f1 = floatValues.at(0);
					glm::vec2 value = glm::vec2(f1);
					m_ShaderParameters.push_back(value);
				}
				
				break;
			}
			case BE::ShaderInputType::Float3:
			{
				std::string name = m_ShaderParameter.ParameterName;
				std::vector<float> floatValues = StringToMulitFloatValue(m_ShaderParameter.ParameterVlaue);
				if (floatValues.size() > 1) {
					float f1 = floatValues.at(0);
					float f2 = floatValues.at(1);
					float f3 = floatValues.at(2);
					glm::vec3 value = glm::vec3(f1, f2, f3);
					m_ShaderParameters.push_back(value);
				}
				else if (floatValues.size() == 1) {
					float f1 = floatValues.at(0);
					glm::vec3 value = glm::vec3(f1);
					m_ShaderParameters.push_back(value);
				}
				break;
			}
			case BE::ShaderInputType::Float4:
			{
				std::string name = m_ShaderParameter.ParameterName;
				std::vector<float> floatValues = StringToMulitFloatValue(m_ShaderParameter.ParameterVlaue);
				if (floatValues.size() > 1) {
					float f1 = floatValues.at(0);
					float f2 = floatValues.at(1);
					float f3 = floatValues.at(2);
					float f4 = floatValues.at(3);
					glm::vec4 value = glm::vec4(f1, f2, f3, f4);
					m_ShaderParameters.push_back(value);
				}
				else if (floatValues.size() == 1) {
					float f1 = floatValues.at(0);
					glm::vec4 value = glm::vec4(f1);
					m_ShaderParameters.push_back(value);
				}
				break;
			}
			case BE::ShaderInputType::Mat3:
			{
				std::string name = m_ShaderParameter.ParameterName;
				std::vector<float> floatValues = StringToMulitFloatValue(m_ShaderParameter.ParameterVlaue);
				if (floatValues.size() > 1) {
					float f1 = floatValues.at(0);
					float f2 = floatValues.at(1);
					glm::vec2 value = glm::vec2(f1, f2);
					m_ShaderParameters.push_back(value);
				}
				else if (floatValues.size() == 1) {
					float f1 = floatValues.at(0);
					glm::mat3 value = glm::mat3(f1);
					m_ShaderParameters.push_back(value);
				}
				break;
			}
			case BE::ShaderInputType::Mat4:
			{
				std::string name = m_ShaderParameter.ParameterName;
				std::vector<float> floatValues = StringToMulitFloatValue(m_ShaderParameter.ParameterVlaue);
				if (floatValues.size() > 1) {
					float f1 = floatValues.at(0);
					float f2 = floatValues.at(1);
					glm::vec2 value = glm::vec2(f1, f2);
					m_ShaderParameters.push_back(value);
				}
				else if (floatValues.size() == 1) {
					float f1 = floatValues.at(0);
					glm::mat3 value = glm::mat3(f1);
					m_ShaderParameters.push_back(value);
				}
				break;
			}
			case BE::ShaderInputType::Int:
			{
				std::string name = m_ShaderParameter.ParameterName;
				int value = std::stoi(m_ShaderParameter.ParameterVlaue);
				m_ShaderParameters.push_back(value);
				break;
			}
			case BE::ShaderInputType::Bool:
			{
				std::string name = m_ShaderParameter.ParameterName;
				bool value = true;
				if(m_ShaderParameter.ParameterVlaue != "true")
					value = false;
				m_ShaderParameters.push_back(value);
				break;
			}
			case BE::ShaderInputType::Texture2D:
			{
				std::string name = m_ShaderParameter.ParameterName;
				Ref<Texture2D> value = Texture2D::Create(1, 1);
				m_ShaderParameters.push_back(value);
				break;
			}
			}
		}
	}
}