#include "Window.h"
#include "GLFWWindow.h"

namespace Airwave {
	std::unique_ptr<Window> Window::Create(uint32_t width, uint32_t height, const char* title) {
#ifdef PLATFORM_WIN32
		return std::make_unique<GLFWWindow>(width, height, title);
#elif PLATFORM_LINUX
		return std::make_unique<GLFWWindow>(width, height, title);
#elif PLATFORM_MACOS
		return std::make_unique<WindowMacOS>(width, height, title);
#endif
		return nullptr;
	}

}

