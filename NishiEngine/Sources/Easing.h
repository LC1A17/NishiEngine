#pragma once

//イージング
class Easing
{
public://メンバ関数
	static float EaseIn(float time, float startTime, float length, float endTime);//EaseIn
	//static float EaseOut(float time, float startTime, float length, float endTime);//EaseOut
	static float EaseInCubic(float x) { return x * x * x; }

private://メンバ変数

};