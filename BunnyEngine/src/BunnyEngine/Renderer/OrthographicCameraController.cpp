#include "BEpch.h"
#include "OrthographicCameraController.h"

#include "BunnyEngine/Core/Input.h"
#include "BunnyEngine/Core/KeyCodes.h"

namespace BE {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool roration)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(roration)
	{

	}
	void OrthographicCameraController::OnUpdate()
	{
		float ts = BE::Timestep::GetSeconds();
		if (Input::IsKeyPressed(BE_KEY_A)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(BE_KEY_D)) {
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(BE_KEY_S)) {
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(BE_KEY_W)) {
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		}
		if (m_Rotation) {
			if (Input::IsKeyPressed(BE_KEY_E)) {
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}
			if (Input::IsKeyPressed(BE_KEY_Q)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel;

	}
	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset()*0.25;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());

		return false;
	}
}
