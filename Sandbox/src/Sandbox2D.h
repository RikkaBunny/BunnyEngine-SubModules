#pragma once

#include "BunnyEngine.h"


class Sandbox2D : public BE::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttcah() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(BE::Event& event) override;
private:

	BE::Ref<BE::Shader> m_Shader;
	BE::Ref<BE::VertexArray> m_VertexArray;
	BE::Ref<BE::Texture2D> m_Texture;

	BE::OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = glm::vec4(1, 1, 1, 1);

};

