#include "GameCameraController.h"

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera/Camera.h"

namespace Airwave {

	GameCameraController::GameCameraController(Camera* camera)
		:CameraController(camera)
	{
	}
	GameCameraController::~GameCameraController()
	{
	}
	void GameCameraController::Update(float deltaTime)
	{
		// 最终移动方向
		glm::vec3 moveDir(0.0f);

		// 获取摄像机的方向向量
		auto front = glm::cross(m_Camera->GetUp(), m_Camera->GetRight());
		auto right = m_Camera->GetRight();
		auto up = m_Camera->GetUp();

		// 根据按键更新移动方向
		if (m_KeyMap[GLFW_KEY_W])
		{
			moveDir += front;
		}
		if (m_KeyMap[GLFW_KEY_S])
		{
			moveDir -= front;
		}
		if (m_KeyMap[GLFW_KEY_A])
		{
			moveDir -= right;
		}
		if (m_KeyMap[GLFW_KEY_D])
		{
			moveDir += right;
		}
		if (m_KeyMap[GLFW_KEY_Q])
		{
			moveDir += up;
		}
		if (m_KeyMap[GLFW_KEY_E])
		{
			moveDir -= up;
		}

		// 如果有移动输入
		if (glm::length(moveDir) != 0.0f)
		{
			moveDir = glm::normalize(moveDir); // 标准化移动方向
			m_Camera->SetPosition(m_Camera->GetPosition() + moveDir * m_Speed * deltaTime); // 移动摄像机
		}

	}

	void GameCameraController::OnCursor(float xpos, float ypos)
	{
		float deltaX = (xpos - m_CurrentX) * m_Sensitivity;
		float deltaY = (ypos - m_CurrentY) * m_Sensitivity;

		if (m_RightMousePressed)
		{
			pitch(-deltaY);
			yaw(-deltaX);
		}

		m_CurrentX = xpos;
		m_CurrentY = ypos;
	}

	void GameCameraController::pitch(float angle)
	{
		m_Pitch += angle;
		if (m_Pitch > 89.0f || m_Pitch < -89.0f)
		{
			m_Pitch -= angle;
			return;
		}
		//在gamecameracontrol的情况下，pitch不会影响摄像机的位置，所以这里不需要实现
		auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_Camera->GetRight());
		m_Camera->SetUp(glm::vec3(mat * glm::vec4(m_Camera->GetUp(), 0.0f)));
	}
	void GameCameraController::yaw(float angle)
	{
		auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		m_Camera->SetRight(glm::vec3(mat * glm::vec4(m_Camera->GetRight(), 0.0f)));
		m_Camera->SetUp(glm::vec3(mat * glm::vec4(m_Camera->GetUp(), 0.0f)));
	}
}