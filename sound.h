//====================================
//
// サウンド処理 [ sound.h ]
// Author: Asuma Nishio
//
//=====================================

//**************************
// 二重インクルードガード
//**************************
#ifndef _SOUND_H_ 
#define _SOUND_H_ 

//**************************
// インクルードファイル宣言
//**************************


//******************************
// サウンドクラスを定義
//******************************
class CSound
{
public:
	// サウンドの種類列挙型
	typedef enum
	{
		SOUND_LABEL_TITLE_BGM = 0,	// タイトルBGM
		SOUND_LABEL_BREAKSUPON_SE,  // スポーン破壊
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	// サウンド情報の構造体定義
	typedef struct
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
	} SOUNDINFO;

	// サウンドの情報列挙型宣言
	SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
	{
		{"data/BGM/titlebgm.wav", -1},	 // タイトル
		{"data/SE/Break.wav",0},		 // スポーン破壊
	};

	IXAudio2* m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
	int m_SoundCount;										// サウンドのカウント
	int m_SoundInterval;									// インターバルカウント

};
#endif