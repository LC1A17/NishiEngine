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
	safe_delete(titleBack);//タイトル、リザルトの背景
	safe_delete(gameTitleLogo);//ゲームタイトルのロゴ
	safe_delete(spaceLogo);//PRESS SPACEのロゴ
	safe_delete(stageSelectLogo);//STAGE SELECTのロゴ
	safe_delete(aLogo);//Aのロゴ
	safe_delete(dLogo);//Dのロゴ
	safe_delete(leftArrowLogo);//←のロゴ
	safe_delete(rightArrowLogo);//→のロゴ
	safe_delete(stage1Logo);//STAGE1のロゴ
	safe_delete(stage2Logo);//STAGE2のロゴ
	safe_delete(stage3Logo);//STAGE3のロゴ
	safe_delete(startLogo);//STARTのロゴ
	safe_delete(distanceUI);//進行度ゲージ本体
	safe_delete(distanceBarUI);//進行度ゲージの赤バー
	safe_delete(stageClearLogo);//STAGE CLEARのロゴ
	safe_delete(gameoverLogo);//GAME OVERのロゴ
	safe_delete(nextLogo);//NEXTのロゴ
	safe_delete(retryLogo);//RETRYのロゴ
	safe_delete(titleLogo);//TITLEのロゴ
	safe_delete(blackBack);//シーン遷移用黒背景
	safe_delete(moveAD);//操作説明のUI
	safe_delete(distanceStartUI);//進行度ゲージのSTARTのロゴ
	safe_delete(distanceGoalUI);//進行度ゲージのGOALのロゴ
	safe_delete(gameBack);//ゲーム中の背景
	safe_delete(dangerUI);//警告演出の！マークのUI
	safe_delete(scoreBoard);//スコアボード
	safe_delete(resultScoreBoard);//リザルトのスコアボード
	safe_delete(scoreGetSprite);//スコア獲得スプライト
	safe_delete(armorGetSprite);//バリア獲得スプライト

	for (int i = 0; i < 5; i++)
	{
		safe_delete(scoreNumber0[i]);//スコア表示用の0
		safe_delete(scoreNumber1[i]);//スコア表示用の1
		safe_delete(scoreNumber2[i]);//スコア表示用の2
		safe_delete(scoreNumber3[i]);//スコア表示用の3
		safe_delete(scoreNumber4[i]);//スコア表示用の4
		safe_delete(scoreNumber5[i]);//スコア表示用の5
		safe_delete(scoreNumber6[i]);//スコア表示用の6
		safe_delete(scoreNumber7[i]);//スコア表示用の7
		safe_delete(scoreNumber8[i]);//スコア表示用の8
		safe_delete(scoreNumber9[i]);//スコア表示用の9

		safe_delete(resultScoreNumber0[i]);//リザルトのスコア表示用の0
		safe_delete(resultScoreNumber1[i]);//リザルトのスコア表示用の1
		safe_delete(resultScoreNumber2[i]);//リザルトのスコア表示用の2
		safe_delete(resultScoreNumber3[i]);//リザルトのスコア表示用の3
		safe_delete(resultScoreNumber4[i]);//リザルトのスコア表示用の4
		safe_delete(resultScoreNumber5[i]);//リザルトのスコア表示用の5
		safe_delete(resultScoreNumber6[i]);//リザルトのスコア表示用の6
		safe_delete(resultScoreNumber7[i]);//リザルトのスコア表示用の7
		safe_delete(resultScoreNumber8[i]);//リザルトのスコア表示用の8
		safe_delete(resultScoreNumber9[i]);//リザルトのスコア表示用の9
	}

	//モデル関連
	safe_delete(skydomeModel);//天球
	safe_delete(railModel);//レール
	safe_delete(playerModel);//プレイヤー
	safe_delete(block1Model);//障害物1
	safe_delete(block2Model);//障害物2
	safe_delete(block3Model);//障害物3
	safe_delete(item1Model);//アイテム1
	safe_delete(item2Model);//アイテム2
	safe_delete(item3Model);//アイテム3
	safe_delete(playerBarrierPartsModel);//プレイヤーバリアパーツ
	safe_delete(playerLandingPartsModel);//プレイヤー着地衝撃波パーツ
	safe_delete(waterModel);//水面

	//オブジェクト関連
	safe_delete(skydome);//天球
	safe_delete(player);//プレイヤー
	safe_delete(playerBarrierParts);//プレイヤーバリアパーツ
	safe_delete(playerLandingParts);//プレイヤー着地衝撃波パーツ
	safe_delete(waterPlane);//水面

	for (int i = 0; i < 5; i++)
	{
		safe_delete(rail[i]);//レール
	}
	for (int i = 0; i < 100; i++)
	{
		safe_delete(block1[i]);//障害物1
		safe_delete(block2[i]);//障害物2
		safe_delete(block3[i]);//障害物3
		safe_delete(item1[i]);//アイテム1
		safe_delete(item2[i]);//アイテム2
		safe_delete(item3[i]);//アイテム3
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

	//変数の初期化
	InitializeVariable();//各種変数の初期化処理

	//スプライト初期化
	CreateSprite();//スプライト読み込み、生成処理

	//デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	SettingSprite();//スプライトの初期座標調整処理

	//パーティクル初期化
	InitializeParticleManager();

	//ライト生成
	lightGroup = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup);
	WaterObject::SetLightGroup(lightGroup);

	//3Dオブジェクト初期化
	CreateObject3d();//3Dオブジェクト読み込み、生成処理
	SettingObject3d();//3Dオブジェクトの初期座標、回転、スケール調整処理
	
	//カメラ注視点をセット
	camera->SetTarget(cameraPosition);
	camera->SetDistance(6.0f);

	firstInitialize = true;//初期化完了
}

void GameScene::Update()
{
	LogoFlash();//ロゴ点滅処理
	ScoreCharge();//スコア加算処理
	CalculationScoreBoard();//スコアボードの表示用計算
	ControlParticles();//各種パーティクル生成処理
	ItemGetLogo();//スコア獲得の演出

	//シーン遷移処理
	if (sceneChange) { SceneChange(nextScene); }
	//オブジェクトの移動、回転処理
	if (scene == StartLogo || scene == Game) { ObjectMoveAndRotation(); }

	//タイトルシーン
	if (scene == Title)
	{
		//SPACEキーでステージセレクトシーンへ移行
		if (input->TriggerKey(DIK_SPACE) && !sceneChange)
		{//シーン遷移が始まったら入力受付を停止
			SceneChangeStart(StageSelect);//シーン遷移開始前処理
		}
	}
	//ステージセレクトシーン
	else if (scene == StageSelect)
	{
		//シーン遷移終了まで入力受付を停止
		if (!sceneChange)
		{
			//SPACEキーで選択したステージシーンへ移行
			if (input->TriggerKey(DIK_SPACE))
			{
				SceneChangeStart(StartLogo);//シーン遷移開始前処理
			}
			//Aでステージ-1
			if (input->TriggerKey(DIK_A) && selectStageNumber > 1.0f)
			{//一番左のステージにいない場合、左のステージへ移動
				ChoiceMove(-1.0f, false);//ステージ-1
			}
			//Dでステージ+1
			if (input->TriggerKey(DIK_D) && selectStageNumber < maxStageNumber)
			{//一番右のステージにいない場合、右のステージへ移動
				ChoiceMove(1.0f, false);//ステージ+1
			}
		}
	}
	//スタート演出シーン
	else if (scene == StartLogo) { LogoUpAndDown(Game); }//ロゴ上下移動処理
	//ゲームシーン
	else if (scene == Game)
	{
		//プレイヤー左移動
		if (input->TriggerKey(DIK_A) && !sideJump) { PlayerMoveStart(true); }
		//プレイヤー右移動
		if (input->TriggerKey(DIK_D) && !sideJump) { PlayerMoveStart(false); }
		PlayerMove();//プレイヤー移動中の処理
		PlayerCollision();//プレイヤーとオブジェクトの当たり判定
		StageManager();//ステージ生成処理

		//ゴールしたらリザルトシーンに移行
		if (gameTime >= gameTimeLimit) { scene = ClearLogo; }
	}
	//クリア演出シーン
	else if (scene == ClearLogo) { LogoUpAndDown(ClearResult); }//ロゴ上下移動処理}
	//ゲームオーバー演出シーン
	else if (scene == GameoverLogo) { LogoUpAndDown(GameoverResult); }//ロゴ上下移動処理
	//クリアリザルトシーンorゲームオーバーリザルトシーン
	else if (scene == ClearResult || scene == GameoverResult)
	{
		//SPACEキーでカーソルを合わせた方のシーンへ移行
		if (input->TriggerKey(DIK_SPACE) && !sceneChange)
		{
			stageChange = false;
			//左の選択肢
			if (resultSelect == 0.0f)
			{
				SceneChangeStart(StartLogo);//シーン遷移開始処理
			}
			//右の選択肢
			else if (resultSelect == 1.0f)
			{
				SceneChangeStart(Title);//シーン遷移開始処理
			}
		}
		//Aで左の選択肢
		if (input->TriggerKey(DIK_A) && resultSelect > 0.0f && !sceneChange)
		{//一番左の選択肢でない場合、左の選択肢へ移動
			ChoiceMove(-1.0f, true);//選択肢-1
		}
		//Dで右の選択肢へ
		if (input->TriggerKey(DIK_D) && resultSelect < maxResultSelectNumber && !sceneChange)
		{//一番右の選択肢でない場合、右の選択肢へ移動
			ChoiceMove(1.0f, true);//選択肢+1
		}
	}

	lightGroup->Update();//ライト更新
	camera->Update();//カメラ更新
	skydome->Update();//天球更新
	waterPlane->Update();//水面更新
	player->Update();//プレイヤー更新
	playerBarrierParts->Update();//プレイヤーバリアパーツ更新
	playerMoveParticle->Update();//プレイヤー移動パーティクル更新
	playerHitParticle->Update();//プレイヤー被弾パーティクル更新
	playerLandingParticle->Update();//プレイヤー着地パーティクル更新
	laserParticle->Update();//レーザーパーティクル更新
	dangerParticle->Update();//警告演出パーティクル更新
	itemGetParticle->Update();//アイテム獲得パーティクル更新
	playerPowerLandingParticle->Update();//強化プレイヤー着地パーティクル更新
	blockBreakParticle->Update();//障害物破壊パーティクル更新

	for (int i = 0; i < 5; i++)
	{
		rail[i]->Update();//レール
	}
	//障害物、アイテムなど
	for (int i = 0; i < 100; i++)
	{
		block1[i]->Update();
		block2[i]->Update();
		block3[i]->Update();
		item1[i]->Update();
		item2[i]->Update();
		item3[i]->Update();
	}
}

void GameScene::Draw()
{
	//コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	//背景スプライト描画
	Sprite::PreDraw(cmdList);//背景スプライト描画前処理

	DrawSprite();//スプライトの描画処理

	Sprite::PostDraw();//スプライト描画後処理
	dxCommon->ClearDepthBuffer();//深度バッファクリア

	//水オブジェクトの描画
	WaterObject::PreDraw(cmdList);//水オブジェクト描画前処理

	//水面
	if (scene == StartLogo || scene == Game || scene == ClearLogo || scene == GameoverLogo) { waterPlane->Draw(); }
	
	WaterObject::PostDraw();//水オブジェクト描画後処理

	//3Dオブジェクトの描画
	Object3d::PreDraw(cmdList);//オブジェクト描画前処理

	DrawObject3D();//3Dオブジェクトの描画処理

	Object3d::PostDraw();//オブジェクト描画後処理

	//パーティクルの描画
	playerMoveParticle->Draw(cmdList);
	playerHitParticle->Draw(cmdList);
	playerLandingParticle->Draw(cmdList);
	laserParticle->Draw(cmdList);

	//前景スプライト描画
	//前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	DrawForegroundSprite();//前景スプライトの描画処理

	//デバッグテキストの描画
	debugText->DrawAll(cmdList);

	//スプライト描画後処理
	Sprite::PostDraw();
}

