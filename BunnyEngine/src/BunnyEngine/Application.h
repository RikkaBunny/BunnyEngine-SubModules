#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "BunnyEngine/Events/ApplicationEvent.h"
#include "Window.h"
#include "BunnyEngine/LayerStack.h"

#include "BunnyEngine/ImGui/ImGuiLayer.h"

#include "BunnyEngine/Core/Timestep.h"

//#include "BunnyEngine/Renderer/Shader.h"
//#include "BunnyEngine/Renderer/Buffer.h"
//#include "BunnyEngine/Renderer/VertexArray.h"
//
//#include "BunnyEngine/Renderer/OrthographicCamera.h"


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

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;

		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;
		
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
