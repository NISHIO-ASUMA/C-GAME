//====================================
//
// �v���C���[���� [ player.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _PLAYER_H_ // ���̃}�N����`������ĂȂ�������
#define _PLAYER_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "object.h"
#include "character.h"
#include "model.h"
#include "motion.h"
#include "state.h"

//**********************
// �}�N����`
//**********************
#define MAX_MODEL (19) // �v���C���[���f����

//**********************
// �O���錾
//**********************
class CShadow;
class CInputKeyboard;
class CCamera;

//*************************
// �v���C���[�N���X���`
//*************************
class CPlayer : public CObject
{
public:
	// �v���C���[���
	enum PLAYERSTATE
	{
		PLAYERSTATE_NONE = 0,
		PLAYERSTATE_DAMAGE,
		PLAYERSTATE_NORMAL,
		PLAYERSTATE_APPER,
		PLAYERSTATE_DEATH,
		PLAYERSTATE_MAX
	};

	CPlayer(int nPriority = static_cast<int>(CObject::PRIORITY::PLAYER));
	~CPlayer();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, const int nIdxParson,const char * pFilename); // ��������
	PLAYERSTATE GetState(void) { return m_State; }

	D3DXVECTOR3 GetPos(void) { return m_pos; } // ���݂̍��W���擾
	D3DXVECTOR3 GetRot(void) { return m_rot; } // ���݂̊p�x���擾
	D3DXVECTOR3 GetRotDest(void) { return m_rotDest; }

	void SetRotDest(D3DXVECTOR3 rotDest) { m_rotDest = rotDest; }
	void MoveKey(CInputKeyboard* pInputKeyBoard, CCamera* pCamera);

private:
	void UpdateIdxPlayer(int nIdx, CInputKeyboard* pInputKey);	// �ԍ����Ƃ̍X�V�֐�

	int m_StateCount;		// ��ԊǗ��J�E���^�[
	int m_nIdxTexture;		// �e�N�X�`��ID

	D3DXVECTOR3 m_move;		// �ړ���
	PLAYERSTATE m_State;	// ��ԊǗ�
	D3DXVECTOR3 m_rotDest;  // �ړI�p

	D3DXVECTOR3 m_pos;		// �K�w�\����邽�ߗp
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_rot;		// �p�x
	D3DXMATRIX m_mtxworld;	// �}�g���b�N�X

	CModel* m_apModel[MAX_MODEL]; // �g�����f���̃|�C���^
	int m_nNumAll;		   // ���f������
	CMotion* m_pMotion;		// ���[�V�����̃|�C���^
	int m_type;				// ���[�V�����̎�ޕϐ�
	bool m_isLanding;		// ���n����
	bool m_isJump;			// �W�����v����
	bool m_isMoving;
	float m_size;
	static int m_nIdxPlayer; // �v���C���[�̎��ʔԍ�
	const char* m_pFilename;

	CShadow* m_pShadow;
	CState* m_pState;   // ��ԊǗ��N���X�̃|�C���^

	bool m_isAttack;
};

#endif