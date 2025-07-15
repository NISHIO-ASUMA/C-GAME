//======================================
//
// UI全体を管理する処理 [ uimanager.h ]
// Author: Asuma Nishio
//
//======================================

#ifndef _UIMANAGER_H_ // このマクロ定義がされてなかったら
#define _UIMANAGER_H_ // 2重インクルード防止のマクロ定義

//****************************
// インクルードファイル宣言
//****************************
#include "ui.h"
#include <vector>

//****************************
// UI管理クラスを定義
//****************************
class CUimanager
{
public:
	CUimanager();
	~CUimanager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static CUi* Create(D3DXVECTOR3 pos,int nType,float fWidth,float fHeight);

private:
	static std::vector<CUi*>m_Uiobject; // UIの動的配列
	static int m_nIdxUi;			// UIのインデックス番号
};

#endif