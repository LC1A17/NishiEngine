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
		Title, Select, Start, Game, End, Clear, Gameover, ClearResult, GameoverResult
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
	Sprite* arrowSprite = nullptr;//矢印のスプライト
	Sprite* selectLogo = nullptr;//ステージセレクトのロゴ
	Sprite* stage1Logo = nullptr;//ステージ1のロゴ
	Sprite* stage2Logo = nullptr;//ステージ2のロゴ
	Sprite* stage3Logo = nullptr;//ステージ3のロゴ
	Sprite* titleLogo = nullptr;//タイトルロゴ
	Sprite* clearLogo = nullptr;//クリアロゴ
	Sprite* gameoverLogo = nullptr;//ゲームオーバーロゴ
	Sprite* nextLogo = nullptr;//NEXTロゴ
	Sprite* retryLogo = nullptr;//RETRYロゴ
	Sprite* returnLogo = nullptr;//TITLEロゴ
	Sprite* startLogo = nullptr;//Startロゴ
	Sprite* aLogo = nullptr;//aロゴ
	Sprite* dLogo = nullptr;//dロゴ
	Sprite* leftLogo = nullptr;//←ロゴ
	Sprite* rightLogo = nullptr;//→ロゴ
	Sprite* spaceLogo = nullptr;//spaceロゴ
	Sprite* lengthSprite = nullptr;//ステージのバー
	Sprite* timeBarSprite = nullptr;//時間バー
	Sprite* clearResultLogo = nullptr;//リザルト用クリアロゴ
	Sprite* gameoverResultLogo = nullptr;//リザルト用ゲームオーバーロゴ

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
	XMFLOAT3 eSca[100];//敵のサイズ
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
	XMFLOAT2 arrowPos = { 280.0f, 210.0f };//矢印の座標
	XMFLOAT2 startPos = { 363.0f, -150.0f };//スタートロゴの座標
	XMFLOAT2 timeBarPos = { 138.0f, 42.0f };//タイムバーの座標
	XMFLOAT2 clearPos = { 40.0f, -150.0f };//クリアロゴの座標
	XMFLOAT2 gameoverPos = { 153.0f, -150.0f };//ゲームオーバーロゴの座標

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
	float pBullSpead[5];//弾の速度
	float partsInitCD[5];//パーツのCDの最大値
	float partsInitHP[5];//パーツの耐久値の最大値
	float partsInitTime[5];//弾の生存時間の最大値
	float partsRecovery[5];//パーツの耐久値回復量

	//初期化必要
	float shotNum = 0.0f;//攻撃パターン
	float partsNum = 0.0f;//装着中のパーツの種類
	float partsCD = 20.0f;//パーツのCD
	float partsHP = 0.0f;//パーツの耐久値
	float pBullAriveTime[100];//弾が生きている時間
	float attackNum[100];//弾の処理パターン
	bool isShot = false;//攻撃中かどうか
	bool isChange = false;//プレイヤーのパーツ変更中かどうか
	bool pBullArive[100];//弾が生きているかの判定
	bool pPartsArive[100];//プレイヤーのパーツが生きているかの判定
	bool parts01Arive[100];//パーツ1が生きているかの判定
	bool parts02Arive[100];//パーツ2が生きているかの判定
	bool parts03Arive[100];//パーツ3が生きているかの判定
	bool parts04Arive[100];//パーツ4が生きているかの判定

	//敵関連
	//初期化不要
	float enemyPosY = 2.0f;//基本となるY座標
	float enemyPosZ = 50.0f;//基本となるZ座標
	float moveEPosZ = -0.3f;//自動で前に進む速度
	float deadLinePos = -10.0f;//画面買いに消えるタイミング

	//初期化必要
	float enemySpawnTimer = 0.0f;//敵の出現までのカウント
	float enemySpawnInterval = 10.0f;//敵出現までの間隔
	bool enemy01Arive[100];//敵1が生きているかの判定
	bool enemy02Arive[100];//敵2が生きているかの判定
	bool enemy03Arive[100];//敵3が生きているかの判定
	bool enemy04Arive[100];//敵4が生きているかの判定

	//システム関連
	//初期化不要
	float domeRotSpead = 0.05f;//天球の回転速度
	float waterPosSpead = 0.1f;//水の流れる速度
	float partsRotSpead = 15.0f;//パーツの回転速度
	float gameTimeLimit = 3600.0f;//ゲーム終了時間
	float timeBarLength = 3.0f;//時間バー計算用

	//初期化必要
	float gameTime = 0.0f;//ゲーム経過時間
	float score = 0.0f;//進んだ距離

	//敵射撃関連
	//初期化不要
	float eBullSpead[1];//弾の速度

	//初期化必要
	float eBullAriveTime[100];//弾が生きている時間
	bool eBullArive[100];//弾が生きているかの判定

	//タイトル関連
	float spaceLogoCount = 0.0f;//ロゴ点滅カウント
	float spaceLogoInterval = 25.0f;//ロゴ点滅間隔
	bool spaceHide = false;//矢印の表示

	//ステージセレクト関連
	float selectNum = 1.0f;//選択中のステージ
	float selectLogoCount = 0.0f;//ロゴ点滅カウント
	float selectLogoInterval = 25.0f;//ロゴ点滅間隔
	float arrowSpead = 10.0f;//矢印の移動速度
	float arrowMoveCount = 0.0f;//矢印の移動カウント
	float arrowMoveTime = 30.0f;//矢印の移動時間
	bool selectMove = false;//選択中かどうか
	bool selectHide = false;//矢印の表示
	bool selectMoveDire = false;//移動方向

	//スタート処理関連
	bool startEnd = false;//スタート処理中か
	bool startDire = false;//ロゴの移動方向
	bool startStop = false;//スタートロゴが停止中か
	float startSpead = 10.0f;//ロゴの移動速度
	float startMoveCount = 0.0f;//ロゴの移動カウント
	float startMoveTime = 40.0f;//ロゴの移動時間
	float startStopTime = 60.0f;//ロゴの停止時間

	//クリア処理関連
	float clearSelectCount = 0.0f;//ロゴ点滅カウント
	float clearSelectInterval = 25.0f;//ロゴ点滅間隔
	bool clearSelect = false;//選択肢の位置
	bool clearSelectHide = false;//選択肢の表示

	//ゲームオーバー処理関連
	float gameoverSelectCount = 0.0f;//ロゴ点滅カウント
	float gameoverSelectInterval = 25.0f;//ロゴ点滅間隔
	bool gameoverSelect = false;//選択肢の位置
	bool gameoverSelectHide = false;//選択肢の表示
};