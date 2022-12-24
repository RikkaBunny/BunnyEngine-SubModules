#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "BunnyEngine/Events/ApplicationEvent.h"
#include "Window.h"
#include "BunnyEngine/LayerStack.h"

#include "BunnyEngine/ImGui/ImGuiLayer.h"


namespace BE {

	class BE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() {return *s_Instance;}
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;

		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
