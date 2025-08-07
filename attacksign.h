//====================================
//
// 攻撃前兆演出 [ attacksign.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _ATTACKSIGN_H_ // このマクロ定義がされてなかったら
#define _ATTACKSIGN_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "billboard.h"

//**********************
// 攻撃前兆クラスを定義
//**********************
class CAttackSign : public CBillboard
{
public:
	// コンストラクタ・デストラクタ
	CAttackSign(int nPriority = static_cast<int>(CObject::PRIORITY::BILLBOARD));
	~CAttackSign();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// セッター
	void SetTexture(void);

	// 静的メンバ関数
	static CAttackSign* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	int m_nidxTex;		// テクスチャインデックス
};

#endif

