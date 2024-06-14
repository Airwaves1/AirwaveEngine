#include "PerspectiveCamera.h"



namespace Airwave{



	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float, float)
		:m_FOV(fov), m_Aspect(aspect), m_Near(0.1f), m_Far(100.0f)
	{

	}

	PerspectiveCamera::~PerspectiveCamera()
	{
	}


	glm::mat4 PerspectiveCamera::GetProjectionMatrix()
	{
		return glm::perspective(glm::radians(m_FOV), m_Aspect, m_Near, m_Far);
	}

	void PerspectiveCamera::ScaleZoom(float offset)
	{
		auto front = glm::cross(m_Up, m_Right);
		m_Position += front * offset;
	}

}