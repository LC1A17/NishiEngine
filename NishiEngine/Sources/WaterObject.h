#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "Model.h"
#include "Camera.h"
#include "LightGroup.h"

//3D�I�u�W�F�N�g
class WaterObject
{
private://�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�T�u�N���X
	//�p�C�v���C���Z�b�g
	struct PipelineSet
	{
		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootsignature;
		//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	//�萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj;//�r���[�v���W�F�N�V�����s��
		XMMATRIX world;//���[���h�s��
		XMFLOAT3 cameraPos;//�J�������W�i���[���h���W�j
	};

public://�ÓI�����o�֐�
	//�ÓI������
	static void StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);
	//�O���t�B�b�N�p�C�v���C���̐���
	static void CreateGraphicsPipeline();
	//�J�����̃Z�b�g
	static void SetCamera(Camera* camera) { WaterObject::camera = camera; }
	//���C�g�O���[�v�̃Z�b�g
	static void SetLightGroup(LightGroup* lightGroup) { WaterObject::lightGroup = lightGroup; }
	//�`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	//�`��㏈��
	static void PostDraw();
	//3D�I�u�W�F�N�g����
	static WaterObject* Create(Model* model = nullptr);

private://�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�p�C�v���C��
	static PipelineSet pipelineSet;
	//�J����
	static Camera* camera;
	//���C�g
	static LightGroup* lightGroup;

public://�����o�֐�
	bool Initialize();
	//���t���[������
	void Update();
	//�`��
	void Draw();
	//���W�̎擾
	const XMFLOAT3& GetPosition() { return position; }
	//��]�̎擾
	const XMFLOAT3& GetRotation() { return rotation; }
	//���W�̐ݒ�
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	//�X�P�[���̐ݒ�
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	//���f���̃Z�b�g
	void SetModel(Model* model) { this->model = model; }
	//�r���{�[�h�t���O�̃Z�b�g
	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

private://�����o�ϐ�
	ComPtr<ID3D12Resource> constBuffB0;//�萔�o�b�t�@
	//�F
	XMFLOAT4 color = { 1,1,1,1 };
	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	//���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//�e�I�u�W�F�N�g
	WaterObject* parent = nullptr;
	//���f��
	Model* model = nullptr;
	//�r���{�[�h
	bool isBillboard = false;
};