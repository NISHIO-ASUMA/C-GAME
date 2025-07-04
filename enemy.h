//====================================
//
// 敵の処理 [ enemy.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _ENEMY_H_ // このマクロ定義がされてなかったら
#define _ENEMY_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "character.h"

//**************************
// 敵クラスを定義
//**************************
class CEnemy : public CObject2D
{
public:
	typedef enum
	{
		ENEMYTYPE_NONE = 0,
		ENEMYTYPE_FACE,
		ENEMYTYPE_GOLD,
		ENEMYTYPE_MAX
	}ENEMYTYPE;

	typedef enum
	{
		ENEMYSTATE_NONE = 0,
		ENEMYSTATE_NORMAL,
		ENEMYSTATE_DAMAGE,
		ENEMYSTATE_MAX
	}ENEMYSTATE;

	CEnemy(int nPriority = 5);
	~CEnemy();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemy* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, ENEMYTYPE nType);
	static int GetEnemy(void) { return m_NumEnemy; }

	void SetType(const ENEMYTYPE type);
	ENEMYTYPE GetType(void) const { return m_Type; }
	void HitEnemy(int nDamage);

	void SetTexture(ENEMYTYPE Type);

private:
	D3DXVECTOR3 m_move;		// 移動量
	ENEMYTYPE m_Type;				// 種類
	ENEMYSTATE m_State;		// 敵の状態
	int m_StateCount;		// 状態管理用

	static int m_NumEnemy;			// 敵数管理
	int m_nIdxTexture;
	int m_nInterval;
};
#endif
