#pragma once

namespace OpenGL_Core
{
    /// <summary>
    /// 时间类
    /// </summary>
    class Time
    {
    public:
        /// <summary>
        /// 游戏时间缩放系数 （请勿小于0）
        /// </summary>
        static double TimeScale;

        /// <summary>
        /// 上一帧渲染用时
        /// </summary>
        /// <returns></returns>
        inline static float DeltaTime() { return (float)_deltaTime; }
        /// <summary>
        /// 游戏运行时长
        /// </summary>
        /// <returns></returns>
        inline static float RunTime() { return (float)_time; }
        /// <summary>
        /// 当前渲染帧数
        /// </summary>
        /// <returns></returns>
        inline static unsigned int FrameCount() { return _frameCount; }
        /// <summary>
        /// 游戏真实运行时间
        /// </summary>
        /// <returns></returns>
        inline static float RealtimeSinceStartup() { return (float)_realtimeSinceStartup; }
        inline static float FPS() { return _fps; }

        /// <summary>
        /// 更新时间数据
        /// </summary>
        /// <param name="runTime"></param>
        static void Update(double runTime);

    private:
        /// <summary>
        /// 游戏真实运行时长
        /// </summary>
        static double _realtimeSinceStartup;
        /// <summary>
        /// 游戏运行时长
        /// </summary>
        static double _time;
        /// <summary>
        /// 上一帧渲染用时
        /// </summary>
        static double _deltaTime;
        /// <summary>
        /// 当前渲染帧数
        /// </summary>
        static unsigned int _frameCount;
        static float _fps;
    };


}