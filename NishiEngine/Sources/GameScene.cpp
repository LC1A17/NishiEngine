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
	if (!Sprite::LoadTexture(1, L"Resources/sprite/back.png"))
	{
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(4, L"Resources/sprite/title.png"))
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
	titleBack = Sprite::Create(4, { 0.0f,0.0f });

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

	for (int i = 0; i < 100; i++)
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

	//各種初期化
	for (int i = 0; i < 100; i++)
	{
		ePos[i] = { 1000.0f, 1000.0f, 1000.f };
		pPartsPos[i] = { 1000, 1000, 1000 };
		pBulletPos[i] = { 1000, 1000, 1000 };
		pBulletArive[i] = false;
		pBulletAriveTime[i] = 0;
		enemyArive[i] = false;
		partsArive[i] = false;
		pPartsArive[i] = false;
		partsPos[i] = { 1000, 1000, 1000 };
		pBullet[i]->SetPosition(pBulletPos[i]);
		parts[i]->SetPosition(partsPos[i]);
		enemy[i]->SetPosition(ePos[i]);
		playerParts[i]->SetPosition(pPartsPos[i]);
		enemy[i]->SetScale({ 1, 1, 1 });
		parts[i]->SetScale({ 1, 1, 1 });
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
				for (int i = 0; i < 100; i++)
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

				if (pPartsArive[0])
				{
					//画面上に存在しない弾を一つ選んで自機の位置にセット
					for (int i = 0; i < 100; i++)
					{
						if (!pBulletArive[i])
						{
							pBulletPos[i] = pPartsPos[0];
							pBullet[i]->SetPosition({ pBulletPos[i] });
							pBulletArive[i] = true;
							pBulletAriveTime[i] = 0;
							pBulletCount = 0;
							break;
						}
					}
				}

				if (pPartsArive[1])
				{
					//画面上に存在しない弾を一つ選んで自機の位置にセット
					for (int i = 0; i < 100; i++)
					{
						if (!pBulletArive[i])
						{
							pBulletPos[i] = pPartsPos[1];
							pBullet[i]->SetPosition({ pBulletPos[i] });
							pBulletArive[i] = true;
							pBulletAriveTime[i] = 0;
							pBulletCount = 0;
							break;
						}
					}
				}

				if (pPartsArive[2])
				{
					//画面上に存在しない弾を一つ選んで自機の位置にセット
					for (int i = 0; i < 100; i++)
					{
						if (!pBulletArive[i])
						{
							pBulletPos[i] = pPartsPos[2];
							pBullet[i]->SetPosition({ pBulletPos[i] });
							pBulletArive[i] = true;
							pBulletAriveTime[i] = 0;
							pBulletCount = 0;
							break;
						}
					}
				}

				if (pPartsArive[3])
				{
					//画面上に存在しない弾を一つ選んで自機の位置にセット
					for (int i = 0; i < 100; i++)
					{
						if (!pBulletArive[i])
						{
							pBulletPos[i] = pPartsPos[3];
							pBullet[i]->SetPosition({ pBulletPos[i] });
							pBulletArive[i] = true;
							pBulletAriveTime[i] = 0;
							pBulletCount = 0;
							break;
						}
					}
				}
			}
		}

		//WASDキーで移動(カメラも追従)
		if (input->TriggerKey(DIK_W))
		{
			if (pPos.y < 4.0f)
			{
				pPos.y += 1.0f;

				for (int i = 0; i < 100; i++)
				{
					if (pPartsArive[i])
					{
						pPartsPos[i].y += 1.0f;
					}
				}
			}
		}
		if (input->TriggerKey(DIK_S))
		{
			if (pPos.y > 0.0f)
			{
				pPos.y -= 1.0f;

				for (int i = 0; i < 100; i++)
				{
					if (pPartsArive[i])
					{
						pPartsPos[i].y -= 1.0f;
					}
				}
			}
		}
		if (input->TriggerKey(DIK_A))
		{
			if (pPos.x > -4.0f)
			{
				pPos.x -= 2.0f; skydomePos.x -= 2.0f;

				for (int i = 0; i < 100; i++)
				{
					if (pPartsArive[i])
					{
						pPartsPos[i].x -= 2.0f;
					}
				}
			}
		}
		if (input->TriggerKey(DIK_D))
		{
			if (pPos.x < 4.0f)
			{
				pPos.x += 2.0f; skydomePos.x += 2.0f;

				for (int i = 0; i < 100; i++)
				{
					if (pPartsArive[i])
					{
						pPartsPos[i].x += 2.0f;
					}
				}
			}
		}

		pPos.z += 0.1f; cameraPos.z += 0.1f; skydomePos.z += 0.1f;

		skydome->SetPosition(skydomePos);
		player->SetPosition(pPos);
		camera->SetTarget(cameraPos);

		for (int i = 0; i < 100; i++)
		{
			if (pPartsArive[i])
			{
				pPartsPos[i].z += 0.1f;
			}

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

		//敵のスポーン
		enemySpawnTimer++;

		//画面に出現していない敵をスポーンさせる
		if (enemySpawnTimer > enemySpawnInterval)
		{
			for (int i = 0; i < 100; i++)
			{
				if (!enemyArive[i])
				{
					int xPos = rand() % 5;
					if (xPos == 0) { xPos = -4.0f; }
					else if (xPos == 1) { xPos = -2.0f; }
					else if (xPos == 2) { xPos = 0.0f; }
					else if (xPos == 3) { xPos = 2.0f; }
					else if (xPos == 4) { xPos = 4.0f; }

					int yPos = rand() % 5;
					if (yPos == 0) { yPos = 0.0f; }
					else if (yPos == 1) { yPos = 1.0f; }
					else if (yPos == 2) { yPos = 2.0f; }
					else if (yPos == 3) { yPos = 3.0f; }
					else if (yPos == 4) { yPos = 4.0f; }

					ePos[i].x = xPos;
					ePos[i].y = yPos;
					ePos[i].z = pPos.z + 50.0f;
					enemyArive[i] = true;
					enemySpawnTimer = 0;
					enemy[i]->SetPosition(ePos[i]);
					break;
				}
			}
		}
		
		//パーツ更新
		for (int i = 0; i < 100; i++)
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

		for (int i = 0; i < 100; i++)
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
		for (int i = 0; i < 100; i++)
		{
			if (pBullet[i])
			{
				for (int j = 0; j < 100; j++)
				{
					if (enemyArive[j])
					{
						float a = pBulletPos[i].x - ePos[j].x;
						float b = pBulletPos[i].y - ePos[j].y;
						float c = pBulletPos[i].z - ePos[j].z;
						float d = sqrt(a * a + b * b + c * c);

						if (d <= 1)
						{
							deadEnemyCount++;
							pBulletArive[i] = false;
							enemyArive[j] = false;

							//パーツの生成
							for (int k = 0; k < 100; k++)
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

		//敵とプレイヤーの当たり判定
		for (int i = 0; i < 100; i++)
		{
			if (enemyArive[i])
			{
				float a = pPos.x - ePos[i].x;
				float b = pPos.y - ePos[i].y;
				float c = pPos.z - ePos[i].z;
				float d = sqrt(a * a + b * b + c * c);

				if (d <= 1)
				{
					partsCount--;
					ePos[i] = { 1000, 1000, 1000 };
					enemyArive[i] = false;

					if (partsCount == 0)
					{
						pPartsPos[0] = { 1000, 1000, 1000 };
						pPartsArive[0] = false;
						playerParts[0]->SetPosition({ pPartsPos[0] });
					}
					else if (partsCount == 1)
					{
						pPartsPos[1] = { 1000, 1000, 1000 };
						pPartsArive[1] = false;
						playerParts[1]->SetPosition({ pPartsPos[1] });
					}
					else if (partsCount == 2)
					{
						pPartsPos[2] = { 1000, 1000, 1000 };
						pPartsArive[2] = false;
						playerParts[2]->SetPosition({ pPartsPos[2] });
					}
					else if (partsCount == 3)
					{
						pPartsPos[3] = { 1000, 1000, 1000 };
						pPartsArive[3] = false;
						playerParts[3]->SetPosition({ pPartsPos[3] });
					}
					else if (partsCount < 0)
					{
						scene = End;
					}
				}
			}
		}

		//パーツとプレイヤーの当たり判定
		for (int i = 0; i < 100; i++)
		{
			if (partsArive[i])
			{
				float a = pPos.x - partsPos[i].x;
				float b = pPos.y - partsPos[i].y;
				float c = pPos.z - partsPos[i].z;
				float d = sqrt(a * a + b * b + c * c);

				if (d <= 1)
				{
					if (partsCount < 4)
					{
						partsCount++;
					}

					partsPos[i] = { 1000, 1000, 1000 };
					partsArive[i] = false;
					parts[i]->SetPosition({ partsPos[i] });
				}
			}
		}

		//プレイヤーのパーツ更新
		if (partsCount == 1)
		{
			if (!pPartsArive[0])
			{
				pPartsPos[0] = pPos;
				pPartsPos[0].x -= 2.0f;
				pPartsArive[0] = true;
				playerParts[0]->SetPosition({ pPartsPos[0] });
			}
		}
		else if (partsCount == 2)
		{
			if (!pPartsArive[1])
			{
				pPartsPos[1] = pPos;
				pPartsPos[1].x += 2.0f;
				pPartsArive[1] = true;
				playerParts[1]->SetPosition({ pPartsPos[1] });
			}
		}
		
		else if (partsCount == 3)
		{
			if (!pPartsArive[2])
			{
				pPartsPos[2] = pPos;
				pPartsPos[2].y += 1.0f;
				pPartsArive[2] = true;
				playerParts[2]->SetPosition({ pPartsPos[2] });
			}
		}
		else if (partsCount == 4)
		{
			if (!pPartsArive[3])
			{
				pPartsPos[3] = pPos;
				pPartsPos[3].y -= 1.0f;
				pPartsArive[3] = true;
				playerParts[3]->SetPosition({ pPartsPos[3] });
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
			pPos = { 0, 2, 3 };
			skydomePos = { 0, 0, 0 };
			domeRot = { 0, 0, 0 };

			enemySpawnTimer = 0;
			deadEnemyCount = 0;
			pBulletCount = 0;
			partsCount = 0;

			groundWater->SetPosition({ 0, 0, 0 });
			player->SetPosition(pPos);
			camera->SetTarget(cameraPos);
			camera->SetDistance(6.0f);
			skydome->SetPosition(skydomePos);
			skydome->SetRotation(domeRot);

			for (int i = 0; i < 100; i++)
			{
				ePos[i] = { 1000, 1000, 1000 };
				pPartsPos[i] = { 1000, 1000, 1000 };
				pBulletPos[i] = { 1000, 1000, 1000 };
				pBulletArive[i] = false;
				pBulletAriveTime[i] = 0;
				partsArive[i] = false;
				partsPos[i] = { 1000, 1000, 1000 };
				enemyArive[i] = false;
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

	for (int i = 0; i < 100; i++)
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
		titleBack->Draw();
	}
	else if (scene == Game)
	{
		gameBack->Draw();
	}
	else if (scene == End)
	{
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

		for (int i = 0; i < 100; i++)
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