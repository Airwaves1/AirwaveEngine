#pragma once

#include <QVector3D>
#include <QMatrix4x4>
#include <qmath.h>
#include <qopenglfunctions.h>

// Defines several possible options for camera movement.
// Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = -45.0f;


class Camera
{
public:
    Camera(QVector3D position = QVector3D(0.0f, 0.0f, 3.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    ~Camera();

    QMatrix4x4 getViewMatrix();

    void processKeyboard(Camera_Movement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);	//constrainPitch: 是否限制俯仰角
    void processMouseScroll(float yoffset);


private:
    void updateCameraVectors();

public:
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

};

