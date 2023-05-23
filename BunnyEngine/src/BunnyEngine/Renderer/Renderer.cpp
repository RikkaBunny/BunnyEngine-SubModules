#include "BEpch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"
namespace BE {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	Renderer::RenderPipeline Renderer::s_RenderPipeline = Renderer::RenderPipeline::DeferredRendering;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
		Renderer2D::InitQuad();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& worldTransform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_WorldTransform", worldTransform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	//void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& worldTransform)
	//{
	//	shader->Bind();
	//	shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
	//	shader->UploadUniformMat4("u_WorldTransform", worldTransform);
	//	vertexArray->Bind();
	//	RenderCommand::DrawIndexed(vertexArray);
	//}

}
