//============================================
//
// �v���C���[��ԊǗ����� [ playerstate.h ]
// Author: Asuma Nishio
//
//=============================================

#ifndef _PLAYERSTATE_H_ // ���̃}�N����`������ĂȂ�������
#define _PLAYERSTATE_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**************************
// �C���N���[�h�t�@�C���錾
//**************************
#include "state.h"

//**************************
// �O���錾
//**************************
class CPlayer;

//*********************************
// �v���C���[�̏�ԊǗ�
//*********************************
class CPlayerStateBase : public CStateBase
{
public:
	CPlayerStateBase();
	~CPlayerStateBase();

	void SetOwner(CPlayer* pPlayer) { m_pPlayer = pPlayer; } // �|�C���^�Z�b�g

	// �X�e�[�g���n�܂�Ƃ��Ɉ�x�����Ă΂��֐�
	virtual void OnStart() {}

	// �X�e�[�g���X�V�����Ƃ��ɌĂ΂��֐�
	virtual void OnUpdate() {}

	// �X�e�[�g���I�����鎞�Ɉ�x�����Ă΂��֐�
	virtual void OnExit() {}

protected:
	CPlayer* m_pPlayer;		// �v���C���[�̃|�C���^
};

#endif