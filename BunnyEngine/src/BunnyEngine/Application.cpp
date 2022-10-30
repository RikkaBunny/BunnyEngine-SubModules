#include "BEpch.h"
#include "Application.h"

#include "BunnyEngine/Events/ApplicationEvent.h"
#include "BunnyEngine/Log.h"

namespace BE {

	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {

		WindowResizeEvent e(1280, 720);
		BE_TRACE(e);
		while (true);
	}

}
