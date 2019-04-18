#pragma once

class Time
{
public:
	static float DeltaTime();
	static float RunTime();
	static size_t FrameCount();

	// ֡ѭ������ǰ����
	static void Update(double runTime);

private:
	static double _time;
	static double _lastTime;
	static double _deltaTime;
	static size_t _frameCount;
};

