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
	CUi(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CUi();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CUi* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

	void SetTexture(void);
	void SetBasePos(D3DXVECTOR3 pos) { m_BasePos = pos; }

private:
	D3DXVECTOR3 m_BasePos;    // 解像度に依存しない基準座標
	float m_BaseWidth;  // 基準サイズの横幅
	float m_BaseHeight; // 基準サイズの高さ
	bool m_bUseBasePos; // 基準座標設定

};

#endif