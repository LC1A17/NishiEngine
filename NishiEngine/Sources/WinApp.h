#pragma once
#include <Windows.h>

class WinApp
{
public://静的メンバ変数
	//ウィンドウサイズ
	static const wchar_t winName[];//名前
	static const int winWidth;//横幅
	static const int winHeight;//縦幅

public://静的メンバ関数
	//ウィンドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public://メンバ関数
	//ゲームウィンドウの作成
	void CreateWin();
	//ゲームウィンドウの破棄
	void TerminateWin();
	//メッセージの処理
	bool MessageLoop();
	//ウィンドウハンドルの取得
	HWND GetHwnd() { return hwnd; }
	//ウィンドウクラスの取得
	HINSTANCE GetInstance() { return wndClass.hInstance; }

private://メンバ変数
	//Window関連
	HWND hwnd = nullptr;//ウィンドウハンドル
	WNDCLASSEX wndClass{};//ウィンドウクラス
};