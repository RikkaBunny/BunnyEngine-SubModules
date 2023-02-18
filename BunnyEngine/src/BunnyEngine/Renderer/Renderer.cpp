#include "BEpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
namespace BE {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
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
