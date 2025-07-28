//====================================
//
// テクスチャ処理 [ texture.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _TEXTURE_H_ // このマクロ定義がされてなかったら
#define _TEXTURE_H_ // 2重インクルード防止のマクロ定義

//*****************************
// テクスチャ管理クラスを定義
//*****************************
class CTexture
{
public:
	CTexture();
	~CTexture();

	HRESULT Load(void);
	void UnLoad(void);
	int Register(const char* pFileName);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

	static inline constexpr int NUM_TEXTURE = 128;	// 最大テクスチャ数

private:
	// テクスチャ読み込み配列を作成
	const char* TexName[NUM_TEXTURE] =
	{
		"data\\TEXTURE\\explosion000.png",
		"data\\TEXTURE\\effect000.jpg",
		"data\\TEXTURE\\bullet002.png",
		"data\\TEXTURE\\num001.png",
		"data\\TEXTURE\\number000.png",
		"data\\TEXTURE\\field100.jpg",
		"data\\TEXTURE\\trash.png",
		"data\\TEXTURE\\billboard_wepon.png",
		"data\\TEXTURE\\field000.jpeg",
		"data\\TEXTURE\\bg.jpeg",
		"data\\TEXTURE\\DomeTex.jpg",
		"data\\TEXTURE\\shadow000.jpg",
		"data\\TEXTURE\\lifegage.png",
		"data\\TEXTURE\\lifeframe.png",
		"data\\TEXTURE\\score001.png",
		"data\\TEXTURE\\pause_retry.png",
		"data\\TEXTURE\\pause_continue.png",
		"data\\TEXTURE\\pause_quit.png",
		"data\\TEXTURE\\PauseBack.png",
		"data\\TEXTURE\\title000.png",
		"data\\TEXTURE\\title001.png",
		"data\\TEXTURE\\tutorial_menu.png",
	};

	LPDIRECT3DTEXTURE9 m_apTexture[NUM_TEXTURE];	// テクスチャポインタ
	static int m_nNumAll;							// テクスチャカウント変数
};

#endif