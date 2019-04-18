#pragma once

class Time
{
public:
	static float DeltaTime();
	static float RunTime();
	static size_t FrameCount();

	// 帧循环动作前调用
	static void Update(double runTime);

private:
	static double _time;
	static double _lastTime;
	static double _deltaTime;
	static size_t _frameCount;
};

