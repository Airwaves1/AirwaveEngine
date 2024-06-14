#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Windows/Window.h"
#include "Windows/GLFWWindow.h"

#include "Utils/Log.h"

namespace Airwave {

	OpenGLContext::OpenGLContext(Window* window)
		: m_Window(window)
	{
		if (!window) {
			LOG_ERROR("Window is nullptr");
		}
	}

	OpenGLContext::~OpenGLContext()
	{
	}

	void OpenGLContext::Init()
	{
		int state = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!state) {
			LOG_ERROR("Failed to initialize GLAD");
			return;
		}

		unsigned char* glVersion;
		unsigned char* glVendor;
		unsigned char* glRenderer;

		glVersion = (unsigned char*)glGetString(GL_VERSION);
		glVendor = (unsigned char*)glGetString(GL_VENDOR);
		glRenderer = (unsigned char*)glGetString(GL_RENDERER);

		std::string glVendorStr((char*)glVendor);
		std::string glVersionStr((char*)glVersion);
		std::string glRendererStr((char*)glRenderer);

		LOG_INFO("OpenGL Version: {0}", glVersionStr);
		LOG_INFO("OpenGL Vendor: {0}", glVendorStr);
		LOG_INFO("OpenGL Renderer: {0}", glRendererStr);



	}


}