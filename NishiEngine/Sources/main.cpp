#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "GameScene.h"
#include "LightGroup.h"
#include "ParticleManager.h"

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;	
	Audio* audio = nullptr;
	GameScene* gameScene = nullptr;

	win = new WinApp();
	win->CreateWin();
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(win);

#pragma region
	//入力
	input = Input::GetInstance();
	if (!input->Initialize(win->GetInstance(), win->GetHwnd())) {
		assert(0);
		return 1;
	}
	//オーディオ
	audio = new Audio();
	if (!audio->Initialize()) {
		assert(0);
		return 1;
	}
	//スプライト
	if (!Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::winWidth, WinApp::winHeight)) {
		assert(0);
		return 1;
	}
	//3Dオブジェクト
	Object3d::StaticInitialize(dxCommon->GetDevice());
	//水オブジェクト
	WaterObject::StaticInitialize(dxCommon->GetDevice());
	//ライト
	LightGroup::StaticInitialize(dxCommon->GetDevice());
	//パーティクルマネージャ
	ParticleManager::GetInstance()->Initialize(dxCommon->GetDevice());
#pragma endregion

	//ゲームシーン
	gameScene = new GameScene();
	gameScene->Initialize(dxCommon, input, audio);

	while (true)
	{
		//メッセージループ
		if (win->MessageLoop()) {	break; }

		//入力
		input->Update();
		//ゲームシーン
		gameScene->Update();

		//描画開始
		dxCommon->PreDraw();
		//ゲームシーンの描画
		gameScene->Draw();
		//描画終了
		dxCommon->PostDraw();

		//ESCで強制終了
		if (input->PushKey(DIK_ESCAPE))
		{
			break;
		}
	}
	//各種解放
	safe_delete(gameScene);
	safe_delete(audio);
	safe_delete(dxCommon);

	//ゲームウィンドウの破棄
	win->TerminateWin();
	safe_delete(win);

	return 0;
}