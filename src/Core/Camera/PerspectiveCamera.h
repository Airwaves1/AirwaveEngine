#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "Camera.h"

namespace Airwave {

	class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(float fov, float aspect, float near, float far);
		~PerspectiveCamera();

		virtual glm::mat4 GetProjectionMatrix() override;
		virtual void ScaleZoom(float offset) override;

	public:
		void SetFOV(float fov) { m_FOV = fov; }
		void SetAspect(float aspect) { m_Aspect = aspect; }
		void SetNear(float near) { m_Near = near; }
		void SetFar(float far) { m_Far = far; }

		float GetFOV() const { return m_FOV; }
		float GetAspect() const { return m_Aspect; }
		float GetNear() const { return m_Near; }
		float GetFar() const { return m_Far; }


	private:
		float m_FOV;
		float m_Aspect;
		float m_Near;
		float m_Far;
	};

}


#endif // !PERSPECTIVE_CAMERA_H
