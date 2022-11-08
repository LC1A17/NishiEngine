#include "Easing.h"

float Easing::EaseIn(float time, float startTime, float length, float endTime)
{
	float x = time / endTime;
	float v = EaseInCubic(x);
	float ret = length * v + startTime;
	return ret;
}

/*
float Easing::EaseOut(float time, float startTime, float length, float endTime)
{
	return time;
}
*/