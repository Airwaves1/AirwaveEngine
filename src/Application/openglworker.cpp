#include "openglworker.h"
#include <qtimer.h>

OpenGLWoker::OpenGLWoker(OpenGLWidget* openGLWidget, QObject* parent)
    : QObject(parent), m_openGLWidget(openGLWidget)
{
    // 创建定时器，每隔一段时间触发一次 timeout 信号
    QTimer* timer = new QTimer(this);

    // 连接 timeout 信号到 renderWork 槽函数
    connect(timer, &QTimer::timeout, this, &OpenGLWoker::renderWork);

    // 设置定时器间隔，例如每隔16毫秒触发一次，实现约60帧每秒的渲染
    timer->start(16);

}

OpenGLWoker::~OpenGLWoker()
{
}

void OpenGLWoker::renderWork()
{

    m_openGLWidget->render();

}
