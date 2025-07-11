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
#include "model.h"

//**********************
// 前方宣言
//**********************
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
		TYPE_ACTION,		// 腕攻撃
		TYPE_MAX
	};

	//***************************
	// 行動パターン列挙型
	//***************************
	enum ATTACKPATTERN
	{
		PATTERN_NONE, // 何も行動していない
		PATTERN_HAND, // 殴り
		PATTERN_BULLET, // 弾発射
		PATTERN_CIRCLE, // 振り回し攻撃
		PATTERN_MAX
	};

	CBoss(int nPriority = static_cast<int>(CObject::PRIORITY::BOSS));
	~CBoss();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBoss* Create(D3DXVECTOR3 pos, float fSize);

	D3DXVECTOR3 GetPos(void) { return m_pos;}
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	float GetSize(void) { return m_fSize; }

	bool CollisionRightHand(D3DXVECTOR3* pPos);
	CModel*GetModelPartType(CModel::PARTTYPE modelpart);

private:

	static constexpr int NUMMODELS = 21; // 使うモデル数

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXMATRIX m_mtxworld;	// マトリックス

	CModel* m_pModel[NUMMODELS];
	CMotion* m_pMotion;

	int m_type;			   // モーションの種類変数
	float m_fSize;		   // サイズ

	int m_nCoolTime;	// クールタイム
	bool m_isAttacked;  // 攻撃しているか
};

#endif
