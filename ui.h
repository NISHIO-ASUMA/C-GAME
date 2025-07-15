//====================================
//
// UI処理 [ ui.h ]
// Author: Asuma Nishio
//
//=====================================
#ifndef _UI_H_ // このマクロ定義がされてなかったら
#define _UI_H_ // 2重インクルード防止のマクロ定義

//****************************
// インクルードファイル宣言
//****************************
#include "object2D.h"

//****************************
// UIクラスを定義
//****************************
class CUi : public CObject2D
{
public:

	//**********************************************
	// UIの種類定義  ( TODO : 後ほど列挙名の変更 )
	//***********************************************
	enum SCENETYPE
	{
		SCENETYPE_NONE,
		SCENETYPE_GAME,
		SCENETYPE_MAX
	};

	CUi(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CUi();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CUi* Create(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight);

	void SetTexture(int nType);

private:
	int m_nTexIdxType;		// テクスチャインデックス

	//****************
	// UI構造体定義  
	//****************
	struct UI
	{

	};

	UI* m_pUi; // 構造体変数
};

#endif