#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include "GraphicContext.h"

namespace Airwave {

	class Window;

	class OpenGLContext : public GraphicContext {
	public:
		OpenGLContext(Window* window);
		virtual ~OpenGLContext();

		virtual void Init() ;
	private:
		Window* m_Window;

	};

}


#endif // !OPENGL_CONTEXT_H
