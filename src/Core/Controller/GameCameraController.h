#ifndef GAME_CAMERA_CONTROLLER_H
#define GAME_CAMERA_CONTROLLER_H

#include "CameraController.h"


namespace Airwave {
	
	class Camera;

	class GameCameraController : public CameraController {

	public:
		explicit GameCameraController(Camera* camera = nullptr);
		virtual ~GameCameraController();

		virtual void Update(float deltaTime) override;

		virtual void OnCursor(float xpos, float ypos) override;


	public:
		void SetSpeed(float speed) { m_Speed = speed; }


	private:
		void pitch(float angle);
		void yaw(float angle);

	private:
		float m_Pitch = 0.0f;

		float m_Speed = 1.0f;


	};

}


#endif // !GAME_CAMERA_CONTROLLER_H
