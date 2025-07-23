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
#include "object.h"
#include "objectX.h"

//**************************
// �O���錾
//**************************
class CParameter;
class CStateMachine;
class CEnemyStateBase;

//**************************
// �G�N���X���`
//**************************
class CEnemy : public CObjectX
{
public:
	//*********************
	// �G�̍s���Ǘ��񋓌^  ������X�e�[�g�}�V�[���ɂ���
	//*********************
	enum STATE
	{
		STATE_FALL,
		STATE_ROLLING,
		STATE_DEATH,
		STATE_MAX
	};

	//*********************
	// �G�̎�ޗ񋓌^
	//*********************
	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_RIGHT,
		TYPE_KEFT,
		TYPE_MAX
	};

	// ����͏����@�X�e�[�g�}�V�������
	typedef enum
	{
		ENEMYSTATE_NONE = 0,
		ENEMYSTATE_NORMAL,
		ENEMYSTATE_DAMAGE,
		ENEMYSTATE_MAX
	}ENEMYSTATE;

	CEnemy(int nPriority = static_cast<int>(CObject::PRIORITY::ENEMY));
	~CEnemy();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemy* Create(const char* pFileName, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE nType, int nHp);
	static int GetEnemy(void) { return m_NumEnemy; }

	void SetType(const TYPE type) { m_Type = type; }
	TYPE GetType(void) const { return m_Type; }

	void HitEnemy(int nDamage);
	bool Collision(D3DXVECTOR3* pPos);

	void ChangeState(CEnemyStateBase* pNewState, int id);

private:
	D3DXVECTOR3 m_move;		// �ړ���
	TYPE m_Type;			// ���
	ENEMYSTATE m_State;		// �G�̏��
	int m_StateCount;		// ��ԊǗ��p

	static int m_NumEnemy;	// �G���Ǘ�
	int m_nMoveCount;		// �ړ��p�����Ԃ̃J�E���g

	CParameter* m_pParam;	// �p�����[�^�[�N���X�|�C���^
	CStateMachine* m_pStateMachine;	// �X�e�[�g���N���X�̃|�C���^

};
#endif
