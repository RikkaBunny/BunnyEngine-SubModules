#include <BunnyEngine.h>

#include <glm/gtc/matrix_transform.hpp>

#include "../imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public BE::Layer {
public:
	ExampleLayer()
		:Layer("Example") {

	}

	void OnUpdate() override {
		//BE_INFO("Examplelayer::Update");

		if (BE::Input::IsKeyPressed(BE_KEY_TAB)) {
			BE_TRACE("Tab key is pressed!");
		}

	}

	void OnEvent(BE::Event& event) override {
		//BE_TRACE("{0}", event);
		if (event.GetEventType() == BE::EventType::KeyPressed) {
			BE::KeyPressedEvent& e = (BE::KeyPressedEvent&)event;
			BE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

};

class RenderLayer : public BE::Layer {
public:
	RenderLayer()
		:Layer("Render"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) {
		BE_TRACE("RenderLayer Star!");

		m_VertexArray.reset(BE::VertexArray::Create());

		float vertices[4 * 5] = {
			-0.5f,-0.5f,0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		BE::Ref<BE::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(BE::VertexBuffer::Create(vertices, sizeof(vertices)));

		BE::BufferLayout layout = {
			{BE::ShaderDataType::Float3, "a_Position"},
			{BE::ShaderDataType::Float2, "a_UV"}
		};

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

		BE::Ref<BE::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(BE::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			layout(location = 2) in vec2 a_UV;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_WorldTransform;
			
			out vec4 v_Color;
			void main(){
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
				gl_Position = u_ViewProjection * u_WorldTransform * vec4(a_Position, 1.0);
			}

		)";
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			uniform vec3 u_Color;			

			in vec4 v_Color;
			void main(){
				color = vec4(0.8,0.2,0.3, 1.0);
				color = v_Color;
				color = vec4(u_Color,1.0f);
			}

		)";

		std::string textureVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_UV;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_WorldTransform;
			
			out vec2 v_TexCoord;
			void main(){
				v_TexCoord = a_UV;
				gl_Position = vec4(a_Position, 1.0);
				gl_Position = u_ViewProjection * u_WorldTransform * vec4(a_Position, 1.0);
			}

		)";
		std::string textureFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			uniform sampler2D u_Texture;			
			uniform vec3 u_Color;

			in vec2 v_TexCoord;
			void main(){

				color = texture(u_Texture, v_TexCoord) ;
				color *=  vec4(u_Color,1.0f);
			}

		)";

		m_TextureShader = BE::Shader::Create("texture", textureVertexSrc, textureFragmentSrc);
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = BE::Texture2D::Create("assets/textures/UVChecker_21.png");
		std::dynamic_pointer_cast<BE::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<BE::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
		m_LogTexture = BE::Texture2D::Create("assets/textures/log.png");
		std::dynamic_pointer_cast<BE::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<BE::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate() override {
		//BE_INFO("RenderLayer Update!");
		//BE_TRACE("Delta Time : {0}s  ({1}ms)", BE::Timestep::GetSeconds(), BE::Timestep::GetMilliseconds());
		float ts = BE::Timestep::GetSeconds();
		if (BE::Input::IsKeyPressed(BE_KEY_RIGHT)) {
			m_CameraPosition.x -= m_CameraSpeed * ts;
		}
		if (BE::Input::IsKeyPressed(BE_KEY_LEFT)) {
			m_CameraPosition.x += m_CameraSpeed * ts;
		}
		if (BE::Input::IsKeyPressed(BE_KEY_UP)) {
			m_CameraPosition.y -= m_CameraSpeed * ts;
		}
		if (BE::Input::IsKeyPressed(BE_KEY_DOWN)) {
			m_CameraPosition.y += m_CameraSpeed * ts;
		}
		if (BE::Input::IsKeyPressed(BE_KEY_E)) {
			m_CameraRotation += 180.0f * ts;
		}
		if (BE::Input::IsKeyPressed(BE_KEY_Q)) {
			m_CameraRotation -= 180.0f * ts;
		}
		if (BE::Input::IsKeyPressed(BE_KEY_A)) {
			m_VertexPosition.x -= m_CameraSpeed * ts;
		}
		if (BE::Input::IsKeyPressed(BE_KEY_D)) {
			m_VertexPosition.x += m_CameraSpeed * ts;
		}
		if (BE::Input::IsKeyPressed(BE_KEY_S)) {
			m_VertexPosition.y -= m_CameraSpeed * ts;
		}
		if (BE::Input::IsKeyPressed(BE_KEY_W)) {
			m_VertexPosition.y += m_CameraSpeed * ts;
		}

		BE::RenderCommand::Clear({ 0.8,0.2,0.3,1 });

		BE::Renderer::BeginScene(m_Camera);

		
		glm::mat4 worldTransform = glm::translate(glm::mat4(1.0f), m_VertexPosition);
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		 
		//BE::Material material = new BE::Material(m_shader);
		//BE::MaterialInstance mi = new BE::MaterialInstance(material);
		//mi->Set("u_Color", glm::vec4(1, 1, 1, 1));
		//squareMesh->SetMaterial(mi); 
		std::dynamic_pointer_cast<BE::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<BE::OpenGLShader>(m_TextureShader)->UploadUniformFloat3("u_Color", m_SquareColor);
		m_Texture->Bind();
		//BE::Renderer::Submit(m_VertexArray, m_Shader, glm::mat4(1.0f));
		BE::Renderer::Submit(m_VertexArray, m_TextureShader, worldTransform);

		auto textureShader = m_ShaderLibrary.Get("Texture");
		m_LogTexture->Bind();
		BE::Renderer::Submit(m_VertexArray, textureShader, worldTransform);

		BE::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(BE::Event& event) override {
		//BE_TRACE("{0}", event);
		if (event.GetEventType() == BE::EventType::KeyPressed) {
			BE::KeyPressedEvent& e = (BE::KeyPressedEvent&)event;
			//BE_TRACE("{0}", (char)e.GetKeyCode());
			
		}
	}

private:
	glm::vec3 m_CameraPosition = glm::vec3(0.0f);
	float m_CameraRotation = 0;
	float m_CameraSpeed = 10.0f;

	glm::vec3 m_VertexPosition = glm::vec3(0.0f);

	BE::ShaderLibray m_ShaderLibrary;
	BE::Ref<BE::Shader> m_TextureShader;
	BE::Ref<BE::VertexArray> m_VertexArray;
	BE::Ref<BE::Texture> m_Texture;
	BE::Ref<BE::Texture> m_LogTexture;

	BE::OrthographicCamera m_Camera;

	glm::vec3 m_SquareColor = glm::vec3(1, 1, 1);
};

class Sandbox : public BE::Application {

public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushLayer(new RenderLayer());
	}

	~Sandbox() {

	}

};


BE::Application* BE::CreateApplication() {
	return new Sandbox();
}