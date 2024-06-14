#pragma once	
#include "CameraController.h"


namespace Airwave {

	class TrackBallCameraController : public CameraController
	{

	public:
		TrackBallCameraController(Camera* camera = nullptr);
		~TrackBallCameraController();

		void OnCursor(float xpos, float ypos) override;
		void OnScroll(double offset) override;

		void Update(float deltaTime) override;


	private:
		void UpdatePitch(float angle);
		void UpdateYaw(float angle);
		void UpdateDistance(float delta);

	private:
		float m_MoveSpeed = 0.01f;

	};

}

