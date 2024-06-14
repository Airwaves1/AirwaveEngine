#ifndef GRAPHIC_CONTEXT_H
#define GRAPHIC_CONTEXT_H

#include "Utils/Common.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Airwave {
	class Window;
	
	class GraphicContext {
	public:
		GraphicContext(const GraphicContext&) = delete;
		GraphicContext& operator=(const GraphicContext&) = delete;

		virtual ~GraphicContext() = default;

		static std::unique_ptr<GraphicContext> Create(Window* window);

	protected:
		GraphicContext() = default;
	};

}

#endif // !GRAPHIC_CONTEXT_H