void GameScene::InitializeVariable()
{
	//初回起動時のみ実行
	if (!firstInitialize)
	{
		for (int i = 0; i < 5; i++)
		{
			laserCollisionLimit[i] = 70.0f;//レーザーの判定の出現時間
			railPosition[i] = { -8.0f + (4.0f * i), 2.0f, 0.0f };//レールの座標設定
			railScale[i] = { 0.1f, 0.1f, 10000.0f };//レールのスケール設定
		}
	}

	preScore = 0;//加算前のスコア
	postScore = 0;//加算後のスコア
	scoreBoard0 = 0;//スコアボード[0]0000
	scoreBoard1 = 0;//スコアボード0[0]000
	scoreBoard2 = 0;//スコアボード00[0]00
	scoreBoard3 = 0;//スコアボード000[0]0
	scoreBoard4 = 0;//スコアボード0000[0]

	gameTime = 0.0f;//ゲーム開始からの経過時間

	//ゲーム終了時間
	if (selectStageNumber == 1.0f) { gameTimeLimit = 900.0f; }
	else if (selectStageNumber == 2.0f) { gameTimeLimit = 950.0f; }
	else { gameTimeLimit = 1000.0f; }

	deadLinePosition = -10.0f;//オブジェクトが自然消滅するライン

	sideJump = false;//移動中かどうか
	sideJumpDirection = false;//移動の向き(false:左 true:右)
	sideJumpTime = 0.0f;//移動時間
	sideJumpCount = 0.0f;//移動量
	playerRailPosition = 2.0f;//プレイヤーのいるレール(一番左から0)
	equipItem = 0.0f;//装着中のパーツの種類

	resultSelect = 0.0f;//リザルトの選択肢をリセット

	dangerUIArive = false;//警告演出が出現しているかの判定
	dangerUICount = 0.0f;//警告演出が出現してからの経過時間

	isPlayerHitParticles = false;//プレイヤー被弾エフェクト生成中かの判定
	isPlayerHitParticlesCount = 0.0f;//プレイヤー被弾エフェクト生成開始からの経過時間

	isPlayerLandingParticles = false;//プレイヤー着地エフェクト生成中かの判定
	isPlayerLandingParticlesCount = 0.0f;//プレイヤー着地エフェクト生成開始からの経過時間

	isLaserParticles = false;//レーザーパーティクル生成中かの判定
	isLaserParticlesCount = 0.0f;//レーザーエフェクト生成開始からの経過時間

	isDangerParticles = false;//警告演出パーティクル生成中かの判定
	isDangerParticlesCount = 0.0f;//警告演出エフェクト生成開始からの経過時間

	isItemGetParticles = false;//アイテム獲得パーティクル生成中かの判定
	isItemGetParticlesCount = 0.0f;//アイテム獲得エフェクト生成開始からの経過時間

	isPlayerPowerLandingParticles = false;//強化プレイヤー着地エフェクト生成中かの判定
	isPlayerPowerLandingParticlesCount = 0.0f;//強化プレイヤー着地エフェクト生成開始からの経過時間

	isBlockBreakParticles = false;//障害物破壊エフェクト生成中かの判定
	isBlockBreakParticlesCount = 0.0f;//障害物破壊エフェクト生成開始からの経過時間

	distanceBarPosition = { 138.0f, 42.0f };//タイムバーの座標
	stageClearLogoPosition = { 40.0f, -150.0f };//STAGE CLEARのロゴの座標を変更
	gameoverLogoPosition = { 153.0f, -150.0f };//GAME OVERのロゴの座標を変更
	scoreGetSpritePosition = { 1000.0f, 1000.0f };//スコア獲得スプライトの座標
	armorGetSpritePosition = { 1000.0f, 1000.0f };//バリア獲得スプライトの座標

	cameraPosition = { 0.0f, 5.5f, 0.0f };//カメラの座標
	skydomePosition = initializeCoordinate;//天球の座標
	playerPosition = { 0.0f, 2.0f, 3.0f };//プレイヤーの座標
	playerRotation = initializeCoordinate;//プレイヤーの回転
	playerBarrierPartsPosition = offScreenPosition;//プレイヤーバリアパーツの座標
	waterPosition = initializeCoordinate;//水面の座標

	for (int i = 0; i < 5; i++)
	{
		laserCollisionCount[i] = 0.0f;//レーザーの判定が出現してからの経過時間
		laserCollision[i] = false;//レーザーが出現しているかの判定
		laserStartPosition[i] = offScreenPosition;//レーザー開始座標
		laserLengthPosition[i] = offScreenPosition;//レーザーの長さ測定用座標
	}
	for (int i = 0; i < 100; i++)
	{
		block1Arive[i] = false;//障害物1が生きているかの判定
		block2Arive[i] = false;//障害物2が生きているかの判定
		block3Arive[i] = false;//障害物3が生きているかの判定
		item1Arive[i] = false;//アイテム1が生きているかの判定
		item2Arive[i] = false;//アイテム2が生きているかの判定
		item3Arive[i] = false;//アイテム3が生きているかの判定

		block1Position[i] = offScreenPosition;//障害物1の座標
		block1Rotation[i] = initializeCoordinate;//障害物1の回転
		block1Scale[i] = initializeCoordinate;//障害物1のスケール
		block2Position[i] = offScreenPosition;//障害物2の座標
		block2Rotation[i] = initializeCoordinate;//障害物2の回転
		block2Scale[i] = initializeCoordinate;//障害物2のスケール
		block3Position[i] = offScreenPosition;//障害物3の座標
		block3Rotation[i] = initializeCoordinate;//障害物3の回転
		block3Scale[i] = initializeCoordinate;//障害物3のスケール

		item1Position[i] = offScreenPosition;//アイテム1の座標
		item1Rotation[i] = initializeCoordinate;//アイテム1の回転
		item1Scale[i] = initializeCoordinate;//アイテム1のスケール
		item2Position[i] = offScreenPosition;//アイテム2の座標
		item2Rotation[i] = initializeCoordinate;//アイテム2の回転
		item2Scale[i] = initializeCoordinate;//アイテム2のスケール
		item3Position[i] = offScreenPosition;//アイテム3の座標
		item3Rotation[i] = initializeCoordinate;//アイテム3の回転
		item3Scale[i] = initializeCoordinate;//アイテム3のスケール
	}
}

