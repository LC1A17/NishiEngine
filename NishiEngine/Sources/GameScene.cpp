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
	safe_delete(titleSprite);
	safe_delete(gameSprite);
	safe_delete(resultSprite);

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
	if (!Sprite::LoadTexture(1, L"Resources/sprite/title.png"))
	{//タイトル
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(2, L"Resources/sprite/back.png"))
	{//ゲーム
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(3, L"Resources/sprite/result.png"))
	{//リザルト
		assert(0);
		return;
	}

	//背景スプライト生成
	titleSprite = Sprite::Create(1, { 0.0f,0.0f });//タイトル
	gameSprite = Sprite::Create(2, { 0.0f,0.0f });//ゲーム
	resultSprite = Sprite::Create(3, { 0.0f,0.0f });//リザルト

	//パーティクルマネージャ生成
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(camera);

	//テクスチャ0番に読み込み
	Sprite::LoadTexture(0, L"Resources/sprite/tex1.png");

	//ライト生成
	lightGroup = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup);
	WaterObject::SetLightGroup(lightGroup);

	//モデル読み込み
	domeModel = Model::CreateFromOBJ("skydome");//天球
	railModel = Model::CreateFromOBJ("sphere");//レーン
	playerModel = Model::CreateFromOBJ("player");//プレイヤー
	enemy01Model = Model::CreateFromOBJ("enemy");//敵1
	enemy02Model = Model::CreateFromOBJ("enemy");//敵2
	pBullModel = Model::CreateFromOBJ("pBullet");//プレイヤーの弾
	eBullModel = Model::CreateFromOBJ("pBullet");//敵の弾
	parts1Model = Model::CreateFromOBJ("parts");//落ちているパーツ1
	parts2Model = Model::CreateFromOBJ("parts");//落ちているパーツ2
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
		e01Sca[i] = { 1.0f, 1.0f, 1.0f };
		enemy01[i]->SetPosition(e01Pos[i]);
		enemy01[i]->SetScale(e01Sca[i]);
		enemy02[i] = Object3d::Create(enemy02Model);
		e02Pos[i] = deadPos;
		e02Rot[i] = { 0.0f, 0.0f, 0.0f };
		e02Sca[i] = { 1.0f, 1.0f, 1.0f };
		enemy02[i]->SetPosition(e02Pos[i]);
		enemy02[i]->SetScale(e02Sca[i]);
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
		partsSca[i] = { 1.0f, 1.0f, 1.0f };
		parts02[i]->SetPosition(parts02Pos[i]);
		parts02[i]->SetScale(partsSca[i]);
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

	//パーツのCDの最大値
	partsInitCD[0] = 20.0f;
	partsInitCD[1] = 20.0f;
	partsInitCD[2] = 30.0f;

	//パーツの耐久値の最大値
	partsInitHP[0] = 0.0f;
	partsInitHP[1] = 10.0f;
	partsInitHP[2] = 10.0f;

	//弾の生存時間の最大値
	partsInitTime[0] = 40.0f;
	partsInitTime[1] = 40.0f;
	partsInitTime[2] = 60.0f;

	//パーツの耐久値回復量
	partsRecovery[0] = 0.0f;
	partsRecovery[1] = 5.0f;
	partsRecovery[2] = 5.0f;

	//敵の速度
	eBullSpead[0] = 1.0f;

	for (int i = 0; i < 4; i++)
	{
		partsNum[i] = 0.0f;//装着中のパーツの種類
		partsCD[i] = 0.0f;//パーツのCD
		partsHP[i] = 0.0f;//パーツの耐久値
		partsSelect[i] = false;//パーツが選択中かどうか
	}

	partsSelect[0] = true;//0を選択中にする

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
		enemy01Arive[i] = false;//敵1が生きているかの判定
		enemy02Arive[i] = false;//敵2が生きているかの判定
	}

	//カメラ注視点をセット
	camera->SetTarget(cameraPos);
	camera->SetDistance(6.0f);
}

