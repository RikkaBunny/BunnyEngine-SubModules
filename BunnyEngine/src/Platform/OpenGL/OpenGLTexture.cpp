#include "BEpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"


namespace BE {

	void OpenGLTexture2D::Load_8_Bit_Texture(const std::string& path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

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

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	void OpenGLTexture2D::Load_16_Bit_Texture(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_us* data = stbi_load_16(path.c_str(), &width, &height, &channels, 0);

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

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	void OpenGLTexture2D::Load_HDR_Texture(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		float* data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);

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

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_FLOAT, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data;
		
		float* dataf;

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

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height,int type)
	{
		m_Width = width;
		m_Height = height;

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		BE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		uint32_t whiteTextureData;
		int width1, height1, channels1;
		std::string bulePath = "Resources/Icons/ContentBrowser/Blue.png";
		stbi_uc* data;

		switch (type)
		{
		case 0:
			whiteTextureData = 0xffffffff;
			SetData(&whiteTextureData, sizeof(uint32_t));
			break;
		case 1:
			whiteTextureData = 0x88888888;
			SetData(&whiteTextureData, sizeof(uint32_t));
			break;
		case 2:
			
			whiteTextureData = 0x00000000;
			SetData(&whiteTextureData, sizeof(uint32_t));
			break;
		case 3:
			stbi_set_flip_vertically_on_load(1);
			data = stbi_load(bulePath.c_str(), &width1, &height1, &channels1, 0);
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			break;
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	float* OpenGLTexture2D::GetData()
	{
		uint32_t bpc = m_DataFormat == GL_RGBA ? 4 : 3;
		float* data = new float[m_Width * m_Height * bpc];
		//todo  固定默认为float数据，float 4bytes。 以后改为动态.
		glGetTextureImage(m_RendererID, 0, m_DataFormat, GL_FLOAT, m_Width * m_Height * bpc * 4, data);
		return data;
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpc = m_DataFormat == GL_RGBA ? 4 : 3;
		BE_CORE_ASSERT(size == m_Width * m_Height * bpc);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}