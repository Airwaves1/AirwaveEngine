#include "openglwidget.h"

#include <QSurfaceFormat>
#include <QVector3D>
#include <QMatrix4x4>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

#include "Utils/utils.h"
#include "Utils/logger.h"

#include "shader.h"
#include "camera.h"
#include "mytexture.h"
#include "model.h"
#include "mesh.h"


/*
    临时准备的数据
*/
GLfloat vertices[] = {
    // Positions          // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

GLfloat lightVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


QVector3D cubePositions[] = {
    QVector3D(0.0f,  0.0f,  0.0f),
    QVector3D(2.0f,  5.0f, -15.0f),
    QVector3D(-1.5f, -2.2f, -2.5f),
    QVector3D(-3.8f, -2.0f, -12.3f),
    QVector3D(2.4f, -0.4f, -3.5f),
    QVector3D(-1.7f,  3.0f, -7.5f),
    QVector3D(1.3f, -2.0f, -2.5f),
    QVector3D(1.5f,  2.0f, -2.5f),
    QVector3D(1.5f,  0.2f, -1.5f),
    QVector3D(-1.3f,  1.0f, -1.5f)
};

GLuint VBO = 0;
GLuint cubeVAO = 0;
GLuint lightVAO = 0;


OpenGLWidget::OpenGLWidget(QWidget *parent)
{
    QSurfaceFormat format;
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);

    //初始化相机，设置相机的位置，观察方向，上方向，俯仰角和偏航角
    m_camera = new Camera(QVector3D(0.0f, 0.0f, 3.0f), QVector3D(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    m_lightPos = QVector3D(0.5f, 1.0f, 1.0f);
    m_flashlight = 0;

    setFocusPolicy(Qt::StrongFocus);

    m_lastFrame = 0.0f;
    m_deltaTime = 0.0f;
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();

    OPENGL_EXTRA_FUNCTIONS->glDeleteVertexArrays(1, &cubeVAO);
    OPENGL_EXTRA_FUNCTIONS->glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    doneCurrent();

}

void OpenGLWidget::render()
{
    update();
}

void OpenGLWidget::setLightPos(const QVector3D &lightPos)
{
    Logger::debug("set light pos: %1, %2, %3", lightPos.x(), lightPos.y(), lightPos.z());
    m_lightPos = lightPos;

}

void OpenGLWidget::initializeGL()
{
    // 初始化OpenGL状态

    initializeOpenGLFunctions();

    // 设置清空颜色缓冲区时的颜色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);


    m_frameTimer.start();
    m_deltaTime = 0.0f;

    // 设置摄像机的投影矩阵等属性
    m_camera->MovementSpeed = 0.05f;
    m_camera->MouseSensitivity = 0.1f;
    m_camera->Zoom = 45.0f;

    glEnable(GL_DEPTH_TEST);
    //背面剔除
    //glEnable(GL_CULL_FACE);


    prepareShaderProgram();
    prepareTexture();
    prepareModel();
    prepareMesh();

    prepareVAO_cube();
    prepareVAO_light();

}

void OpenGLWidget::resizeGL(int w, int h)
{

}


void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 使用QElapsedTimer实现更平滑的动画
    static QElapsedTimer timer;
    if (!timer.isValid()) {
        timer.start();
    }

    qint64 elapsedMilliseconds = timer.elapsed();
    float seconds = elapsedMilliseconds / 1000.0f;
    GLfloat currentFrame = seconds;
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;

    doMovement();

    drawCube();
    drawLight();

}

void OpenGLWidget::prepareShaderProgram()
{
    m_cubeShader = new Shader( "./assets/shader/cube.vert", "./assets/shader/cube.frag");
    m_lightingShader = new Shader( "./assets/shader/light.vert", "./assets/shader/light.frag");
}

void OpenGLWidget::prepareTexture()
{
    m_texture1 = new MyTexture("./assets/texture/container2.png", 0);
    m_texture2 = new MyTexture("./assets/texture/container2_specular.png", 1);
}

void OpenGLWidget::prepareModel()
{

}

void OpenGLWidget::prepareMesh()
{

}

void OpenGLWidget::prepareVAO_cube()
{
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(void*)(3*sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(void*)(6*sizeof(GLfloat)));

OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

void OpenGLWidget::prepareVAO_light()
{
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(lightVAO);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

void OpenGLWidget::drawCube()
{
    //激活着色器程序
    m_cubeShader->begin();

    //材质
    m_cubeShader->setUniformInt("material.diffuse", 0);
    m_texture1->bind();

    m_cubeShader->setUniformInt("material.specular", 1);
    m_texture2->bind();


    m_cubeShader->setUniformVec3("material.specular", 0.5f, 0.5f, 0.5f);
    m_cubeShader->setUniformFloat("material.shininess", 32.0f);

    //平行光
    m_cubeShader->setUniformVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    m_cubeShader->setUniformVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    m_cubeShader->setUniformVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    m_cubeShader->setUniformVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    //点光源
    m_cubeShader->setUniformVec3("pointLight.position", m_lightPos);
    m_cubeShader->setUniformVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
    m_cubeShader->setUniformVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
    m_cubeShader->setUniformVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
    m_cubeShader->setUniformFloat("pointLight.constant", 1.0f);
    m_cubeShader->setUniformFloat("pointLight.linear", 0.09);
    m_cubeShader->setUniformFloat("pointLight.quadratic", 0.032);

    //聚光灯
    m_cubeShader->setUniformVec3("spotLight.position", m_camera->Position);
    m_cubeShader->setUniformVec3("spotLight.direction", m_camera->Front);
    m_cubeShader->setUniformVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    m_cubeShader->setUniformVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    m_cubeShader->setUniformVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    m_cubeShader->setUniformFloat("spotLight.constant", 1.0f);
    m_cubeShader->setUniformFloat("spotLight.linear", 0.09);
    m_cubeShader->setUniformFloat("spotLight.quadratic", 0.032);
    m_cubeShader->setUniformFloat("spotLight.cutOff", cos(qDegreesToRadians(12.5f)));
    m_cubeShader->setUniformFloat("spotLight.outerCutOff", cos(qDegreesToRadians(15.0f)));
    m_cubeShader->setUniformInt("spotLight.flashLight", m_flashlight);

    m_cubeShader->setUniformVec3("viewPos", m_camera->Position);

    m_projection.setToIdentity();
    m_projection.perspective(m_camera->Zoom,  (float)width() / (float)height(), 0.1f, 100.0f);
    m_view = m_camera->getViewMatrix();
    m_model.setToIdentity();

    m_cubeShader->setUniformMat4("projection", m_projection);
    m_cubeShader->setUniformMat4("view", m_view);
    m_cubeShader->setUniformMat4("model", m_model);

    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(cubeVAO);

    for (unsigned int i = 0; i < 10; i++)
    {
        m_model.setToIdentity();
        m_model.translate(cubePositions[i]);
        float angle = 20.0f * i;
        m_model.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));
        m_cubeShader->setUniformMat4("model", m_model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
    m_cubeShader->end();

}

void OpenGLWidget::drawLight()
{
    //周期
    float period = 1;

    //计算正弦函数，将其映射到【-1，1】之间
    float sinValue = sin(m_frameTimer.elapsed() / 1000.0f * period);

    //缩放正弦函数的输出，使模型在Z轴上来回摆动
    float ZTranslation = sinValue * 2.0f;	//增大缩放因子，使摆动更明显

    m_lightPos = QVector3D(m_lightPos.x(), m_lightPos.y(), ZTranslation);

    m_lightingShader->begin();
    m_projection.setToIdentity();
    m_projection.perspective(m_camera->Zoom,  (float)width() / (float)height(), 0.1f, 100.0f);
    m_view = m_camera->getViewMatrix();
    m_model.setToIdentity();
    m_model.translate(m_lightPos);
    m_model.scale(0.2f);
    m_lightingShader->setUniformMat4("projection", m_projection);
    m_lightingShader->setUniformMat4("view", m_view);
    m_lightingShader->setUniformMat4("model", m_model);

    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
    m_lightingShader->end();
}

void OpenGLWidget::drawModel()
{

}

void OpenGLWidget::drwaTestModel()
{

}














void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {

        m_lastMousePos = event->pos();
    }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        m_lastMousePos = QPoint(); // 重置为无效值
    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton)
    {
        if (!m_lastMousePos.isNull())
        {
            int xoffset = event->position().x() - m_lastMousePos.x();
            int yoffset = m_lastMousePos.y() - event->position().y(); // 此处颠倒了 y 方向
            m_lastMousePos = event->pos();

            m_camera->processMouseMovement(xoffset, yoffset);
        }
    }
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    // 获取滚轮滚动的角度
    int delta = event->angleDelta().y();

    // 根据滚动的角度进行相应的处理
    if (delta > 0)
    {
        // 滚轮向上滚动
        // 进行相应的操作，例如放大
        m_camera->processMouseScroll(5.0f); // 你可能需要根据具体情况调整参数
    }
    else if (delta < 0)
    {
        // 滚轮向下滚动
        // 进行相应的操作，例如缩小
        m_camera->processMouseScroll(-5.0f); // 你可能需要根据具体情况调整参数
    }

    // 阻止事件传递给父类（如果有父类）
    event->accept();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    //按下F打开手电筒
    if (event->key() == Qt::Key_F)
    {
        m_flashlight = 1 - m_flashlight;
    }

    // 设置按键状态
    m_keyStates[event->key()] = true;
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    // 设置按键状态
    m_keyStates[event->key()] = false;

    if (event->key() == Qt::Key_F)
    {

    }
}

void OpenGLWidget::doMovement()
{

    // 按键状态
    if (m_keyStates[Qt::Key_W])
    {
        m_camera->processKeyboard(Camera_Movement::FORWARD, m_deltaTime);
    }
    if (m_keyStates[Qt::Key_S])
    {
        m_camera->processKeyboard(Camera_Movement::BACKWARD, m_deltaTime);
    }
    if (m_keyStates[Qt::Key_A])
    {
        m_camera->processKeyboard(Camera_Movement::LEFT, m_deltaTime);
    }
    if (m_keyStates[Qt::Key_D])
    {
        m_camera->processKeyboard(Camera_Movement::RIGHT, m_deltaTime);
    }
    if (m_keyStates[Qt::Key_Q])
    {
        m_camera->processKeyboard(Camera_Movement::UP, m_deltaTime);
    }
    if (m_keyStates[Qt::Key_E])
    {
        m_camera->processKeyboard(Camera_Movement::DOWN, m_deltaTime);
    }

    //按住Shift给速度加倍
    if (m_keyStates[Qt::Key_Shift])
    {
        m_camera->MovementSpeed = 6.0f;
    }
    if (!m_keyStates[Qt::Key_Shift])
    {
        m_camera->MovementSpeed = 3.0f;
    }
}
