//====================================
//
// ボス処理 [ boss.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _BOSS_H_ // このマクロ定義がされてなかったら
#define _BOSS_H_ // 2重インクルード防止のマクロ定義

//******************************
// インクルードファイル
//******************************
#include "object.h"

//**********************
// 前方宣言
//**********************
class CModel;
class CMotion;

//**********************
// ボスクラスを定義
//**********************
class CBoss : public CObject
{
public:
	//***************************
	// モーション列挙型宣言
	//***************************
	enum TYPE
	{
		TYPE_NEUTRAL = 0,	// ニュートラル
		TYPE_MOVE,			// 移動
		TYPE_ACTION,		// アクション
		TYPE_JUMP,			// ジャンプ
		TYPE_LANDING,		// 着地
		TYPE_MAX
	};

	CBoss(int nPriority = static_cast<int>(CObject::PRIORITY::BOSS));
	~CBoss();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBoss* Create(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPos(void) { return m_pos;}
	D3DXVECTOR3 GetRot(void) { return m_rot; }

private:
	static const int m_nNumModels = 21;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXMATRIX m_mtxworld;	// マトリックス

	CModel* m_pModel[m_nNumModels];
	CMotion* m_pMotion;

	int m_nNumAll;		   // モデル総数
	int m_type;			   // モーションの種類変数

};

#endif
