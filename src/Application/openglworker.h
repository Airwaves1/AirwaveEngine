#pragma once

#include <qobject.h>
#include <OpenGL/OpenGLWidget.h>


class QTimer;

class OpenGLWoker : public QObject
{
    Q_OBJECT
public:
    OpenGLWoker(OpenGLWidget* openGLWidget, QObject *parent = nullptr);
    ~OpenGLWoker();

public:
    void renderWork();


private:
    OpenGLWidget* m_openGLWidget = nullptr;

    QTimer* timer = nullptr;
};

