#pragma once

#include "BunnyEngine/Renderer/Texture.h"
#include <glad/glad.h>

namespace BE {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint32_t width, uint32_t height, int type);
		virtual ~OpenGLTexture2D();
		
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		virtual float* GetData() override;
		virtual void SetRendererID(uint32_t rendererID)  override { m_RendererID = rendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		void Load_8_Bit_Texture(const std::string& path);
		void Load_16_Bit_Texture(const std::string& path);
		void Load_HDR_Texture(const std::string& path);
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		GLenum m_InternalFormat , m_DataFormat;
		uint32_t m_RendererID;
	};


}