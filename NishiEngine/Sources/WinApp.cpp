﻿#include "WinApp.h"
//#include <imgui_impl_win32.h>

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

const wchar_t WinApp::winName[] = L"SEA STORM";//名前
const int WinApp::winWidth = 1280;//横幅
const int WinApp::winHeight = 720;//縦幅

//ウィンドウプロシージャ
LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	//{
	//	return 1;
	//}

	//メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY://ウィンドウが破棄された
		PostQuitMessage(0);//OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // 標準の処理を行う
}

void WinApp::CreateWin()
{
	//ウィンドウクラスの設定
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WindowProc;//ウィンドウプロシージャ
	wndClass.lpszClassName = winName;//ウィンドウクラス名
	wndClass.hInstance = GetModuleHandle(nullptr);//ウィンドウハンドル
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定

	RegisterClassEx(&wndClass);//ウィンドウクラスをOSに登録

	//ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, winWidth, winHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//自動でサイズ補正

	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(wndClass.lpszClassName,//クラス名
		winName,//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,//タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT,//表示X座標（OSに任せる）
		CW_USEDEFAULT,//表示Y座標（OSに任せる）
		wrc.right - wrc.left,//ウィンドウ横幅
		wrc.bottom - wrc.top,//ウィンドウ縦幅
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		wndClass.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//オプション

	ShowWindow(hwnd, SW_SHOWMAXIMIZED);//ウィンドウ表示
}

void WinApp::TerminateWin()
{
	//ウィンドウクラスを登録解除
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}

bool WinApp::MessageLoop()
{
	MSG msg{};//メッセージ

	//メッセージがある？
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);//キー入力メッセージの処理
		DispatchMessage(&msg);//ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT)//終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}