#pragma once

//�C�[�W���O
class Easing
{
public://�����o�֐�
	static float EaseIn(float time, float startTime, float length, float endTime);//EaseIn
	//static float EaseOut(float time, float startTime, float length, float endTime);//EaseOut
	static float EaseInCubic(float x) { return x * x * x; }

private://�����o�ϐ�

};