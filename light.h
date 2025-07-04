//====================================
//
// ライト処理 [ light.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _LIGHT_H_ // このマクロ定義がされてなかったら
#define _LIGHT_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************


//**********************
// マクロ定義
//**********************
#define NUM_LIGHT (3)

//**********************
// ライトクラスを定義
//**********************
class CLight
{
public:
	CLight();
	~CLight();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	D3DLIGHT9 m_aLight[NUM_LIGHT]; // ライト数
	D3DXVECTOR3 m_vecDir[NUM_LIGHT];	// ベクトル
};

#endif