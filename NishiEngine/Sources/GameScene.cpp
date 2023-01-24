#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include <imgui.h>
#include <cstdlib>
#include <ctime>
#include <math.h>

using namespace DirectX;

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	safe_delete(lightGroup);

	//スプライト関連
	safe_delete(backSprite);//基礎背景
	safe_delete(titleLogo);//ゲームタイトルロゴ
	safe_delete(spaceLogo);//SPACEロゴ
	safe_delete(stageSelectLogo);//STAGE SELECTロゴ
	safe_delete(aLogo);//Aロゴ
	safe_delete(dLogo);//Dロゴ
	safe_delete(leftArrowLogo);//←ロゴ
	safe_delete(rightArrowLogo);//→ロゴ
	safe_delete(stage1Logo);//STAGE1ロゴ
	safe_delete(stage2Logo);//STAGE2ロゴ
	safe_delete(stage3Logo);//STAGE3ロゴ
	safe_delete(startLogo);//STARTロゴ
	safe_delete(timeGaugeSprite);//進行ゲージ
	safe_delete(clockHandsSprite);//進行度
	safe_delete(stageClearLogo);//STAGE CLEARロゴ
	safe_delete(gameoverLogo);//GAME OVERロゴ
	safe_delete(nextLogo);//NEXTロゴ
	safe_delete(retryLogo);//RETRYロゴ
	safe_delete(titleLogo);//TITLEロゴ

	//モデル関連
	safe_delete(domeModel);
	safe_delete(railModel);
	safe_delete(playerModel);
	safe_delete(enemy01Model);
	safe_delete(enemy02Model);
	safe_delete(enemy03Model);
	safe_delete(enemy04Model);
	safe_delete(pBullModel);
	safe_delete(parts1Model);
	safe_delete(parts2Model);
	safe_delete(parts3Model);
	safe_delete(parts4Model);
	safe_delete(playerPartsModel);
	safe_delete(waterModel);

	//オブジェクト関連
	safe_delete(dome);
	safe_delete(player);
	safe_delete(waterPlane);

	for (int i = 0; i < 5; i++)
	{
		safe_delete(rail[i]);
	}
	for (int i = 0; i < 100; i++)
	{
		safe_delete(enemy01[i]);
		safe_delete(enemy02[i]);
		safe_delete(enemy03[i]);
		safe_delete(enemy04[i]);
		safe_delete(pBull[i]);
		safe_delete(parts01[i]);
		safe_delete(parts02[i]);
		safe_delete(parts03[i]);
		safe_delete(parts04[i]);
		safe_delete(playerParts[i]);
	}
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio * audio)
{
	//nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);
	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	srand(time(NULL));//乱数初期化

	//カメラ生成
	camera = new DebugCamera(WinApp::winWidth, WinApp::winHeight, input);

	//3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);
	WaterObject::SetCamera(camera);

	//デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/font/debugfont.png"))
	{
		assert(0);
		return ;
	}

	//デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	//テクスチャ読み込み
	if (!Sprite::LoadTexture(100, L"Resources/sprite/back.png"))
	{//基礎背景
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(101, L"Resources/sprite/title_logo.png"))
	{//ゲームタイトルロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(102, L"Resources/sprite/space.png"))
	{//SPACEロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(103, L"Resources/sprite/stage_select.png"))
	{//STAGE SELECTロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(104, L"Resources/sprite/A.png"))
	{//Aロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(105, L"Resources/sprite/D.png"))
	{//Dロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(106, L"Resources/sprite/leftArrow.png"))
	{//←ロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(107, L"Resources/sprite/rightArrow.png"))
	{//→ロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(108, L"Resources/sprite/stage1.png"))
	{//STAGE1ロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(109, L"Resources/sprite/stage2.png"))
	{//STAGE2ロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(110, L"Resources/sprite/stage3.png"))
	{//STAGE3ロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(111, L"Resources/sprite/start.png"))
	{//STARTロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(112, L"Resources/sprite/length.png"))
	{//進行ゲージ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(113, L"Resources/sprite/timeBar.png"))
	{//進行度
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(114, L"Resources/sprite/stage_clear.png"))
	{//STAGE CLEARロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(115, L"Resources/sprite/game_over.png"))
	{//GAME OVERロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(116, L"Resources/sprite/next.png"))
	{//NEXTロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(117, L"Resources/sprite/retry.png"))
	{//RETRYロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(118, L"Resources/sprite/title.png"))
	{//TITLEロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(119, L"Resources/sprite/black.png"))
	{//シーン遷移用黒背景
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(120, L"Resources/sprite/move_a_d.png"))
	{//操作説明
		assert(0);
		return;
	}

	//背景スプライト生成
	backSprite = Sprite::Create(100, { 0.0f,0.0f });//基礎背景
	titleLogo = Sprite::Create(101, { 0.0f,0.0f });//ゲームタイトルロゴ
	spaceLogo = Sprite::Create(102, { 0.0f,0.0f });//SPACEロゴ
	stageSelectLogo = Sprite::Create(103, { 0.0f,0.0f });//STAGE SELECTロゴ
	aLogo = Sprite::Create(104, { 0.0f,0.0f });//Aロゴ
	dLogo = Sprite::Create(105, { 0.0f,0.0f });//Dロゴ
	leftArrowLogo = Sprite::Create(106, { 0.0f,0.0f });//←ロゴ
	rightArrowLogo = Sprite::Create(107, { 0.0f,0.0f });//→ロゴ
	stage1Logo = Sprite::Create(108, { 0.0f,0.0f });//STAGE1ロゴ
	stage2Logo = Sprite::Create(109, { 0.0f,0.0f });//STAGE2ロゴ
	stage3Logo = Sprite::Create(110, { 0.0f,0.0f });//STAGE3ロゴ
	startLogo = Sprite::Create(111, { 0.0f,0.0f });//STARTロゴ
	timeGaugeSprite = Sprite::Create(112, { 0.0f,0.0f });//進行ゲージ
	clockHandsSprite = Sprite::Create(113, { 0.0f,0.0f });//進行度
	stageClearLogo = Sprite::Create(114, { 0.0f,0.0f });//STAGE CLEARロゴ
	gameoverLogo = Sprite::Create(115, { 0.0f,0.0f });//GAME OVERロゴ
	nextLogo = Sprite::Create(116, { 0.0f,0.0f });//NEXTロゴ
	retryLogo = Sprite::Create(117, { 0.0f,0.0f });//RETRYロゴ
	returnLogo = Sprite::Create(118, { 0.0f,0.0f });//TITLEロゴ
	blackBox = Sprite::Create(119, { 0.0f,0.0f });//シーン遷移用黒背景
	moveAD = Sprite::Create(120, { 0.0f,0.0f });//操作説明

	//スプライトの座標調整
	titleLogo->SetPosition({ 143.5f, 70.0f });//ゲームタイトルロゴ
	spaceLogo->SetPosition({ 448.0f, 500.0f });//SPACEロゴ
	stageSelectLogo->SetPosition({ 43.5f, 50.0f });//STAGE SELECTロゴ
	aLogo->SetPosition({ 505.0f, 580.0f });//Aロゴ
	dLogo->SetPosition({ 647.0f, 580.0f });//Dロゴ
	leftArrowLogo->SetPosition({ 405.0f, 580.0f });//←ロゴ
	rightArrowLogo->SetPosition({ 747.0f, 580.0f });//→ロゴ
	stage1Logo->SetPosition({ 220.0f, 330.0f });//STAGE1ロゴ
	stage2Logo->SetPosition({ 520.0f, 330.0f });//STAGE2ロゴ
	stage3Logo->SetPosition({ 820.0f, 330.0f });//STAGE3ロゴ
	startLogo->SetPosition(startLogoPos);//STARTロゴ
	timeGaugeSprite->SetPosition({ 128.0f, 50.0f });//進行ゲージ
	clockHandsSprite->SetPosition(clockHandsPos);//進行度
	stageClearLogo->SetPosition(stageClearLogoPos);//STAGE CLEARロゴ
	gameoverLogo->SetPosition(gameoverLogoPos);//GAME OVERロゴ
	nextLogo->SetPosition({ 77.5f, 430.0f });//NEXTロゴ
	retryLogo->SetPosition({ 49.0f, 430.0f });//RETRYロゴ
	returnLogo->SetPosition({ 705.0f, 430.0f });//TITLEロゴ
	blackBox->SetPosition(boxPosition);//シーン遷移座標
	blackBox->SetSize({ 1280.0f, 720.0f });//シーン遷移サイズ
	moveAD->SetPosition({ 0.0f, 634.0f });//操作説明

	//パーティクルマネージャ生成
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(camera);

	//テクスチャ0番に読み込み
	Sprite::LoadTexture(0, L"Resources/sprite/waterDown.jpg");

	//ライト生成
	lightGroup = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup);
	WaterObject::SetLightGroup(lightGroup);

	//モデル読み込み
	domeModel = Model::CreateFromOBJ("skydome");//天球
	railModel = Model::CreateFromOBJ("enemy3");//レーン
	playerModel = Model::CreateFromOBJ("player");//プレイヤー
	enemy01Model = Model::CreateFromOBJ("enemy");//敵1
	enemy02Model = Model::CreateFromOBJ("enemy2");//敵2
	enemy03Model = Model::CreateFromOBJ("enemy3");//敵3
	enemy04Model = Model::CreateFromOBJ("enemy");//敵4
	pBullModel = Model::CreateFromOBJ("pBullet");//プレイヤーの弾
	eBullModel = Model::CreateFromOBJ("pBullet");//敵の弾
	parts1Model = Model::CreateFromOBJ("parts");//落ちているパーツ1
	parts2Model = Model::CreateFromOBJ("parts");//落ちているパーツ2
	parts3Model = Model::CreateFromOBJ("parts");//落ちているパーツ3
	parts4Model = Model::CreateFromOBJ("parts");//落ちているパーツ4
	playerPartsModel = Model::CreateFromOBJ("playerParts");//プレイヤーのパーツ
	waterModel = Model::CreateFromOBJ("ground");//水

	//3Dオブジェクト生成
	//天球
	dome = Object3d::Create(domeModel);
	dome->SetScale(domeSca);
	//プレイヤー
	player = Object3d::Create(playerModel);
	player->SetPosition(pPos);
	//水
	waterPlane = WaterObject::Create(waterModel);
	waterPlane->SetScale(waterSca);
	//レーン
	railPos[0] = { -8.0f, 2.0f, 0.0f };
	railPos[1] = { -4.0f, 2.0f, 0.0f };
	railPos[2] = {	0.0f, 2.0f, 0.0f };
	railPos[3] = {	4.0f, 2.0f, 0.0f };
	railPos[4] = {	8.0f, 2.0f, 0.0f };

	for (int i = 0; i < 5; i++)
	{
		rail[i] = Object3d::Create(railModel);
		railSca[i] = { 0.1f, 0.1f, 10000.0f };
		rail[i]->SetPosition(railPos[i]);
		rail[i]->SetScale(railSca[i]);
	}
	for (int i = 0; i < 100; i++)
	{//敵、弾、パーツなど
		enemy01[i] = Object3d::Create(enemy01Model);
		e01Pos[i] = deadPos;
		e01Rot[i] = { 0.0f, 0.0f, 0.0f };
		eSca[i] = { 1.0f, 1.0f, 1.0f };
		enemy01[i]->SetPosition(e01Pos[i]);
		enemy01[i]->SetScale(eSca[i]);
		enemy02[i] = Object3d::Create(enemy02Model);
		e02Pos[i] = deadPos;
		e02Rot[i] = { 0.0f, 0.0f, 0.0f };
		enemy02[i]->SetPosition(e02Pos[i]);
		enemy02[i]->SetScale(eSca[i]);
		enemy03[i] = Object3d::Create(enemy03Model);
		e03Pos[i] = deadPos;
		e03Rot[i] = { 0.0f, 0.0f, 0.0f };
		enemy03[i]->SetPosition(e03Pos[i]);
		enemy03[i]->SetScale(eSca[i]);
		enemy04[i] = Object3d::Create(enemy04Model);
		e04Pos[i] = deadPos;
		e04Rot[i] = { 0.0f, 0.0f, 0.0f };
		enemy04[i]->SetPosition(e04Pos[i]);
		enemy04[i]->SetScale(eSca[i]);
		pBull[i] = Object3d::Create(pBullModel);
		pBullPos[i] = deadPos;
		pBull[i]->SetPosition(eBullPos[i]);
		eBull[i] = Object3d::Create(eBullModel);
		eBullPos[i] = deadPos;
		eBull[i]->SetPosition(eBullPos[i]);
		parts01[i] = Object3d::Create(parts1Model);
		parts01Pos[i] = deadPos;
		partsSca[i] = { 1.0f, 1.0f, 1.0f };
		parts01[i]->SetPosition(parts01Pos[i]);
		parts01[i]->SetScale(partsSca[i]);
		parts02[i] = Object3d::Create(parts2Model);
		parts02Pos[i] = deadPos;
		parts02[i]->SetPosition(parts02Pos[i]);
		parts02[i]->SetScale(partsSca[i]);
		parts03[i] = Object3d::Create(parts3Model);
		parts03Pos[i] = deadPos;
		parts03[i]->SetPosition(parts03Pos[i]);
		parts03[i]->SetScale(partsSca[i]);
		parts04[i] = Object3d::Create(parts4Model);
		parts04Pos[i] = deadPos;
		parts04[i]->SetPosition(parts04Pos[i]);
		parts04[i]->SetScale(partsSca[i]);
		playerParts[i] = Object3d::Create(playerPartsModel);
		pPartsPos[i] = deadPos;
		pPartsRot[i] = { 0.0f, 0.0f, 0.0f };
		playerParts[i]->SetPosition(pPartsPos[i]);
		playerParts[i]->SetPosition(pPartsRot[i]);
	}

	//各種数値初期化
	//プレイヤーの弾の速度
	pBullSpead[0] = 1.5f;
	pBullSpead[1] = 1.5f;
	pBullSpead[2] = 1.0f;
	pBullSpead[3] = 1.0f;
	pBullSpead[4] = 0.0f;

	//パーツのCDの最大値
	partsInitCD[0] = 40.0f;
	partsInitCD[1] = 40.0f;
	partsInitCD[2] = 160.0f;
	partsInitCD[3] = 40.0f;
	partsInitCD[4] = 100.0f;

	//パーツの耐久値の最大値
	partsInitHP[0] = 0.0f;
	partsInitHP[1] = 10.0f;
	partsInitHP[2] = 3.0f;
	partsInitHP[3] = 5.0f;
	partsInitHP[4] = 5.0f;

	//弾の生存時間の最大値
	partsInitTime[0] = 40.0f;
	partsInitTime[1] = 40.0f;
	partsInitTime[2] = 60.0f;
	partsInitTime[3] = 40.0f;
	partsInitTime[4] = 0.0f;

	//パーツの耐久値回復量
	partsRecovery[0] = 0.0f;
	partsRecovery[1] = 1.0f;
	partsRecovery[2] = 1.0f;
	partsRecovery[3] = 1.0f;
	partsRecovery[4] = 1.0f;

	//敵の速度
	eBullSpead[0] = 1.0f;

	for (int i = 0; i < 100; i++)
	{	
		pBullAriveTime[i] = 0.0f;//弾が生きている時間
		eBullAriveTime[i] = 0.0f;//弾が生きている時間
		attackNum[i] = 0.0f;//弾の処理パターン
		pBullArive[i] = false;//弾が生きているかの判定
		eBullArive[i] = false;//弾が生きているかの判定
		pPartsArive[i] = false;//プレイヤーのパーツが生きているかの判定
		parts01Arive[i] = false;//パーツ1が生きているかの判定
		parts02Arive[i] = false;//パーツ2が生きているかの判定
		parts03Arive[i] = false;//パーツ3が生きているかの判定
		parts04Arive[i] = false;//パーツ4が生きているかの判定
		enemy01Arive[i] = false;//敵1が生きているかの判定
		enemy02Arive[i] = false;//敵2が生きているかの判定
		enemy03Arive[i] = false;//敵3が生きているかの判定
		enemy04Arive[i] = false;//敵4が生きているかの判定
	}

	//カメラ注視点をセット
	camera->SetTarget(cameraPos);
	camera->SetDistance(6.0f);
}

void GameScene::Update()
{
	if (isSceneChange)
	{
		sceneChange();//シーン遷移
	}

	if (scene == Title)
	{//タイトル
		//プレイヤー操作関連
		if (input->TriggerKey(DIK_SPACE) && !isSceneChange)
		{//ゲームシーンへ移行
			isSceneChange = true;//シーン遷移
			sceneChangeCount = 0.0f;//カウントリセット
		}
		if (sceneChangeCount == 45.0f)
		{
			logoFlashCount = 0.0f;//ロゴ点滅カウント
			logoHid = false;//ロゴの表示
			scene = StageSelect;//シーン移行
		}
		logoFlash();//ロゴ点滅処理
	}
	else if (scene == StageSelect)
	{//ステージセレクト
		//プレイヤー操作関連
		if (input->TriggerKey(DIK_SPACE) && !isSceneChange)
		{//選択中のステージへ移行
			isSceneChange = true;//シーン遷移
			sceneChangeCount = 0.0f;//カウントリセット
		}
		if (input->TriggerKey(DIK_A) && selectStageNum > 1.0f && !isSceneChange)
		{//一番左にいない場合、左に移動
			selectStageNum -= 1.0f;//ステージ-1
			logoFlashCount = 0.0f;//カウントリセット
			logoHid = false;//ロゴの表示
		}
		if (input->TriggerKey(DIK_D) && selectStageNum < maxStageNum && !isSceneChange)
		{//一番右にいない場合、右に移動
			selectStageNum += 1.0f;//ステージ+1
			logoFlashCount = 0.0f;//カウントリセット
			logoHid = false;//ロゴの表示
		}
		if (sceneChangeCount == 45.0f)
		{
			logoFlashCount = 0.0f;//ロゴ点滅カウント
			logoHid = false;//ロゴの表示
			selectStageNum = 1.0f;//選択中のステージ
			scene = Start;//シーン移行
		}

		logoFlash();//ロゴ点滅処理
	}
	else if (scene == Start)
	{//スタート演出
		//システム関連
		//前に自動で進み続ける
		pPos.z += movePosZ;//プレイヤー前
		cameraPos.z += movePosZ;//カメラ前
		domePos.z += movePosZ;//天球前
		dome->SetPosition(domePos);//天球の座標更新
		player->SetPosition(pPos);//プレイヤーの座標更新
		player->SetRotation(pRot);//プレイヤーの回転量更新
		camera->SetTarget(cameraPos);//カメラの座標更新

		if (!logoMove)
		{//ロゴ移動中
			logoMoveCount += 1.0f;//カウント増加

			if (!logoMoveDirection)
			{//下移動
				startLogoPos.y = startLogoPos.y + logoMoveSpead;//座標変更

				if (logoMoveCount >= logoMoveTime)
				{//移動終了
					logoMoveCount = 0.0f;//カウントリセット
					logoMoveDirection = true;//向きを変更
					logoMove = true;//停止中に変更
				}
			}
			else
			{//上移動
				startLogoPos.y = startLogoPos.y - logoMoveSpead;//座標変更

				if (logoMoveCount >= logoMoveTime)
				{//移動終了
					logoMoveCount = 0.0f;//カウントリセット
					logoMoveDirection = false;//向きを変更
					logoMove = false;//移動中に変更
					scene = Game;//シーン移行
				}
			}
			startLogo->SetPosition(startLogoPos);
		}
		else
		{//ロゴ停止中
			logoMoveCount += 1.0f;//カウント増加

			if (logoMoveCount >= logoStopTime)
			{//停止終了
				logoMoveCount = 0.0f;//カウントリセット
				logoMove = false;//移動中に変更
			}
		}
	}
	else if (scene == Game)
	{//ゲーム
		//タイム処理
		clockHandsPos.x = clockHandsPos.x + (988 / gameTimeLimit);
		clockHandsSprite->SetPosition(clockHandsPos);

		if (input->TriggerKey(DIK_SPACE) && !isMove)
		{//射撃
			shotNum = partsNum;//パーツの種類に応じた弾をセット
			isShot = true;//弾のセット開始
		}
		//プレイヤー移動関連
		if (input->TriggerKey(DIK_A) && !isMove)
		{//左移動
			if (pRail > 0.0f)
			{//一番左にいないなら移動開始
				pOldPos = pPos;//現在のプレイヤー座標を保存
				moveTime = 0.0f;//移動時間をリセット
				moveCount = 0.0f;//移動量をリセット
				pRail -= 1.0f;//レーンの移動
				moveDire = false;//左に移動
				isMove = true;//移動開始
				//shotNum = partsNum;//パーツの種類に応じた弾をセット
				//isShot = true;//弾のセット開始

				for (int i = 0; i < 100; i++)
				{//パーツが生きている場合
					if (pPartsArive[i])
					{//現在のプレイヤーパーツ座標を保存
						pPartsOldPos[i] = pPartsPos[i];
					}
				}
			}
		}
		if (input->TriggerKey(DIK_D) && !isMove)
		{//右移動
			if (pRail < 4.0f)
			{//一番右にいないなら移動開始
				pOldPos = pPos;//現在のプレイヤー座標を保存
				moveTime = 0.0f;//移動時間をリセット
				moveCount = 0.0f;//移動量をリセット
				pRail += 1.0f;//レーンの移動
				moveDire = true;//右に移動
				isMove = true;//移動開始
				//shotNum = partsNum;//パーツの種類に応じた弾をセット
				//isShot = true;//弾のセット開始

				for (int i = 0; i < 100; i++)
				{//パーツが生きている場合
					if (pPartsArive[i])
					{//現在のプレイヤーパーツ座標を保存
						pPartsOldPos[i] = pPartsPos[i];
					}
				}
			}
		}
		if (isShot)//CTが0かつ移動をしていない場合
		{//弾のセット
			if (shotNum == 0.0f)
			{//通常弾
				for (int i = 0; i < 100; i++)
				{
					if (!pBullArive[i])
					{
						isShot = false;//弾のセット終了
						pBullArive[i] = true;//弾を生成
						pBullAriveTime[i] = partsInitTime[0];//弾の生存時間を設定
						attackNum[i] = shotNum;//弾の挙動を設定
						pBullPos[i] = pPos;//弾を自機の位置にセット
						pBull[i]->SetPosition(pBullPos[i]);
						break;
					}
				}
			}
			else if (shotNum == 1.0f)
			{//三列弾
				for (int i = 0; i < 100; i++)
				{
					if (!pBullArive[i])
					{
						pBullArive[i] = true;//弾を生成
						pBullAriveTime[i] = partsInitTime[1];//弾の生存時間を設定
						attackNum[i] = shotNum;//弾の挙動を設定
						pBullPos[i] = pPos;//弾を自機の位置にセット
						pBull[i]->SetPosition(pBullPos[i]);
						break;
					}
				}
				for (int i = 0; i < 100; i++)
				{
					if (!pBullArive[i])
					{
						pBullArive[i] = true;//弾を生成
						pBullAriveTime[i] = partsInitTime[1];//弾の生存時間を設定
						attackNum[i] = shotNum;//弾の挙動を設定
						pBullPos[i] = pPartsPos[0];//弾を自機の位置にセット
						pBull[i]->SetPosition(pBullPos[i]);
						break;
					}
				}
				for (int i = 0; i < 100; i++)
				{
					if (!pBullArive[i])
					{
						isShot = false;//弾のセット終了
						pBullArive[i] = true;//弾を生成
						pBullAriveTime[i] = partsInitTime[1];//弾の生存時間を設定
						attackNum[i] = shotNum;//弾の挙動を設定
						pBullPos[i] = pPartsPos[1];//弾を自機の位置にセット
						pBull[i]->SetPosition(pBullPos[i]);
						break;
					}
				}
			}
			else if (shotNum == 2.0f)
			{//貫通弾
				for (int i = 0; i < 100; i++)
				{
					if (!pBullArive[i])
					{
						isShot = false;//弾のセット終了
						pBullArive[i] = true;//弾を生成
						pBullAriveTime[i] = partsInitTime[2];//弾の生存時間を設定
						attackNum[i] = shotNum;//弾の挙動を設定
						pBullPos[i] = pPos;//弾を自機の位置にセット
						pBull[i]->SetPosition(pBullPos[i]);
						break;
					}
				}
			}
		}
		for (int i = 0; i < 100; i++)
		{//弾の移動
			if (pBullArive[i])
			{
				pBullAriveTime[i] -= 1.0f;//生存時間を減らす

				//弾の種類毎に挙動を変える
				if (attackNum[i] == 0) { pBullPos[i].z += pBullSpead[0]; }
				else if (attackNum[i] == 1) { pBullPos[i].z += pBullSpead[1]; }
				else if (attackNum[i] == 2) { pBullPos[i].z += pBullSpead[2]; }

				if (pBullAriveTime[i] <= 0.0f)
				{//生存時間が0になったら消滅
					pBullArive[i] = false;//消滅させる
					pBullPos[i] = deadPos;//画面外に移動
				}

				pBull[i]->SetPosition(pBullPos[i]);
			}
		}
		if (isMove)
		{//プレイヤーの移動
			if (moveDire)
			{//右に移動
				moveTime += moveTimeSpead;//移動時間をカウント
				moveCount += moveCountSpead;//移動量をカウント
				pPos.x += moveCountSpead;//X座標を移動
				pPos.y = pOldPos.y + (2.45f - gravity * moveTime);//Y座標を移動
				pRot.z -= movePRot;//Z軸の回転
				pRot.y -= movePRot;//Y軸の回転
				cameraPos.x += moveCountSpead;//カメラの移動

				for (int i = 0; i < 100; i++)
				{//パーツが生きている場合はパーツも移動と回転
					if (pPartsArive[i])
					{
						pPartsPos[i].x += moveCountSpead;//X座標の移動
						pPartsPos[i].y = pPartsOldPos[i].y + (2.45f - gravity * moveTime);//Y座標の移動
						pPartsRot[i].z -= movePRot;//Z軸の回転
						pPartsRot[i].y -= movePRot;//Y軸の回転
					}
				}
				//移動量が規定値になったら移動終了
				if (moveCount > moveFinish) { isMove = false; }
			}
			else if (!moveDire)
			{//左に移動
				moveTime += moveTimeSpead;//移動時間をカウント
				moveCount -= moveCountSpead;//移動量をカウント
				pPos.x -= moveCountSpead;//X座標を移動
				pPos.y = pOldPos.y + (2.45f - gravity * moveTime);//Y座標を移動
				pRot.z += movePRot;//Z軸の回転
				pRot.y += movePRot;//Y軸の回転
				cameraPos.x -= moveCountSpead;//カメラの移動

				for (int i = 0; i < 100; i++)
				{//パーツが生きている場合はパーツも移動と回転
					if (pPartsArive[i])
					{
						pPartsPos[i].x -= moveCountSpead;//X座標の移動
						pPartsPos[i].y = pPartsOldPos[i].y + (2.45f - gravity * moveTime);//Y座標の移動
						pPartsRot[i].z += movePRot;//Z軸の回転
						pPartsRot[i].y += movePRot;//Y軸の回転
					}
				}
				//移動量が規定値になったら移動終了
				if (moveCount < -moveFinish) { isMove = false; }
			}
		}
		if (isChange)
		{//プレイヤーのパーツ更新
			if (partsNum == 0.0f)
			{//通常弾
				for (int j = 0; j < 100; j++)
				{//パーツを剥がす
					if (pPartsArive[j])
					{
						pPartsArive[j] = false;//パーツを消滅
						pPartsPos[j] = deadPos;//画面外に移動
						playerParts[j]->SetPosition(pPartsPos[j]);
					}
				}
			}
			else if (partsNum == 1.0f)
			{//三列弾
				for (int j = 0; j < 2; j++)
				{//左右のレーンにパーツを設置
					pPartsArive[j] = true;//パーツを生成
					pPartsPos[j] = pPos;//プレイヤーの位置に一旦移動
				}

				pPartsPos[0].x -= 4.0f;//プレイヤーの左のレーンに移動
				pPartsPos[1].x += 4.0f;//プレイヤーの右のレーンに移動

				for (int j = 0; j < 2; j++)
				{
					playerParts[j]->SetPosition(pPartsPos[j]);
				}
			}
			else if (partsNum == 2.0f)
			{//貫通弾
				pPartsArive[0] = true;//パーツを生成
				pPartsPos[0] = pPos;//プレイヤーの位置に移動
				playerParts[0]->SetPosition(pPartsPos[0]);
			}
		}

		//システム関連
		//前に自動で進み続ける
		pPos.z += movePosZ;//プレイヤー前
		cameraPos.z += movePosZ;//カメラ前
		domePos.z += movePosZ;//天球前
		deadLinePos += movePosZ;//敵消滅ライン
		dome->SetPosition(domePos);//天球の座標更新
		player->SetPosition(pPos);//プレイヤーの座標更新
		player->SetRotation(pRot);//プレイヤーの回転量更新
		camera->SetTarget(cameraPos);//カメラの座標更新
		
		for (int i = 0; i < 100; i++)
		{
			if (pPartsArive[i])
			{//プレイヤーのパーツが生きている場合
				pPartsPos[i].z += movePosZ;//プレイヤーのパーツ前
			}
			if (enemy01Arive[i])
			{//敵01が生きている場合
				e01Pos[i].z += moveEPosZ / 2;

				if (e01Pos[i].z < deadLinePos)
				{//画面外に退避
					e01Pos[i] = deadPos;
					enemy01Arive[i] = false;
				}
			}
			if (enemy02Arive[i])
			{//敵02が生きている場合
				e02Pos[i].z += moveEPosZ;

				if (e02Pos[i].z < deadLinePos)
				{//画面外に退避
					e02Pos[i] = deadPos;
					enemy02Arive[i] = false;
				}
			}
			if (enemy03Arive[i])
			{//敵03が生きている場合
				if (e03Pos[i].z < deadLinePos)
				{//画面外に退避
					e03Pos[i] = deadPos;
					enemy03Arive[i] = false;
				}
			}
			if (enemy04Arive[i])
			{//敵04が生きている場合
				if (e04Pos[i].z < deadLinePos)
				{//画面外に退避
					e04Pos[i] = deadPos;
					enemy04Arive[i] = false;
				}
			}
			if (parts01Arive[i])
			{//パーツ1が生きている場合
				if (parts01Pos[i].z < deadLinePos)
				{//画面外に退避
					parts01Pos[i] = deadPos;
					parts01Arive[i] = false;
				}
			}
			if (parts02Arive[i])
			{//パーツ2が生きている場合
				if (parts02Pos[i].z < deadLinePos)
				{//画面外に退避
					parts02Pos[i] = deadPos;
					parts02Arive[i] = false;
				}
			}
			if (parts03Arive[i])
			{//パーツ3が生きている場合
				if (parts03Pos[i].z < deadLinePos)
				{//画面外に退避
					parts03Pos[i] = deadPos;
					parts03Arive[i] = false;
				}
			}
			if (parts04Arive[i])
			{//パーツ1が生きている場合
				if (parts04Pos[i].z < deadLinePos)
				{//画面外に退避
					parts04Pos[i] = deadPos;
					parts04Arive[i] = false;
				}
			}

			playerParts[i]->SetPosition(pPartsPos[i]);//プレイヤーのパーツ座標更新
			playerParts[i]->SetRotation(pPartsRot[i]);//プレイヤーのパーツ回転量更新
			enemy01[i]->SetPosition(e01Pos[i]);//敵1の座標更新
			enemy02[i]->SetPosition(e02Pos[i]);//敵2の座標更新
			enemy03[i]->SetPosition(e03Pos[i]);//敵3の座標更新
			enemy04[i]->SetPosition(e04Pos[i]);//敵4の座標更新
			parts01[i]->SetPosition(parts01Pos[i]);//パーツ1の座標更新
			parts02[i]->SetPosition(parts02Pos[i]);//パーツ2の座標更新
			parts03[i]->SetPosition(parts03Pos[i]);//パーツ3の座標更新
			parts04[i]->SetPosition(parts04Pos[i]);//パーツ4の座標更新
		}

		//天球の回転
		XMFLOAT3 rotation = dome->GetRotation();//現在の回転量を取得
		rotation.y += domeRotSpead;//回転させる
		dome->SetRotation(rotation);
		//水の流れ
		XMFLOAT3 waterPosition = waterPlane->GetPosition();//現在の移動量を取得
		waterPosition.z -= waterPosSpead;//移動させる
		waterPlane->SetPosition(waterPosition);
		//パーツの回転
		for (int i = 0; i < 100; i++)
		{
			if (parts01Arive[i])
			{//生きているパーツだけ回転させる
				XMFLOAT3 rotation = parts01[i]->GetRotation();//現在の回転量を取得
				rotation.y += partsRotSpead;//回転させる
				parts01[i]->SetRotation(rotation);
			}
			if (parts02Arive[i])
			{//生きているパーツだけ回転させる
				XMFLOAT3 rotation = parts02[i]->GetRotation();//現在の回転量を取得
				rotation.x += partsRotSpead;//回転させる
				parts02[i]->SetRotation(rotation);
			}
			if (parts03Arive[i])
			{//生きているパーツだけ回転させる
				XMFLOAT3 rotation = parts03[i]->GetRotation();//現在の回転量を取得
				rotation.z += partsRotSpead;//回転させる
				parts03[i]->SetRotation(rotation);
			}
			if (parts04Arive[i])
			{//生きているパーツだけ回転させる
				XMFLOAT3 rotation = parts04[i]->GetRotation();//現在の回転量を取得
				rotation.x += partsRotSpead;//回転させる
				rotation.z += partsRotSpead;//回転させる
				parts04[i]->SetRotation(rotation);
			}
		}

		for (int i = 0; i < 100; i++)
		{//弾と敵1の当たり判定
			if (pBullArive[i])
			{//弾の生存確認
				for (int j = 0; j < 100; j++)
				{
					if (enemy01Arive[j])
					{//敵の生存確認
						float a = pBullPos[i].x - e01Pos[j].x;
						float b = pBullPos[i].y - e01Pos[j].y;
						float c = pBullPos[i].z - e01Pos[j].z;
						float d = sqrt(a * a + b * b + c * c);

						if (d <= 1)
						{//当たっていたらお互い消滅
							if (attackNum[i] != 2.0f)
							{//貫通弾は消滅しない
								pBullArive[i] = false;//弾消滅
								pBullPos[i] = deadPos;//画面外に移動
							}

							enemy01Arive[j] = false;//敵消滅

							for (int k = 0; k < 100; k++)
							{
								if (!parts01Arive[k])
								{//パーツの生成
									parts01Arive[k] = true;//パーツ生成
									parts01Pos[k] = e01Pos[j];//敵の死亡位置にパーツ設置
									parts01[k]->SetPosition(parts01Pos[k]);
									break;
								}
							}

							e01Pos[j] = deadPos;//画面外に移動
							pBull[i]->SetPosition(pBullPos[i]);
							enemy01[j]->SetPosition(e01Pos[j]);
						}
					}
					if (enemy02Arive[j])
					{//敵の生存確認
						float a = pBullPos[i].x - e02Pos[j].x;
						float b = pBullPos[i].y - e02Pos[j].y;
						float c = pBullPos[i].z - e02Pos[j].z;
						float d = sqrt(a * a + b * b + c * c);

						if (d <= 1)
						{//当たっていたらお互い消滅
							if (attackNum[i] != 2.0f)
							{//貫通弾は消滅しない
								pBullArive[i] = false;//弾消滅
								pBullPos[i] = deadPos;//画面外に移動
							}
							enemy02Arive[j] = false;//敵消滅

							/*
							for (int k = 0; k < 100; k++)
							{
								if (!parts02Arive[k])
								{//パーツの生成
									parts02Arive[k] = true;//パーツ生成
									parts02Pos[k] = e02Pos[j];//敵の死亡位置にパーツ設置
									parts02[k]->SetPosition(parts02Pos[k]);
									break;
								}
							}
							*/

							e02Pos[j] = deadPos;//画面外に移動
							pBull[i]->SetPosition(pBullPos[i]);
							enemy02[j]->SetPosition(e02Pos[j]);
						}
					}
					if (enemy03Arive[j])
					{//敵の生存確認
						float a = pBullPos[i].x - e03Pos[j].x;
						float b = pBullPos[i].y - e03Pos[j].y;
						float c = pBullPos[i].z - e03Pos[j].z;
						float d = sqrt(a * a + b * b + c * c);

						if (d <= 1)
						{//当たっていたらお互い消滅
							if (attackNum[i] != 2.0f)
							{//貫通弾は消滅しない
								pBullArive[i] = false;//弾消滅
								pBullPos[i] = deadPos;//画面外に移動
							}
							//enemy03Arive[j] = false;//敵消滅

							/*
							for (int k = 0; k < 100; k++)
							{
								if (!parts03Arive[k])
								{//パーツの生成
									parts03Arive[k] = true;//パーツ生成
									parts03Pos[k] = e03Pos[j];//敵の死亡位置にパーツ設置
									parts03[k]->SetPosition(parts03Pos[k]);
									break;
								}
							}
							*/

							//e03Pos[j] = deadPos;//画面外に移動
							pBull[i]->SetPosition(pBullPos[i]);
							//enemy03[j]->SetPosition(e03Pos[j]);
						}
					}
					if (enemy04Arive[j])
					{//敵の生存確認
						float a = pBullPos[i].x - e04Pos[j].x;
						float b = pBullPos[i].y - e04Pos[j].y;
						float c = pBullPos[i].z - e04Pos[j].z;
						float d = sqrt(a * a + b * b + c * c);

						if (d <= 1)
						{//当たっていたらお互い消滅
							if (attackNum[i] != 2.0f)
							{//貫通弾は消滅しない
								pBullArive[i] = false;//弾消滅
								pBullPos[i] = deadPos;//画面外に移動
							}
							enemy04Arive[j] = false;//敵消滅

							for (int k = 0; k < 100; k++)
							{
								if (!parts04Arive[k])
								{//パーツの生成
									parts04Arive[k] = true;//パーツ生成
									parts04Pos[k] = e04Pos[j];//敵の死亡位置にパーツ設置
									parts04[k]->SetPosition(parts04Pos[k]);
									break;
								}
							}

							e04Pos[j] = deadPos;//画面外に移動
							pBull[i]->SetPosition(pBullPos[i]);
							enemy04[j]->SetPosition(e04Pos[j]);
						}
					}
				}
			}
		}
		for (int i = 0; i < 100; i++)
		{//プレイヤーとパーツの当たり判定
			if (!isMove)
			{//移動中は拾えない
				if (parts01Arive[i])
				{//パーツ1の生存確認
					float a = pPos.x - parts01Pos[i].x;
					float b = pPos.y - parts01Pos[i].y;
					float c = pPos.z - parts01Pos[i].z;
					float d = sqrt(a * a + b * b + c * c);

					if (d <= 1)
					{//当たっていたら獲得
						if (partsNum == 0.0f)
						{//通常弾以外のパーツの場合はスルー
							partsNum = 1.0f;//パーツの種類を設定
							partsCD = partsInitCD[1];//パーツのCDを設定
							partsHP = partsInitHP[1];//パーツの耐久を設定
							isChange = true;//変更開始
						}
						else
						{//それ以外の場合は現在のパーツの耐久値を回復させる
							partsHP += partsRecovery[1];//パーツの耐久を回復
						}

						parts01Arive[i] = false;//パーツ1を消滅
						parts01Pos[i] = deadPos;//画面外に移動
						parts01[i]->SetPosition(parts01Pos[i]);
					}
				}
				if (parts02Arive[i])
				{//パーツ2の生存確認
					float a = pPos.x - parts02Pos[i].x;
					float b = pPos.y - parts02Pos[i].y;
					float c = pPos.z - parts02Pos[i].z;
					float d = sqrt(a * a + b * b + c * c);

					if (d <= 1)
					{//当たっていたら獲得
						if (partsNum == 0.0f)
						{//通常弾以外のパーツの場合はスルー
							partsNum = 2.0f;//パーツの種類を設定
							partsCD = partsInitCD[1];//パーツのCDを設定
							partsHP = partsInitHP[1];//パーツの耐久を設定
							isChange = true;//変更開始
						}
						else
						{//それ以外の場合は現在のパーツの耐久値を回復させる
							partsHP += partsRecovery[1];//パーツの耐久を回復
						}

						parts02Arive[i] = false;//パーツ2を消滅
						parts02Pos[i] = deadPos;//画面外に移動
						parts02[i]->SetPosition(parts02Pos[i]);
					}
				}
				if (parts03Arive[i])
				{//パーツ3の生存確認
					float a = pPos.x - parts03Pos[i].x;
					float b = pPos.y - parts03Pos[i].y;
					float c = pPos.z - parts03Pos[i].z;
					float d = sqrt(a * a + b * b + c * c);

					if (d <= 1)
					{//当たっていたら獲得
						for (int j = 0; j < 4; j++)
						{
							if (partsNum == 0.0f)
							{//通常弾以外のパーツの場合はスルー
								partsNum = 3.0f;//パーツの種類を設定
								partsCD = partsInitCD[2];//パーツのCDを設定
								partsHP = partsInitHP[2];//パーツの耐久を設定
								isChange = true;//変更開始
							}
							else
							{//それ以外の場合は現在のパーツの耐久値を回復させる
								partsHP += partsRecovery[2];//パーツの耐久を回復
							}
						}

						parts03Arive[i] = false;//パーツ3を消滅
						parts03Pos[i] = deadPos;//画面外に移動
						parts03[i]->SetPosition(parts03Pos[i]);
					}
				}
				if (parts04Arive[i])
				{//パーツ4の生存確認
					float a = pPos.x - parts04Pos[i].x;
					float b = pPos.y - parts04Pos[i].y;
					float c = pPos.z - parts04Pos[i].z;
					float d = sqrt(a * a + b * b + c * c);

					if (d <= 1)
					{//当たっていたら獲得
						if (partsNum == 0.0f)
						{//通常弾以外のパーツの場合はスルー
							partsNum = 4.0f;//パーツの種類を設定
							partsCD = partsInitCD[3];//パーツのCDを設定
							partsHP = partsInitHP[3];//パーツの耐久を設定
							isChange = true;//変更開始
						}
						else
						{//それ以外の場合は現在のパーツの耐久値を回復させる
							partsHP += partsRecovery[3];//パーツの耐久を回復
						}

						parts04Arive[i] = false;//パーツ4を消滅
						parts04Pos[i] = deadPos;//画面外に移動
						parts04[i]->SetPosition(parts04Pos[i]);
					}
				}
			}
		}
		for (int i = 0; i < 100; i++)
		{//プレイヤーと敵の当たり判定
			if (!isMove)
			{//移動中は無敵
				if (enemy01Arive[i])
				{//敵の生存確認
					float a = pPos.x - e01Pos[i].x;
					float b = pPos.y - e01Pos[i].y;
					float c = pPos.z - e01Pos[i].z;
					float d = sqrt(a * a + b * b + c * c);

					if (d <= 1)
					{//当たっていたら敵を消滅
						enemy01Arive[i] = false;//敵を消滅
						e01Pos[i] = deadPos;//画面外に移動
						enemy01[i]->SetPosition(e01Pos[i]);

						if (partsNum != 0.0f)
						{//選択中のパーツが破壊される
							partsNum = 0.0f;//パーツを通常弾に戻す
							partsCD = partsInitCD[0];//パーツのCDを通常弾に戻す
							partsHP = partsInitHP[0];//パーツのHPを初期化
							isChange = true;//変更開始
						}
						else if (partsNum == 0.0f)
						{//ゲームオーバー
							scene = Gameover;
						}
					}
				}
				if (enemy02Arive[i])
				{//敵の生存確認
					float a = pPos.x - e02Pos[i].x;
					float b = pPos.y - e02Pos[i].y;
					float c = pPos.z - e02Pos[i].z;
					float d = sqrt(a * a + b * b + c * c);

					if (d <= 1)
					{//当たっていたら敵を消滅
						enemy02Arive[i] = false;//敵を消滅
						e02Pos[i] = deadPos;//画面外に移動
						enemy02[i]->SetPosition(e02Pos[i]);

						if (partsNum != 0.0f)
						{//選択中のパーツが破壊される
							partsNum = 0.0f;//パーツを通常弾に戻す
							partsCD = partsInitCD[0];//パーツのCDを通常弾に戻す
							partsHP = partsInitHP[0];//パーツのHPを初期化
							isChange = true;//変更開始
						}
						else if (partsNum == 0.0f)
						{//ゲームオーバー
							scene = Gameover;
						}
					}
				}
				if (enemy03Arive[i])
				{//敵の生存確認
					float a = pPos.x - e03Pos[i].x;
					float b = pPos.y - e03Pos[i].y;
					float c = pPos.z - e03Pos[i].z;
					float d = sqrt(a * a + b * b + c * c);

					if (d <= 1)
					{//当たっていたら敵を消滅
						enemy03Arive[i] = false;//敵を消滅
						e03Pos[i] = deadPos;//画面外に移動
						enemy03[i]->SetPosition(e03Pos[i]);

						if (partsNum != 0.0f)
						{//選択中のパーツが破壊される
							partsNum = 0.0f;//パーツを通常弾に戻す
							partsCD = partsInitCD[0];//パーツのCDを通常弾に戻す
							partsHP = partsInitHP[0];//パーツのHPを初期化
							isChange = true;//変更開始
						}
						else if (partsNum == 0.0f)
						{//ゲームオーバー
							scene = Gameover;
						}
					}
				}
				if (enemy04Arive[i])
				{//敵の生存確認
					float a = pPos.x - e04Pos[i].x;
					float b = pPos.y - e04Pos[i].y;
					float c = pPos.z - e04Pos[i].z;
					float d = sqrt(a * a + b * b + c * c);

					if (d <= 1)
					{//当たっていたら敵を消滅
						enemy04Arive[i] = false;//敵を消滅
						e04Pos[i] = deadPos;//画面外に移動
						enemy04[i]->SetPosition(e04Pos[i]);

						if (partsNum != 0.0f)
						{//選択中のパーツが破壊される
							partsNum = 0.0f;//パーツを通常弾に戻す
							partsCD = partsInitCD[0];//パーツのCDを通常弾に戻す
							partsHP = partsInitHP[0];//パーツのHPを初期化
							isChange = true;//変更開始
						}
						else if (partsNum == 0.0f)
						{//ゲームオーバー
							scene = Gameover;
						}
					}
				}
			}
		}

		//敵の出現処理
		enemySpawnTimer++;//タイム処理
		if (enemySpawnTimer >= enemySpawnInterval)
		{//敵をスポーンさせる
			for (int i = 0; i < 100; i++)
			{
				int enemyNum = rand() % 3;//出現させる敵の種類をランダムに選択
				if (enemyNum == 0)
				{//敵1を出現
					if (!enemy01Arive[i])
					{//画面上にいない敵を選択
						//X座標をランダムに決定
						int xPos = rand() % 5;
						if (xPos == 0) { xPos = -8.0f; }
						else if (xPos == 1) { xPos = -4.0f; }
						else if (xPos == 2) { xPos = 0.0f; }
						else if (xPos == 3) { xPos = 4.0f; }
						else if (xPos == 4) { xPos = 8.0f; }

						enemy01Arive[i] = true;//敵を生成
						e01Pos[i].x = xPos;//敵のX座標を設定
						e01Pos[i].y = enemyPosY;//敵のY座標を設定
						e01Pos[i].z = pPos.z + enemyPosZ;//敵のZ座標を設定
						enemySpawnTimer = 0.0f;//タイマーをリセット
						enemy01[i]->SetPosition(e01Pos[i]);
						break;
					}
				}
				else if (enemyNum == 1)
				{//敵2を出現
					if (!enemy02Arive[i])
					{//画面上にいない敵を選択
						//X座標をランダムに決定
						int xPos = rand() % 5;
						if (xPos == 0) { xPos = -8.0f; }
						else if (xPos == 1) { xPos = -4.0f; }
						else if (xPos == 2) { xPos = 0.0f; }
						else if (xPos == 3) { xPos = 4.0f; }
						else if (xPos == 4) { xPos = 8.0f; }

						enemy02Arive[i] = true;//敵を生成
						e02Pos[i].x = xPos;//敵のX座標を設定
						e02Pos[i].y = enemyPosY;//敵のY座標を設定
						e02Pos[i].z = pPos.z + enemyPosZ;//敵のZ座標を設定
						enemySpawnTimer = 0.0f;//タイマーをリセット
						enemy02[i]->SetPosition(e02Pos[i]);
						break;
					}
				}
				else if (enemyNum == 2)
				{//敵3を出現
					if (!enemy03Arive[i])
					{//画面上にいない敵を選択
						//X座標をランダムに決定
						int xPos = rand() % 5;
						if (xPos == 0) { xPos = -8.0f; }
						else if (xPos == 1) { xPos = -4.0f; }
						else if (xPos == 2) { xPos = 0.0f; }
						else if (xPos == 3) { xPos = 4.0f; }
						else if (xPos == 4) { xPos = 8.0f; }

						enemy03Arive[i] = true;//敵を生成
						e03Pos[i].x = xPos;//敵のX座標を設定
						e03Pos[i].y = enemyPosY;//敵のY座標を設定
						e03Pos[i].z = pPos.z + enemyPosZ;//敵のZ座標を設定
						enemySpawnTimer = 0.0f;//タイマーをリセット
						enemy03[i]->SetPosition(e03Pos[i]);
						break;
					}
				}
				else if (enemyNum == 3)
				{//敵4を出現
					if (!enemy04Arive[i])
					{//画面上にいない敵を選択
						//X座標をランダムに決定
						int xPos = rand() % 5;
						if (xPos == 0) { xPos = -8.0f; }
						else if (xPos == 1) { xPos = -4.0f; }
						else if (xPos == 2) { xPos = 0.0f; }
						else if (xPos == 3) { xPos = 4.0f; }
						else if (xPos == 4) { xPos = 8.0f; }

						enemy04Arive[i] = true;//敵を生成
						e04Pos[i].x = xPos;//敵のX座標を設定
						e04Pos[i].y = enemyPosY;//敵のY座標を設定
						e04Pos[i].z = pPos.z + enemyPosZ;//敵のZ座標を設定
						enemySpawnTimer = 0.0f;//タイマーをリセット
						enemy04[i]->SetPosition(e04Pos[i]);
						break;
					}
				}
			}
		}

		//一定時間経過したらリザルトシーンに移行
		score += 1.0f;//距離加算
		gameTime += 1.0f;//タイム処理
		if (gameTime >= gameTimeLimit) { scene = Clear; }
	}
	else if (scene == End)
	{//リザルト
		//プレイヤー操作関連
		if (input->TriggerKey(DIK_SPACE))
		{//SPACE入力でタイトルシーンへ移行
			//初期化処理
			cameraPos = { 0.0f, 4.5f, 0.0f };//カメラの座標
			domePos = { 0.0f, 0.0f, 0.0f };//天球の座標
			domeRot = { 0.0f, 0.0f, 0.0f };//天球の回転量
			pPos = { 0.0f, 2.0f, 3.0f };//プレイヤーの座標
			pRot = { 0.0f, 0.0f, 0.0f };//プレイヤーの回転量
			waterPos = { 0.0f, 0.0f, 0.0f };//水の座標
			pRail = 2.0f;//プレイヤーのいるレール(一番左から0)
			moveTime = 0.0f;//移動時間をカウント
			moveCount = 0.0f;//移動量をカウント
			isMove = false;//移動中かどうか
			moveDire = false;//移動の向き(false:左 true:右)
			shotNum = 0.0f;//攻撃パターン
			isShot = false;//攻撃中かどうか
			isChange = false;//プレイヤーのパーツ変更中かどうか
			enemySpawnTimer = 0.0f;//敵の出現までのカウント
			enemySpawnInterval = 10.0f;//敵出現までの間隔
			gameTime = 0.0f;//ゲーム経過時間
			score = 0.0f;//進んだ距離
			deadLinePos = -10.0f;//死亡ライン

			//レーン
			railPos[0] = { -8.0f, 2.0f, 0.0f };
			railPos[1] = { -4.0f, 2.0f, 0.0f };
			railPos[2] = { 0.0f, 2.0f, 0.0f };
			railPos[3] = { 4.0f, 2.0f, 0.0f };
			railPos[4] = { 8.0f, 2.0f, 0.0f };
		
			partsNum = 0.0f;//装着中のパーツの種類
			partsCD = 20.0f;//パーツのCT
			partsHP = 0.0f;//パーツの耐久値

			clockHandsPos = { 138.0f, 42.0f };//タイムバーの座標
			clockHandsSprite->SetPosition(clockHandsPos);

			for (int i = 0; i < 100; i++)
			{
				e01Pos[i] = deadPos;//敵1の座標
				e01Rot[i] = { 0.0f, 0.0f, 0.0f };//敵1の回転
				e02Pos[i] = deadPos;//敵2の座標
				e02Rot[i] = { 0.0f, 0.0f, 0.0f };//敵2の回転
				e03Pos[i] = deadPos;//敵3の座標
				e03Rot[i] = { 0.0f, 0.0f, 0.0f };//敵3の回転
				e04Pos[i] = deadPos;//敵4の座標
				e04Rot[i] = { 0.0f, 0.0f, 0.0f };//敵4の回転
				pBullPos[i] = deadPos;//プレイヤーの弾の座標
				eBullPos[i] = deadPos;//プレイヤーの弾の座標
				parts01Pos[i] = deadPos;//パーツ1の座標
				parts02Pos[i] = deadPos;//パーツ2の座標
				parts03Pos[i] = deadPos;//パーツ3の座標
				parts04Pos[i] = deadPos;//パーツ4の座標
				pPartsPos[i] = deadPos;//プレイヤーのパーツの座標
				pPartsRot[i] = { 0.0f, 0.0f, 0.0f };//プレイヤーのパーツの回転量
				pBullAriveTime[i] = 0.0f;//弾が生きている時間
				eBullAriveTime[i] = 0.0f;//弾が生きている時間
				attackNum[i] = 0.0f;//弾の処理パターン
				pBullArive[i] = false;//弾が生きているかの判定
				eBullArive[i] = false;//弾が生きているかの判定
				pPartsArive[i] = false;//プレイヤーのパーツが生きているかの判定
				parts01Arive[i] = false;//パーツ1が生きているかの判定
				parts02Arive[i] = false;//パーツ2が生きているかの判定
				parts03Arive[i] = false;//パーツ3が生きているかの判定
				parts04Arive[i] = false;//パーツ4が生きているかの判定
				enemy01Arive[i] = false;//敵1が生きているかの判定
				enemy02Arive[i] = false;//敵2が生きているかの判定
				enemy03Arive[i] = false;//敵3が生きているかの判定
				enemy04Arive[i] = false;//敵4が生きているかの判定
			}

			scene = Title;//シーン移行
		}
	}
	else if (scene == Clear)
	{
		if (!logoMove)
		{//ロゴ移動中
			logoMoveCount += 1.0f;//カウント増加

			if (!logoMoveDirection)
			{//下移動
				stageClearLogoPos.y = stageClearLogoPos.y + logoMoveSpead;//座標変更

				if (logoMoveCount >= logoMoveTime)
				{//移動終了
					logoMoveCount = 0.0f;//カウントリセット
					logoMoveDirection = true;//向きを変更
					logoMove = true;//停止中に変更
				}
			}
			else
			{//上移動
				stageClearLogoPos.y = stageClearLogoPos.y - logoMoveSpead;//座標変更

				if (logoMoveCount >= logoMoveTime)
				{//移動終了
					//isSceneChange = true;//シーン遷移
				}
			}
			stageClearLogo->SetPosition(stageClearLogoPos);

			if (sceneChangeCount == 45.0f)
			{
				stageClearLogo->SetPosition({ 40.0f, 50.0f });//STAGE CLEARロゴを移動
				logoMoveCount = 0.0f;//カウントリセット
				logoMoveDirection = false;//向きを変更
				logoMove = false;//移動中に変更
				scene = ClearResult;//シーン移行
			}
		}
		else
		{//ロゴ停止中
			logoMoveCount += 1.0f;//カウント増加

			if (logoMoveCount >= logoStopTime)
			{//停止終了
				logoMoveCount = 0.0f;//カウントリセット
				logoMove = false;//移動中に変更
				isSceneChange = true;//シーン遷移
			}
		}
	}
	else if (scene == Gameover)
	{
		if (!logoMove)
		{//ロゴ移動中
			logoMoveCount += 1.0f;//カウント増加

			if (!logoMoveDirection)
			{//下移動
				gameoverLogoPos.y = gameoverLogoPos.y + logoMoveSpead;//座標変更

				if (logoMoveCount >= logoMoveTime)
				{//移動終了
					logoMoveCount = 0.0f;//カウントリセット
					logoMoveDirection = true;//向きを変更
					logoMove = true;//停止中に変更
				}
			}
			else
			{//上移動
				gameoverLogoPos.y = gameoverLogoPos.y - logoMoveSpead;//座標変更

				if (logoMoveCount >= logoMoveTime)
				{//移動終了
					//isSceneChange = true;//シーン遷移
				}
			}
			gameoverLogo->SetPosition(gameoverLogoPos);

			if (sceneChangeCount == 45.0f)
			{
				gameoverLogo->SetPosition({ 153.0f, 50.0f });//GAME OVERロゴを移動
				logoMoveCount = 0.0f;//カウントリセット
				logoMoveDirection = false;//向きを変更
				logoMove = false;//移動中に変更
				scene = GameoverResult;//シーン移行
			}
		}
		else
		{//ロゴ停止中
			logoMoveCount += 1.0f;//カウント増加

			if (logoMoveCount >= logoStopTime)
			{//停止終了
				logoMoveCount = 0.0f;//カウントリセット
				logoMove = false;//移動中に変更
				isSceneChange = true;//シーン遷移
			}
		}
	}
	else if (scene == ClearResult)
	{
		//プレイヤー操作関連
		if (input->TriggerKey(DIK_SPACE) && !isSceneChange)
		{//選択中のステージへ移行
			isSceneChange = true;//シーン遷移
			cameraPos = { 0.0f, 4.5f, 0.0f };//カメラの座標
			domePos = { 0.0f, 0.0f, 0.0f };//天球の座標
			domeRot = { 0.0f, 0.0f, 0.0f };//天球の回転量
			pPos = { 0.0f, 2.0f, 3.0f };//プレイヤーの座標
			pRot = { 0.0f, 0.0f, 0.0f };//プレイヤーの回転量
			waterPos = { 0.0f, 0.0f, 0.0f };//水の座標
			pRail = 2.0f;//プレイヤーのいるレール(一番左から0)
			moveTime = 0.0f;//移動時間をカウント
			moveCount = 0.0f;//移動量をカウント
			isMove = false;//移動中かどうか
			moveDire = false;//移動の向き(false:左 true:右)
			shotNum = 0.0f;//攻撃パターン
			isShot = false;//攻撃中かどうか
			isChange = false;//プレイヤーのパーツ変更中かどうか
			enemySpawnTimer = 0.0f;//敵の出現までのカウント
			enemySpawnInterval = 10.0f;//敵出現までの間隔
			gameTime = 0.0f;//ゲーム経過時間
			score = 0.0f;//進んだ距離
			deadLinePos = -10.0f;//死亡ライン

			//レーン
			railPos[0] = { -8.0f, 2.0f, 0.0f };
			railPos[1] = { -4.0f, 2.0f, 0.0f };
			railPos[2] = { 0.0f, 2.0f, 0.0f };
			railPos[3] = { 4.0f, 2.0f, 0.0f };
			railPos[4] = { 8.0f, 2.0f, 0.0f };

			partsNum = 0.0f;//装着中のパーツの種類
			partsCD = 20.0f;//パーツのCT
			partsHP = 0.0f;//パーツの耐久値

			clockHandsPos = { 138.0f, 42.0f };//タイムバーの座標
			clockHandsSprite->SetPosition(clockHandsPos);

			for (int i = 0; i < 100; i++)
			{
				e01Pos[i] = deadPos;//敵1の座標
				e01Rot[i] = { 0.0f, 0.0f, 0.0f };//敵1の回転
				e02Pos[i] = deadPos;//敵2の座標
				e02Rot[i] = { 0.0f, 0.0f, 0.0f };//敵2の回転
				e03Pos[i] = deadPos;//敵3の座標
				e03Rot[i] = { 0.0f, 0.0f, 0.0f };//敵3の回転
				e04Pos[i] = deadPos;//敵4の座標
				e04Rot[i] = { 0.0f, 0.0f, 0.0f };//敵4の回転
				pBullPos[i] = deadPos;//プレイヤーの弾の座標
				eBullPos[i] = deadPos;//プレイヤーの弾の座標
				parts01Pos[i] = deadPos;//パーツ1の座標
				parts02Pos[i] = deadPos;//パーツ2の座標
				parts03Pos[i] = deadPos;//パーツ3の座標
				parts04Pos[i] = deadPos;//パーツ4の座標
				pPartsPos[i] = deadPos;//プレイヤーのパーツの座標
				pPartsRot[i] = { 0.0f, 0.0f, 0.0f };//プレイヤーのパーツの回転量
				pBullAriveTime[i] = 0.0f;//弾が生きている時間
				eBullAriveTime[i] = 0.0f;//弾が生きている時間
				attackNum[i] = 0.0f;//弾の処理パターン
				pBullArive[i] = false;//弾が生きているかの判定
				eBullArive[i] = false;//弾が生きているかの判定
				pPartsArive[i] = false;//プレイヤーのパーツが生きているかの判定
				parts01Arive[i] = false;//パーツ1が生きているかの判定
				parts02Arive[i] = false;//パーツ2が生きているかの判定
				parts03Arive[i] = false;//パーツ3が生きているかの判定
				parts04Arive[i] = false;//パーツ4が生きているかの判定
				enemy01Arive[i] = false;//敵1が生きているかの判定
				enemy02Arive[i] = false;//敵2が生きているかの判定
				enemy03Arive[i] = false;//敵3が生きているかの判定
				enemy04Arive[i] = false;//敵4が生きているかの判定
			}
		}
		if (input->TriggerKey(DIK_A) && resultSelect && !isSceneChange)
		{//一番左にいない場合、左に移動
			logoFlashCount = 0.0f;//カウントリセット
			logoHid = false;//ロゴの表示
			resultSelect = false;
		}
		if (input->TriggerKey(DIK_D) && !resultSelect && !isSceneChange)
		{//一番右にいない場合、右に移動
			logoFlashCount = 0.0f;//カウントリセット
			logoHid = false;//ロゴの表示
			resultSelect = true;
		}
		if (sceneChangeCount == 45.0f)
		{
			stageClearLogo->SetPosition({ 40.0f, -150.0f });//STAGE CLEARロゴを移動
			logoFlashCount = 0.0f;//ロゴ点滅カウント
			logoHid = false;//ロゴの表示

			//シーン移行
			if (!resultSelect) { scene = Start; }
			else if (resultSelect) { scene = Title; }

			resultSelect = false;
		}

		logoFlash();//ロゴ点滅処理
	}
	else if (scene == GameoverResult)
	{
		//プレイヤー操作関連
		if (input->TriggerKey(DIK_SPACE) && !isSceneChange)
		{//選択中のステージへ移行
			isSceneChange = true;//シーン遷移
			cameraPos = { 0.0f, 4.5f, 0.0f };//カメラの座標
			domePos = { 0.0f, 0.0f, 0.0f };//天球の座標
			domeRot = { 0.0f, 0.0f, 0.0f };//天球の回転量
			pPos = { 0.0f, 2.0f, 3.0f };//プレイヤーの座標
			pRot = { 0.0f, 0.0f, 0.0f };//プレイヤーの回転量
			waterPos = { 0.0f, 0.0f, 0.0f };//水の座標
			pRail = 2.0f;//プレイヤーのいるレール(一番左から0)
			moveTime = 0.0f;//移動時間をカウント
			moveCount = 0.0f;//移動量をカウント
			isMove = false;//移動中かどうか
			moveDire = false;//移動の向き(false:左 true:右)
			shotNum = 0.0f;//攻撃パターン
			isShot = false;//攻撃中かどうか
			isChange = false;//プレイヤーのパーツ変更中かどうか
			enemySpawnTimer = 0.0f;//敵の出現までのカウント
			enemySpawnInterval = 10.0f;//敵出現までの間隔
			gameTime = 0.0f;//ゲーム経過時間
			score = 0.0f;//進んだ距離
			deadLinePos = -10.0f;//死亡ライン

			//レーン
			railPos[0] = { -8.0f, 2.0f, 0.0f };
			railPos[1] = { -4.0f, 2.0f, 0.0f };
			railPos[2] = { 0.0f, 2.0f, 0.0f };
			railPos[3] = { 4.0f, 2.0f, 0.0f };
			railPos[4] = { 8.0f, 2.0f, 0.0f };

			partsNum = 0.0f;//装着中のパーツの種類
			partsCD = 20.0f;//パーツのCT
			partsHP = 0.0f;//パーツの耐久値

			clockHandsPos = { 138.0f, 42.0f };//タイムバーの座標
			clockHandsSprite->SetPosition(clockHandsPos);

			for (int i = 0; i < 100; i++)
			{
				e01Pos[i] = deadPos;//敵1の座標
				e01Rot[i] = { 0.0f, 0.0f, 0.0f };//敵1の回転
				e02Pos[i] = deadPos;//敵2の座標
				e02Rot[i] = { 0.0f, 0.0f, 0.0f };//敵2の回転
				e03Pos[i] = deadPos;//敵3の座標
				e03Rot[i] = { 0.0f, 0.0f, 0.0f };//敵3の回転
				e04Pos[i] = deadPos;//敵4の座標
				e04Rot[i] = { 0.0f, 0.0f, 0.0f };//敵4の回転
				pBullPos[i] = deadPos;//プレイヤーの弾の座標
				eBullPos[i] = deadPos;//プレイヤーの弾の座標
				parts01Pos[i] = deadPos;//パーツ1の座標
				parts02Pos[i] = deadPos;//パーツ2の座標
				parts03Pos[i] = deadPos;//パーツ3の座標
				parts04Pos[i] = deadPos;//パーツ4の座標
				pPartsPos[i] = deadPos;//プレイヤーのパーツの座標
				pPartsRot[i] = { 0.0f, 0.0f, 0.0f };//プレイヤーのパーツの回転量
				pBullAriveTime[i] = 0.0f;//弾が生きている時間
				eBullAriveTime[i] = 0.0f;//弾が生きている時間
				attackNum[i] = 0.0f;//弾の処理パターン
				pBullArive[i] = false;//弾が生きているかの判定
				eBullArive[i] = false;//弾が生きているかの判定
				pPartsArive[i] = false;//プレイヤーのパーツが生きているかの判定
				parts01Arive[i] = false;//パーツ1が生きているかの判定
				parts02Arive[i] = false;//パーツ2が生きているかの判定
				parts03Arive[i] = false;//パーツ3が生きているかの判定
				parts04Arive[i] = false;//パーツ4が生きているかの判定
				enemy01Arive[i] = false;//敵1が生きているかの判定
				enemy02Arive[i] = false;//敵2が生きているかの判定
				enemy03Arive[i] = false;//敵3が生きているかの判定
				enemy04Arive[i] = false;//敵4が生きているかの判定
			}
		}
		if (input->TriggerKey(DIK_A) && resultSelect && !isSceneChange)
		{//一番左にいない場合、左に移動
			logoFlashCount = 0.0f;//カウントリセット
			logoHid = false;//ロゴの表示
			resultSelect = false;
		}
		if (input->TriggerKey(DIK_D) && !resultSelect && !isSceneChange)
		{//一番右にいない場合、右に移動
			logoFlashCount = 0.0f;//カウントリセット
			logoHid = false;//ロゴの表示
			resultSelect = true;
		}
		if (sceneChangeCount == 45.0f)
		{
			gameoverLogo->SetPosition({ 153.0f, -150.0f });//GAME OVERロゴを移動
			logoFlashCount = 0.0f;//ロゴ点滅カウント
			logoHid = false;//ロゴの表示
			//シーン移行
			if (!resultSelect) { scene = Start; }
			else if (resultSelect) { scene = Title; }

			resultSelect = false;
		}

		logoFlash();//ロゴ点滅処理
	}

	//CreateParticles();//パーティクル生成
	lightGroup->Update();
	camera->Update();
	particleMan->Update();
	dome->Update();
	waterPlane->Update();
	player->Update();

	for (int i = 0; i < 5; i++)
	{
		rail[i]->Update();
	}
	for (int i = 0; i < 100; i++)
	{
		enemy01[i]->Update();
		enemy02[i]->Update();
		enemy03[i]->Update();
		enemy04[i]->Update();
		pBull[i]->Update();
		eBull[i]->Update();
		parts01[i]->Update();
		parts02[i]->Update();
		parts03[i]->Update();
		parts04[i]->Update();
		playerParts[i]->Update();
	}
}

void GameScene::Draw()
{
	//コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	//背景スプライト描画
	Sprite::PreDraw(cmdList);//背景スプライト描画前処理

	//背景スプライト描画
	backSprite->Draw();//基礎背景

	Sprite::PostDraw();//スプライト描画後処理
	dxCommon->ClearDepthBuffer();//深度バッファクリア

	//水オブジェクトの描画
	WaterObject::PreDraw(cmdList);//水オブジェクト描画前処理

	//水
	if (scene == Start || scene == Game || scene == Clear || scene == Gameover) { waterPlane->Draw(); }
	
	WaterObject::PostDraw();//水オブジェクト描画後処理

	//3Dオブジェクトの描画
	Object3d::PreDraw(cmdList);//オブジェクト描画前処理

	if (scene == Start || scene == Game || scene == Clear || scene == Gameover)
	{
		dome->Draw();
		player->Draw();

		for (int i = 0; i < 5; i++)
		{
			rail[i]->Draw();
		}
		for (int i = 0; i < 100; i++)
		{
			if (enemy01Arive[i]) { enemy01[i]->Draw(); }
			if (enemy02Arive[i]) { enemy02[i]->Draw(); }
			if (enemy03Arive[i]) { enemy03[i]->Draw(); }
			if (enemy04Arive[i]) { enemy04[i]->Draw(); }
			if (pBullArive[i]) { pBull[i]->Draw(); }
			if (eBullArive[i]) { eBull[i]->Draw(); }
			if (parts01Arive[i]) { parts01[i]->Draw(); }
			if (parts02Arive[i]) { parts02[i]->Draw(); }
			if (parts03Arive[i]) { parts03[i]->Draw(); }
			if (parts04Arive[i]) { parts04[i]->Draw(); }
			if (pPartsArive[i]) { playerParts[i]->Draw(); }
		}
	}

	Object3d::PostDraw();//オブジェクト描画後処理

	//パーティクルの描画
	particleMan->Draw(cmdList);

	//前景スプライト描画
	//前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	if (scene == Title)
	{//タイトル
		titleLogo->Draw();//ゲームタイトルロゴ
		if (!logoHid) { spaceLogo->Draw(); }//SPACEロゴ
	}
	else if (scene == StageSelect)
	{//ステージセレクト
		stageSelectLogo->Draw();//STAGE SELECTロゴ
		aLogo->Draw();//Aロゴ
		dLogo->Draw();//Dロゴ
		leftArrowLogo->Draw();//←ロゴ
		rightArrowLogo->Draw();//→ロゴ

		if (logoHid)
		{//非表示中
			if (selectStageNum != 1.0f)
			{//選択中のステージ以外は表示する
				stage1Logo->Draw();//STAGE1ロゴ
			}
			if (selectStageNum != 2.0f)
			{//選択中のステージ以外は表示する
				stage2Logo->Draw();//STAGE2ロゴ
			}
			if (selectStageNum != 3.0f)
			{//選択中のステージ以外は表示する
				stage3Logo->Draw();//STAGE3ロゴ
			}
		}
		else
		{//表示中
			stage1Logo->Draw();//STAGE1ロゴ
			stage2Logo->Draw();//STAGE2ロゴ
			stage3Logo->Draw();//STAGE3ロゴ
		}
	}
	else if (scene == Start || scene == Game || scene == Clear || scene == Gameover)
	{//ゲーム
		moveAD->Draw();//操作説明
		startLogo->Draw();//STARTロゴ
		timeGaugeSprite->Draw();//進行ゲージ
		clockHandsSprite->Draw();//進行度
		stageClearLogo->Draw();//STAGE CLEARロゴ
		gameoverLogo->Draw();//GAME OVERロゴ
	}
	else if (scene == ClearResult)
	{//クリアリザルト
		stageClearLogo->Draw();//STAGE CLEARロゴ

		if (logoHid)
		{//非表示中
			if (resultSelect)
			{//選択中の項目以外は表示する
				nextLogo->Draw();//NEXTロゴ
			}
			else
			{//選択中の項目以外は表示する
				returnLogo->Draw();//TITLEロゴ
			}
		}
		else
		{//表示中
			nextLogo->Draw();//NEXTロゴ
			returnLogo->Draw();//TITLEロゴ
		}
	}
	else if (scene == GameoverResult)
	{//ゲームオーバーリザルト
		gameoverLogo->Draw();//GAME OVERロゴ

		if (logoHid)
		{//非表示中
			if (resultSelect)
			{//選択中の項目以外は表示する
				retryLogo->Draw();//RETRYロゴ
			}
			else
			{//選択中の項目以外は表示する
				returnLogo->Draw();//TITLEロゴ
			}
		}
		else
		{//表示中
			retryLogo->Draw();//RETRYロゴ
			returnLogo->Draw();//TITLEロゴ
		}
	}
	blackBox->Draw();//シーン遷移用黒背景

	//デバッグテキストの描画
	debugText->DrawAll(cmdList);

	//スプライト描画後処理
	Sprite::PostDraw();
}

void GameScene::logoFlash()
{
	//ロゴ点滅処理
	logoFlashCount += 1.0f;//カウント増加
	if (logoFlashCount >= logoFlashInterval)
	{//カウントが一定になったら表示or非表示切り替え
		logoFlashCount = 0.0f;//カウントリセット
		if (logoHid) { logoHid = false; }//非表示中なら表示に切り替え
		else if (!logoHid) { logoHid = true; }//表示中なら非表示に切り替え
	}
}

void GameScene::sceneChange()
{
	sceneChangeCount += 1.0f;
	boxPosition.x += 28.4f;
	boxPosition.y += 16.0f;
	blackBox->SetPosition(boxPosition);

	if (sceneChangeCount >= 90.0f)
	{
		isSceneChange = false;
		sceneChangeCount = 0.0f;
		boxPosition.x = -1280.0f;
		boxPosition.y = -720.0f;
		blackBox->SetPosition(boxPosition);//シーン遷移座標
	}
}

void GameScene::CreateParticles()
{
	for (int i = 0; i < 10; i++)
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
	}
}