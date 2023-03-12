#pragma once

#include "BunnyEngine/Renderer/OrthographicCamera.h"
#include "BunnyEngine/Core/Timestep.h"

#include "BunnyEngine/Events/ApplicationEvent.h"
#include "BunnyEngine/Events/MouseEvent.h"


namespace BE {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate();
		void OnEvent(Event& e);
		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation = false;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 2.5f, m_CameraRotationSpeed = 180.0f;
	};

}

