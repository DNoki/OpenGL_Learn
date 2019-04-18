#include "Time.h"

double Time::_time = 0.0f;
double Time::_lastTime = 0.0f;
double Time::_deltaTime = 0.0f;
size_t Time::_frameCount = 0;

float Time::DeltaTime() { return (float)_deltaTime; }

float Time::RunTime() { return (float)_time; }

size_t Time::FrameCount(){	return _frameCount;}

void Time::Update(double runTime)
{
	_lastTime = _time;
	_time = runTime;
	_deltaTime = _time - _lastTime;
	_frameCount++;
}