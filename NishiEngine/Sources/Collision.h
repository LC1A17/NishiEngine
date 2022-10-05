#pragma once
#include "CollisionPrimitive.h"

using namespace DirectX;

//�����蔻��w���p�[�N���X
class Collision
{
public:
	//���ƕ��ʂ̓����蔻��
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, XMVECTOR* inter = nullptr);
	//�_�ƎO�p�`�̍ŋߐړ_�����߂�
	static void ClosestPtPoint2Triangle(const XMVECTOR& point, const Triangle& triangle, XMVECTOR* closest);
	//���Ɩ@���t���O�p�`�̓�����`�F�b�N
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr);
	//���C�ƕ��ʂ̓����蔻��
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, XMVECTOR* inter = nullptr);
	//���C�Ɩ@���t���O�p�`�̓����蔻��
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, XMVECTOR* inter = nullptr);
	//���C�Ƌ��̓����蔻��
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, XMVECTOR* inter = nullptr);
};