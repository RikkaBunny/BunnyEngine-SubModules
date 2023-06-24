#include "BEpch.h"
#include "OpenGLTextureCubeMap.h"

#include "stb_image.h"


namespace BE {

	void OpenGLTextureCubeMap::Load_8_Bit_Texture(const std::string& path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		// 设置纹理参数
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// 创建纹理数据
		for (int i = 0; i < 6; i++) {
			std::string firstPart = path.substr(0, path.size() - 5);
			std::string secondPart = path.substr(path.size() - 4, 4);
			std::string fileName = firstPart + std::to_string(i) + secondPart;
			stbi_uc* data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);

			BE_CORE_ASSERT(data, "Failed to load image!");

			m_Width = width;
			m_Height = height;

			if (channels == 4) {
				m_InternalFormat = GL_RGBA8;
				m_DataFormat = GL_RGBA;
			}
			else if (channels == 3) {
				m_InternalFormat = GL_RGB8;
				m_DataFormat = GL_RGB;
			}

			BE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);

		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void OpenGLTextureCubeMap::Load_16_Bit_Texture(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		// 设置纹理参数
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// 创建纹理数据
		for (int i = 0; i < 6; i++) {
			std::string firstPart = path.substr(0, path.size() - 5);
			std::string secondPart = path.substr(path.size() - 4, 4);
			std::string fileName = firstPart + std::to_string(i) + secondPart;
			stbi_us* data = stbi_load_16(fileName.c_str(), &width, &height, &channels, 0);

			BE_CORE_ASSERT(data, "Failed to load image!");

			m_Width = width;
			m_Height = height;

			if (channels == 4) {
				m_InternalFormat = GL_RGBA16;
				m_DataFormat = GL_RGBA;
			}
			else if (channels == 3) {
				m_InternalFormat = GL_RGB16;
				m_DataFormat = GL_RGB;
			}

			BE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);

		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		
	}

	void OpenGLTextureCubeMap::Load_HDR_Texture(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		// 设置纹理参数
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// 创建纹理数据
		for (int i = 0; i < 6; i++) {
			std::string firstPart = path.substr(0, path.size() - 5);
			std::string secondPart = path.substr(path.size() - 4, 4);
			std::string fileName = firstPart + std::to_string(i) + secondPart;
			float* data = stbi_loadf(fileName.c_str(), &width, &height, &channels, 0);

			BE_CORE_ASSERT(data, "Failed to load image!");

			m_Width = width;
			m_Height = height;

			if (channels == 4) {
				m_InternalFormat = GL_RGBA16F;
				m_DataFormat = GL_RGBA;
			}
			else if (channels == 3) {
				m_InternalFormat = GL_RGBA16F;
				m_DataFormat = GL_RGB;
			}

			BE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_FLOAT, data);
			stbi_image_free(data);

		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}

	OpenGLTextureCubeMap::OpenGLTextureCubeMap(const std::string& path)
		:m_Path(path)
	{

		int is_16bit = stbi_is_16_bit(path.c_str());
		int is_hdr = stbi_is_hdr(path.c_str());

		if (is_hdr) {
			Load_HDR_Texture(path);
			return;
		}
		else if (is_16bit) {
			Load_16_Bit_Texture(path);
			return;
		}
		else {
			Load_8_Bit_Texture(path);
		}
	}

	
	OpenGLTextureCubeMap::~OpenGLTextureCubeMap()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	float* OpenGLTextureCubeMap::GetData()
	{
		return 0;
		uint32_t bpc = m_DataFormat == GL_RGBA ? 4 : 3;
		float* data = new float[m_Width * m_Height * bpc];
		//todo  固定默认为float数据，float 4bytes。 以后改为动态.
		glGetTextureImage(m_RendererID, 0, m_DataFormat, GL_FLOAT, m_Width * m_Height * bpc * 4, data);
		return data;
	}

	void OpenGLTextureCubeMap::SetData(void* data, uint32_t size)
	{
		return ;

		uint32_t bpc = m_DataFormat == GL_RGBA ? 4 : 3;
		BE_CORE_ASSERT(size == m_Width * m_Height * bpc);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTextureCubeMap::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}