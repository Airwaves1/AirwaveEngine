#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <QMatrix4x4>
#include <QVector3D>
#include <QPoint>
#include <QHash>

#include <QElapsedTimer>


class Shader;
class Camera;
class MyTexture;
class Model;
class Mesh;


class OpenGLWidget : public QOpenGLWidget , protected QOpenGLFunctions
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

    void render();
    void setLightPos(const QVector3D& lightPos);

protected:
    //OpenGL相关
    void initializeGL() override;           // 负责初始化OpenGL状态
    void resizeGL(int w, int h) override;   // 当窗口大小改变时，负责更新OpenGL状态
    void paintGL() override;                // 负责渲染OpenGL

private:
    void prepareShaderProgram();
    void prepareTexture();
    void prepareModel();
    void prepareMesh();
    void prepareVAO_cube();
    void prepareVAO_light();

    void drawCube();
    void drawLight();
    void drawModel();
    void drwaTestModel();



protected:
    //鼠标事件
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    //键盘事件
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void doMovement();




private:
    QElapsedTimer m_frameTimer;
    GLfloat m_deltaTime = 0.0f;   // 当前帧遇上一帧的时间差
    GLfloat m_lastFrame = 0.0f;   // 上一帧的时间

    //键盘状态
    QHash<int, bool> m_keyStates;
    //鼠标上一次位置
    QPoint m_lastMousePos;
    //手电筒是否开启
    int m_flashlight = 0;

    //camera
    Camera* m_camera = nullptr;
    //变换矩阵
    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    //光源位置
    QVector3D m_lightPos;


    //shader
    Shader* m_lightingShader = nullptr;
    Shader* m_cubeShader = nullptr;
    //texture
    MyTexture* m_texture1 = nullptr;
    MyTexture* m_texture2 = nullptr;


};

#endif // OPENGLWIDGET_H
