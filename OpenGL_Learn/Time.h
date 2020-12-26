#pragma once

namespace OpenGL_Learn
{
    class Time
    {
    public:
        // 游戏时间缩放系数 （请勿小于0）
        static double TimeScale;

        // 上一帧渲染用时
        inline static float DeltaTime() { return (float)_deltaTime; }
        // 游戏运行时长
        inline static float RunTime() { return (float)_time; }
        // 当前渲染帧数
        inline static unsigned int FrameCount() { return _frameCount; }
        // 游戏真实运行时间
        inline static float RealtimeSinceStartup() { return (float)_realtimeSinceStartup; }
        inline static float FPS() { return _fps; }

        // 更新时间数据
        static void Update(double runTime);

    private:
        // 游戏真实运行时长
        static double _realtimeSinceStartup;
        // 游戏运行时长
        static double _time;
        // 上一帧渲染用时
        static double _deltaTime;
        // 当前渲染帧数
        static unsigned int _frameCount;
        static float _fps;
    };


}