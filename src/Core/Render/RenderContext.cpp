#include "RenderContext.h"

namespace Airwave {

	RenderContext::RenderContext(Window* window) {
		m_GraphicContext = Airwave::GraphicContext::Create(window);
	}

	RenderContext::~RenderContext() {
	
	}


}