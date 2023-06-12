#pragma once

#include "BunnyEngine/Core/Window.h"

#include <GLFW/glfw3.h>
#include "BunnyEngine/Renderer/GraphicsContext.h"



namespace BE {

	class WindowsWindow : public Window {

	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		void SetWindowPos(int windowPosX, int windowPosY) override;
		void SetWindowSize(int windowSizeX, int windowSizeY) override;
		void SetWindowIconify() override { glfwIconifyWindow(m_Window); }
		void SetWindowMaximize() override { glfwMaximizeWindow(m_Window); }
		void SetWindowRestore() override { glfwRestoreWindow(m_Window); }

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline WindowState GetWindowState() const override { return m_Data.WindowState; }

		//Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enable) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window;}
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			WindowState WindowState = WindowState::NORMAL;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

		const char* m_IconPath = "Resources/Icons/Common/BunnyIcon.png";
	};

}

