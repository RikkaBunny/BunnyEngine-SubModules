#include "BEpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace BE {


	struct Renderer2DStorage {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Shader> ScreenVisibleBuffer;
		Ref<Texture2D> WhiteTexture;

	};

	struct RendererPlaneStorage {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
		Ref<Texture2D> BlueTexture;
		Ref<Texture2D> BlackTexture;
	};

	static Renderer2DStorage* s_Data;
	static RendererPlaneStorage* s_PlaneData;
	//static Ref<Shader> ScreenVisibleBuffer;
	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();

		float vertices[4 * 5] = {
			-0.5f,-0.5f,0.0f, 0.0f,0.0f, 
			0.5f, -0.5f, 0.0f, 1.0f,0.0f,
			0.5f, 0.5f, 0.0f, 1.0f,1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f,1.0f
		};

		Ref<VertexBuffer> m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		};

		m_VertexBuffer->SetLayout(layout);
		s_Data->QuadVertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<IndexBuffer> m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(m_IndexBuffer);
		
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		//s_Data->ScreenVisibleBuffer = Shader::Create("Assets/shaders/ScreenVisibleBuffer.glsl");
		s_Data->ScreenVisibleBuffer = Shader::Create("Assets/shaders/DeferredPBRRenderer.glsl");

		s_Data->TextureShader = Shader::Create("Assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
		InitQuad();
	}

	void Renderer2D::InitQuad()
	{
		s_PlaneData = new RendererPlaneStorage();
		s_PlaneData->QuadVertexArray = VertexArray::Create();

		float vertices[4 * 11] = {
			-0.5f,-0.5f,0.0f, 0.0f,0.0f,1.0f, 0.0f,0.0f, 0.707107f,0.707107f,0.0f,
			0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f, 1.0f,0.0f, -0.707107f,0.707107f,0.0f,
			0.5f, 0.5f, 0.0f, 0.0f,0.0f,1.0f, 1.0f,1.0f, -0.707107f,-0.707107f,0.0f,
			-0.5f, 0.5f, 0.0f, 0.0f,0.0f,1.0f, 0.0f,1.0f, 0.707107f,-0.707107f,0.0f
		};

		Ref<VertexBuffer> m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float3, "a_Normal"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float3, "a_Tangent"}
		};

		m_VertexBuffer->SetLayout(layout);
		s_PlaneData->QuadVertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<IndexBuffer> m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_PlaneData->QuadVertexArray->SetIndexBuffer(m_IndexBuffer);

		s_PlaneData->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_PlaneData->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_PlaneData->TextureShader = Shader::Create("Assets/shaders/DeferredPBR.glsl");
		s_PlaneData->TextureShader->Bind();
		s_PlaneData->TextureShader->SetInt("u_AlbedoTexture", 0);
		s_PlaneData->TextureShader->SetInt("u_NormalTexture", 1);
		s_PlaneData->TextureShader->SetInt("u_MetallicTexture", 2);
		s_PlaneData->TextureShader->SetInt("u_RoughnessTexture", 3);
		s_PlaneData->TextureShader->SetInt("u_AoTexture", 4);
		//return s_Data;
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		RenderCommand::BeginScene();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", viewProj);
		s_PlaneData->TextureShader->Bind();
		s_PlaneData->TextureShader->SetMat4("u_ViewProjection", viewProj);
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		RenderCommand::BeginScene();

		glm::mat4 viewProj = camera.GetViewProjection();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", viewProj);
		s_PlaneData->TextureShader->Bind();
		s_PlaneData->TextureShader->SetMat4("u_ViewProjection", viewProj);
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		RenderCommand::BeginScene();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	void Renderer2D::BeginScene(const glm::mat4 viewProjectionMatrix)
	{
		RenderCommand::BeginScene();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", viewProjectionMatrix);
		s_PlaneData->TextureShader->Bind();
		s_PlaneData->TextureShader->SetMat4("u_ViewProjection", viewProjectionMatrix);
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat2("u_TexTiling", glm::vec2(1.0f)); 
		s_Data->WhiteTexture->Bind();

		glm::mat4 worldTransform = glm::translate(glm::mat4(1.0f), position) * /*rotaion*/ glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data->TextureShader->SetMat4("u_WorldTransform", worldTransform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color, const glm::vec2& TexTiling)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, color, TexTiling);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color, const glm::vec2& TexTiling)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat2("u_TexTiling", TexTiling);
		texture->Bind();

		glm::mat4 worldTransform = glm::translate(glm::mat4(1.0f), position) * /*rotaion*/ glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data->TextureShader->SetMat4("u_WorldTransform", worldTransform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat2("u_TexTiling", glm::vec2(1.0f));
		s_Data->WhiteTexture->Bind();

		glm::mat4 worldTransform = glm::translate(glm::mat4(1.0f), position) 
			* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data->TextureShader->SetMat4("u_WorldTransform", worldTransform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const glm::vec4& color, const glm::vec2& TexTiling)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, color, TexTiling);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const glm::vec4& color, const glm::vec2& TexTiling)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat2("u_TexTiling", TexTiling);
		texture->Bind();

		glm::mat4 worldTransform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data->TextureShader->SetMat4("u_WorldTransform", worldTransform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat2("u_TexTiling", glm::vec2(1.0f));
		s_Data->WhiteTexture->Bind();

		glm::mat4 worldTransform = transform;
		s_Data->TextureShader->SetMat4("u_WorldTransform", worldTransform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D> texture, const glm::vec4& color, const glm::vec2& TexTiling)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat2("u_TexTiling", TexTiling);
		texture->Bind();

		glm::mat4 worldTransform = transform;
		s_Data->TextureShader->SetMat4("u_WorldTransform", worldTransform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& component, const int entityID)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", component.Color);
		s_Data->TextureShader->SetFloat2("u_TexTiling", glm::vec2(component.Tiling));
		s_Data->TextureShader->SetInt("u_EntityID", entityID);
		if (component.Texture) {
			component.Texture->Bind();
		}
		else
		{
			s_Data->WhiteTexture->Bind();
		}

		s_Data->TextureShader->SetMat4("u_WorldTransform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawPBRQuad(const glm::mat4& transform, const QuadRendererComponent& component, const int entityID)
	{
		s_PlaneData->TextureShader->Bind();
		s_PlaneData->TextureShader->SetFloat4("u_Color", component.Color);
		s_PlaneData->TextureShader->SetFloat2("u_TexTiling", glm::vec2(component.Tiling));
		s_PlaneData->TextureShader->SetFloat("u_Metallic", component.Metallic);
		s_PlaneData->TextureShader->SetFloat("u_Roughness", component.Roughness);
		s_PlaneData->TextureShader->SetFloat("u_Emissive", component.Emissive);
		s_PlaneData->TextureShader->SetInt("u_EntityID", entityID);
		if (component.u_AlbedoTexture) {
			component.u_AlbedoTexture->Bind(0);
		}
		else
		{
			s_PlaneData->WhiteTexture->Bind(0);
		}

		if (component.u_RoughnessTexture) {
			component.u_RoughnessTexture->Bind(3);
		}
		else
		{
			s_PlaneData->WhiteTexture->Bind(3);
		}

		if (component.u_AoTexture) {
			component.u_AoTexture->Bind(4);
		}
		else
		{
			s_PlaneData->WhiteTexture->Bind(4);
		}
		//s_Data->WhiteTexture = Texture2D::Create(1, 1);
		//uint32_t blueTextureData = 0x0000ffff;
		//s_Data->WhiteTexture->SetData(&blueTextureData, sizeof(uint32_t));
		if (component.u_NormalTexture) {
			component.u_NormalTexture->Bind(1);
		}
		else
		{
			s_PlaneData->WhiteTexture->Bind(1);
		}
		//s_Data->WhiteTexture = Texture2D::Create(1, 1);
		//uint32_t blackTextureData = 0x000000ff;
		//s_Data->WhiteTexture->SetData(&blackTextureData, sizeof(uint32_t));
		if (component.u_MetallicTexture) {
			component.u_MetallicTexture->Bind(2);
		}
		else
		{
			s_PlaneData->WhiteTexture->Bind(2);
		}

		s_PlaneData->TextureShader->SetMat4("u_WorldTransform", transform);

		s_PlaneData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_PlaneData->QuadVertexArray);
	}

	void Renderer2D::DrawScreenVisibleBuffer(Framebuffer* framebuffer, const int outBufferType)
	{
		if (outBufferType == 0) {
			//s_Data->ScreenVisibleBuffer = Shader::Create("Assets/shaders/ScreenVisibleBuffer.glsl");
			s_Data->ScreenVisibleBuffer = Shader::Create("Assets/shaders/DeferredPBRRenderer.glsl");
		}
		else {
			s_Data->ScreenVisibleBuffer = Shader::Create("Assets/shaders/ScreenVisibleBuffer.glsl");
			//s_Data->ScreenVisibleBuffer = Shader::Create("Assets/shaders/DeferredPBRRenderer.glsl");
		}
		s_Data->ScreenVisibleBuffer->Bind();
		s_Data->ScreenVisibleBuffer->SetUniformBuffer("LightDirection");
		s_Data->ScreenVisibleBuffer->SetInt("u_OutBufferType", outBufferType);
		s_Data->ScreenVisibleBuffer->SetInt("u_GBufferA", 0);
		s_Data->ScreenVisibleBuffer->SetInt("u_GBufferB", 1);
		s_Data->ScreenVisibleBuffer->SetInt("u_GBufferC", 2);
		s_Data->ScreenVisibleBuffer->SetInt("u_GBufferD", 3);
		s_Data->ScreenVisibleBuffer->SetInt("u_DepthBuffer", 4);
		framebuffer->BindAttachment(0, 0);
		framebuffer->BindAttachment(1, 1);
		framebuffer->BindAttachment(2, 2);
		framebuffer->BindAttachment(3, 3);
		framebuffer->BindAttachment(0, 4, true);
		//if (buffer) {
		//	buffer->Bind();
		//}
		//else
		//{
		//	s_Data->WhiteTexture->Bind();
		//}
		//s_Data->WhiteTexture->Bind();
		s_Data->QuadVertexArray->Bind();

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}


}