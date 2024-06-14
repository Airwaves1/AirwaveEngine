#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "Utils/Common.h"


namespace Airwave {

	class Camera;

	class CameraController {
	public:
		CameraController(Camera* camera = nullptr);
		virtual ~CameraController();

		virtual void Update(float deltaTime);

		virtual void OnMouse(int button, int action, double xpos, double ypos);
		virtual void OnCursor(float xpos, float ypos);
		virtual void OnKey(int key, int action, int mods);
		virtual void OnScroll(double offset);	// offset 可能为 +1 或者为 -1

	public:
		void SetCamera(Camera* camera) { m_Camera = camera; }
		Camera* GetCamera() { return m_Camera; }

		void SetSensitivity(float sensitivity) { m_Sensitivity = sensitivity; }
		float GetSensitivity() const { return m_Sensitivity; }

		void SetScaleSpeed(float speed) { m_ScaleSpeed = speed; }
		float GetScaleSpeed() const { return m_ScaleSpeed; }


	protected:
		bool m_LeftMousePressed = false;
		bool m_RightMousePressed = false;
		bool m_MiddleMousePressed = false;

		float m_CurrentX = 0.0f;
		float m_CurrentY = 0.0f;

		std::unordered_map<int, bool> m_KeyMap;	//键盘按键状态

		Camera* m_Camera = nullptr; //存储当前控制的那一个相机

		float m_Sensitivity = 0.1f;	//灵敏度

		float m_ScaleSpeed = 0.2f;	//缩放速度
	};
}


#endif // !CAMERA_CONTROLLER_H
