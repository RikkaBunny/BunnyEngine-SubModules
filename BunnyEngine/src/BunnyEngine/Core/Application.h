#pragma once

#include "BunnyEngine/Core/Core.h"
#include "BunnyEngine/Events/Event.h"
#include "BunnyEngine/Events/ApplicationEvent.h"
#include "BunnyEngine/Core/Window.h"
#include "BunnyEngine/Core/LayerStack.h"

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
		Application(const std::string& name = "Bunny Engine");
		virtual ~Application();

		void Run();
		void Close() { m_Running = false; }

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() {return *s_Instance;}
		inline Window& GetWindow() { return *m_Window; }
		inline ImGuiLayer& GetImGuiLayer() { return *m_ImGuiLayer; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;
		
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
