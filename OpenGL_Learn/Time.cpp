#include "Time.h"

namespace OpenGL_Learn
{
    double Time::TimeScale = 1.0;

    double Time::_realtimeSinceStartup = 0.0;
    double Time::_time = 0.0;
    double Time::_deltaTime = 0.0;
    unsigned int Time::_frameCount = 0;
    float Time::_fps = 0.0;

    void Time::Update(double runTime)
    {
        // 上一帧的游戏运行时长（用于计算）
        static double _lastTime = 0.0f;

        _lastTime = _realtimeSinceStartup;
        _realtimeSinceStartup = runTime;
        _deltaTime = (_realtimeSinceStartup - _lastTime) * TimeScale;
        _time += _deltaTime;
        _frameCount++;

        // 计算FPS
        static int counter = 0;
        static float totalFPS = 0.0f;
        counter++;
        totalFPS += 1.0f / Time::DeltaTime();
        if (counter >= 10)
        {
            _fps = totalFPS / counter;
            counter = 0;
            totalFPS = 0.0f;
        }
    }
}