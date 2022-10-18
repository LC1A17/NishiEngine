#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "WaterObject.h"
#include "ParticleManager.h"
#include "DebugText.h"
#include "Audio.h"
#include "DebugCamera.h"
#include "LightGroup.h"

//ゲームシーン
class GameScene
{
private://エイリアス
	//Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://サブクラス
	//ループ分岐用
	enum Scene
	{
		Title, Game, End
	};

private://静的メンバ変数
	static const int debugTextTexNumber = 0;

public://メンバ関数
	//コンストクラタ
	GameScene();
	//デストラクタ
	~GameScene();
	//初期化
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);
	//毎フレーム処理
	void Update();
	//描画
	void Draw();
	//パーティクル生成
	void CreateParticles();

private://メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText;	

	//ゲームシーン用
	DebugCamera* camera = nullptr;
	ParticleManager* particleMan = nullptr;

	LightGroup* lightGroup = nullptr;
	float ambientColor0[3] = { 1,1,1 };
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };
	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };
	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//モデル関連
	Model* skydomeModel = nullptr;//天球のモデル
	Model* groundModel = nullptr;//地面のモデル
	Model* playerModel = nullptr;//プレイヤーのモデル
	Model* pBulletModel = nullptr;//プレイヤーの弾のモデル
	Model* enemyModel = nullptr;//敵のモデル
	Model* partsModel = nullptr;//落ちているパーツのモデル
	Model* playerPartsModel = nullptr;//プレイヤーのパーツのモデル

	//オブジェクト関連
	Object3d* skydome = nullptr;//天球のオブジェクト
	WaterObject* groundWater = nullptr;//地面のオブジェクト
	Object3d* ground = nullptr;//地面のオブジェクト
	Object3d* player = nullptr;//プレイヤーのオブジェクト
	Object3d* pBullet[100];//プレイヤーの弾のオブジェクト
	Object3d* enemy[100];//敵のオブジェクト
	Object3d* parts[100];//落ちているパーツのオブジェクト
	Object3d* playerParts[100];//プレイヤーのパーツのオブジェクト

	//スプライト関連
	Sprite* titleBack = nullptr;//タイトル背景のスプライト
	Sprite* gameBack = nullptr;//ゲーム背景のスプライト
	Sprite* resultBack = nullptr;//リザルト背景のスプライト

	//オブジェクト座標関連
	XMFLOAT3 skydomePos = { 0, 0, 0 };//天球の座標
	XMFLOAT3 cameraPos = { 0, 4.5, 0 };//カメラの座標
	XMFLOAT3 pPos = { 0, 2, 3 };//プレイヤーの座標
	XMFLOAT3 pBulletPos[100];//プレイヤーの弾の座標
	XMFLOAT3 pBulletSpeed = { 0, 0, 1 };//プレイヤーの弾のスピード
	XMFLOAT3 ePos[100];//敵の座標
	XMFLOAT3 partsPos[100];//パーツの座標
	XMFLOAT3 pPartsPos[100];//プレイヤーのパーツの座標
	XMFLOAT3 domeRot = { 0, 0, 0 };//天球の回転

	//処理関連
	int scene = Title;//ゲームシーン
	int enemyCount = 100;//配置された敵の数
	int deadEnemyCount = 0;//撃破した敵の数
	int partsCount = 0;//パーツの取得数

	bool pBulletArive[100];//弾が生きているかの判定
	int pBulletAriveTime[100];//弾が生きている時間
	int pBulletCount = 0;//弾発射までのカウント
	int pBulletInterval = 20;//弾発射までの時間

	bool enemyArive[100];//敵が生きているかの判定
	bool partsArive[100];//パーツが生きているかの判定
	bool pPartsArive[100];//プレイヤーのパーツが生きているかの判定

	int enemySpawnTimer = 0;//敵の出現までのカウント
	int enemySpawnInterval = 60;//敵出現までの間隔
	int enemySpawnRate = 1;//一度に出る敵の出現数
};