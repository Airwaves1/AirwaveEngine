#include "GraphicContext.h"
#include "OpenGL/OpenGLContext.h"
namespace Airwave {
	std::unique_ptr<GraphicContext> GraphicContext::Create(Window* window) {
#ifdef GRAPHICS_API_OPENGL
		return std::make_unique<OpenGLContext>(window);
#elif GRAPHICS_API_VULKAN
		return std::make_unique<VulkanContext>(window);
#endif 
		return nullptr;
	}


}