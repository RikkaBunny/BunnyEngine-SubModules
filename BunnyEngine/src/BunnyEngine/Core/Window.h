#pragma once

#include "BEpch.h"

#include "BunnyEngine/Core/Core.h"
#include "BunnyEngine/Events/Event.h"


namespace BE {
	
	enum class WindowState {
		NORMAL,
		MINIMIZED,
		MAXIMIZED
	};

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Bunny Engine",
			uint32_t width = 1600,
			uint32_t height = 900)
			: Title(title), Width(width), Height(height) {

		}
	};

	class BE_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		virtual void SetWindowPos(int windowPosX, int windowPosY) = 0;
		virtual void SetWindowSize(int windowSizeX, int windowSizeY) = 0;
		virtual void SetWindowIconify() = 0;
		virtual void SetWindowMaximize() = 0;
		virtual void SetWindowRestore() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual WindowState GetWindowState() const = 0;

		//Window attributes 
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}