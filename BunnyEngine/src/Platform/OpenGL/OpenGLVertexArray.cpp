#include "BEpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace BE {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
			case BE::ShaderDataType::Float:
				return GL_FLOAT;
			case BE::ShaderDataType::Float2:
				return GL_FLOAT;
			case BE::ShaderDataType::Float3:
				return GL_FLOAT;
			case BE::ShaderDataType::Float4:
				return GL_FLOAT;
			case BE::ShaderDataType::Mat3:
				return GL_FLOAT;
			case BE::ShaderDataType::Mat4:
				return GL_FLOAT;
			case BE::ShaderDataType::Int:
				return GL_INT;
			case BE::ShaderDataType::Int2:
				return GL_INT;
			case BE::ShaderDataType::Int3:
				return GL_INT;
			case BE::ShaderDataType::Int4:
				return GL_INT;
			case BE::ShaderDataType::Bool:
				return GL_BOOL;
		}
		BE_CORE_ASSERT(false, "Unknown ShaderDataType !");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		BE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements.size(), "VertexBuffer Has No Layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& vertexLayout = vertexBuffer->GetLayout();
		for (const auto& element : vertexLayout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				vertexLayout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}
