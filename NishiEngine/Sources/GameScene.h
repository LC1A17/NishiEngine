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

	//スプライト関連
	Sprite* titleSprite = nullptr;//タイトル背景のスプライト
	Sprite* gameSprite = nullptr;//ゲーム背景のスプライト
	Sprite* resultSprite = nullptr;//リザルト背景のスプライト

	//モデル関連
	Model* domeModel = nullptr;//天球のモデル
	Model* railModel = nullptr;//レールのモデル
	Model* playerModel = nullptr;//プレイヤーのモデル
	Model* enemy01Model = nullptr;//敵1のモデル
	Model* enemy02Model = nullptr;//敵2のモデル
	Model* enemy03Model = nullptr;//敵3のモデル
	Model* enemy04Model = nullptr;//敵4のモデル
	Model* pBullModel = nullptr;//プレイヤーの弾のモデル
	Model* eBullModel = nullptr;//敵の弾のモデル
	Model* parts1Model = nullptr;//落ちているパーツ1のモデル
	Model* parts2Model = nullptr;//落ちているパーツ2のモデル
	Model* parts3Model = nullptr;//落ちているパーツ3のモデル
	Model* parts4Model = nullptr;//落ちているパーツ4のモデル
	Model* playerPartsModel = nullptr;//プレイヤーのパーツのモデル
	Model* waterModel = nullptr;//水のモデル

	//オブジェクト関連
	Object3d* dome = nullptr;//天球のオブジェクト
	Object3d* rail[5];//レールのオブジェクト
	Object3d* player = nullptr;//プレイヤーのオブジェクト
	Object3d* enemy01[100];//敵1のオブジェクト
	Object3d* enemy02[100];//敵2のオブジェクト
	Object3d* enemy03[100];//敵3のオブジェクト
	Object3d* enemy04[100];//敵4のオブジェクト
	Object3d* pBull[100];//プレイヤーの弾のオブジェクト
	Object3d* eBull[100];//敵の弾のオブジェクト
	Object3d* parts01[100];//落ちているパーツ1のオブジェクト
	Object3d* parts02[100];//落ちているパーツ2のオブジェクト
	Object3d* parts03[100];//落ちているパーツ3のオブジェクト
	Object3d* parts04[100];//落ちているパーツ4のオブジェクト
	Object3d* playerParts[100];//プレイヤーのパーツのオブジェクト
	WaterObject* waterPlane = nullptr;//水のオブジェクト

	//オブジェクト座標関連
	//初期化不要
	XMFLOAT3 domeSca = { 3.0f, 3.0f, 3.0f };//天球のサイズ
	XMFLOAT3 railSca[5];//レールのサイズ
	XMFLOAT3 pSca = { 1.0f, 1.0f, 1.0f };//プレイヤーのサイズ
	XMFLOAT3 pOldPos = { 0.0f, 0.0f, 0.0f };//プレイヤーの移動前の座標
	XMFLOAT3 e01Sca[100];//敵1のサイズ
	XMFLOAT3 e02Sca[100];//敵2のサイズ
	XMFLOAT3 partsSca[100];//パーツのサイズ
	XMFLOAT3 pPartsOldPos[100];//プレイヤーのパーツの移動前の座標
	XMFLOAT3 deadPos = { 1000.0f, 1000.0f, 1000.0f };//画面外の座標
	XMFLOAT3 waterSca = { 50.0f, 1.0f, 50.0f };//水のサイズ

	//初期化必要
	XMFLOAT3 cameraPos = { 0.0f, 4.5f, 0.0f };//カメラの座標
	XMFLOAT3 domePos = { 0.0f, 0.0f, 0.0f };//天球の座標
	XMFLOAT3 domeRot = { 0.0f, 0.0f, 0.0f };//天球の回転量
	XMFLOAT3 railPos[5];//レールの座標
	XMFLOAT3 pPos = { 0.0f, 2.0f, 3.0f };//プレイヤーの座標
	XMFLOAT3 pRot = { 0.0f, 0.0f, 0.0f };//プレイヤーの回転量
	XMFLOAT3 e01Pos[100];//敵1の座標
	XMFLOAT3 e01Rot[100];//敵1の回転
	XMFLOAT3 e02Pos[100];//敵2の座標
	XMFLOAT3 e02Rot[100];//敵2の回転
	XMFLOAT3 e03Pos[100];//敵3の座標
	XMFLOAT3 e03Rot[100];//敵3の回転
	XMFLOAT3 e04Pos[100];//敵4の座標
	XMFLOAT3 e04Rot[100];//敵4の回転
	XMFLOAT3 pBullPos[100];//プレイヤーの弾の座標
	XMFLOAT3 eBullPos[100];//敵の弾の座標
	XMFLOAT3 parts01Pos[100];//パーツ1の座標
	XMFLOAT3 parts02Pos[100];//パーツ2の座標
	XMFLOAT3 parts03Pos[100];//パーツ1の座標
	XMFLOAT3 parts04Pos[100];//パーツ2の座標
	XMFLOAT3 pPartsPos[100];//プレイヤーのパーツの座標
	XMFLOAT3 pPartsRot[100];//プレイヤーのパーツの回転量
	XMFLOAT3 waterPos = { 0.0f, 0.0f, 0.0f };//水の座標

	//シーン関連
	int scene = Title;//ゲームシーン

	//プレイヤー移動関連
	//初期化不要
	float moveTimeSpead = 0.0125f;//移動時間
	float moveCountSpead = 0.2f;//移動量
	float movePRot = 36.0f;//移動中のプレイヤーの回転量
	float movePosZ = 0.3f;//自動で前に進む速度
	float moveFinish = 3.9f;//移動終了の値
	float gravity = 9.8f;//重力

	//初期化必要
	float pRail = 2.0f;//プレイヤーのいるレール(一番左から0)
	float moveTime = 0.0f;//移動時間をカウント
	float moveCount = 0.0f;//移動量をカウント
	bool isMove = false;//移動中かどうか
	bool moveDire = false;//移動の向き(false:左 true:右)

	//プレイヤー射撃関連
	//初期化不要
	float pBullSpead[3];//弾の速度
	float partsInitCD[3];//パーツのCDの最大値
	float partsInitHP[3];//パーツの耐久値の最大値
	float partsInitTime[3];//弾の生存時間の最大値
	float partsRecovery[3];//パーツの耐久値回復量

	//初期化必要
	float shotNum = 0;//攻撃パターン
	float partsNum[4];//装着中のパーツの種類
	float partsCD[4];//パーツのCD
	float partsHP[4];//パーツの耐久値
	float pBullAriveTime[100];//弾が生きている時間
	float attackNum[100];//弾の処理パターン
	bool isShot = false;//攻撃中かどうか
	bool isChange = false;//プレイヤーのパーツ変更中かどうか
	bool partsSelect[4];//パーツが選択中かどうか
	bool pBullArive[100];//弾が生きているかの判定
	bool pPartsArive[100];//プレイヤーのパーツが生きているかの判定
	bool parts01Arive[100];//パーツ1が生きているかの判定
	bool parts02Arive[100];//パーツ2が生きているかの判定

	//敵関連
	//初期化不要
	float enemyPosY = 2.0f;//基本となるY座標
	float enemyPosZ = 50.0f;//基本となるZ座標

	//初期化必要
	float enemySpawnTimer = 0.0f;//敵の出現までのカウント
	float enemySpawnInterval = 60.0f;//敵出現までの間隔
	bool enemy01Arive[100];//敵1が生きているかの判定
	bool enemy02Arive[100];//敵2が生きているかの判定

	//システム関連
	//初期化不要
	float domeRotSpead = 0.05f;//天球の回転速度
	float waterPosSpead = 0.1f;//水の流れる速度
	float partsRotSpead = 15.0f;//パーツの回転速度
	float gameTimeLimit = 3600.0;//ゲーム終了時間

	//初期化必要
	float gameTime = 0.0f;//ゲーム経過時間
	float score = 0.0f;//進んだ距離

	//敵射撃関連
	//初期化不要
	float eBullSpead[1];//弾の速度
	float eBullAriveTime[100];//弾が生きている時間
	bool eBullArive[100];//弾が生きているかの判定
};