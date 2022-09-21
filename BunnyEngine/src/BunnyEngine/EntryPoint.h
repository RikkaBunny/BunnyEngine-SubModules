#pragma once

#ifdef BE_PLATFORM_WINDOWS

extern BE::Application* BE::CreateApplication();

int main(int argc, char** argv) {
	auto app = BE::CreateApplication();
	app->Run();
	delete app;
}

#endif