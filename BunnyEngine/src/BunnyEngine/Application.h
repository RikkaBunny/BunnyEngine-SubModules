#pragma once
#include "Core.h"

namespace BE {

	class BE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}