void GameScene::Update()
{
	if (scene == Title)
	{//タイトル
		//プレイヤー操作関連
		if (input->TriggerKey(DIK_SPACE))
		{//SPACE入力でゲームシーンへ移行
			scene = Game;//シーン移行
		}
	}
	else if (scene == Game)
	{//ゲーム
		//プレイヤー射撃関連
		if (!isShot && !isMove)
		{//レーン移動をしていない上でCDが残っている場合
			if (input->TriggerKey(DIK_UP))
			{//パーツスロット上選択
				for (int i = 0; i < 4; i++) { partsSelect[i] = false; }//一旦すべてfalse
				partsSelect[0] = true;//0をtrue
				isChange = true;//変更開始
			}
			if (input->TriggerKey(DIK_DOWN))
			{//パーツスロット下選択
				for (int i = 0; i < 4; i++) { partsSelect[i] = false; }//一旦すべてfalse
				partsSelect[1] = true;//1をtrue
				isChange = true;//変更開始
			}
			if (input->TriggerKey(DIK_LEFT))
			{//パーツスロット左選択
				for (int i = 0; i < 4; i++) { partsSelect[i] = false; }//一旦すべてfalse
				partsSelect[2] = true;//2をtrue
				isChange = true;//変更開始
			}
			if (input->TriggerKey(DIK_RIGHT))
			{//パーツスロット右選択
				for (int i = 0; i < 4; i++) { partsSelect[i] = false; }//一旦すべてfalse
				partsSelect[3] = true;//3をtrue
				isChange = true;//変更開始
			}
			for (int i = 0; i < 4; i++)
			{//CD処理
				//選択中のパーツのCDを減らす
				if (partsSelect[i]) { partsCD[i] -= 1.0f; }
				if (partsCD[i] <= 0.0f)
				{//選択中のパーツのCDが0になったら弾をセット
					shotNum = partsNum[i];//装着中のパーツの弾をセット
					isShot = true;//弾のセット開始

					//通常弾以外のパーツの場合、耐久値を減少
					if (partsNum[i] != 0.0f) { partsHP[i] -= 1.0f; }
					if (partsNum[i] != 0.0f && partsHP[i] <= 0.0f)//パーツの耐久値が0になった場合
					{//パーツの破壊処理
						partsNum[i] = 0.0f;//パーツを通常弾に戻す
						partsCD[i] = partsInitCD[0];//パーツのCDを通常弾に戻す
						partsHP[i] = partsInitHP[0];//パーツのHPを初期化
						isChange = true;//変更開始
					}
					//CDの再計算
					if (partsNum[i] == 0.0f) { partsCD[i] = partsInitCD[0]; }
					else if (partsNum[i] == 1.0f) { partsCD[i] = partsInitCD[1]; }
					else if (partsNum[i] == 2.0f) { partsCD[i] = partsInitCD[2]; }
				}
			}
		}
		else if (isShot && !isMove)//CDが0かつ移動をしていない場合
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

				for (int i = 0; i < 100; i++)
				{//パーツが生きている場合
					if (pPartsArive[i])
					{//現在のプレイヤーパーツ座標を保存
						pPartsOldPos[i] = pPartsPos[i];
					}
				}
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
			for (int i = 0; i < 4; i++)
			{
				if (partsSelect[i])
				{//選択中のパーツの情報を取得
					if (partsNum[i] == 0.0f)
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
					else if (partsNum[i] == 1.0f)
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
					else if (partsNum[i] == 2.0f)
					{//貫通弾
						pPartsArive[0] = true;//パーツを生成
						pPartsPos[0] = pPos;//プレイヤーの位置に移動
						playerParts[0]->SetPosition(pPartsPos[0]);
					}
				}
			}
		}

		//システム関連
		//前に自動で進み続ける
		pPos.z += movePosZ;//プレイヤー前
		cameraPos.z += movePosZ;//カメラ前
		domePos.z += movePosZ;//天球前
		dome->SetPosition(domePos);//天球の座標更新
		player->SetPosition(pPos);//プレイヤーの座標更新
		player->SetRotation(pRot);//プレイヤーの回転量更新
		camera->SetTarget(cameraPos);//カメラの座標更新
		
		for (int i = 0; i < 100; i++)
		{//プレイヤーのパーツが生きている場合
			if (pPartsArive[i])
			{
				pPartsPos[i].z += movePosZ;//プレイヤーのパーツ前
			}

			playerParts[i]->SetPosition(pPartsPos[i]);//プレイヤーのパーツ座標更新
			playerParts[i]->SetRotation(pPartsRot[i]);//プレイヤーのパーツ回転量更新
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

							e02Pos[j] = deadPos;//画面外に移動
							pBull[i]->SetPosition(pBullPos[i]);
							enemy02[j]->SetPosition(e02Pos[j]);
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
						for (int j = 0; j < 4; j++)
						{
							if (partsSelect[j])
							{
								if (partsNum[j] == 0.0f)
								{//通常弾以外のパーツの場合はスルー
									partsNum[j] = 1.0f;//パーツの種類を設定
									partsCD[j] = partsInitCD[1];//パーツのCDを設定
									partsHP[j] = partsInitHP[1];//パーツの耐久を設定
									isChange = true;//変更開始
								}
								else
								{//それ以外の場合は現在のパーツの耐久値を回復させる
									if (partsHP[j] < partsInitHP[j])
									{//最大値以上にはならない
										partsHP[j] += partsRecovery[1];//パーツの耐久を回復
									}
								}
							}
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
						for (int j = 0; j < 4; j++)
						{
							if (partsSelect[j])
							{
								if (partsNum[j] == 0.0f)
								{//通常弾以外のパーツの場合はスルー
									partsNum[j] = 2.0f;//パーツの種類を設定
									partsCD[j] = partsInitCD[1];//パーツのCDを設定
									partsHP[j] = partsInitHP[1];//パーツの耐久を設定
									isChange = true;//変更開始
								}
								else
								{//それ以外の場合は現在のパーツの耐久値を回復させる
									if (partsHP[j] < partsInitHP[j])
									{//最大値以上にはならない
										partsHP[j] += partsRecovery[1];//パーツの耐久を回復
									}
								}
							}
						}

						parts02Arive[i] = false;//パーツ2を消滅
						parts02Pos[i] = deadPos;//画面外に移動
						parts02[i]->SetPosition(parts02Pos[i]);
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

						for (int j = 0; j < 4; j++)
						{
							if (partsSelect[j])
							{
								if (partsNum[j] != 0.0f)
								{//選択中のパーツが破壊される
									partsNum[j] = 0.0f;//パーツを通常弾に戻す
									partsCD[j] = partsInitCD[0];//パーツのCDを通常弾に戻す
									partsHP[j] = partsInitHP[0];//パーツのHPを初期化
									isChange = true;//変更開始
								}
								else if (partsNum[j] == 0.0f)
								{//ゲームオーバー
									scene = End;
								}
							}
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

						for (int j = 0; j < 4; j++)
						{
							if (partsSelect[j])
							{
								if (partsNum[j] != 0.0f)
								{//選択中のパーツが破壊される
									partsNum[j] = 0.0f;//パーツを通常弾に戻す
									partsCD[j] = partsInitCD[0];//パーツのCDを通常弾に戻す
									partsHP[j] = partsInitHP[0];//パーツのHPを初期化
									isChange = true;//変更開始
								}
								else if (partsNum[j] == 0.0f)
								{//ゲームオーバー
									scene = End;
								}
							}
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
				int enemyNum = rand() % 2;//出現させる敵の種類をランダムに選択
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
						enemySpawnTimer = 0;//タイマーをリセット
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
						enemySpawnTimer = 0;//タイマーをリセット
						enemy02[i]->SetPosition(e02Pos[i]);
						break;
					}
				}
			}
		}

		//一定時間経過したらリザルトシーンに移行
		score += 1.0f;//距離加算
		gameTime += 1.0f;//タイム処理
		if (gameTime >= gameTimeLimit) { scene = End; }
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
			shotNum = 0;//攻撃パターン
			isShot = false;//攻撃中かどうか
			isChange = false;//プレイヤーのパーツ変更中かどうか
			enemySpawnTimer = 0.0f;//敵の出現までのカウント
			enemySpawnInterval = 60.0f;//敵出現までの間隔
			gameTime = 0.0f;//ゲーム経過時間
			score = 0.0f;//進んだ距離

			//レーン
			railPos[0] = { -8.0f, 2.0f, 0.0f };
			railPos[1] = { -4.0f, 2.0f, 0.0f };
			railPos[2] = { 0.0f, 2.0f, 0.0f };
			railPos[3] = { 4.0f, 2.0f, 0.0f };
			railPos[4] = { 8.0f, 2.0f, 0.0f };
			
			for (int i = 0; i < 4; i++)
			{
				partsNum[i] = 0.0f;//装着中のパーツの種類
				partsCD[i] = 0.0f;//パーツのCD
				partsHP[i] = 0.0f;//パーツの耐久値
				partsSelect[i] = false;//パーツが選択中かどうか
			}

			partsSelect[0] = true;//0を選択中にする

			for (int i = 0; i < 100; i++)
			{
				e01Pos[i] = deadPos;//敵1の座標
				e01Rot[i] = { 0.0f, 0.0f, 0.0f };//敵1の回転
				e02Pos[i] = deadPos;//敵2の座標
				e02Rot[i] = { 0.0f, 0.0f, 0.0f };//敵2の回転
				pBullPos[i] = deadPos;//プレイヤーの弾の座標
				eBullPos[i] = deadPos;//プレイヤーの弾の座標
				parts01Pos[i] = deadPos;//パーツ1の座標
				parts02Pos[i] = deadPos;//パーツ2の座標
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
				enemy01Arive[i] = false;//敵1が生きているかの判定
				enemy02Arive[i] = false;//敵2が生きているかの判定
			}

			scene = Title;//シーン移行
		}
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
		pBull[i]->Update();
		eBull[i]->Update();
		parts01[i]->Update();
		parts02[i]->Update();
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
	if (scene == Title) { titleSprite->Draw(); }
	else if (scene == Game) { gameSprite->Draw(); }
	else if (scene == End) { resultSprite->Draw(); }

	Sprite::PostDraw();//スプライト描画後処理
	dxCommon->ClearDepthBuffer();//深度バッファクリア

	//水オブジェクトの描画
	WaterObject::PreDraw(cmdList);//水オブジェクト描画前処理

	if (scene == Game) { waterPlane->Draw(); }
	
	WaterObject::PostDraw();//水オブジェクト描画後処理

	//3Dオブジェクトの描画
	Object3d::PreDraw(cmdList);//オブジェクト描画前処理

	if (scene == Game)
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
			if (pBullArive[i]) { pBull[i]->Draw(); }
			if (eBullArive[i]) { eBull[i]->Draw(); }
			if (parts01Arive[i]) { parts01[i]->Draw(); }
			if (parts02Arive[i]) { parts02[i]->Draw(); }
			if (pPartsArive[i]) { playerParts[i]->Draw(); }
		}
	}

	Object3d::PostDraw();//オブジェクト描画後処理

	//パーティクルの描画
	particleMan->Draw(cmdList);

	//前景スプライト描画
	//前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	//デバッグテキストの描画
	debugText->DrawAll(cmdList);

	//スプライト描画後処理
	Sprite::PostDraw();
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