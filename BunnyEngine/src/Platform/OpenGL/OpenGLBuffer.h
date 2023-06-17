#pragma once
#include "BunnyEngine/Renderer/Buffer.h"
#include <glad/glad.h>

namespace BE {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; };
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void UnBind() const;
		virtual uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class OpenGLUniformBuffer
	{
	public:
		template<typename T>
		void SetUniformBuffer(const T& buffer, unsigned int bindingPoint)
		{
			GLuint ubo;
			glGenBuffers(1, &ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(buffer), &buffer, GL_STATIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			//BE_CORE_INFO("Bind Uniform Buffer");
		}

	};
	
}


