#pragma once

namespace OpenGL_Learn
{
    class Time
    {
    public:
        // ��Ϸʱ������ϵ�� ������С��0��
        static double TimeScale;

        // ��һ֡��Ⱦ��ʱ
        inline static float DeltaTime() { return (float)_deltaTime; }
        // ��Ϸ����ʱ��
        inline static float RunTime() { return (float)_time; }
        // ��ǰ��Ⱦ֡��
        inline static unsigned int FrameCount() { return _frameCount; }
        // ��Ϸ��ʵ����ʱ��
        inline static float RealtimeSinceStartup() { return (float)_realtimeSinceStartup; }
        inline static float FPS() { return _fps; }

        // ����ʱ������
        static void Update(double runTime);

    private:
        // ��Ϸ��ʵ����ʱ��
        static double _realtimeSinceStartup;
        // ��Ϸ����ʱ��
        static double _time;
        // ��һ֡��Ⱦ��ʱ
        static double _deltaTime;
        // ��ǰ��Ⱦ֡��
        static unsigned int _frameCount;
        static float _fps;
    };


}