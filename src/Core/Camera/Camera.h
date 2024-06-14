#ifndef CAMERA_H
#define CAMERA_H

#include "Utils/Common.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Airwave {

	class Camera {
	public:
		Camera();

		virtual ~Camera();

		glm::mat4 GetViewMatrix() { return glm::lookAt(m_Position, m_Position + (glm::cross(m_Up, m_Right)), m_Up); }
		virtual glm::mat4 GetProjectionMatrix() { return glm::identity<glm::mat4>(); }

		virtual void ReSetCamera();	//重置摄像机
		virtual void ScaleZoom(float offset) { }

	public:
		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetPosition(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }
		const glm::vec3& GetPosition() const { return m_Position; }

		void SetUp(const glm::vec3& up) { m_Up = up; }
		void SetUp(float x, float y, float z) { m_Up = glm::vec3(x, y, z); }
		const glm::vec3& GetUp() const { return m_Up; }


		void SetRight(const glm::vec3& right) { m_Right = right; }
		void SetRight(float x, float y, float z) { m_Right = glm::vec3(x, y, z); }
		const glm::vec3& GetRight() const { return m_Right; }

	protected:

		virtual	void UpdateCameraVectors();

		glm::vec3 m_Position { 0.0f, 0.0f, 5.0f };
		glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Right{ 1.0f, 0.0f, 0.0f };
		glm::vec3 m_WorldUp;

		float m_Zoom { 45.0f };
	};


}



#endif // !CAMERA_H
