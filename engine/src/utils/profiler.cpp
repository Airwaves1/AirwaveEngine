#include "utils/profiler.hpp"
#include <iostream>

// 开始记录 CPU 帧时间
void Profiler::beginFrame()
{
    startTime = std::chrono::high_resolution_clock::now();
}

// 结束 CPU 计时，并计算 FPS
void Profiler::endFrame()
{
    auto endTime = std::chrono::high_resolution_clock::now();
    cpuFrameTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    // 计算 FPS
    frameTimes.push_back(cpuFrameTime);
    if (frameTimes.size() > 100) 
        frameTimes.erase(frameTimes.begin());

    double avgFrameTime = 0.0;
    for (double t : frameTimes) avgFrameTime += t;
    avgFrameTime /= frameTimes.size();
    fps = (avgFrameTime > 0) ? int(1000.0 / avgFrameTime) : 0;
}

// 获取 CPU 渲染时间
double Profiler::getCPUFrameTime() const
{
    return cpuFrameTime;
}

// 获取 FPS
int Profiler::getFPS() const
{
    return fps;
}

// 开始 GPU 计时
void Profiler::beginGPUTime()
{
    if (gpuQueryActive) return;
    glGenQueries(2, gpuQuery);
    glQueryCounter(gpuQuery[0], GL_TIMESTAMP);
    gpuQueryActive = true;
}

// 结束 GPU 计时并计算时间
void Profiler::endGPUTime()
{
    if (!gpuQueryActive) return;

    glQueryCounter(gpuQuery[1], GL_TIMESTAMP);

    GLint available = 0;
    while (!available) {
        glGetQueryObjectiv(gpuQuery[1], GL_QUERY_RESULT_AVAILABLE, &available);
    }

    GLuint64 startTime, endTime;
    glGetQueryObjectui64v(gpuQuery[0], GL_QUERY_RESULT, &startTime);
    glGetQueryObjectui64v(gpuQuery[1], GL_QUERY_RESULT, &endTime);

    gpuFrameTime = (endTime - startTime) / 1000000.0; // 转换为毫秒
    glDeleteQueries(2, gpuQuery);
    gpuQueryActive = false;
}

// 获取 GPU 渲染时间
double Profiler::getGPUFrameTime() const
{
    return gpuFrameTime;
}
