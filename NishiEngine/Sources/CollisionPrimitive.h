#pragma once
#include <DirectXMath.h>

using namespace DirectX;

//�����蔻��v���~�e�B�u

//��
struct Sphere
{
	XMVECTOR center = { 0,0,0,1 };//���S���W
	float radius = 1.0f;//���a
};

//����
struct Plane
{
	XMVECTOR normal = { 0,1,0,0 };//�@���x�N�g��
	float distance = 0.0f;//���_�i0, 0, 0�j����̋���
};

//���C
struct Ray
{
	XMVECTOR start = { 0,0,0,1 };//�n�_���W
	XMVECTOR dir = { 1,0,0,0 };//����
};

//�@���t���O�p�`�i���v��肪�\�ʁj
class Triangle
{
public:
	//���_���W3��
	XMVECTOR p0;
	XMVECTOR p1;
	XMVECTOR p2;

	//�@���x�N�g��
	XMVECTOR normal;

	//�@���̌v�Z
	void ComputeNormal();
};