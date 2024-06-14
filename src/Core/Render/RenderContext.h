#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "Utils/Common.h"
#include "GraphicContext.h"
#include "OpenGL/OpenGLContext.h"


namespace Airwave {

	class Window;

	class RenderContext {
	public:
		RenderContext(Window* window);
		~RenderContext();

		GraphicContext* GetOpenGLContext() { return m_GraphicContext.get(); }

	private:
		std::shared_ptr<GraphicContext> m_GraphicContext;
		
	};

}


#endif // !RENDER_CONTEXT_H
