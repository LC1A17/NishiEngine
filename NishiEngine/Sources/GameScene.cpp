#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include <imgui.h>

using namespace DirectX;

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	safe_delete(lightGroup);

	//モデル関連
	safe_delete(skydomeModel);
	safe_delete(groundModel);
	safe_delete(playerModel);
	safe_delete(pBulletModel);
	safe_delete(enemyModel);
	safe_delete(partsModel);
	safe_delete(playerPartsModel);

	//オブジェクト関連
	safe_delete(skydome);
	safe_delete(ground);
	safe_delete(player);
	//safe_delete(pBullet);
	//safe_delete(enemy);
	//safe_delete(parts);
	//safe_delete(sphere);

	//スプライト関連
	safe_delete(gameBack);
	safe_delete(resultBack);
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
	if (!Sprite::LoadTexture(1, L"Resources/sprite/back.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(3, L"Resources/sprite/result.png"))
	{
		assert(0);
		return;
	}

	//背景スプライト生成
	gameBack = Sprite::Create(1, { 0.0f,0.0f });
	resultBack = Sprite::Create(3, { 0.0f,0.0f });

	//パーティクルマネージャ生成
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(camera);

	//テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/sprite/tex1.png");

	//ライト生成
	lightGroup = LightGroup::Create();
	//3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup);
	WaterObject::SetLightGroup(lightGroup);

	//モデル読み込み
	skydomeModel = Model::CreateFromOBJ("skydome");
	groundModel = Model::CreateFromOBJ("ground");
	playerModel = Model::CreateFromOBJ("player");
	pBulletModel = Model::CreateFromOBJ("pBullet");
	enemyModel = Model::CreateFromOBJ("enemy");
	partsModel = Model::CreateFromOBJ("parts");
	playerPartsModel = Model::CreateFromOBJ("playerParts");

	//3Dオブジェクト生成
	skydome = Object3d::Create(skydomeModel);
	skydome->SetScale({ 3, 3, 3 });
	ground = Object3d::Create(groundModel);
	ground->SetScale({ 10000, 1, 10000 });
	groundWater = WaterObject::Create(groundModel);
	groundWater->SetScale({ 50, 1, 50 });
	player = Object3d::Create(playerModel);

	for (int i = 0; i < 10; i++)
	{
		pBullet[i] = Object3d::Create(pBulletModel);
		enemy[i] = Object3d::Create(enemyModel);
		parts[i] = Object3d::Create(partsModel);
		playerParts[i] = Object3d::Create(playerPartsModel);
	}

	player->SetPosition(pPos);

	//カメラ注視点をセット
	camera->SetTarget(cameraPos);
	camera->SetDistance(6.0f);

	{
		ePos[0] = { 0, 0.5, 30 };
		ePos[1] = { 20, 0.5, 30 };
		ePos[2] = { -20, 0.5, 30 };
		ePos[3] = { 40, 0.5, 30 };
		ePos[4] = { -40, 0.5, 30 };
		ePos[5] = { 0, 0.5, 50 };
		ePos[6] = { 10, 0.5, 50 };
		ePos[7] = { -10, 0.5, 50 };
		ePos[8] = { 20, 0.5, 50 };
		ePos[9] = { -20, 0.5, 50 };
	}

	//各種初期化
	for (int i = 0; i < 10; i++)
	{
		pPartsPos[i] = { 1000, 1000, 1000 };
		pBulletPos[i] = { 1000, 1000, 1000 };
		pBulletArive[i] = false;
		pBulletAriveTime[i] = 0;
		enemyArive[i] = true;
		partsArive[i] = false;
		pPartsArive[i] = false;
		partsPos[i] = { 1000, 1000, 1000 };
		pBullet[i]->SetPosition(pBulletPos[i]);
		parts[i]->SetPosition(partsPos[i]);
		enemy[i]->SetPosition(ePos[i]);
		playerParts[i]->SetPosition(pPartsPos[i]);
		enemy[i]->SetScale({ 2, 2, 2 });
		parts[i]->SetScale({ 2, 2, 2 });
	}
}

void GameScene::Update()
{
	//タイトル
	if (scene == Title)
	{
		//プレイヤー操作関連
		//SPACE入力でゲームへ移行
		if (input->TriggerKey(DIK_SPACE))
		{
			scene = Game;
		}
	}
	//ゲーム
	else if (scene == Game)
	{
		//プレイヤー操作関連
		//デバッグ用
		if (input->TriggerKey(DIK_R))
		{
			scene = End;
		}
		//SPACE入力で弾を発射
		if (input->TriggerKey(DIK_SPACE))
		{
			if (pBulletInterval <= pBulletCount)
			{
				//画面上に存在しない弾を一つ選んで自機の位置にセット
				for (int i = 0; i < 10; i++)
				{
					if (!pBulletArive[i])
					{
						pBulletPos[i] = pPos;
						pBullet[i]->SetPosition({ pBulletPos[i] });
						pBulletArive[i] = true;
						pBulletAriveTime[i] = 0;
						pBulletCount = 0;
						break;
					}
				}
			}
		}

		//WASDキーで移動(カメラも追従)
		if (input->PushKey(DIK_W))
		{
			pPos.z += 0.5f; cameraPos.z += 0.5f;

			for (int i = 0; i < 10; i++)
			{
				if (pPartsArive[i])
				{
					pPartsPos[i].z += 0.5;
				}
			}
		}
		if (input->PushKey(DIK_S))
		{
			pPos.z -= 0.5f; cameraPos.z -= 0.5f;

			for (int i = 0; i < 10; i++)
			{
				if (pPartsArive[i])
				{
					pPartsPos[i].z -= 0.5;
				}
			}
		}
		if (input->PushKey(DIK_A))
		{
			pPos.x -= 0.5f; cameraPos.x -= 0.5f;

			for (int i = 0; i < 10; i++)
			{
				if (pPartsArive[i])
				{
					pPartsPos[i].x -= 0.5;
				}
			}
		}
		if (input->PushKey(DIK_D))
		{
			pPos.x += 0.5f; cameraPos.x += 0.5f;

			for (int i = 0; i < 10; i++)
			{
				if (pPartsArive[i])
				{
					pPartsPos[i].x += 0.5;
				}
			}
		}
		player->SetPosition(pPos);
		camera->SetTarget(cameraPos);

		for (int i = 0; i < 10; i++)
		{
			playerParts[i]->SetPosition({ pPartsPos[i] });
		}

		//システム関連
		{
			//天球の回転
			XMFLOAT3 rotation = skydome->GetRotation();
			rotation.y += 0.05f;
			skydome->SetRotation(rotation);

			XMFLOAT3 waterPosition = groundWater->GetPosition();
			waterPosition.z -= 0.1f;
			groundWater->SetPosition(waterPosition);
		}

		//プレイヤーのパーツ更新
		if (partsCount == 1)
		{
			if (!pPartsArive[0])
			{
				pPartsPos[0] = pPos;
				pPartsPos[0].x -= 1;
				pPartsArive[0] = true;
				playerParts[0]->SetPosition({ pPartsPos[0] });
			}
		}
		else if (partsCount == 2)
		{
			if (!pPartsArive[1])
			{
				pPartsPos[1] = pPos;
				pPartsPos[1].x += 1;
				pPartsArive[1] = true;
				playerParts[1]->SetPosition({ pPartsPos[0] });
			}
		}
		else if (partsCount == 3)
		{
			if (!pPartsArive[2])
			{
				pPartsPos[2] = pPos;
				pPartsPos[2].x -= 2;
				pPartsArive[2] = true;
				playerParts[2]->SetPosition({ pPartsPos[0] });
			}
		}
		else if (partsCount == 4)
		{
			if (!pPartsArive[3])
			{
				pPartsPos[3] = pPos;
				pPartsPos[3].x += 2;
				pPartsArive[3] = true;
				playerParts[3]->SetPosition({ pPartsPos[0] });
			}
		}
		
		//パーツ更新
		for (int i = 0; i < 10; i++)
		{
			if (partsArive[i])
			{
				XMFLOAT3 rotation = parts[i]->GetRotation();
				rotation.y += 15.0f;
				parts[i]->SetRotation(rotation);
			}
		}

		//弾更新
		pBulletCount++;

		for (int i = 0; i < 10; i++)
		{
			if (pBulletArive[i])
			{
				pBulletAriveTime[i]++;
				pBulletPos[i].z += 1.5;
				pBullet[i]->SetPosition({ pBulletPos[i] });
				
				if (pBulletAriveTime[i] >= 40)
				{
					pBulletPos[i] = { 1000, 1000, 1000 };
					pBullet[i]->SetPosition({ pBulletPos[i] });
					pBulletArive[i] = false;
				}
			}
		}

		//弾と敵の当たり判定
		for (int i = 0; i < 10; i++)
		{
			if (pBullet[i])
			{
				for (int j = 0; j < 10; j++)
				{
					if (enemyArive[j])
					{
						float a = pBulletPos[i].x - ePos[j].x;
						float b = pBulletPos[i].z - ePos[j].z;
						float c = sqrt(a * a + b * b);

						if (c <= 3)
						{
							deadEnemyCount++;
							pBulletArive[i] = false;
							enemyArive[j] = false;

							//パーツの生成
							for (int k = 0; k < 10; k++)
							{
								if (!partsArive[k])
								{
									partsPos[k] = ePos[j];
									partsArive[k] = true;
									parts[k]->SetPosition({ partsPos[k] });
									break;
								}
							}

							pBulletPos[i] = { 1000, 1000, 1000 };
							ePos[j] = { 1000, 1000, 1000 };
							pBullet[i]->SetPosition({ pBulletPos[i] });
							enemy[j]->SetPosition({ ePos[j] });
						}
					}
				}
			}
		}

		//パーツとプレイヤーの当たり判定
		for (int i = 0; i < 10; i++)
		{
			if (partsArive[i])
			{
				float a = pPos.x - partsPos[i].x;
				float b = pPos.z - partsPos[i].z;
				float c = sqrt(a * a + b * b);

				if (c <= 3)
				{
					partsCount++;
					partsPos[i] = { 1000, 1000, 1000 };
					partsArive[i] = false;
					parts[i]->SetPosition({ partsPos[i] });
				}
			}
		}

		//敵が全滅したらリザルトに移行
		if (deadEnemyCount >= enemyCount) { scene = End; }
	}
	//リザルト
	else if (scene == End)
	{
		//プレイヤー操作関連
		//SPACE入力でゲームへ移行
		if (input->TriggerKey(DIK_SPACE))
		{
			//初期化処理
			cameraPos = { 0, 4.5, 0 };
			pPos = { 0, 1, 0 };
			domeRot = { 0, 0, 0 };

			deadEnemyCount = 0;
			pBulletCount = 0;
			partsCount = 0;

			groundWater->SetPosition({ 0, 0, 0 });
			player->SetPosition(pPos);
			camera->SetTarget(cameraPos);
			camera->SetDistance(6.0f);
			skydome->SetRotation(domeRot);

			{
				ePos[0] = { 0, 0.5, 30 };
				ePos[1] = { 20, 0.5, 30 };
				ePos[2] = { -20, 0.5, 30 };
				ePos[3] = { 40, 0.5, 30 };
				ePos[4] = { -40, 0.5, 30 };
				ePos[5] = { 0, 0.5, 50 };
				ePos[6] = { 10, 0.5, 50 };
				ePos[7] = { -10, 0.5, 50 };
				ePos[8] = { 20, 0.5, 50 };
				ePos[9] = { -20, 0.5, 50 };
			}

			for (int i = 0; i < 10; i++)
			{
				pPartsPos[i] = { 1000, 1000, 1000 };
				pBulletPos[i] = { 1000, 1000, 1000 };
				pBulletArive[i] = false;
				pBulletAriveTime[i] = 0;
				partsArive[i] = false;
				partsPos[i] = { 1000, 1000, 1000 };
				enemyArive[i] = true;
				pPartsArive[i] = false;
				parts[i]->SetPosition(partsPos[i]);
				enemy[i]->SetPosition(ePos[i]);
				pBullet[i]->SetPosition(pBulletPos[i]);
				playerParts[i]->SetPosition(pPartsPos[i]);
			}

			//シーン移行
			scene = Title;
		}
	}

	//CreateParticles();//パーティクル生成
	lightGroup->Update();
	camera->Update();
	particleMan->Update();
	skydome->Update();
	ground->Update();
	groundWater->Update();
	player->Update();

	for (int i = 0; i < 10; i++)
	{
		pBullet[i]->Update();
		enemy[i]->Update();
		parts[i]->Update();
		playerParts[i]->Update();
	}
}

void GameScene::Draw()
{
	//コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	//背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	//背景スプライト描画
	//gameBack->Draw();

	if (scene == Title)
	{
		gameBack->Draw();
	}
	else if (scene == End)
	{
		gameBack->Draw();
		resultBack->Draw();
	}

	//スプライト描画後処理
	Sprite::PostDraw();
	//深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
	//水オブジェクトの描画
	WaterObject::PreDraw(cmdList);

	if (scene == Game)
	{
		groundWater->Draw();
	}
	
	WaterObject::PostDraw();

	//3Dオブジェクトの描画
	Object3d::PreDraw(cmdList);

	if (scene == Game)
	{
		skydome->Draw();
		//ground->Draw();
		player->Draw();

		for (int i = 0; i < 10; i++)
		{
			if (pBulletArive[i])
			{
				pBullet[i]->Draw();
			}

			if (enemyArive[i])
			{
				enemy[i]->Draw();
			}

			if (partsArive[i])
			{
				parts[i]->Draw();
			}

			if (pPartsArive[i])
			{
				playerParts[i]->Draw();
			}
		}
	}

	Object3d::PostDraw();

	//パーティクルの描画
	particleMan->Draw(cmdList);
#pragma endregion

#pragma region 前景スプライト描画
	//前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	//デバッグテキストの描画
	debugText->DrawAll(cmdList);

	//スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
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