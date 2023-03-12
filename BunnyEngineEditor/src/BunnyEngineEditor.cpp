// ---------Entry Point ---------------
#include <BunnyEngine/Core/EntryPoint.h>
// ------------------------------------

#include <BunnyEngine.h>

#include <glm/gtc/matrix_transform.hpp>

#include "../imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace BE {
	class BunnyEngineEditor : public Application {

	public:
		BunnyEngineEditor() 
			:Application("BunnyEngine Editor")
		{
			PushLayer(new EditorLayer());
		}

		~BunnyEngineEditor() {

		}

	};


	BE::Application* CreateApplication() {
		return new BunnyEngineEditor();
	}
}