#pragma once
#include <BunnyEngine.h>

#ifdef BE_PLATFORM_WINDOWS

extern BE::Application* BE::CreateApplication();

int main(int argc, char** argv) {
	
	BE::Log::Init();
	BE_INFO("Hello Bunny!");
	BE_PROFILE_BEGIN_SESSION("Startup", "BunnyEngineProfile - Startup.json");
	auto app = BE::CreateApplication();
	BE_PROFILE_END_SESSION();

	BE_PROFILE_BEGIN_SESSION("Runtime", "BunnyEngineProfile - Runtime.json");
	app->Run();
	BE_PROFILE_END_SESSION();

	BE_PROFILE_BEGIN_SESSION("Shutdown", "BunnyEngineProfile - Shutdown.json");
	delete app;
	BE_PROFILE_END_SESSION();
}

#endif