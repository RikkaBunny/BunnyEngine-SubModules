#pragma once
#include "BunnyEngine/Renderer/Framebuffer.h"

namespace BE {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();
		
		virtual void Bind() override;
		virtual void UnBind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { BE_CORE_ASSERT(index < m_ColorAttachments.size()); return m_ColorAttachments[index]; }
		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
		
		void Invalidate();
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecifications;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};

}


