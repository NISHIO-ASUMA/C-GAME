//============================================
//
// ��ԊǗ����� [ state.h ]
// Author: Asuma Nishio
//
//=============================================

#ifndef _STATE_H_ // ���̃}�N����`������ĂȂ�������
#define _STATE_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**********************
// ��ԊǗ��N���X���`
//**********************
class CState
{
public:
	//***********************
	// ��Ԃ̎��
	//***********************
	enum STATE
	{
		STATE_NONE,
		STATE_NORMAL,
		STATE_DAMAGE,
		STATE_INVINCIBLE,
		STATE_DEATH,
		STATE_MAX
	};

	CState();
	~CState();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static CState* Create(void);

	void SetState(int state) { m_state = state; }
	int GetState(void) { return m_state; }

private:
	int m_state;		// ��ԊǗ��ϐ�
	int m_StateCount;	// �J�E���^�[
};

#endif
