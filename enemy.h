//====================================
//
// �G�̏��� [ enemy.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _ENEMY_H_ // ���̃}�N����`������ĂȂ�������
#define _ENEMY_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "character.h"

//**************************
// �G�N���X���`
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
	D3DXVECTOR3 m_move;		// �ړ���
	ENEMYTYPE m_Type;				// ���
	ENEMYSTATE m_State;		// �G�̏��
	int m_StateCount;		// ��ԊǗ��p

	static int m_NumEnemy;			// �G���Ǘ�
	int m_nIdxTexture;
	int m_nInterval;
};
#endif
