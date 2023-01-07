#include "BEpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace BE {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_windowHandle(windowHandle)
	{
		BE_CORE_ASSERT(windowHandle, "Window handle is null!")
	}
	void BE::OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		BE_CORE_ASSERT(status, "Failed to initiailize Glad!");

		BE_CORE_INFO("OpenGL Info : ");
		BE_CORE_INFO("    Vendor : {0} ", glGetString(GL_VENDOR));
		BE_CORE_INFO("    Renderer : {0} ", glGetString(GL_RENDERER));
		BE_CORE_INFO("    Version : {0} ", glGetString(GL_VERSION));
	}

	void BE::OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

}

