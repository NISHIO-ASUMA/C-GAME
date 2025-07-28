//=========================================
//
// ボス体力処理 [ bosslifegage.h ]
// Author: Asuma Nishio
//
//=========================================

#ifndef _BOSSLIFEGAGE_H_ // このマクロ定義がされてなかったら
#define _BOSSLIFEGAGE_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "object.h"
#include "object2D.h"

//**********************
// 前方宣言
//**********************
class CBoss;

//***************************
// ボス体力管理クラスを定義
//***************************
class CBossLifeGage : public CObject2D
{
public:
	CBossLifeGage(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CBossLifeGage();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBossLifeGage* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);

private:
	int m_nIdxTex;	// テクスチャインデックス
	CBoss* m_pBoss; // ボスのポインタ

};

#endif

