
#include "Utils/Common.h"
#include "TrackBallCameraController.h"
#include "Camera/Camera.h"


namespace Airwave {

	TrackBallCameraController::TrackBallCameraController(Camera* camera)
		:CameraController(camera)
	{
	}

	TrackBallCameraController::~TrackBallCameraController()
	{
	}

	void TrackBallCameraController::OnCursor(float xpos, float ypos)
	{
		if (m_LeftMousePressed)
		{
			//调整相机的各个参数
			//1.计算鼠标的偏移量
			float deltaX = (xpos - m_CurrentX) * m_Sensitivity;
			float deltaY = (m_CurrentY - ypos) * m_Sensitivity;

			//2.分开pitch和yaw各级计算
			UpdatePitch(deltaY);
			UpdateYaw(-deltaX);
		}
		//鼠标中键按下，平移
		else if (m_MiddleMousePressed)
		{
			//调整相机的各个参数
			//1.计算鼠标的偏移量
			float deltaX = (xpos - m_CurrentX) * m_MoveSpeed;
			float deltaY = (m_CurrentY - ypos) * m_MoveSpeed;

			m_Camera->SetPosition(m_Camera->GetPosition() - m_Camera->GetRight() * deltaX - m_Camera->GetUp() * deltaY);
		}

		m_CurrentX = xpos;
		m_CurrentY = ypos;
	}

	void TrackBallCameraController::OnScroll(double offset)
	{
		m_Camera->ScaleZoom(offset * m_ScaleSpeed);
	}

	void TrackBallCameraController::Update(float deltaTime)
	{

	}



	void TrackBallCameraController::UpdatePitch(float angle)
	{
		//绕着m_right旋转
		auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_Camera->GetRight());

		//影响当前相机的up向量和位置
		auto up = mat * glm::vec4(m_Camera->GetUp(), 0.0f);
		m_Camera->SetUp(glm::vec3(up));

		auto pos = mat * glm::vec4(m_Camera->GetPosition(), 1.0f);
		m_Camera->SetPosition(glm::vec3(pos));

	}

	void TrackBallCameraController::UpdateYaw(float angle)
	{
		//绕着m_up旋转
		auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_Camera->GetUp());

		//影响当前相机的right向量和位置
		auto right = mat * glm::vec4(m_Camera->GetRight(), 0.0f);
		m_Camera->SetRight(glm::vec3(right));

		auto up = mat * glm::vec4(m_Camera->GetUp(), 0.0f);
		m_Camera->SetUp(glm::vec3(up));

		auto pos = mat * glm::vec4(m_Camera->GetPosition(), 1.0f);
		m_Camera->SetPosition(glm::vec3(pos));
	}

	void TrackBallCameraController::UpdateDistance(float delta)
	{
		//影响当前相机的位置

	}

}