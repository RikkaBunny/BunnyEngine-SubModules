#include <BunnyEngine.h>

class Sandbox : public BE::Application {

public:
	Sandbox() {

	}

	~Sandbox() {

	}

};


BE::Application* BE::CreateApplication() {
	return new Sandbox();
}