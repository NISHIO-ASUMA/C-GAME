//====================================
//
// テクスチャ処理 [ texture.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _TEXTURE_H_ // このマクロ定義がされてなかったら
#define _TEXTURE_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************


//**********************
// マクロ定義
//**********************
#define MAX_TEXTURE (128)	// 最大テクスチャ枚数

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

private:
	// テクスチャ読み込み配列を作成
	const char* TexName[MAX_TEXTURE] =
	{
		"data\\TEXTURE\\gold.jpg",
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
	};

	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE];	// テクスチャポインタ
	static int m_nNumAll;							// テクスチャカウント変数
};

#endif