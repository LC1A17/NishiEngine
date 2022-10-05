#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>

class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
	STDMETHOD_(void, OnStreamEnd) (THIS) {};
	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};
	STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) { delete[] pBufferContext; };
	STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
	STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
};

class Audio
{
private://エイリアス
	//Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://サブクラス
	//チャンクヘッダ
	struct Chunk
	{
		char	id[4];//チャンク毎のID
		int		size;//チャンクサイズ
	};
	//RIFFヘッダチャンク
	struct RiffHeader
	{
		Chunk	chunk;//"RIFF"
		char	type[4];//"WAVE"
	};
	//FMTチャンク
	struct FormatChunk
	{
		Chunk		chunk;//"fmt "
		WAVEFORMAT	fmt;//波形フォーマット
	};

public://メンバ関数
	//初期化
	bool Initialize();
	//サウンドファイルの読み込みと再生
	void PlayWave(const char* filename);

private://メンバ変数
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	XAudio2VoiceCallback voiceCallback;
};