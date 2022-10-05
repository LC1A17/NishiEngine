#pragma once
#include <Windows.h>
#include <wrl.h>
#include <dinput.h>
#define DIRECTINPUT_VERSION     0x0800//DirectInputのバージョン指定

class Input
{
public:
	struct MouseMove
	{
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};

private://エイリアス
	//Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://静的メンバ変数
	static Input* GetInstance();

public://メンバ関数
	//初期化
	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	//毎フレーム処理
	void Update();
	//キーの押下をチェック
	bool PushKey(BYTE keyNumber);
	//キーのトリガーをチェック
	bool TriggerKey(BYTE keyNumber);
	//キーの左ボタン押下をチェック
	bool PushMouseLeft();
	//キーの中ボタン押下をチェック
	bool PushMouseMiddle();
	//キーの左ボタントリガーをチェック
	bool TriggerMouseLeft();
	//キーの中ボタントリガーをチェック
	bool TriggerMouseMiddle();
	//マウス移動量を取得
	MouseMove GetMouseMove();

private://メンバ関数
	Input() = default;
	Input(const Input&) = delete;
	~Input() = default;
	Input& operator=(const Input&) = delete;

private://メンバ変数
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devKeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};
};