void GameScene::CreateSprite()
{
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/font/debugfont.png"))
	{//デバッグテキスト用テクスチャ読み込み
		assert(0);
		return;
	}

	//水のテクスチャ
	Sprite::LoadTexture(0, L"Resources/sprite/waterDown.jpg");

	if (!Sprite::LoadTexture(100, L"Resources/sprite/back.jpg"))
	{//タイトル、リザルトの背景
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(101, L"Resources/sprite/title_logo.png"))
	{//ゲームタイトルのロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(102, L"Resources/sprite/title_explanation.png"))
	{//PRESS SPACEのロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(103, L"Resources/sprite/stage_select.png"))
	{//STAGE SELECTのロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(104, L"Resources/sprite/A.png"))
	{//Aのロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(105, L"Resources/sprite/D.png"))
	{//Dのロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(106, L"Resources/sprite/left_arrow.png"))
	{//←のロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(107, L"Resources/sprite/right_arrow.png"))
	{//→のロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(108, L"Resources/sprite/stage1.png"))
	{//STAGE1のロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(109, L"Resources/sprite/stage2.png"))
	{//STAGE2のロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(110, L"Resources/sprite/stage3.png"))
	{//STAGE3のロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(111, L"Resources/sprite/start.png"))
	{//STARTのロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(112, L"Resources/sprite/distance.png"))
	{//進行度ゲージ本体
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(113, L"Resources/sprite/distance_bar.png"))
	{//進行度ゲージの赤バー
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(114, L"Resources/sprite/stage_clear.png"))
	{//STAGE CLEARのロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(115, L"Resources/sprite/game_over.png"))
	{//GAME OVERのロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(116, L"Resources/sprite/next.png"))
	{//NEXTのロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(117, L"Resources/sprite/retry.png"))
	{//RETRYのロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(118, L"Resources/sprite/title.png"))
	{//TITLEのロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(119, L"Resources/sprite/black.png"))
	{//シーン遷移用のスプライト
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(120, L"Resources/sprite/move_a_d.png"))
	{//操作説明のUI
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(121, L"Resources/sprite/distance_start.png"))
	{//進行度ゲージのSTARTのロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(122, L"Resources/sprite/distance_goal.png"))
	{//進行度ゲージのGOALのロゴ
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(123, L"Resources/sprite/back.png"))
	{//ゲーム中の背景
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(124, L"Resources/sprite/danger.png"))
	{//警告演出の！マークのUI
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(125, L"Resources/sprite/score.png"))
	{//スコアボードのUI
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(126, L"Resources/sprite/scoreGet.png"))
	{//スコア獲得のスプライト
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(127, L"Resources/sprite/armorGet.png"))
	{//バリア獲得のスプライト
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(200, L"Resources/font/0.png"))
	{//スコア表示用の0
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(201, L"Resources/font/1.png"))
	{//スコア表示用の1
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(202, L"Resources/font/2.png"))
	{//スコア表示用の2
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(203, L"Resources/font/3.png"))
	{//スコア表示用の3
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(204, L"Resources/font/4.png"))
	{//スコア表示用の4
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(205, L"Resources/font/5.png"))
	{//スコア表示用の5
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(206, L"Resources/font/6.png"))
	{//スコア表示用の6
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(207, L"Resources/font/7.png"))
	{//スコア表示用の7
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(208, L"Resources/font/8.png"))
	{//スコア表示用の8
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(209, L"Resources/font/9.png"))
	{//スコア表示用の9
		assert(0);
		return;
	}

	titleBack = Sprite::Create(100, { 0.0f, 0.0f });//タイトル、リザルトの背景
	gameTitleLogo = Sprite::Create(101, { 0.0f, 0.0f });//ゲームタイトルのロゴ
	spaceLogo = Sprite::Create(102, { 0.0f, 0.0f });//PRESS SPACEのロゴ
	stageSelectLogo = Sprite::Create(103, { 0.0f, 0.0f });//STAGE SELECTのロゴ
	aLogo = Sprite::Create(104, { 0.0f, 0.0f });//Aのロゴ
	dLogo = Sprite::Create(105, { 0.0f, 0.0f });//Dのロゴ
	leftArrowLogo = Sprite::Create(106, { 0.0f, 0.0f });//←のロゴ
	rightArrowLogo = Sprite::Create(107, { 0.0f, 0.0f });//→のロゴ
	stage1Logo = Sprite::Create(108, { 0.0f, 0.0f });//STAGE1のロゴ
	stage2Logo = Sprite::Create(109, { 0.0f, 0.0f });//STAGE2のロゴ
	stage3Logo = Sprite::Create(110, { 0.0f, 0.0f });//STAGE3のロゴ
	startLogo = Sprite::Create(111, { 0.0f, 0.0f });//STARTのロゴ
	distanceUI = Sprite::Create(112, { 0.0f, 0.0f });//進行度ゲージ本体
	distanceBarUI = Sprite::Create(113, { 0.0f, 0.0f });//進行度ゲージの赤バー
	stageClearLogo = Sprite::Create(114, { 0.0f, 0.0f });//STAGE CLEARのロゴ
	gameoverLogo = Sprite::Create(115, { 0.0f, 0.0f });//GAME OVERのロゴ
	nextLogo = Sprite::Create(116, { 0.0f, 0.0f });//NEXTのロゴ
	retryLogo = Sprite::Create(117, { 0.0f, 0.0f });//RETRYのロゴ
	titleLogo = Sprite::Create(118, { 0.0f, 0.0f });//TITLEのロゴ
	blackBack = Sprite::Create(119, { 0.0f, 0.0f });//シーン遷移用黒背景
	moveAD = Sprite::Create(120, { 0.0f, 0.0f });//操作説明のUI
	distanceStartUI = Sprite::Create(121, { 0.0f, 0.0f });//進行度ゲージのSTARTのロゴ
	distanceGoalUI = Sprite::Create(122, { 0.0f, 0.0f });//進行度ゲージのGOALのロゴ
	gameBack = Sprite::Create(123, { 0.0f, 0.0f });//ゲーム中の背景
	dangerUI = Sprite::Create(124, { 0.0f, 0.0f });//警告演出の！マークのUI
	scoreBoard = Sprite::Create(125, { 0.0f, 0.0f });//スコアボード
	resultScoreBoard = Sprite::Create(125, { 0.0f, 0.0f });//リザルトのスコアボード
	scoreGetSprite = Sprite::Create(126, { 0.0f, 0.0f });//スコア獲得のスプライト
	armorGetSprite = Sprite::Create(127, { 0.0f, 0.0f });//バリア獲得のスプライト

	for (int i = 0; i < 5; i++)
	{
		scoreNumber0[i] = nullptr;//スコア表示用の0
		scoreNumber1[i] = nullptr;//スコア表示用の1
		scoreNumber2[i] = nullptr;//スコア表示用の2
		scoreNumber3[i] = nullptr;//スコア表示用の3
		scoreNumber4[i] = nullptr;//スコア表示用の4
		scoreNumber5[i] = nullptr;//スコア表示用の5
		scoreNumber6[i] = nullptr;//スコア表示用の6
		scoreNumber7[i] = nullptr;//スコア表示用の7
		scoreNumber8[i] = nullptr;//スコア表示用の8
		scoreNumber9[i] = nullptr;//スコア表示用の9
		scoreNumber0[i] = Sprite::Create(200, { 0.0f, 0.0f });//スコア表示用の0
		scoreNumber1[i] = Sprite::Create(201, { 0.0f, 0.0f });//スコア表示用の1
		scoreNumber2[i] = Sprite::Create(202, { 0.0f, 0.0f });//スコア表示用の2
		scoreNumber3[i] = Sprite::Create(203, { 0.0f, 0.0f });//スコア表示用の3
		scoreNumber4[i] = Sprite::Create(204, { 0.0f, 0.0f });//スコア表示用の4
		scoreNumber5[i] = Sprite::Create(205, { 0.0f, 0.0f });//スコア表示用の5
		scoreNumber6[i] = Sprite::Create(206, { 0.0f, 0.0f });//スコア表示用の6
		scoreNumber7[i] = Sprite::Create(207, { 0.0f, 0.0f });//スコア表示用の7
		scoreNumber8[i] = Sprite::Create(208, { 0.0f, 0.0f });//スコア表示用の8
		scoreNumber9[i] = Sprite::Create(209, { 0.0f, 0.0f });//スコア表示用の9

		resultScoreNumber0[i] = nullptr;//リザルトのスコア表示用の0
		resultScoreNumber1[i] = nullptr;//リザルトのスコア表示用の1
		resultScoreNumber2[i] = nullptr;//リザルトのスコア表示用の2
		resultScoreNumber3[i] = nullptr;//リザルトのスコア表示用の3
		resultScoreNumber4[i] = nullptr;//リザルトのスコア表示用の4
		resultScoreNumber5[i] = nullptr;//リザルトのスコア表示用の5
		resultScoreNumber6[i] = nullptr;//リザルトのスコア表示用の6
		resultScoreNumber7[i] = nullptr;//リザルトのスコア表示用の7
		resultScoreNumber8[i] = nullptr;//リザルトのスコア表示用の8
		resultScoreNumber9[i] = nullptr;//リザルトのスコア表示用の9
		resultScoreNumber0[i] = Sprite::Create(200, { 0.0f, 0.0f });//リザルトのスコア表示用の0
		resultScoreNumber1[i] = Sprite::Create(201, { 0.0f, 0.0f });//リザルトのスコア表示用の1
		resultScoreNumber2[i] = Sprite::Create(202, { 0.0f, 0.0f });//リザルトのスコア表示用の2
		resultScoreNumber3[i] = Sprite::Create(203, { 0.0f, 0.0f });//リザルトのスコア表示用の3
		resultScoreNumber4[i] = Sprite::Create(204, { 0.0f, 0.0f });//リザルトのスコア表示用の4
		resultScoreNumber5[i] = Sprite::Create(205, { 0.0f, 0.0f });//リザルトのスコア表示用の5
		resultScoreNumber6[i] = Sprite::Create(206, { 0.0f, 0.0f });//リザルトのスコア表示用の6
		resultScoreNumber7[i] = Sprite::Create(207, { 0.0f, 0.0f });//リザルトのスコア表示用の7
		resultScoreNumber8[i] = Sprite::Create(208, { 0.0f, 0.0f });//リザルトのスコア表示用の8
		resultScoreNumber9[i] = Sprite::Create(209, { 0.0f, 0.0f });//リザルトのスコア表示用の9
	}
}

void GameScene::SettingSprite()
{
	//初回起動時のみ
	if (!firstInitialize)
	{
		gameTitleLogo->SetPosition({ 143.5f,  70.0f });//ゲームタイトルのロゴ
		spaceLogo->SetPosition({ 328.0f, 480.0f });//PRESS SPACEのロゴ
		stageSelectLogo->SetPosition({ 43.5f,  50.0f });//STAGE SELECTのロゴ
		aLogo->SetPosition({ 505.0f, 580.0f });//Aのロゴ
		dLogo->SetPosition({ 647.0f, 580.0f });//Dのロゴ
		leftArrowLogo->SetPosition({ 405.0f, 580.0f });//←のロゴ
		rightArrowLogo->SetPosition({ 747.0f, 580.0f });//→のロゴ
		stage1Logo->SetPosition({ 220.0f, 330.0f });//STAGE1のロゴ
		stage2Logo->SetPosition({ 520.0f, 330.0f });//STAGE2のロゴ
		stage3Logo->SetPosition({ 820.0f, 330.0f });//STAGE3のロゴ
		distanceUI->SetPosition({ 128.0f,  50.0f });//進行度ゲージ本体
		nextLogo->SetPosition({ 77.5f, 430.0f });//NEXTのロゴ
		retryLogo->SetPosition({ 49.0f, 430.0f });//RETRYのロゴ
		titleLogo->SetPosition({ 705.0f, 430.0f });//TITLEのロゴ
		blackBack->SetSize({ 1536.0f, 864.0f });//シーン遷移用黒背景のサイズ調整
		moveAD->SetPosition({ 0.0f, 634.0f });//操作説明のUI
		distanceStartUI->SetPosition({ 92.0f,  20.0f });//進行度ゲージのSTARTのロゴ
		distanceGoalUI->SetPosition({ 1090.0f,  20.0f });//進行度ゲージのGOALのロゴ
		dangerUI->SetPosition({ 600.0f, 200.0f });//警告演出の！マークのUI
		scoreBoard->SetPosition({ 960.0f, 672.0f });//スコアボード
		resultScoreBoard->SetPosition({ 480.0f, 322.0f });//リザルトのスコアボード
		blackBack->SetPosition(blackBackPosition);//シーン遷移用黒背景

		for (int i = 0; i < 5; i++)
		{
			scoreNumber0[i]->SetPosition({ 1124.0f + (30.0f * i), 672.0f });//スコア表示用の0
			scoreNumber1[i]->SetPosition({ 1124.0f + (30.0f * i), 672.0f });//スコア表示用の1
			scoreNumber2[i]->SetPosition({ 1124.0f + (30.0f * i), 672.0f });//スコア表示用の2
			scoreNumber3[i]->SetPosition({ 1124.0f + (30.0f * i), 672.0f });//スコア表示用の3
			scoreNumber4[i]->SetPosition({ 1124.0f + (30.0f * i), 672.0f });//スコア表示用の4
			scoreNumber5[i]->SetPosition({ 1124.0f + (30.0f * i), 672.0f });//スコア表示用の5
			scoreNumber6[i]->SetPosition({ 1124.0f + (30.0f * i), 672.0f });//スコア表示用の6
			scoreNumber7[i]->SetPosition({ 1124.0f + (30.0f * i), 672.0f });//スコア表示用の7
			scoreNumber8[i]->SetPosition({ 1124.0f + (30.0f * i), 672.0f });//スコア表示用の8
			scoreNumber9[i]->SetPosition({ 1124.0f + (30.0f * i), 672.0f });//スコア表示用の9

			resultScoreNumber0[i]->SetPosition({ 644.0f + (30.0f * i), 322.0f });//リザルトのスコア表示用の0
			resultScoreNumber1[i]->SetPosition({ 644.0f + (30.0f * i), 322.0f });//リザルトのスコア表示用の1
			resultScoreNumber2[i]->SetPosition({ 644.0f + (30.0f * i), 322.0f });//リザルトのスコア表示用の2
			resultScoreNumber3[i]->SetPosition({ 644.0f + (30.0f * i), 322.0f });//リザルトのスコア表示用の3
			resultScoreNumber4[i]->SetPosition({ 644.0f + (30.0f * i), 322.0f });//リザルトのスコア表示用の4
			resultScoreNumber5[i]->SetPosition({ 644.0f + (30.0f * i), 322.0f });//リザルトのスコア表示用の5
			resultScoreNumber6[i]->SetPosition({ 644.0f + (30.0f * i), 322.0f });//リザルトのスコア表示用の6
			resultScoreNumber7[i]->SetPosition({ 644.0f + (30.0f * i), 322.0f });//リザルトのスコア表示用の7
			resultScoreNumber8[i]->SetPosition({ 644.0f + (30.0f * i), 322.0f });//リザルトのスコア表示用の8
			resultScoreNumber9[i]->SetPosition({ 644.0f + (30.0f * i), 322.0f });//リザルトのスコア表示用の9
		}
	}

	//初期化時に毎回調整
	startLogo->SetPosition(startLogoPosition);//STARTのロゴ
	distanceBarUI->SetPosition(distanceBarPosition);//進行度ゲージの赤バー
	stageClearLogo->SetPosition(stageClearLogoPosition);//STAGE CLEARのロゴ
	gameoverLogo->SetPosition(gameoverLogoPosition);//GAME OVERのロゴ
	scoreGetSprite->SetPosition(scoreGetSpritePosition);//スコア獲得のスプライト
	armorGetSprite->SetPosition(armorGetSpritePosition);//バリア獲得のスプライト
}

void GameScene::InitializeParticleManager()
{
	//プレイヤー移動パーティクル
	playerMoveParticle = ParticleManager::GetInstance();
	playerMoveParticle->SetCamera(camera);
	//プレイヤー被弾パーティクル
	playerHitParticle = ParticleManager::GetInstance();
	playerHitParticle->SetCamera(camera);
	//プレイヤー着地パーティクル
	playerLandingParticle = ParticleManager::GetInstance();
	playerLandingParticle->SetCamera(camera);
	//レーザーパーティクル
	laserParticle = ParticleManager::GetInstance();
	laserParticle->SetCamera(camera);
	//警告演出パーティクル
	dangerParticle = ParticleManager::GetInstance();
	dangerParticle->SetCamera(camera);
	//アイテム獲得パーティクル
	itemGetParticle = ParticleManager::GetInstance();
	itemGetParticle->SetCamera(camera);
	//強化プレイヤー着地パーティクル
	playerPowerLandingParticle = ParticleManager::GetInstance();
	playerPowerLandingParticle->SetCamera(camera);
	//障害物破壊パーティクル
	blockBreakParticle = ParticleManager::GetInstance();
	blockBreakParticle->SetCamera(camera);
}

void GameScene::CreateObject3d()
{
	//モデル読み込み
	skydomeModel = Model::CreateFromOBJ("skydome");//天球
	railModel = Model::CreateFromOBJ("enemy3");//レール
	playerModel = Model::CreateFromOBJ("player");//プレイヤー
	block1Model = Model::CreateFromOBJ("enemy3");//障害物1
	block2Model = Model::CreateFromOBJ("enemy3");//障害物2
	block3Model = Model::CreateFromOBJ("enemy3");//障害物3
	item1Model = Model::CreateFromOBJ("enemy2");//アイテム1
	item2Model = Model::CreateFromOBJ("parts");//アイテム2
	item3Model = Model::CreateFromOBJ("parts");//アイテム3
	playerBarrierPartsModel = Model::CreateFromOBJ("parts01");//プレイヤーバリアパーツ
	playerLandingPartsModel = Model::CreateFromOBJ("parts01");//プレイヤー着地衝撃波パーツ
	waterModel = Model::CreateFromOBJ("ground");//水面

	//3Dオブジェクト生成
	skydome = Object3d::Create(skydomeModel);//天球
	player = Object3d::Create(playerModel);//プレイヤー
	playerBarrierParts = Object3d::Create(playerBarrierPartsModel);//プレイヤーバリアパーツ
	playerLandingParts = Object3d::Create(playerLandingPartsModel);//プレイヤー着地衝撃波パーツ
	waterPlane = WaterObject::Create(waterModel);//水面

	for (int i = 0; i < 5; i++)
	{
		rail[i] = Object3d::Create(railModel);//レール
	}

	for (int i = 0; i < 100; i++)
	{
		block1[i] = Object3d::Create(block1Model);//障害物1
		block2[i] = Object3d::Create(block2Model);//障害物2
		block3[i] = Object3d::Create(block3Model);//障害物3
		item1[i] = Object3d::Create(item1Model);//アイテム1
		item2[i] = Object3d::Create(item2Model);//アイテム2
		item3[i] = Object3d::Create(item3Model);//アイテム3
	}
}

void GameScene::SettingObject3d()
{
	//初回起動時のみ
	if (!firstInitialize)
	{
		skydome->SetScale(skydomeScale);//天球のスケール
		player->SetScale(playerScale);//プレイヤーのスケール
		waterPlane->SetScale(waterScale);//水面のスケール

		for (int i = 0; i < 5; i++)
		{
			rail[i]->SetPosition(railPosition[i]);//レールの座標
			rail[i]->SetScale(railScale[i]);//レールのスケール
		}
	}
	//初期化時に毎回調整
	skydome->SetPosition(initializeCoordinate);//天球の座標
	skydome->SetRotation(initializeCoordinate);//天球の回転
	player->SetPosition(playerPosition);//プレイヤーの座標
	player->SetRotation(initializeCoordinate);//プレイヤーの回転
	playerBarrierParts->SetPosition(offScreenPosition);//プレイヤーバリアパーツの座標
	playerBarrierParts->SetRotation(initializeCoordinate);//プレイヤーバリアパーツの回転
	playerBarrierParts->SetScale(initializeCoordinate);//プレイヤーバリアパーツのスケール
	playerLandingParts->SetPosition(offScreenPosition);//プレイヤー着地衝撃波パーツの座標
	playerLandingParts->SetRotation(initializeCoordinate);//プレイヤー着地衝撃波パーツの回転
	playerLandingParts->SetScale(initializeCoordinate);//プレイヤー着地衝撃波パーツのスケール
	waterPlane->SetPosition(initializeCoordinate);//水面の座標

	for (int i = 0; i < 100; i++)
	{
		block1[i]->SetPosition(block1Position[i]);//障害物1の座標
		block1[i]->SetRotation(block1Rotation[i]);//障害物1の回転
		block1[i]->SetScale(block1Scale[i]);//障害物1のスケール
		block2[i]->SetPosition(block2Position[i]);//障害物2の座標
		block2[i]->SetRotation(block2Rotation[i]);//障害物2の回転
		block2[i]->SetScale(block2Scale[i]);//障害物2のスケール
		block3[i]->SetPosition(block3Position[i]);//障害物3の座標
		block3[i]->SetRotation(block3Rotation[i]);//障害物3の回転
		block3[i]->SetScale(block3Scale[i]);//障害物3のスケール

		item1[i]->SetPosition(item1Position[i]);//アイテム1の座標
		item1[i]->SetRotation(item1Rotation[i]);//アイテム1の回転
		item1[i]->SetScale(item1Scale[i]);//アイテム1のスケール
		item2[i]->SetPosition(item2Position[i]);//アイテム2の座標
		item2[i]->SetRotation(item2Rotation[i]);//アイテム2の回転
		item2[i]->SetScale(item2Scale[i]);//アイテム2のスケール
		item3[i]->SetPosition(item3Position[i]);//アイテム2の座標
		item3[i]->SetRotation(item3Rotation[i]);//アイテム2の回転
		item3[i]->SetScale(item3Scale[i]);//アイテム2のスケール
	}
}

void GameScene::DrawSprite()
{
	if (scene == StartLogo || scene == Game || scene == ClearLogo || scene == GameoverLogo)
	{
		gameBack->Draw();//ゲーム中の背景
	}
	else
	{
		titleBack->Draw();//タイトル、リザルトの背景
	}
}

void GameScene::DrawObject3D()
{
	if (scene == StartLogo || scene == Game || scene == ClearLogo || scene == GameoverLogo)
	{
		skydome->Draw();//天球

		if (scene != GameoverLogo)
		{
			player->Draw();//プレイヤー
		}
		if (equipItem == 1.0f)
		{
			playerBarrierParts->Draw();//プレイヤーバリアパーツ
		}
		for (int i = 0; i < 5; i++)
		{
			rail[i]->Draw();//レール
		}
		//障害物、アイテム
		for (int i = 0; i < 100; i++)
		{
			if (block1Arive[i]) { block1[i]->Draw(); }
			if (block2Arive[i]) { block2[i]->Draw(); }
			if (block3Arive[i]) { block3[i]->Draw(); }
			if (item1Arive[i]) { item1[i]->Draw(); }
			if (item2Arive[i]) { item2[i]->Draw(); }
			if (item3Arive[i]) { item3[i]->Draw(); }
		}
	}
}

void GameScene::DrawForegroundSprite()
{
	//タイトルシーン
	if (scene == Title)
	{
		gameTitleLogo->Draw();//ゲームタイトルのロゴ
		if (logoHide) { spaceLogo->Draw(); }//PRESS SPACEのロゴ
	}
	//ステージセレクトシーン
	else if (scene == StageSelect)
	{
		stageSelectLogo->Draw();//STAGE SELECTのロゴ
		aLogo->Draw();//Aのロゴ
		dLogo->Draw();//Dのロゴ
		leftArrowLogo->Draw();//←のロゴ
		rightArrowLogo->Draw();//→のロゴ

		//非表示中
		if (!logoHide)
		{
			//選択中のステージ以外は表示する
			if (selectStageNumber != 1.0f)
			{
				stage1Logo->Draw();//STAGE1のロゴ
			}
			if (selectStageNumber != 2.0f)
			{
				stage2Logo->Draw();//STAGE2のロゴ
			}
			if (selectStageNumber != 3.0f)
			{
				stage3Logo->Draw();//STAGE3のロゴ
			}
		}
		//表示中
		else
		{
			stage1Logo->Draw();//STAGE1のロゴ
			stage2Logo->Draw();//STAGE2のロゴ
			stage3Logo->Draw();//STAGE3のロゴ
		}
	}
	//ゲームシーン
	else if (scene == StartLogo || scene == Game || scene == ClearLogo || scene == GameoverLogo)
	{
		moveAD->Draw();//操作説明のUI
		startLogo->Draw();//STARTのロゴ
		distanceUI->Draw();//進行度ゲージ本体
		distanceBarUI->Draw();//進行度ゲージの赤バー
		distanceStartUI->Draw();//進行度ゲージのSTARTのロゴ
		distanceGoalUI->Draw();//進行度ゲージのGOALのロゴ

		//警告演出の！マークのUI
		if (dangerUIArive)
		{
			if (logoHide) { dangerUI->Draw(); }
		}

		scoreGetSprite->Draw();//スコア獲得のスプライト
		armorGetSprite->Draw();//バリア獲得のスプライト
		stageClearLogo->Draw();//STAGE CLEARのロゴ
		gameoverLogo->Draw();//GAME OVERのロゴ
		scoreBoard->Draw();//スコアボード
		DrawScoreBoard(false);//スコアボードの数字描画
	}
	//クリアリザルトシーン
	else if (scene == ClearResult)
	{
		stageClearLogo->Draw();//STAGE CLEARのロゴ

		//非表示中
		if (!logoHide)
		{
			//選択中の項目以外は表示する
			if (resultSelect != 0.0f)
			{
				if (selectStageNumber != maxStageNumber)
				{
					nextLogo->Draw();//NEXTのロゴ
				}
				else
				{
					retryLogo->Draw();//RETRYのロゴ
				}
			}
			else
			{
				titleLogo->Draw();//TITLEのロゴ
			}
		}
		//表示中
		else
		{
			if (selectStageNumber != maxStageNumber)
			{
				nextLogo->Draw();//NEXTのロゴ
			}
			else
			{
				retryLogo->Draw();//RETRYのロゴ
			}
			titleLogo->Draw();//TITLEのロゴ
		}
	}
	//ゲームオーバーリザルトシーン
	else if (scene == GameoverResult)
	{
		gameoverLogo->Draw();//GAME OVERのロゴ

		//非表示中
		if (!logoHide)
		{
			//選択中の項目以外は表示する
			if (resultSelect != 0.0f)
			{
				retryLogo->Draw();//RETRYのロゴ
			}
			else
			{
				titleLogo->Draw();//TITLEのロゴ
			}
		}
		//表示中
		else
		{
			retryLogo->Draw();//RETRYのロゴ
			titleLogo->Draw();//TITLEのロゴ
		}
	}
	if (scene == ClearResult || scene == GameoverResult)
	{
		resultScoreBoard->Draw();//スコアボード
		DrawScoreBoard(true);//スコアボードの数字描画
	}
	blackBack->Draw();//シーン遷移用黒背景
}

/*
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
		particleManager->Add(60, pos, vel, acc, 1.0f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 0.0f });
	}
}
*/

void GameScene::CreatePlayerMoveParticles()
{
	for (int i = 0; i < 3; i++)
	{
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = playerPosition.x;
		pos.y = playerPosition.y;
		pos.z = playerPosition.z;

		const float rnd_vel = 0.05f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		playerMoveParticle->Add(60, pos, vel, acc, 0.75f, 0.0f, { 0.0f, 0.6f, 1.0f, 1.0f }, { 0.0f, 0.6f, 1.0f, 0.0f });
	}
}

void GameScene::CreatePlayerHitParticles()
{
	for (int i = 0; i < 100; i++)
	{
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = playerHitParticlesPosition.x;
		pos.y = playerHitParticlesPosition.y;
		pos.z = playerHitParticlesPosition.z + 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		playerHitParticle->Add(60, pos, vel, acc, 1.0f, 0.0f, { 0.5f, 0.1f, 0.1f, 1.0f }, { 0.5f, 0.1f, 0.1f, 0.0f });
	}
}

void GameScene::CreatePlayerLandingParticles()
{
	for (int i = 0; i < 50; i++)
	{
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = playerLandingParticlesPosition.x;
		pos.y = playerLandingParticlesPosition.y - 0.5f;
		pos.z = playerLandingParticlesPosition.z + 3.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		playerLandingParticle->Add(60, pos, vel, acc, 1.0f, 0.0f, { 0.0f, 0.6f, 0.1f, 1.0f }, { 0.0f, 0.6f, 0.1f, 0.0f });
	}
}

void GameScene::CreateLaserParticles()
{
	for (int i = 0; i < 25; i++)
	{
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = laserParticlesPosition.x;
		pos.y = laserParticlesPosition.y;
		pos.z = laserParticlesPosition.z;

		const float rnd_vel = 0.01f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.01f;
		acc.z = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		laserParticle->Add(400, pos, vel, acc, 2.0f, 0.0f, { 0.1f, 1.0f, 0.7f, 1.0f }, { 0.1f, 1.0f, 0.7f, 0.0f });
	}
}

void GameScene::CreateDangerParticles()
{
	for (int i = 0; i < 25; i++)
	{
		const float rnd_pos = 1500.0f;
		XMFLOAT3 pos{};
		pos.x = dangerParticlesPosition.x;
		pos.y = dangerParticlesPosition.y;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.01f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.00005f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc;

		//追加
		dangerParticle->Add(400, pos, vel, acc, 4.0f, 0.0f, { 0.6f, 0.0f, 0.3f, 0.1f }, { 0.6f, 0.0f, 0.3f, 0.0f });
	}
}

void GameScene::CreateItemGetParticles()
{
	for (int i = 0; i < 50; i++)
	{
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = itemGetParticlesPosition.x;
		pos.y = itemGetParticlesPosition.y;
		pos.z = itemGetParticlesPosition.z + 2.0f;

		const float rnd_vel = 0.2f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.005f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		itemGetParticle->Add(60, pos, vel, acc, 0.4f, 0.0f, { 0.3f, 0.2f, 0.0f, 1.0f }, { 0.3f, 0.2f, 0.0f, 0.0f });
	}
}

void GameScene::CreatePlayerPowerLandingParticles()
{
	for (int i = 0; i < 50; i++)
	{
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = playerLandingParticlesPosition.x;
		pos.y = playerLandingParticlesPosition.y - 0.5f;
		pos.z = playerLandingParticlesPosition.z + 3.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		playerPowerLandingParticle->Add(60, pos, vel, acc, 1.0f, 0.0f, { 1.0f, 0.7f, 0.1f, 0.1f }, { 1.0f, 0.7f, 0.1f, 0.0f });
	}
}

void GameScene::CreateBlockBreakParticles()
{
	for (int i = 0; i < 75; i++)
	{
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = playerHitParticlesPosition.x;
		pos.y = playerHitParticlesPosition.y;
		pos.z = playerHitParticlesPosition.z + 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		playerHitParticle->Add(60, pos, vel, acc, 1.0f, 0.0f, { 0.7f, 1.0f, 0.1f, 1.0f }, { 0.7f, 1.0f, 0.1f, 0.0f });
	}
}

void GameScene::ControlParticles()
{
	//アイテム獲得エフェクト
	if (isItemGetParticles)
	{
		isItemGetParticlesCount += 1.0f;//アイテム獲得エフェクト生成開始からの経過時間増加

		if (isItemGetParticlesCount < isItemGetParticlesLimit)
		{
			CreateItemGetParticles();//アイテム獲得パーティクル生成
		}
		else
		{
			isItemGetParticlesCount = 0.0f;//アイテム獲得エフェクト生成開始からの経過時間をリセット
			isItemGetParticles = false;//アイテム獲得エフェクトの生成を停止
		}
	}
	//障害物破壊エフェクト
	if (isBlockBreakParticles)
	{
		isBlockBreakParticlesCount += 1.0f;//障害物破壊エフェクト生成開始からの経過時間増加

		if (isBlockBreakParticlesCount < isBlockBreakParticlesLimit)
		{
			CreateBlockBreakParticles();//障害物破壊パーティクル生成
		}
		else
		{
			isBlockBreakParticlesCount = 0.0f;//障害物破壊エフェクト生成開始からの経過時間をリセット
			isBlockBreakParticles = false;//障害物破壊エフェクトの生成を停止
		}
	}
	//プレイヤー被弾エフェクト
	if (isPlayerHitParticles)
	{
		isPlayerHitParticlesCount += 1.0f;//プレイヤー被弾エフェクト生成開始からの経過時間増加

		if (isPlayerHitParticlesCount < isPlayerHitParticlesLimit)
		{
			CreatePlayerHitParticles();//プレイヤー被弾パーティクル生成
		}
		else
		{
			isPlayerHitParticlesCount = 0.0f;//プレイヤー被弾エフェクト生成開始からの経過時間をリセット
			isPlayerHitParticles = false;//プレイヤー被弾エフェクトの生成を停止
		}
	}
	//プレイヤー着地パーティクル
	if (isPlayerLandingParticles)
	{
		isPlayerLandingParticlesCount += 1.0f;//プレイヤー着地エフェクト生成開始からの経過時間増加

		if (isPlayerLandingParticlesCount < isPlayerLandingParticlesLimit)
		{
			CreatePlayerLandingParticles();//プレイヤー着地パーティクル生成
		}
		else
		{
			isPlayerLandingParticlesCount = 0.0f;//プレイヤー着地エフェクト生成開始からの経過時間をリセット
			isPlayerLandingParticles = false;//プレイヤー被弾エフェクトの生成を停止
		}
	}
	//強化プレイヤー着地パーティクル
	if (isPlayerPowerLandingParticles)
	{
		isPlayerPowerLandingParticlesCount += 1.0f;//強化プレイヤー着地エフェクト生成開始からの経過時間増加

		if (isPlayerPowerLandingParticlesCount < isPlayerPowerLandingParticlesLimit)
		{
			CreatePlayerPowerLandingParticles();//強化プレイヤー着地パーティクル生成
		}
		else
		{
			isPlayerPowerLandingParticlesCount = 0.0f;//強化プレイヤー着地エフェクト生成開始からの経過時間をリセット
			isPlayerPowerLandingParticles = false;//強化プレイヤー被弾エフェクトの生成を停止
		}
	}
	//レーザー警告演出
	if (dangerUIArive)
	{
		dangerUICount += 1.0f;//レーザー警告演出出現からの経過時間増加

		if (dangerUICount >= dangerUILimit)
		{
			dangerUICount = 0.0f;//レーザー警告演出出現からの経過時間リセット
			dangerUIArive = false;//レーザー警告演出の描画停止
			isLaserParticles = true;//レーザーエフェクトの生成

			for (int i = 0; i < 5; i++)
			{
				if (!laserCollision[i])
				{
					laserCollision[i] = true;//レーザーの判定を生成
					laserStartPosition[i].x = laserParticlesPosition.x;//X座標を設定
					laserStartPosition[i].y = 2.0f;//Y座標を設定
					laserStartPosition[i].z = laserParticlesPosition.z - 60.0f;//Z座標を設定
					laserLengthPosition[i].x = laserParticlesPosition.x;//X座標を設定
					laserLengthPosition[i].y = 2.0f;//Y座標を設定
					laserLengthPosition[i].z = laserParticlesPosition.z - 60.0f;//Z座標を設定
				}
			}
		}
	}
	//警告演出パーティクル
	if (isDangerParticles)
	{
		isDangerParticlesCount += 1.0f;//警告演出エフェクト生成開始からの経過時間増加

		if (isDangerParticlesCount < isDangerParticlesLimit)
		{
			CreateDangerParticles();//警告演出パーティクル生成
		}
		else
		{
			isDangerParticlesCount = 0.0f;//警告演出エフェクト生成開始からの経過時間をリセット
			isDangerParticles = false;//警告演出エフェクトの生成を停止
		}
	}
	//レーザーパーティクル
	if (isLaserParticles)
	{
		isLaserParticlesCount += 1.0f;//レーザーエフェクト生成開始からの経過時間増加

		if (isLaserParticlesCount < isLaserParticlesLimit)
		{
			CreateLaserParticles();//レーザーパーティクル生成
		}
		else
		{
			isLaserParticlesCount = 0.0f;//レーザーエフェクト生成開始からの経過時間をリセット
			isLaserParticles = false;//レーザーエフェクトの生成を停止
		}
	}
}

void GameScene::SceneChangeStart(int selectScene)
{
	nextScene = selectScene;//遷移後のシーンを設定
	sceneChangeCount = 0.0f;//シーン遷移開始からの経過時間リセット
	sceneChange = true;//シーン遷移処理を開始する
}

void GameScene::SceneChange(int selectScene)
{
	sceneChangeCount += 1.0f;//シーン遷移開始からの経過時間カウント
	blackBackPosition.x += 34.0f;//シーン遷移用黒背景X座標変更
	blackBackPosition.y += 20.0f;//シーン遷移用黒背景Y座標変更
	blackBack->SetPosition(blackBackPosition);//シーン遷移用黒背景座標更新

	//黒背景が全画面を覆ったら画面裏を更新
	if (sceneChangeCount == (sceneChangeFinish / 2.0f))
	{
		if (!stageChange)
		{
			//左の選択肢
			if (resultSelect == 0.0f)
			{
				if (scene == ClearResult && selectStageNumber != maxStageNumber)
				{
					selectStageNumber += 1.0f;//現在のステージ+1
				}
				stageChange = true;
			}
			//右の選択肢
			else if (resultSelect == 1.0f)
			{
				selectStageNumber = 1.0f;//ステージの選択肢をリセット
				stageChange = true;
			}
		}

		logoFlashCount = 0.0f;//ロゴ点滅カウントリセット
		logoHide = true;//ロゴの表示を戻す
		scene = selectScene;//選択したシーンへ移行

		//各シーン毎に更新内容を変える
		//タイトル、スタート演出
		if (scene == Title || scene == StartLogo)
		{
			InitializeVariable();//変数の初期化処理
			SettingSprite();//スプライトの初期座標調整処理
			SettingObject3d();//3Dオブジェクトの初期座標調整処理
		}
		//クリア演出、ゲームオーバー演出
		else if (scene == ClearLogo || scene == GameoverLogo)
		{
			logoMoveCount = 0.0f;//ロゴ移動開始からの経過時間リセット
			logoMoveDirection = false;//ロゴ移動の向きをリセット
			logoMove = true;//ロゴ移動を停止
		}
		//クリアリザルト、ゲームオーバーリザルト
		else if (scene == ClearResult || scene == GameoverResult)
		{
			//ロゴを移動
			if (scene == ClearResult)
			{
				stageClearLogoPosition = { 40.0f, 50.0f };//STAGE CLEARのロゴの座標変更
				stageClearLogo->SetPosition(stageClearLogoPosition);//STAGE CLEARのロゴの座標更新
			}
			if (scene == GameoverResult)
			{
				gameoverLogoPosition = { 153.0f, 50.0f };//GAME OVERのロゴの座標変更
				gameoverLogo->SetPosition(gameoverLogoPosition);//GAME OVERロゴの座標更新
			}
		}
	}
	//黒背景の座標を戻して遷移終了
	else if (sceneChangeCount >= sceneChangeFinish)
	{
		sceneChange = false;//シーン遷移終了
		sceneChangeCount = 0.0f;//シーン遷移開始からの経過時間リセット
		blackBackPosition.x = -1536.0f;//シーン遷移用黒背景X座標変更
		blackBackPosition.y = -864.0f;//シーン遷移用黒背景Y座標変更
		blackBack->SetPosition(blackBackPosition);//シーン遷移用黒背景座標更新
	}
}

void GameScene::LogoFlash()
{
	logoFlashCount += 1.0f;//ロゴ点滅からの経過時間カウント

	//カウントが規定値になったらロゴ表示or非表示を切り替え
	if (logoFlashCount >= logoFlashInterval)
	{
		logoFlashCount = 0.0f;//ロゴ点滅からの経過時間リセット
		if (logoHide) { logoHide = false; }//表示中なら非表示に切り替え
		else if (!logoHide) { logoHide = true; }//非表示中なら表示に切り替え
	}
}

void GameScene::ChoiceMove(float increase, bool nowScene)
{
	logoFlashCount = 0.0f;//ロゴ点滅の経過時間リセット
	logoHide = true;//ロゴの表示

	//現在のシーンに合わせて選択肢カーソルを移動
	if (!nowScene) { selectStageNumber += increase; }
	else { resultSelect += increase; }
}

void GameScene::LogoUpAndDown(int selectScene)
{
	//ロゴ移動中
	if (logoMove)
	{
		logoMoveCount += 1.0f;//ロゴ移動開始からの経過時間

		//下移動
		if (!logoMoveDirection)
		{
			//スタート演出時はSTARTのロゴを移動
			if (scene == StartLogo)
			{
				startLogoPosition.y += logoMoveSpeed;//STARTのロゴのY座標変更
			}
			//クリア演出時はSTAGE CLEARのロゴを移動
			else if (scene == ClearLogo)
			{
				stageClearLogoPosition.y += logoMoveSpeed;//STAGE CLEARのロゴのY座標変更
			}
			//ゲームオーバー演出時はGAME OVERのロゴを移動
			else if (scene == GameoverLogo)
			{
				gameoverLogoPosition.y += logoMoveSpeed;//GAME OVERのロゴのY座標変更
			}
			//一定量移動したら一時停止させ、次の移動方向を変更する
			if (logoMoveCount >= logoMoveLimit)
			{
				logoMoveCount = 0.0f;//ロゴ移動開始からの経過時間リセット
				logoMoveDirection = true;//ロゴ移動方向を上に変更
				logoMove = false;//ロゴを停止中に変更
			}
		}
		//上移動
		else
		{
			//スタート演出時はSTARTのロゴを移動
			if (scene == StartLogo)
			{
				startLogoPosition.y -= logoMoveSpeed;//STARTのロゴのY座標変更
			}
			//クリア演出時はSTAGE CLEARのロゴを移動
			else if (scene == ClearLogo)
			{
				stageClearLogoPosition.y -= logoMoveSpeed;//STAGE CLEARのロゴのY座標変更
			}
			//ゲームオーバー演出時はGAME OVERのロゴを移動
			else if (scene == GameoverLogo)
			{
				gameoverLogoPosition.y -= logoMoveSpeed;//GAME OVERのロゴのY座標変更
			}
			//スタート演出時のみ、ここでシーン遷移
			if (logoMoveCount >= logoMoveLimit && StartLogo)
			{
				logoMoveCount = 0.0f;//ロゴ移動開始からの経過時間リセット
				logoMoveDirection = false;//ロゴ移動方向を下に変更
				logoMove = true;//ロゴを移動中に変更
				scene = selectScene;//選択したシーンに移行
			}
		}

		startLogo->SetPosition(startLogoPosition);//STARTのロゴの座標更新
		stageClearLogo->SetPosition(stageClearLogoPosition);//STAGE CLEARのロゴの座標更新
		gameoverLogo->SetPosition(gameoverLogoPosition);//GAME OVERのロゴの座標更新
	}
	//ロゴ停止中
	else
	{
		logoMoveCount += 1.0f;//ロゴ移動開始からの経過時間

		//一定時間停止したら終了
		if (logoMoveCount >= logoStopLimit)
		{
			logoMoveCount = 0.0f;//ロゴ移動開始からの経過時間リセット
			logoMove = true;//ロゴを移動中に変更
			
			//クリア演出時とゲームオーバー演出時はここでシーン遷移
			if (scene == ClearLogo)
			{
				SceneChangeStart(ClearResult);//シーン遷移開始処理
			}
			else if (scene == GameoverLogo)
			{
				SceneChangeStart(GameoverResult);//シーン遷移開始処理
			}
		}
	}
}

void GameScene::ObjectMoveAndRotation()
{
	//天球の回転
	XMFLOAT3 rotation = skydome->GetRotation();//現在の天球回転量を取得
	rotation.y += skydomeRotationSpeed;//天球を回転させる
	skydome->SetRotation(rotation);//天球の回転量更新
	//水の流れ
	XMFLOAT3 waterPosition = waterPlane->GetPosition();//現在の水の座標を取得
	waterPosition.z += waterPositionSpeed;//水を移動させる
	waterPlane->SetPosition(waterPosition);//水の座標更新

	//プレイヤーバリアパーツ装着中はバリアパーツも連動して移動
	if (equipItem == 1.0f)
	{
		//出現時は少しずつ大きくなって出現
		if (playerPartsScale.x < 0.6f)
		{
			playerPartsScale.x += 0.05f;
			playerPartsScale.y += 0.05f;
			playerPartsScale.z += 0.05f;
		}
		playerBarrierPartsPosition.z += autoMoveSpeedZ;
	}

	//回転軸リセット
	if (!sideJump)
	{
		playerRotation.y = 0.0f;//Y軸リセット
		playerRotation.z = 0.0f;//Z軸リセット
	}

	//プレイヤーZ座標移動
	playerPosition.z += autoMoveSpeedZ;
	//プレイヤーX軸回転
	playerRotation.x += autoMoveSpeedZ * 75.0f;
	//カメラZ座標移動
	cameraPosition.z += autoMoveSpeedZ;
	//天球移動
	skydomePosition.z += autoMoveSpeedZ;

	player->SetPosition(playerPosition);//プレイヤーの座標更新
	player->SetRotation(playerRotation);//プレイヤーの回転量更新
	camera->SetTarget(cameraPosition);//カメラの座標更新
	skydome->SetPosition(skydomePosition);//天球の座標更新
	playerBarrierParts->SetPosition(playerBarrierPartsPosition);//プレイヤーバリアパーツの座標更新
	playerBarrierParts->SetRotation(playerRotation);//プレイヤーバリアパーツの回転量更新
	playerBarrierParts->SetScale(playerPartsScale);//プレイヤーバリアパーツのスケール更新

	//レール移動中以外はプレイヤー移動パーティクルを生成
	if (!sideJump) { CreatePlayerMoveParticles(); }

	//スタート演出時は行わない処理
	if (scene == Game)
	{
		//進行度ゲージの更新
		gameTime += 1.0f;//ゲーム開始からの時間カウント
		distanceBarPosition.x += (988.0f / gameTimeLimit);//制限時間から進行度ゲージの座標を計算
		distanceBarUI->SetPosition(distanceBarPosition);//進行度ゲージ座標更新

		deadLinePosition += autoMoveSpeedZ;//オブジェクト消滅ラインをずらす

		//スコアの増加
		if (playerRailPosition == 0.0f || playerRailPosition == 4.0f)
		{
			preScore += 2;//スコア+2
		}
		else
		{
			preScore += 1;//スコア+1
		}

		//レーザーの当たり判定移動
		for (int i = 0; i < 5; i++)
		{
			//生成されているレーザーのみ判定
			if (laserCollision[i])
			{
				laserCollisionCount[i] += 1.0f;//生成からの経過時間増加
				laserStartPosition[i].z -= 0.5f;//レーザーの長さ増加
				laserLengthPosition[i].z -= 2.5f;//レーザーの長さ増加

				//一定時間判定が出たら消滅
				if (laserCollisionCount[i] >= laserCollisionLimit[i])
				{
					laserCollisionCount[i] = 0.0f;//生成からの経過時間リセット
					laserCollision[i] = false;//レーザーの判定消滅
				}
			}
		}

		//障害物、アイテム移動処理
		for (int i = 0; i < 100; i++)
		{
			//障害物1が生きている場合
			if (block1Arive[i])
			{
				block1Position[i].z += autoMoveBlockSpeedZ / 2;//障害物1のZ座標移動

				//出現時は少しずつ大きくなって出現
				if (block1Scale[i].x < 1.0f)
				{
					block1Scale[i].x += 0.0625f;
					block1Scale[i].y += 0.05f;
					block1Scale[i].z += 0.0625f;
				}
				//オブジェクト消滅ラインに触れたら消滅
				if (block1Position[i].z < deadLinePosition)
				{
					block1Position[i] = offScreenPosition;//画面外に退避
					block1Arive[i] = false;//障害物1を削除
					block1Scale[i] = { 0.0f, 0.0f, 0.0f };//障害物1のスケールを0にする
				}
			}
			//障害物2が生きている場合
			if (block2Arive[i])
			{
				block2Position[i].z -= autoMoveBlockSpeedZ / 3.0f;//障害物2のZ座標移動

				//出現時は少しずつ大きくなって出現
				if (block2Scale[i].z < 1.5f)
				{
					block2Scale[i].x += 0.25f;
					block2Scale[i].y += 0.08f;
					block2Scale[i].z += 0.1f;
				}
				//オブジェクト消滅ラインに触れたら消滅
				if (block2Position[i].z < deadLinePosition)
				{
					block2Position[i] = offScreenPosition;//画面外に退避
					block2Arive[i] = false;//障害物2を削除
					block2Scale[i] = { 0.0f, 0.0f, 0.0f };//障害物2のスケールを0にする
				}
			}
			//障害物3が生きている場合
			if (block3Arive[i])
			{
				//出現時は少しずつ大きくなって出現
				if (block3Scale[i].x < 1.5f)
				{
					block3Scale[i].x += 0.1f;
					block3Scale[i].y += 0.08f;
					block3Scale[i].z += 0.1f;
				}
				//オブジェクト消滅ラインに触れたら消滅
				if (block3Position[i].z < deadLinePosition)
				{
					block3Position[i] = offScreenPosition;//画面外に退避
					block3Arive[i] = false;//障害物3を削除
					block3Scale[i] = { 0.0f, 0.0f, 0.0f };//障害物3のスケールを0にする
				}
			}
			//アイテム1が生きている場合
			if (item1Arive[i])
			{
				//出現時は少しずつ大きくなって出現
				if (item1Scale[i].x < 1.0f)
				{
					item1Scale[i].x += 0.0625f;
					item1Scale[i].y += 0.0625f;
					item1Scale[i].z += 0.0625f;
				}
				//オブジェクト消滅ラインに触れたら消滅
				if (item1Position[i].z < deadLinePosition)
				{
					item1Position[i] = offScreenPosition;//画面外に退避
					item1Arive[i] = false;//アイテム1を削除
					item1Scale[i] = { 0.0f, 0.0f, 0.0f };//アイテム1のスケールを0にする
				}
			}
			//アイテム2が生きている場合
			if (item2Arive[i])
			{
				//出現時は少しずつ大きくなって出現
				if (item2Scale[i].x < 1.0f)
				{
					item2Scale[i].x += 0.0625f;
					item2Scale[i].y += 0.0625f;
					item2Scale[i].z += 0.0625f;
				}
				//オブジェクト消滅ラインに触れたら消滅
				if (item2Position[i].z < deadLinePosition)
				{
					item2Position[i] = offScreenPosition;//画面外に退避
					item2Arive[i] = false;//アイテム2を削除
					item2Scale[i] = { 0.0f, 0.0f, 0.0f };//アイテム2のスケールを0にする
				}
			}
			//アイテム3が生きている場合
			if (item3Arive[i])
			{
				//出現時は少しずつ大きくなって出現
				if (item3Scale[i].x < 1.0f)
				{
					item3Scale[i].x += 0.0625f;
					item3Scale[i].y += 0.0625f;
					item3Scale[i].z += 0.0625f;
				}
				//オブジェクト消滅ラインに触れたら消滅
				if (item3Position[i].z < deadLinePosition)
				{
					item3Position[i] = offScreenPosition;//画面外に退避
					item3Arive[i] = false;//アイテム3を削除
					item3Scale[i] = { 0.0f, 0.0f, 0.0f };//アイテム3のスケールを0にする
				}
			}

			block1[i]->SetPosition(block1Position[i]);//障害物1の座標更新
			block2[i]->SetPosition(block2Position[i]);//障害物2の座標更新
			block3[i]->SetPosition(block3Position[i]);//障害物3の座標更新
			block1[i]->SetScale(block1Scale[i]);//障害物1のスケール更新
			block2[i]->SetScale(block2Scale[i]);//障害物2のスケール更新
			block3[i]->SetScale(block3Scale[i]);//障害物3のスケール更新
			item1[i]->SetPosition(item1Position[i]);//アイテム1の座標更新
			item2[i]->SetPosition(item2Position[i]);//アイテム2の座標更新
			item3[i]->SetPosition(item3Position[i]);//アイテム3の座標更新
			item1[i]->SetScale(item1Scale[i]);//アイテム1のスケール更新
			item2[i]->SetScale(item2Scale[i]);//アイテム2のスケール更新
			item3[i]->SetScale(item3Scale[i]);//アイテム3のスケール更新
		}
	}
}

void GameScene::PlayerMoveStart(bool direction)
{
	//左移動
	if (direction)
	{
		//一番左にいないなら移動開始
		if (playerRailPosition > 0.0f)
		{
			playerRailPosition -= 1.0f;//現在いるレールを変更
			sideJumpDirection = false;//左に移動するよう設定
			sideJump = true;//プレイヤー移動開始
		}
	}
	//右移動
	else
	{
		//一番右にいないなら移動開始
		if (playerRailPosition < 4.0f)
		{
			playerRailPosition += 1.0f;//現在いるレールを変更
			sideJumpDirection = true;//右に移動するよう設定
			sideJump = true;//プレイヤー移動開始
		}
	}
	//移動開始時にプレイヤー移動関係の変数を設定、初期化
	if (sideJump)
	{
		oldPlayerPosition = playerPosition;//現在のプレイヤー座標を保存
		sideJumpTime = 0.0f;//プレイヤーが移動開始してからの経過時間をリセット
		sideJumpCount = 0.0f;
	}
}

void GameScene::PlayerMove()
{
	//移動中
	if (sideJump)
	{
		//右に移動
		if (sideJumpDirection)
		{
			sideJumpTime += sideJumpTimeSpeed;//プレイヤーが移動開始してからの経過時間をカウント
			sideJumpCount += sideJumpCountSpeed;
			playerPosition.x += sideJumpCountSpeed;//プレイヤーのX座標移動
			playerPosition.y = oldPlayerPosition.y + (2.45f - gravity * sideJumpTime);//プレイヤーのY座標移動
			playerRotation.y -= sideJumpPlayerRotation;//プレイヤーのY軸回転
			playerRotation.z -= sideJumpPlayerRotation;//プレイヤーのZ軸回転
			cameraPosition.x += sideJumpCountSpeed;//カメラの移動

			//プレイヤーバリアパーツ装着中はバリアパーツも連動して移動、回転
			if (equipItem == 1.0f)
			{
				playerBarrierPartsPosition.x += sideJumpCountSpeed;//バリアパーツのX座標移動
				playerBarrierPartsPosition.y = oldPlayerPosition.y + (2.45f - gravity * sideJumpTime);//バリアパーツのY座標移動
				playerRotation.y -= sideJumpPlayerRotation;//バリアパーツのY軸回転
				playerRotation.z -= sideJumpPlayerRotation;//バリアパーツのZ軸回転
			}
			//移動量が一定値になったら移動終了
			if (sideJumpCount > sideJumpFinishTime)
			{
				//プレイヤーの位置に着地パーティクル生成
				playerLandingParticlesPosition.x = playerPosition.x;
				playerLandingParticlesPosition.y = playerPosition.y;
				playerLandingParticlesPosition.z = playerPosition.z;

				//プレイヤー着地パーティクル生成
				if (equipItem == 0.0f) { isPlayerLandingParticles = true; }
				//強化プレイヤー着地パーティクル生成
				else
				{
					//powerLanding = true;
					isPlayerPowerLandingParticles = true;
				}
				sideJump = false;//レール移動を終了
			}
		}
		//左に移動
		else if (!sideJumpDirection)
		{
			sideJumpTime += sideJumpTimeSpeed;//プレイヤーが移動開始してからの経過時間をカウント
			sideJumpCount -= sideJumpCountSpeed;
			playerPosition.x -= sideJumpCountSpeed;//プレイヤーのX座標移動
			playerPosition.y = oldPlayerPosition.y + (2.45f - gravity * sideJumpTime);//プレイヤーのY座標移動
			playerRotation.y += sideJumpPlayerRotation;//プレイヤーのY軸回転
			playerRotation.z += sideJumpPlayerRotation;//プレイヤーのZ軸回転
			cameraPosition.x -= sideJumpCountSpeed;//カメラの移動

			//プレイヤーバリアパーツ装着中はバリアパーツも連動して移動、回転
			if (equipItem == 1.0f)
			{
				playerBarrierPartsPosition.x -= sideJumpCountSpeed;//バリアパーツのX座標移動
				playerBarrierPartsPosition.y = oldPlayerPosition.y + (2.45f - gravity * sideJumpTime);//バリアパーツのY座標移動
				playerRotation.y += sideJumpPlayerRotation;//バリアパーツのY軸回転
				playerRotation.z += sideJumpPlayerRotation;//バリアパーツのZ軸回転
			}
			//移動量が一定値になったら移動終了
			if (sideJumpCount < -sideJumpFinishTime)
			{
				//プレイヤーの位置に着地パーティクル生成
				playerLandingParticlesPosition.x = playerPosition.x;
				playerLandingParticlesPosition.y = playerPosition.y;
				playerLandingParticlesPosition.z = playerPosition.z;

				//プレイヤー着地パーティクル生成
				if (equipItem == 0.0f) { isPlayerLandingParticles = true; }
				//強化プレイヤー着地パーティクル生成
				else
				{
					//powerLanding = true;
					isPlayerPowerLandingParticles = true;
				}
				sideJump = false;//レール移動を終了
			}
		}
	}
}

void GameScene::ScoreCharge()
{
	//加算前のスコアが1以上かつ加算後のスコアが99999未満なら実行
	if (preScore > 0 && postScore < 99999)
	{
		if (preScore > 9)
		{
			preScore -= 5;//-10
			postScore += 5;//+10
		}
		else
		{
			preScore -= 1;//-1
			postScore += 1;//+1
		}
	}
}

void GameScene::CalculationScoreBoard()
{
	scoreBoard0 = (postScore / 10000) % 10;//[0]0000を算出
	scoreBoard1 = (postScore / 1000) % 10;//0[0]000を算出
	scoreBoard2 = (postScore / 100) % 10;//00[0]00を算出
	scoreBoard3 = (postScore / 10) % 10;//000[0]0を算出
	scoreBoard4 = postScore % 10;//0000[0]を算出
}

void GameScene::StageManager()
{
	//ステージ1
	if (selectStageNumber == 1.0f)
	{
		//ゲーム経過時間に合わせて敵を出現
		if (gameTime == 10.0f)
		{
			SpawnManager(-8.0f, 3.0f);
			SpawnManager(0.0f, 3.0f);
			SpawnManager(8.0f, 3.0f);

			SpawnManager(-4.0f, 12.0f);
			SpawnManager(4.0f, 12.0f);
		}
		else if (gameTime == 70.0f)
		{
			SpawnManager(-4.0f, 3.0f);
			SpawnManager(4.0f, 3.0f);

			SpawnManager(-8.0f, 12.0f);
			SpawnManager(0.0f, 12.0f);
			SpawnManager(8.0f, 12.0f);
		}
		else if (gameTime == 130.0f)
		{
			SpawnManager(-8.0f, 3.0f);
			SpawnManager(0.0f, 3.0f);
			SpawnManager(8.0f, 3.0f);

			SpawnManager(4.0f, 12.0f);

			SpawnManager(-4.0f, 21.0f);
		}
		else if (gameTime == 200.0f)
		{
			SpawnManager(-8.0f, 3.0f);
			SpawnManager(-4.0f, 3.0f);
			SpawnManager(0.0f, 3.0f);

			SpawnManager(8.0f, 12.0f);
		}
		else if (gameTime == 270.0f)
		{
			SpawnManager(0.0f, 3.0f);
			SpawnManager(4.0f, 3.0f);
			SpawnManager(8.0f, 3.0f);

			SpawnManager(-4.0f, 12.0f);

			SpawnManager(8.0f, 21.0f);
		}
		else if (gameTime == 350.0f)
		{
			SpawnManager(-8.0f, 3.0f);
			SpawnManager(0.0f, 3.0f);
			SpawnManager(8.0f, 3.0f);

			SpawnManager(4.0f, 12.0f);
		}
		else if (gameTime == 410.0f)
		{
			SpawnManager(-4.0f, 3.0f);
			SpawnManager(0.0f, 3.0f);
			SpawnManager(4.0f, 3.0f);

			SpawnManager(8.0f, 12.0f);
		}
		else if (gameTime == 470.0f)
		{
			SpawnManager(-8.0f, 3.0f);
			SpawnManager(0.0f, 3.0f);
			SpawnManager(8.0f, 3.0f);

			SpawnManager(-4.0f, 12.0f);

			SpawnManager(4.0f, 21.0f);
		}
		else if (gameTime == 530.0f)
		{
			SpawnManager(-8.0f, 11.0f);
			SpawnManager(-4.0f, 11.0f);
			SpawnManager(0.0f, 11.0f);
			SpawnManager(4.0f, 11.0f);
			SpawnManager(8.0f, 11.0f);
		}
		else if (gameTime == 590.0f)
		{
			SpawnManager(-8.0f, 3.0f);
			SpawnManager(-4.0f, 3.0f);
			SpawnManager(0.0f, 3.0f);
			SpawnManager(4.0f, 3.0f);
			SpawnManager(8.0f, 3.0f);
		}
		else if (gameTime == 650.0f)
		{
			SpawnManager(-8.0f, 11.0f);
			SpawnManager(-4.0f, 11.0f);
			SpawnManager(0.0f, 11.0f);
			SpawnManager(4.0f, 11.0f);
			SpawnManager(8.0f, 11.0f);

			//SpawnManager(0.0f, 21.0f);
		}
		else if (gameTime == 710.0f)
		{
			SpawnManager(-8.0f, 3.0f);
			SpawnManager(-4.0f, 3.0f);
			SpawnManager(0.0f, 3.0f);
			SpawnManager(4.0f, 3.0f);
			SpawnManager(8.0f, 3.0f);
		}
		else if (gameTime == 770.0f)
		{
			SpawnManager(-8.0f, 3.0f);
			SpawnManager(-4.0f, 3.0f);
			SpawnManager(4.0f, 3.0f);
			SpawnManager(8.0f, 3.0f);

			SpawnManager(0.0f, 12.0f);
		}
	}
	//ステージ2
	else if (selectStageNumber == 2.0f)
	{
		//ゲーム経過時間に合わせて敵を出現
		if (gameTime == 10.0f)
		{
			SpawnManager(-8.0f, 3.0f);
			SpawnManager(8.0f, 3.0f);
		}
		else if (gameTime == 30.0f)
		{

			SpawnManager(-4.0f, 1.0f);
			SpawnManager(4.0f, 1.0f);

			SpawnManager(0.0f, 12.0f);
		}
		else if (gameTime == 80.0f)
		{
			SpawnManager(8.0f, 3.0f);

			SpawnManager(-4.0f, 12.0f);
			SpawnManager(4.0f, 12.0f);

			SpawnManager(8.0f, 21.0f);
		}
		else if (gameTime == 100.0f)
		{
			SpawnManager(-8.0f, 1.0f);
			SpawnManager(0.0f, 1.0f);
		}
		else if (gameTime == 160.0f)
		{
			SpawnManager(0.0f, 3.0f);

			SpawnManager(-8.0f, 12.0f);
		}
		else if (gameTime == 180.0f)
		{
			SpawnManager(-4.0f, 1.0f);
			SpawnManager(4.0f, 1.0f);
		}
		else if (gameTime == 240.0f)
		{
			SpawnManager(-8.0f, 3.0f);
			SpawnManager(0.0f, 3.0f);
			SpawnManager(8.0f, 3.0f);

			SpawnManager(-4.0f, 11.0f);
			SpawnManager(4.0f, 11.0f);

			SpawnManager(8.0f, 21.0f);
		}
		else if (gameTime == 300.0f)
		{
			SpawnManager(-8.0f, 3.0f);

			SpawnManager(-4.0f, 1.0f);
			SpawnManager(4.0f, 1.0f);

			SpawnManager(8.0f, 12.0f);
		}
		else if (gameTime == 360.0f)
		{
			SpawnManager(-8.0f, 1.0f);
			SpawnManager(0.0f, 1.0f);

			SpawnManager(8.0f, 3.0f);

			SpawnManager(4.0f, 12.0f);
		}
		else if (gameTime == 400.0f)
		{
			SpawnManager(-4.0f, 1.0f);
			SpawnManager(4.0f, 3.0f);
			SpawnManager(8.0f, 1.0f);

			SpawnManager(0.0f, 12.0f);

			SpawnManager(0.0f, 21.0f);
		}
		else if (gameTime == 460.0f)
		{
			SpawnManager(-4.0f, 3.0f);
			SpawnManager(0.0f, 1.0f);
			SpawnManager(4.0f, 1.0f);

			SpawnManager(8.0f, 12.0f);
		}
		else if (gameTime == 520.0f)
		{
			SpawnManager(-8.0f, 1.0f);
			SpawnManager(-4.0f, 1.0f);
			SpawnManager(0.0f, 3.0f);
			SpawnManager(8.0f, 1.0f);

			SpawnManager(4.0f, 12.0f);
		}
		else if (gameTime == 580.0f)
		{
			SpawnManager(0.0f, 1.0f);
			SpawnManager(4.0f, 1.0f);
			
			SpawnManager(-4.0f, 12.0f);

			SpawnManager(-4.0f, 21.0f);
		}
		else if (gameTime == 610.0f)
		{
			SpawnManager(-8.0f, 3.0f);
			SpawnManager(8.0f, 3.0f);

			SpawnManager(0.0f, 11.0f);
		}
		else if (gameTime == 640.0f)
		{
			SpawnManager(-8.0f, 1.0f);
			SpawnManager(0.0f, 1.0f);
			SpawnManager(4.0f, 3.0f);

			SpawnManager(-4.0f, 12.0f);
		}
		else if (gameTime == 700.0f)
		{
			SpawnManager(0.0f, 1.0f);
			SpawnManager(4.0f, 1.0f);
			SpawnManager(8.0f, 1.0f);

			SpawnManager(-8.0f, 12.0f);
		}
		else if (gameTime == 730.0f)
		{
			SpawnManager(0.0f, 12.0f);
			SpawnManager(-4.0f, 3.0f);

			//SpawnManager(-8.0f, 21.0f);
		}
		else if (gameTime == 760.0f)
		{
			SpawnManager(0.0f, 1.0f);
			SpawnManager(-4.0f, 1.0f);
			SpawnManager(-8.0f, 1.0f);

			SpawnManager(4.0f, 12.0f);
		}
		else if (gameTime == 790.0f)
		{
			SpawnManager(4.0f, 3.0f);
			SpawnManager(-4.0f, 12.0f);
		}
		else if (gameTime == 820.0f)
		{
			SpawnManager(0.0f, 12.0f);
			SpawnManager(4.0f, 12.0f);
			SpawnManager(-4.0f, 12.0f);

			SpawnManager(-8.0f, 3.0f);
			SpawnManager(8.0f, 3.0f);
		}
	}
	//ステージ3
	else
	{
		//ゲーム経過時間に合わせて敵を出現
		if (gameTime == 10.0f)
		{
			SpawnManager(-6.0f, 2.0f);
			SpawnManager(6.0f, 2.0f);
			SpawnManager(0.0f, 11.0f);
		}
		else if (gameTime == 90.0f)
		{
			SpawnManager(-4.0f, 3.0f);
			SpawnManager(8.0f, 3.0f);
			SpawnManager(0.0f, 21.0f);
		}
		else if (gameTime == 110.0f)
		{
			SpawnManager(4.0f, 1.0f);
		}
		else if (gameTime == 130.0f)
		{
			SpawnManager(-4.0f, 12.0f);
		}
		else if (gameTime == 160.0f)
		{
			SpawnManager(-8.0f, 1.0f);
			SpawnManager(2.0f, 2.0f);
			SpawnManager(0.0f, 1.0f);
			SpawnManager(4.0f, 3.0f);
			SpawnManager(0.0f, 12.0f);
		}
		else if (gameTime == 200.0f)
		{
			SpawnManager(8.0f, 1.0f);
			SpawnManager(-4.0f, 1.0f);
			SpawnManager(-8.0f, 12.0f);
		}
		else if (gameTime == 240.0f)
		{
			SpawnManager(4.0f, 3.0f);
			SpawnManager(-8.0f, 1.0f);
			SpawnManager(-6.0f, 2.0f);
		}
		else if (gameTime == 280.0f)
		{
			SpawnManager(-4.0f, 12.0f);
			SpawnManager(8.0f, 3.0f);
			SpawnManager(4.0f, 11.0f);
			SpawnManager(0.0f, 3.0f);
		}
		else if (gameTime == 310.0f)
		{
			SpawnManager(4.0f, 21.0f);
		}
		else if (gameTime == 320.0f)
		{
			SpawnManager(0.0f, 12.0f);
			SpawnManager(8.0f, 12.0f);
			SpawnManager(4.0f, 3.0f);
			SpawnManager(6.0f, 2.0f);
			SpawnManager(-4.0f, 3.0f);
		}
		else if (gameTime == 360.0f)
		{
			SpawnManager(-4.0f, 12.0f);
			SpawnManager(-8.0f, 1.0f);
			SpawnManager(0.0f, 3.0f);
			SpawnManager(8.0f, 1.0f);
		}
		else if (gameTime == 400.0f)
		{
			SpawnManager(-8.0f, 12.0f);
			SpawnManager(-4.0f, 3.0f);
			SpawnManager(0.0f, 3.0f);
			SpawnManager(4.0f, 1.0f);
			SpawnManager(-2.0f, 2.0f);
		}
		else if (gameTime == 440.0f)
		{
			SpawnManager(8.0f, 12.0f);
			SpawnManager(-8.0f, 1.0f);
			SpawnManager(4.0f, 3.0f);
			SpawnManager(0.0f, 1.0f);
		}
		else if (gameTime == 480.0f)
		{
			SpawnManager(0.0f, 12.0f);
			SpawnManager(8.0f, 3.0f);
			SpawnManager(-4.0f, 1.0f);
			SpawnManager(6.0f, 2.0f);
		}
		else if (gameTime == 520.0f)
		{
			SpawnManager(-8.0f, 12.0f);
			SpawnManager(4.0f, 1.0f);
			SpawnManager(0.0f, 3.0f);
			SpawnManager(-8.0f, 3.0f);
		}
		else if (gameTime == 560.0f)
		{
			SpawnManager(0.0f, 12.0f);
			SpawnManager(-4.0f, 12.0f);
			SpawnManager(-2.0f, 2.0f);
			SpawnManager(8.0f, 1.0f);
		}
		else if (gameTime == 580.0f)
		{
			SpawnManager(-8.0f, 21.0f);
		}
		else if (gameTime == 600.0f)
		{
			SpawnManager(4.0f, 3.0f);
			SpawnManager(0.0f, 1.0f);
			SpawnManager(-4.0f, 3.0f);
			SpawnManager(8.0f, 11.0f);
		}
		else if (gameTime == 640.0f)
		{
			SpawnManager(8.0f, 12.0f);
			SpawnManager(-8.0f, 1.0f);
			SpawnManager(2.0f, 2.0f);
		}
		else if (gameTime == 680.0f)
		{
			SpawnManager(-4.0f, 1.0f);
			SpawnManager(8.0f, 1.0f);
			SpawnManager(0.0f, 3.0f);
		}
		else if (gameTime == 720.0f)
		{
			SpawnManager(-4.0f, 12.0f);
			SpawnManager(-8.0f, 3.0f);
			SpawnManager(6.0f, 2.0f);
		}
		else if (gameTime == 760.0f)
		{
			SpawnManager(4.0f, 1.0f);
			SpawnManager(8.0f, 3.0f);
			SpawnManager(-2.0f, 2.0f);
			SpawnManager(8.0f, 21.0f);
		}
		else if (gameTime == 800.0f)
		{
			SpawnManager(4.0f, 12.0f);
			SpawnManager(-8.0f, 1.0f);
			SpawnManager(8.0f, 1.0f);
			SpawnManager(2.0f, 2.0f);
			SpawnManager(-2.0f, 2.0f);
		}
		else if (gameTime == 840.0f)
		{
			SpawnManager(8.0f, 12.0f);
			SpawnManager(-8.0f, 1.0f);
			SpawnManager(8.0f, 1.0f);
			SpawnManager(-6.0f, 2.0f);
			SpawnManager(6.0f, 2.0f);
		}
		else if (gameTime == 880.0f)
		{
			SpawnManager(0.0f, 12.0f);
			SpawnManager(-4.0f, 1.0f);
			SpawnManager(4.0f, 1.0f);
		}
		else if (gameTime == 900.0f)
		{
			SpawnManager(0.0f, 3.0f);
		}
	}
}

void GameScene::SpawnManager(float posX, float objectNum)
{
	for (int i = 0; i < 100; i++)
	{
		//障害物1を生成
		if (objectNum == 1.0f)
		{
			//画面上にいない障害物を選択
			if (!block1Arive[i])
			{
				block1Arive[i] = true;//障害物を生成
				block1Position[i].x = posX;//X座標を設定
				block1Position[i].y = defaultBlockPositionY;//Y座標を設定
				block1Position[i].z = playerPosition.z + defaultBlockPositionZ;//Z座標を設定
				block1Scale[i] = { 0.0f, 0.0f, 0.0f };//スケールを0に設定
				block1[i]->SetPosition(block1Position[i]);//座標を更新
				block1[i]->SetScale(block1Scale[i]);//スケールを更新
				break;
			}
		}
		//障害物2を生成
		else if (objectNum == 2.0f)
		{
			//画面上にいない障害物を選択
			if (!block2Arive[i])
			{
				block2Arive[i] = true;//障害物を生成
				block2Position[i].x = posX;//X座標を設定
				block2Position[i].y = defaultBlockPositionY;//Y座標を設定
				block2Position[i].z = playerPosition.z + defaultBlockPositionZ;//Z座標を設定
				block2Scale[i] = { 0.0f, 0.0f, 0.0f };//スケールを0に設定
				block2[i]->SetPosition(block2Position[i]);//座標を更新
				block2[i]->SetScale(block2Scale[i]);//スケールを更新
				break;
			}
		}
		//障害物3を生成
		else if (objectNum == 3.0f)
		{
			//画面上にいない障害物を選択
			if (!block3Arive[i])
			{
				block3Arive[i] = true;//障害物を生成
				block3Position[i].x = posX;//X座標を設定
				block3Position[i].y = defaultBlockPositionY;//Y座標を設定
				block3Position[i].z = playerPosition.z + defaultBlockPositionZ;//Z座標を設定
				block3Scale[i] = { 0.0f, 0.0f, 0.0f };//スケールを0に設定
				block3[i]->SetPosition(block3Position[i]);//座標を更新
				block3[i]->SetScale(block3Scale[i]);//スケールを更新
				break;
			}
		}
		//アイテム1を生成
		else if (objectNum == 11.0f)
		{
			//画面上にいないアイテムを選択
			if (!item1Arive[i])
			{
				item1Arive[i] = true;//アイテムを生成
				item1Position[i].x = posX;//X座標を設定
				item1Position[i].y = defaultBlockPositionY;//Y座標を設定
				item1Position[i].z = playerPosition.z + defaultBlockPositionZ;//Z座標を設定
				item1Scale[i] = { 0.0f, 0.0f, 0.0f };//スケールを0に設定
				item1[i]->SetPosition(item1Position[i]);//座標を更新
				item1[i]->SetScale(item1Scale[i]);//スケールを更新
				break;
			}
		}
		//アイテム2を生成
		else if (objectNum == 12.0f)
		{
			//画面上にいないアイテムを選択
			if (!item2Arive[i])
			{
				item2Arive[i] = true;//アイテムを生成
				item2Position[i].x = posX;//X座標を設定
				item2Position[i].y = defaultBlockPositionY;//Y座標を設定
				item2Position[i].z = playerPosition.z + defaultBlockPositionZ;//Z座標を設定
				item2Scale[i] = { 0.0f, 0.0f, 0.0f };//スケールを0に設定
				item2[i]->SetPosition(item2Position[i]);//座標を更新
				item2[i]->SetScale(item2Scale[i]);//スケールを更新
				break;
			}
		}
		//レーザーを生成
		else if (objectNum == 21.0f)
		{
			dangerParticlesPosition.x = posX;//X座標を設定
			dangerParticlesPosition.y = 2.0f;//Y座標を設定
			dangerParticlesPosition.z = playerPosition.z + 150.0f;//Z座標を設定
			laserParticlesPosition.x = posX;//X座標を設定
			laserParticlesPosition.y = 2.0f;//Y座標を設定
			laserParticlesPosition.z = playerPosition.z + 150.0f;//Z座標を設定
			logoFlashCount = 0.0f;//ロゴ点滅からの経過時間リセット
			logoHide = true;//ロゴを表示する
			dangerUIArive = true;//警告演出のUIを表示する
			isDangerParticles = true;//警告演出エフェクトを表示する
		}
	}
}

void GameScene::PlayerCollision()
{
	//強化着地
	if (powerLanding)
	{
		powerLandingCount += 1.0f;//強化着地開始からの経過時間増加

		if (powerLandingCount >= powerLandingLimit)
		{
			powerLandingCount = 0.0f;//強化着地開始からの経過時間をリセット
			powerLanding = false;//強化着地の判定を停止
		}
	}

	for (int i = 0; i < 100; i++)
	{
		//プレイヤーと障害物orアイテムの当たり判定
		//障害物2の生存確認
		if (block2Arive[i])
		{
			//当たっているかの計算
			float a = playerPosition.x - block2Position[i].x;
			float b = playerPosition.y - block2Position[i].y;
			float c = playerPosition.z - block2Position[i].z;
			float d = sqrt(a * a + b * b + c * c);

			//当たっていたら障害物を消滅
			if (d <= 3)
			{
				block2Arive[i] = false;//障害物2を消滅
				block2Position[i] = offScreenPosition;//画面外に移動
				block2[i]->SetPosition(block2Position[i]);//障害物2の座標更新

				//強化着地中は障害物を破壊してスコア加算
				if (powerLanding)
				{
					blockBreakParticlesPosition.x = playerPosition.x;//障害物破壊エフェクトのX座標設定
					blockBreakParticlesPosition.y = playerPosition.y;//障害物破壊エフェクトのY座標設定
					blockBreakParticlesPosition.z = playerPosition.z;//障害物破壊エフェクトのZ座標設定
					isBlockBreakParticles = true;//障害物破壊エフェクトを生成
					preScore += 100;//スコア+10
					break;
				}

				playerHitParticlesPosition.x = playerPosition.x;//被弾エフェクトのX座標設定
				playerHitParticlesPosition.y = playerPosition.y;//被弾エフェクトのY座標設定
				playerHitParticlesPosition.z = playerPosition.z;//被弾エフェクトのZ座標設定
				isPlayerHitParticles = true;//被弾エフェクトを生成

				//バリアパーツがない場合はゲームオーバー
				if (equipItem != 0.0f)
				{
					equipItem = 0.0f;//パーツを戻す
					playerBarrierPartsPosition = offScreenPosition;//画面外に移動
					playerPartsScale = initializeCoordinate;//スケールをリセット
					playerBarrierParts->SetPosition(playerBarrierPartsPosition);//座標を更新
					playerBarrierParts->SetScale(playerPartsScale);//スケールを更新
				}
				else if (equipItem == 0.0f)
				{
					scene = GameoverLogo;//ゲームオーバー
				}
			}
		}
		//移動中は当たらない
		if (!sideJump)
		{
			//障害物1の生存確認
			if (block1Arive[i])
			{
				//当たっているかの計算
				float a = playerPosition.x - block1Position[i].x;
				float b = playerPosition.y - block1Position[i].y;
				float c = playerPosition.z - block1Position[i].z;
				float d = sqrt(a * a + b * b + c * c);

				//当たっていたら障害物を消滅
				if (d <= 2)
				{
					block1Arive[i] = false;//障害物1を消滅
					block1Position[i] = offScreenPosition;//画面外に移動
					block1[i]->SetPosition(block1Position[i]);//障害物1の座標更新

					//強化着地中は障害物を破壊してスコア加算
					if (powerLanding)
					{
						blockBreakParticlesPosition.x = playerPosition.x;//障害物破壊エフェクトのX座標設定
						blockBreakParticlesPosition.y = playerPosition.y;//障害物破壊エフェクトのY座標設定
						blockBreakParticlesPosition.z = playerPosition.z;//障害物破壊エフェクトのZ座標設定
						isBlockBreakParticles = true;//障害物破壊エフェクトを生成
						preScore += 100;//スコア+10
						break;
					}

					playerHitParticlesPosition.x = playerPosition.x;//被弾エフェクトのX座標設定
					playerHitParticlesPosition.y = playerPosition.y;//被弾エフェクトのY座標設定
					playerHitParticlesPosition.z = playerPosition.z;//被弾エフェクトのZ座標設定
					isPlayerHitParticles = true;//被弾エフェクトを生成

					//バリアパーツがない場合はゲームオーバー
					if (equipItem != 0.0f)
					{
						equipItem = 0.0f;//パーツを戻す
						playerBarrierPartsPosition = offScreenPosition;//画面外に移動
						playerPartsScale = initializeCoordinate;//スケールをリセット
						playerBarrierParts->SetPosition(playerBarrierPartsPosition);//座標を更新
						playerBarrierParts->SetScale(playerPartsScale);//スケールを更新
					}
					else if (equipItem == 0.0f)
					{
						scene = GameoverLogo;//ゲームオーバー
					}
				}
			}
			//障害物3の生存確認
			if (block3Arive[i])
			{
				//当たっているかの計算
				float a = playerPosition.x - block3Position[i].x;
				float b = playerPosition.y - block3Position[i].y;
				float c = playerPosition.z - block3Position[i].z;
				float d = sqrt(a * a + b * b + c * c);

				//当たっていたら障害物を消滅
				if (d <= 2)
				{
					block3Arive[i] = false;//障害物1を消滅
					block3Position[i] = offScreenPosition;//画面外に移動
					block3[i]->SetPosition(block3Position[i]);//障害物1の座標更新

					//強化着地中は障害物を破壊してスコア加算
					if (powerLanding)
					{
						blockBreakParticlesPosition.x = playerPosition.x;//障害物破壊エフェクトのX座標設定
						blockBreakParticlesPosition.y = playerPosition.y;//障害物破壊エフェクトのY座標設定
						blockBreakParticlesPosition.z = playerPosition.z;//障害物破壊エフェクトのZ座標設定
						isBlockBreakParticles = true;//障害物破壊エフェクトを生成
						preScore += 100;//スコア+10
						break;
					}

					playerHitParticlesPosition.x = playerPosition.x;//被弾エフェクトのX座標設定
					playerHitParticlesPosition.y = playerPosition.y;//被弾エフェクトのY座標設定
					playerHitParticlesPosition.z = playerPosition.z;//被弾エフェクトのZ座標設定
					isPlayerHitParticles = true;//被弾エフェクトを生成

					//バリアパーツがない場合はゲームオーバー
					if (equipItem != 0.0f)
					{
						equipItem = 0.0f;//パーツを戻す
						playerBarrierPartsPosition = offScreenPosition;//画面外に移動
						playerPartsScale = initializeCoordinate;//スケールをリセット
						playerBarrierParts->SetPosition(playerBarrierPartsPosition);//座標を更新
						playerBarrierParts->SetScale(playerPartsScale);//スケールを更新
					}
					else if (equipItem == 0.0f)
					{
						scene = GameoverLogo;//ゲームオーバー
					}
				}
			}
			//アイテム1の生存確認
			if (item1Arive[i])
			{
				//当たっているかの計算
				float a = playerPosition.x - item1Position[i].x;
				float b = playerPosition.y - item1Position[i].y;
				float c = playerPosition.z - item1Position[i].z;
				float d = sqrt(a * a + b * b + c * c);

				//当たっていたら獲得
				if (d <= 1)
				{
					//既に獲得していた場合はスコアに変換
					if (equipItem == 0.0f)
					{
						equipItem = 1.0f;//パーツの種類を設定
						playerPartsScale = initializeCoordinate;//プレイヤーパーツのスケールをリセット
					}
					else
					{
						preScore += 100;//スコア+100
					}

					armorGetBoardArive = true;//バリア獲得のスプライトを出現
					armorGetBoardCount = 15.0f;//バリア獲得のスプライトの経過時間リセット
					armorGetSpritePosition = { 546.0f, 400.0f };//バリア獲得のスプライトの座標設定
					armorGetSprite->SetPosition(armorGetSpritePosition);//バリア獲得のスプライトの座標変更

					itemGetParticlesPosition.x = playerPosition.x;//アイテム獲得エフェクトのX座標設定
					itemGetParticlesPosition.y = playerPosition.y;//アイテム獲得エフェクトのY座標設定
					itemGetParticlesPosition.z = playerPosition.z;//アイテム獲得エフェクトのZ座標設定
					isItemGetParticles = true;//アイテム獲得エフェクトを生成

					playerBarrierPartsPosition = playerPosition;//プレイヤーの座標に生成
					item1Arive[i] = false;//アイテム1を消滅
					item1Position[i] = offScreenPosition;//アイテム1を画面外に移動
					playerBarrierParts->SetPosition(playerBarrierPartsPosition);//プレイヤーバリアパーツの座標更新
					playerBarrierParts->SetScale(playerPartsScale);//プレイヤーバリアパーツのスケール更新
					item1[i]->SetPosition(item1Position[i]);//アイテム1の座標更新
				}
			}
			//アイテム2の生存確認
			if (item2Arive[i])
			{
				//当たっているかの計算
				float a = playerPosition.x - item2Position[i].x;
				float b = playerPosition.y - item2Position[i].y;
				float c = playerPosition.z - item2Position[i].z;
				float d = sqrt(a * a + b * b + c * c);

				//当たっていたら獲得
				if (d <= 1)
				{
					itemGetParticlesPosition.x = playerPosition.x;//アイテム獲得エフェクトのX座標設定
					itemGetParticlesPosition.y = playerPosition.y;//アイテム獲得エフェクトのY座標設定
					itemGetParticlesPosition.z = playerPosition.z;//アイテム獲得エフェクトのZ座標設定
					isItemGetParticles = true;//アイテム獲得エフェクトを生成

					scoreGetBoardArive = true;//スコア獲得のスプライトを出現
					scoreGetBoardCount = 15.0f;//スコア獲得のスプライトの経過時間リセット
					scoreGetSpritePosition = { 528.0f, 400.0f };//スコア獲得のスプライトの座標設定
					scoreGetSprite->SetPosition(scoreGetSpritePosition);//スコア獲得のスプライトの座標変更

					preScore += 100.0f;//スコア+100
					item2Arive[i] = false;//アイテム2を消滅
					item2Position[i] = offScreenPosition;//画面外に移動
					item2[i]->SetPosition(item2Position[i]);//aアイテム2の座標更新
				}
			}
			//アイテム3の生存確認
			if (item3Arive[i])
			{
				//当たっているかの計算
				float a = playerPosition.x - item3Position[i].x;
				float b = playerPosition.y - item3Position[i].y;
				float c = playerPosition.z - item3Position[i].z;
				float d = sqrt(a * a + b * b + c * c);

				//当たっていたら獲得
				if (d <= 1)
				{
					//既に獲得していた場合はスコアに変換
					if (equipItem == 0.0f)
					{
						equipItem = 1.0f;//パーツの種類を設定
						playerPartsScale = initializeCoordinate;//プレイヤーパーツのスケールをリセット
					}
					else
					{
						preScore += 100;//スコア+100
					}

					playerBarrierPartsPosition = playerPosition;//プレイヤーの座標に生成
					item3Arive[i] = false;//アイテム3を消滅
					item3Position[i] = offScreenPosition;//アイテム3を画面外に移動
					playerBarrierParts->SetPosition(playerBarrierPartsPosition);//プレイヤーバリアパーツの座標更新
					playerBarrierParts->SetScale(playerPartsScale);//プレイヤーバリアパーツのスケール更新
					item3[i]->SetPosition(item3Position[i]);//アイテム3の座標更新
				}
			}
		}
	}
	//プレイヤーとレーザーの当たり判定
	for (int i = 0; i < 5; i++)
	{
		//プレイヤー移動中は判定なし
		if (!sideJump && laserCollision[i])
		{
			//当たっているかの計算
			float a = playerPosition.x - laserStartPosition[i].x;
			float b = playerPosition.y - laserStartPosition[i].y;
			float c = sqrt(a * a + b * b);

			//当たっていたらレーザーの判定を消す
			if (c <= 1 && playerPosition.z < laserStartPosition[i].z && playerPosition.z >= laserLengthPosition[i].z)
			{
				laserCollisionCount[i] = 0.0f;//生成からの経過時間リセット
				laserCollision[i] = false;//レーザーの判定消滅
				laserStartPosition[i] = offScreenPosition;//画面外に移動
				laserLengthPosition[i] = offScreenPosition;//画面外に移動

				playerHitParticlesPosition.x = playerPosition.x;//被弾エフェクトのX座標設定
				playerHitParticlesPosition.y = playerPosition.y;//被弾エフェクトのY座標設定
				playerHitParticlesPosition.z = playerPosition.z;//被弾エフェクトのZ座標設定
				isPlayerHitParticles = true;//被弾エフェクトを生成

				//バリアパーツがない場合はゲームオーバー
				if (equipItem != 0.0f)
				{
					equipItem = 0.0f;//パーツを戻す
					playerBarrierPartsPosition = offScreenPosition;//画面外に移動
					playerPartsScale = initializeCoordinate;//スケールをリセット
					playerBarrierParts->SetPosition(playerBarrierPartsPosition);//座標を更新
					playerBarrierParts->SetScale(playerPartsScale);//スケールを更新
				}
				else if (equipItem == 0.0f)
				{
					scene = GameoverLogo;//ゲームオーバー
				}
			}
		}
	}
}

void GameScene::DrawScoreBoard(bool isResult)
{
	//[0]0000の描画
	switch (scoreBoard0)
	{
	case 0:
		if (!isResult) { scoreNumber0[0]->Draw(); }
		else { resultScoreNumber0[0]->Draw(); }
		break;
	case 1:
		if (!isResult) { scoreNumber1[0]->Draw(); }
		else { resultScoreNumber1[0]->Draw(); }
		break;
	case 2:
		if (!isResult) { scoreNumber2[0]->Draw(); }
		else { resultScoreNumber2[0]->Draw(); }
		break;
	case 3:
		if (!isResult) { scoreNumber3[0]->Draw(); }
		else { resultScoreNumber3[0]->Draw(); }
		break;
	case 4:
		if (!isResult) { scoreNumber4[0]->Draw(); }
		else { resultScoreNumber4[0]->Draw(); }
		break;
	case 5:
		if (!isResult) { scoreNumber5[0]->Draw(); }
		else { resultScoreNumber5[0]->Draw(); }
		break;
	case 6:
		if (!isResult) { scoreNumber6[0]->Draw(); }
		else { resultScoreNumber6[0]->Draw(); }
		break;
	case 7:
		if (!isResult) { scoreNumber7[0]->Draw(); }
		else { resultScoreNumber7[0]->Draw(); }
		break;
	case 8:
		if (!isResult) { scoreNumber8[0]->Draw(); }
		else { resultScoreNumber8[0]->Draw(); }
		break;
	case 9:
		if (!isResult) { scoreNumber9[0]->Draw(); }
		else { resultScoreNumber9[0]->Draw(); }
		break;
	default:
		break;
	}
	//0[0]000の描画
	switch (scoreBoard1)
	{
	case 0:
		if (!isResult) { scoreNumber0[1]->Draw(); }
		else { resultScoreNumber0[1]->Draw(); }
		break;
	case 1:
		if (!isResult) { scoreNumber1[1]->Draw(); }
		else { resultScoreNumber1[1]->Draw(); }
		break;
	case 2:
		if (!isResult) { scoreNumber2[1]->Draw(); }
		else { resultScoreNumber2[1]->Draw(); }
		break;
	case 3:
		if (!isResult) { scoreNumber3[1]->Draw(); }
		else { resultScoreNumber3[1]->Draw(); }
		break;
	case 4:
		if (!isResult) { scoreNumber4[1]->Draw(); }
		else { resultScoreNumber4[1]->Draw(); }
		break;
	case 5:
		if (!isResult) { scoreNumber5[1]->Draw(); }
		else { resultScoreNumber5[1]->Draw(); }
		break;
	case 6:
		if (!isResult) { scoreNumber6[1]->Draw(); }
		else { resultScoreNumber6[1]->Draw(); }
		break;
	case 7:
		if (!isResult) { scoreNumber7[1]->Draw(); }
		else { resultScoreNumber7[1]->Draw(); }
		break;
	case 8:
		if (!isResult) { scoreNumber8[1]->Draw(); }
		else { resultScoreNumber8[1]->Draw(); }
		break;
	case 9:
		if (!isResult) { scoreNumber9[1]->Draw(); }
		else { resultScoreNumber9[1]->Draw(); }
		break;
	default:
		break;
	}
	//00[0]00の描画
	switch (scoreBoard2)
	{
	case 0:
		if (!isResult) { scoreNumber0[2]->Draw(); }
		else { resultScoreNumber0[2]->Draw(); }
		break;
	case 1:
		if (!isResult) { scoreNumber1[2]->Draw(); }
		else { resultScoreNumber1[2]->Draw(); }
		break;
	case 2:
		if (!isResult) { scoreNumber2[2]->Draw(); }
		else { resultScoreNumber2[2]->Draw(); }
		break;
	case 3:
		if (!isResult) { scoreNumber3[2]->Draw(); }
		else { resultScoreNumber3[2]->Draw(); }
		break;
	case 4:
		if (!isResult) { scoreNumber4[2]->Draw(); }
		else { resultScoreNumber4[2]->Draw(); }
		break;
	case 5:
		if (!isResult) { scoreNumber5[2]->Draw(); }
		else { resultScoreNumber5[2]->Draw(); }
		break;
	case 6:
		if (!isResult) { scoreNumber6[2]->Draw(); }
		else { resultScoreNumber6[2]->Draw(); }
		break;
	case 7:
		if (!isResult) { scoreNumber7[2]->Draw(); }
		else { resultScoreNumber7[2]->Draw(); }
		break;
	case 8:
		if (!isResult) { scoreNumber8[2]->Draw(); }
		else { resultScoreNumber8[2]->Draw(); }
		break;
	case 9:
		if (!isResult) { scoreNumber9[2]->Draw(); }
		else { resultScoreNumber9[2]->Draw(); }
		break;
	default:
		break;
	}
	//000[0]0の描画
	switch (scoreBoard3)
	{
	case 0:
		if (!isResult) { scoreNumber0[3]->Draw(); }
		else { resultScoreNumber0[3]->Draw(); }
		break;
	case 1:
		if (!isResult) { scoreNumber1[3]->Draw(); }
		else { resultScoreNumber1[3]->Draw(); }
		break;
	case 2:
		if (!isResult) { scoreNumber2[3]->Draw(); }
		else { resultScoreNumber2[3]->Draw(); }
		break;
	case 3:
		if (!isResult) { scoreNumber3[3]->Draw(); }
		else { resultScoreNumber3[3]->Draw(); }
		break;
	case 4:
		if (!isResult) { scoreNumber4[3]->Draw(); }
		else { resultScoreNumber4[3]->Draw(); }
		break;
	case 5:
		if (!isResult) { scoreNumber5[3]->Draw(); }
		else { resultScoreNumber5[3]->Draw(); }
		break;
	case 6:
		if (!isResult) { scoreNumber6[3]->Draw(); }
		else { resultScoreNumber6[3]->Draw(); }
		break;
	case 7:
		if (!isResult) { scoreNumber7[3]->Draw(); }
		else { resultScoreNumber7[3]->Draw(); }
		break;
	case 8:
		if (!isResult) { scoreNumber8[3]->Draw(); }
		else { resultScoreNumber8[3]->Draw(); }
		break;
	case 9:
		if (!isResult) { scoreNumber9[3]->Draw(); }
		else { resultScoreNumber9[3]->Draw(); }
		break;
	default:
		break;
	}

	//0000[0]の描画
	switch (scoreBoard4)
	{
	case 0:
		if (!isResult) { scoreNumber0[4]->Draw(); }
		else { resultScoreNumber0[4]->Draw(); }
		break;
	case 1:
		if (!isResult) { scoreNumber1[4]->Draw(); }
		else { resultScoreNumber1[4]->Draw(); }
		break;
	case 2:
		if (!isResult) { scoreNumber2[4]->Draw(); }
		else { resultScoreNumber2[4]->Draw(); }
		break;
	case 3:
		if (!isResult) { scoreNumber3[4]->Draw(); }
		else { resultScoreNumber3[4]->Draw(); }
		break;
	case 4:
		if (!isResult) { scoreNumber4[4]->Draw(); }
		else { resultScoreNumber4[4]->Draw(); }
		break;
	case 5:
		if (!isResult) { scoreNumber5[4]->Draw(); }
		else { resultScoreNumber5[4]->Draw(); }
		break;
	case 6:
		if (!isResult) { scoreNumber6[4]->Draw(); }
		else { resultScoreNumber6[4]->Draw(); }
		break;
	case 7:
		if (!isResult) { scoreNumber7[4]->Draw(); }
		else { resultScoreNumber7[4]->Draw(); }
		break;
	case 8:
		if (!isResult) { scoreNumber8[4]->Draw(); }
		else { resultScoreNumber8[4]->Draw(); }
		break;
	case 9:
		if (!isResult) { scoreNumber9[4]->Draw(); }
		else { resultScoreNumber9[4]->Draw(); }
		break;
	default:
		break;
	}
}

void GameScene::ItemGetLogo()
{
	//バリア獲得のスプライト
	if (armorGetBoardArive)
	{
		armorGetBoardCount -= 1.0f;//バリア獲得のスプライトの経過時間増加
		armorGetSpritePosition.y -= 2.0f;//バリア獲得のスプライトの座標設定
		armorGetSprite->SetPosition(armorGetSpritePosition);//バリア獲得のスプライトの座標変更
		armorGetSprite->SetColor({ 1, 1, 1, armorGetBoardCount / 15 });

		//経過時間が出現時間を超えたら消滅
		if (armorGetBoardCount <= armorGetBoardLimit)
		{
			armorGetBoardArive = false;//バリア獲得のスプライト消滅
			armorGetBoardCount = 15.0f;//バリア獲得のスプライト経過時間リセット
		}
	}
	//スコア獲得のスプライト
	if (scoreGetBoardArive)
	{
		scoreGetBoardCount -= 1.0f;//スコア獲得のスプライトの経過時間増加
		scoreGetSpritePosition.y -= 2.0f;//スコア獲得のスプライトの座標設定
		scoreGetSprite->SetPosition(scoreGetSpritePosition);//スコア獲得のスプライトの座標変更
		scoreGetSprite->SetColor({ 1, 1, 1, scoreGetBoardCount / 15 });

		//経過時間が出現時間を超えたら消滅
		if (scoreGetBoardCount <= scoreGetBoardLimit)
		{
			scoreGetBoardArive = false;//スコア獲得のスプライト消滅
			scoreGetBoardCount = 15.0f;//スコア獲得のスプライト経過時間リセット
		}
	}
}