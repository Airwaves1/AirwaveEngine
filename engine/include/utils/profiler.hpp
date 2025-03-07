#pragma once
#include <chrono>
#include <vector>
#include <glad/glad.h>

class Profiler
{
public:
    void beginFrame();
    void endFrame();
    
    double getCPUFrameTime() const;
    double getGPUFrameTime() const;
    int getFPS() const;
    
    void beginGPUTime();
    void endGPUTime();
    
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    double cpuFrameTime = 0.0;
    
    GLuint gpuQuery[2] = {0};
    bool gpuQueryActive = false;
    double gpuFrameTime = 0.0;

    std::vector<double> frameTimes;
    int fps = 0;
};
