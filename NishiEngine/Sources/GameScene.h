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
	//ゲームシーンパターン
	enum Scene
	{
		Title, StageSelect, StartLogo, Game, End, ClearLogo, GameoverLogo, ClearResult, GameoverResult
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
	//変数の初期化処理
	void InitializeVariable();
	//スプライト読み込み、生成処理
	void CreateSprite();
	//スプライトの初期座標調整処理
	void SettingSprite();
	//パーティクル初期化処理
	void InitializeParticleManager();
	//3Dオブジェクト読み込み、生成処理
	void CreateObject3d();
	//3Dオブジェクトの初期座標、回転、スケール調整処理
	void SettingObject3d();
	//スプライトの描画処理
	void DrawSprite();
	//3Dオブジェクトの描画処理
	void DrawObject3D();
	//前景スプライトの描画処理
	void DrawForegroundSprite();
	//パーティクル生成
	//void CreateParticles();
	//プレイヤー移動パーティクル生成
	void CreatePlayerMoveParticles();
	//プレイヤー被弾パーティクル生成
	void CreatePlayerHitParticles();
	//プレイヤー着地パーティクル生成
	void CreatePlayerLandingParticles();
	//レーザーパーティクル生成
	void CreateLaserParticles();
	//警告演出パーティクル生成
	void CreateDangerParticles();
	//アイテム獲得パーティクル生成
	void CreateItemGetParticles();
	//各種パーティクル生成処理
	void ControlParticles();
	//シーン遷移開始前処理(int 遷移後のシーン)
	void SceneChangeStart(int selectScene);
	//シーン遷移処理(int 遷移後のシーン)
	void SceneChange(int selectScene);
	//ロゴ点滅処理
	void LogoFlash();
	//選択肢処理(float 増加量, bool 現在のシーン 0:ステージセレクト 1:リザルト)
	void ChoiceMove(float increase, bool nowScene);
	//ロゴ上下移動処理(int 遷移後のシーン)
	void LogoUpAndDown(int selectScene);
	//オブジェクトの移動、回転処理
	void ObjectMoveAndRotation();
	//プレイヤー左右レール移動開始処理(bool 移動方向 左:true 右:false)
	void PlayerMoveStart(bool direction);
	//プレイヤー左右レール移動処理
	void PlayerMove();
	//スコア加算処理
	void ScoreCharge();
	//スコアボードの表示用計算
	void CalculationScoreBoard();
	//ステージ生成処理
	void StageManager();
	//障害物、アイテム、マップ攻撃を生成(1～10:障害物 10～20:アイテム 21～30:マップ攻撃)
	void SpawnManager(float posX, float objectNum);
	//プレイヤーとオブジェクトの当たり判定
	void PlayerCollision();
	//スコアボードの描画
	void DrawScoreBoard(bool isResult);

private://メンバ変数
	DirectXCommon* dxCommon	 = nullptr;
	Input* input			 = nullptr;
	Audio* audio			 = nullptr;
	DebugCamera* camera		 = nullptr;
	DebugText* debugText;

	LightGroup* lightGroup = nullptr;
	float ambientColor0[3] = { 1, 1, 1 };
	float lightDir0[3]	   = { 0, 0, 1 };
	float lightColor0[3]   = { 1, 0, 0 };
	float lightDir1[3]     = { 0, 1, 0 };
	float lightColor1[3]   = { 0, 1, 0 };
	float lightDir2[3]     = { 1, 0, 0 };
	float lightColor2[3]   = { 0, 0, 1 };

	//スプライト関連
	Sprite* titleBack		 = nullptr;//タイトル、リザルトの背景
	Sprite* gameTitleLogo	 = nullptr;//ゲームタイトルのロゴ
	Sprite* spaceLogo		 = nullptr;//PRESS SPACEのロゴ
	Sprite* stageSelectLogo  = nullptr;//STAGE SELECTのロゴ
	Sprite* aLogo			 = nullptr;//Aのロゴ
	Sprite* dLogo			 = nullptr;//Dのロゴ
	Sprite* leftArrowLogo	 = nullptr;//←のロゴ
	Sprite* rightArrowLogo	 = nullptr;//→のロゴ
	Sprite* stage1Logo		 = nullptr;//STAGE1のロゴ
	Sprite* stage2Logo		 = nullptr;//STAGE2のロゴ
	Sprite* stage3Logo		 = nullptr;//STAGE3のロゴ
	Sprite* startLogo		 = nullptr;//STARTのロゴ
	Sprite* distanceUI		 = nullptr;//進行度ゲージ本体
	Sprite* distanceBarUI	 = nullptr;//進行度ゲージの赤バー
	Sprite* stageClearLogo	 = nullptr;//STAGE CLEARのロゴ
	Sprite* gameoverLogo	 = nullptr;//GAME OVERのロゴ
	Sprite* nextLogo		 = nullptr;//NEXTのロゴ
	Sprite* retryLogo		 = nullptr;//RETRYのロゴ
	Sprite* titleLogo		 = nullptr;//TITLEのロゴ
	Sprite* blackBack		 = nullptr;//シーン遷移用黒背景
	Sprite* moveAD			 = nullptr;//操作説明のUI
	Sprite* distanceStartUI  = nullptr;//進行度ゲージのSTARTのロゴ
	Sprite* distanceGoalUI	 = nullptr;//進行度ゲージのGOALのロゴ
	Sprite* gameBack		 = nullptr;//ゲーム中の背景
	Sprite* dangerUI		 = nullptr;//警告演出の！マークのUI
	Sprite* scoreBoard		 = nullptr;//スコアボード
	Sprite* resultScoreBoard = nullptr;//リザルトのスコアボード

	Sprite* scoreNumber0[5];//スコア表示用の0
	Sprite* scoreNumber1[5];//スコア表示用の1
	Sprite* scoreNumber2[5];//スコア表示用の2
	Sprite* scoreNumber3[5];//スコア表示用の3
	Sprite* scoreNumber4[5];//スコア表示用の4
	Sprite* scoreNumber5[5];//スコア表示用の5
	Sprite* scoreNumber6[5];//スコア表示用の6
	Sprite* scoreNumber7[5];//スコア表示用の7
	Sprite* scoreNumber8[5];//スコア表示用の8
	Sprite* scoreNumber9[5];//スコア表示用の9

	Sprite* resultScoreNumber0[5];//リザルトのスコア表示用の0
	Sprite* resultScoreNumber1[5];//リザルトのスコア表示用の1
	Sprite* resultScoreNumber2[5];//リザルトのスコア表示用の2
	Sprite* resultScoreNumber3[5];//リザルトのスコア表示用の3
	Sprite* resultScoreNumber4[5];//リザルトのスコア表示用の4
	Sprite* resultScoreNumber5[5];//リザルトのスコア表示用の5
	Sprite* resultScoreNumber6[5];//リザルトのスコア表示用の6
	Sprite* resultScoreNumber7[5];//リザルトのスコア表示用の7
	Sprite* resultScoreNumber8[5];//リザルトのスコア表示用の8
	Sprite* resultScoreNumber9[5];//リザルトのスコア表示用の9

	//パーティクル関連
	ParticleManager* playerMoveParticle		 = nullptr;//プレイヤー移動パーティクル
	ParticleManager* playerHitParticle		 = nullptr;//プレイヤー被弾パーティクル
	ParticleManager* playerLandingParticle	 = nullptr;//プレイヤー着地パーティクル
	ParticleManager* laserParticle			 = nullptr;//レーザーパーティクル
	ParticleManager* dangerParticle			 = nullptr;//警告演出パーティクル
	ParticleManager* itemGetParticle = nullptr;//アイテム獲得パーティクル

	//モデル関連
	Model* skydomeModel				 = nullptr;//天球
	Model* railModel				 = nullptr;//レール
	Model* playerModel				 = nullptr;//プレイヤー
	Model* block1Model				 = nullptr;//障害物1
	Model* block2Model				 = nullptr;//障害物2
	Model* block3Model				 = nullptr;//障害物3
	Model* item1Model				 = nullptr;//アイテム1
	Model* item2Model				 = nullptr;//アイテム2
	Model* item3Model				 = nullptr;//アイテム3
	Model* playerBarrierPartsModel	 = nullptr;//プレイヤーバリアパーツ
	Model* playerLandingPartsModel	 = nullptr;//プレイヤー着地衝撃波パーツ
	Model* waterModel				 = nullptr;//水面

	//3Dオブジェクト関連
	Object3d* rail[5];//レール
	Object3d* block1[100];//障害物1
	Object3d* block2[100];//障害物2
	Object3d* block3[100];//障害物3	
	Object3d* item1[100];//アイテム1
	Object3d* item2[100];//アイテム2
	Object3d* item3[100];//アイテム3
	Object3d* skydome			 = nullptr;//天球
	Object3d* player			 = nullptr;//プレイヤー
	Object3d* playerBarrierParts = nullptr;//プレイヤーバリアパーツ
	Object3d* playerLandingParts = nullptr;//プレイヤー着地衝撃波パーツ
	WaterObject* waterPlane		 = nullptr;//水面

	//スプライト座標関連
	XMFLOAT2 startLogoPosition		 = {   363.0f,  -150.0f };//STARTのロゴの座標
	XMFLOAT2 distanceBarPosition	 = {   138.0f,    42.0f };//タイムバーの座標
	XMFLOAT2 stageClearLogoPosition	 = {    40.0f,  -150.0f };//STAGE CLEARのロゴの座標
	XMFLOAT2 gameoverLogoPosition	 = {   153.0f,  -150.0f };//GAME OVERのロゴの座標
	XMFLOAT2 blackBackPosition		 = { -1536.0f,  -864.0f };//シーン遷移用黒背景の座標

	//オブジェクト座標関連
	XMFLOAT3 initializeCoordinate		= {     0.0f,     0.0f,     0.0f };//初期位置
	XMFLOAT3 offScreenPosition			= {  1000.0f,  1000.0f,  1000.0f };//画面外の座標
	XMFLOAT3 skydomePosition			= {     0.0f,     0.0f,     0.0f };//天球の座標
	XMFLOAT3 cameraPosition				= {     0.0f,     4.5f,     0.0f };//カメラの座標
	XMFLOAT3 skydomeScale				= {     3.0f,     3.0f,     3.0f };//天球のスケール
	XMFLOAT3 oldPlayerPosition			= {     0.0f,     0.0f,     0.0f };//移動前のプレイヤーの座標
	XMFLOAT3 playerPosition				= {     0.0f,     2.0f,     3.0f };//プレイヤーの座標
	XMFLOAT3 playerRotation				= {     0.0f,     0.0f,     0.0f };//プレイヤーの回転
	XMFLOAT3 playerScale				= {     1.0f,     1.0f,     1.0f };//プレイヤーのスケール
	XMFLOAT3 playerPartsScale			= {     0.0f,     0.0f,     0.0f };//プレイヤーパーツのスケール
	XMFLOAT3 playerBarrierPartsPosition = {  1000.0f,  1000.0f,  1000.0f };//プレイヤーバリアパーツの座標
	XMFLOAT3 waterPosition				= {     0.0f,     0.0f,     0.0f };//水面の座標
	XMFLOAT3 waterScale					= {    50.0f,     1.0f,    50.0f };//水面のスケール
	XMFLOAT3 railPosition[5];//レールの座標
	XMFLOAT3 railScale[5];//レールのスケール
	XMFLOAT3 block1Position[100];//障害物1の座標
	XMFLOAT3 block1Rotation[100];//障害物1の回転
	XMFLOAT3 block1Scale[100];//障害物1のスケール
	XMFLOAT3 block2Position[100];//障害物2の座標
	XMFLOAT3 block2Rotation[100];//障害物2の回転
	XMFLOAT3 block2Scale[100];//障害物2のスケール
	XMFLOAT3 block3Position[100];//障害物3の座標
	XMFLOAT3 block3Rotation[100];//障害物3の回転
	XMFLOAT3 block3Scale[100];//障害物3のスケール
	XMFLOAT3 item1Position[100];//アイテム1の座標
	XMFLOAT3 item1Rotation[100];//アイテム1の回転
	XMFLOAT3 item1Scale[100];//アイテム1のスケール
	XMFLOAT3 item2Position[100];//アイテム2の座標
	XMFLOAT3 item2Rotation[100];//アイテム2の回転
	XMFLOAT3 item2Scale[100];//アイテム2のスケール
	XMFLOAT3 item3Position[100];//アイテム3の座標
	XMFLOAT3 item3Rotation[100];//アイテム3の回転
	XMFLOAT3 item3Scale[100];//アイテム3のスケール
	XMFLOAT3 laserStartPosition[5];//レーザー開始座標
	XMFLOAT3 laserLengthPosition[5];//レーザーの長さ測定用座標

	//パーティクル座標関連
	XMFLOAT3 playerHitParticlesPosition		 = {     0.0f,     0.0f,     0.0f };//プレイヤー被弾パーティクル生成座標
	XMFLOAT3 playerLandingParticlesPosition	 = {     0.0f,     0.0f,     0.0f };//プレイヤー着地パーティクル生成座標
	XMFLOAT3 laserParticlesPosition			 = {     0.0f,     2.0f,     0.0f };//レーザーパーティクル生成座標
	XMFLOAT3 dangerParticlesPosition		 = {	 0.0f,     2.0f,     0.0f };//警告演出パーティクル生成座標
	XMFLOAT3 itemGetParticlesPosition = { 0.0f,     0.0f,     0.0f };//アイテム獲得パーティクル生成座標
	
	//初期化関連
	bool firstInitialize = false;//初回起動時かの判定

	//シーン関連
	int scene = Title;//現在のシーン
	int nextScene = Title;//遷移後のシーン
	bool sceneChange = false;//シーン遷移中かの判定
	float sceneChangeCount = 0.0f;//シーン遷移開始からの経過時間
	float sceneChangeFinish = 90.0f;//シーン遷移の終了時間

	//ロゴ関連
	float logoFlashCount = 0.0f;//ロゴ点滅からの経過時間
	float logoFlashInterval = 20.0f;//ロゴ点滅の間隔
	bool logoHide = true;//ロゴ false:非表示 1:表示
	float logoMoveSpeed = 10.0f;//ロゴの移動速度
	float logoMoveCount = 0.0f;//ロゴ移動開始からの経過時間
	float logoMoveLimit = 40.0f;//ロゴの移動時間
	float logoStopLimit = 60.0f;//ロゴの停止時間
	bool logoMove = true;//ロゴ 0:停止中 1:移動中
	bool logoMoveDirection = false;//ロゴ移動方向 0:下 1:上

	//ステージセレクト関連
	float selectStageNumber = 1.0f;//選択中のステージ
	float maxStageNumber = 3.0f;//ステージの最大数

	//リザルト関連
	float resultSelect = 0.0f;//リザルトの選択肢の位置
	float maxResultSelectNumber = 1.0f;//リザルトの選択肢最大数

	//スコア関連
	int preScore = 0;//加算前のスコア
	int postScore = 0;//加算後のスコア
	int scoreBoard0 = 0;//スコアボード[0]0000
	int scoreBoard1 = 0;//スコアボード0[0]000
	int scoreBoard2 = 0;//スコアボード00[0]00
	int scoreBoard3 = 0;//スコアボード000[0]0
	int scoreBoard4 = 0;//スコアボード0000[0]

	//システム関連
	float gameTime = 0.0f;//ゲーム開始からの経過時間
	float gameTimeLimit = 900.0f;//ゲーム終了時間
	float skydomeRotationSpeed = 0.05f;//天球の回転速度
	float waterPositionSpeed = -0.1f;//水の流れる速度
	float autoMoveSpeedZ = 0.6f;//オブジェクトが自動で前or後に進む速度
	float deadLinePosition = -10.0f;//オブジェクトが自然消滅するライン

	//プレイヤー関連
	bool sideJump = false;//移動中かどうか
	bool sideJumpDirection = false;//移動の向き(false:左 true:右)
	float sideJumpTimeSpeed = 0.0125f;//移動時間の増加スピード
	float sideJumpTime = 0.0f;//移動時間
	float sideJumpCountSpeed = 0.2f;//移動量の増加スピード
	float sideJumpCount = 0.0f;//移動量
	float sideJumpPlayerRotation = 36.0f;//移動中のプレイヤーの回転量
	float sideJumpFinishTime = 3.9f;//移動終了の値
	float gravity = 9.8f;//重力
	float playerRailPosition = 2.0f;//プレイヤーのいるレール(一番左から0)
	float equipItem = 0.0f;//装着中のパーツの種類

	//障害物関連
	float autoMoveBlockSpeedZ = -0.6f;//障害物で前に進む速度
	float defaultBlockPositionY = 2.0f;//基本となるY座標
	float defaultBlockPositionZ = 50.0f;//基本となるZ座標
	bool block1Arive[100];//障害物1が生きているかの判定
	bool block2Arive[100];//障害物2が生きているかの判定
	bool block3Arive[100];//障害物3が生きているかの判定

	//アイテム関連
	bool item1Arive[100];//アイテム1が生きているかの判定
	bool item2Arive[100];//アイテム2が生きているかの判定
	bool item3Arive[100];//アイテム3が生きているかの判定

	//警告演出関連
	bool dangerUIArive = false;//警告演出が出現しているかの判定
	float dangerUICount = 0.0f;//警告演出が出現してからの経過時間
	float dangerUILimit = 60.0f;//警告演出の出現時間

	//レーザー当たり判定関連
	bool laserCollision[5];//レーザーが出現しているかの判定
	float laserCollisionCount[5];//レーザーの判定が出現してからの経過時間
	float laserCollisionLimit[5];//レーザーの判定の出現時間

	//パーティクル関連
	bool isPlayerHitParticles = false;//プレイヤー被弾エフェクト生成中かの判定
	float isPlayerHitParticlesCount = 0.0f;//プレイヤー被弾エフェクト生成開始からの経過時間
	float isPlayerHitParticlesLimit = 10.0f;//プレイヤー被弾エフェクトの生成時間

	bool isPlayerLandingParticles = false;//プレイヤー着地エフェクト生成中かの判定
	float isPlayerLandingParticlesCount = 0.0f;//プレイヤー着地エフェクト生成開始からの経過時間
	float isPlayerLandingParticlesLimit = 10.0f;//プレイヤー着地エフェクトの生成時間

	bool isLaserParticles = false;//レーザーパーティクル生成中かの判定
	float isLaserParticlesCount = 0.0f;//レーザーエフェクト生成開始からの経過時間
	float isLaserParticlesLimit = 20.0f;//レーザーエフェクトの生成時間

	bool isDangerParticles = false;//警告演出パーティクル生成中かの判定
	float isDangerParticlesCount = 0.0f;//警告演出エフェクト生成開始からの経過時間
	float isDangerParticlesLimit = 20.0f;//警告演出エフェクトの生成時間

	bool isItemGetParticles = false;//アイテム獲得パーティクル生成中かの判定
	float isItemGetParticlesCount = 0.0f;//アイテム獲得エフェクト生成開始からの経過時間
	float isItemGetParticlesLimit = 5.0f;//アイテム獲得エフェクトの生成時間
};