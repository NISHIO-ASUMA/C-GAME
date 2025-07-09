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
#include "model.h"
#include "motion.h"

//**********************
// �O���錾
//**********************
class CShadow;
class CInputKeyboard;
class CCamera;
class CState;
class CParameter;

//*************************
// �v���C���[�N���X���`
//*************************
class CPlayer : public CObject
{
public:
	CPlayer(int nPriority = static_cast<int>(CObject::PRIORITY::PLAYER));
	~CPlayer();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, const int nIdxParson,const char * pFilename); // ��������

	D3DXVECTOR3 GetPos(void) { return m_pos; } // ���݂̍��W���擾
	D3DXVECTOR3 GetRot(void) { return m_rot; } // ���݂̊p�x���擾
	D3DXVECTOR3 GetRotDest(void) { return m_rotDest; }

	void SetRotDest(D3DXVECTOR3 rotDest) { m_rotDest = rotDest; }

	static CPlayer* GetIdxPlayer(int Idx); // �C���f�b�N�X�ԍ��Ŏ擾
	int GetPlayerIndex() const { return m_nIdxPlayer; }

	CModel* GetModelPartType(CModel::PARTTYPE modelpart);

	static inline constexpr int MAX_MODEL = 19; // �v���C���[�Ŏg�����f���̐�

private:
	void UpdateNeutralAction(CInputKeyboard* pInputKeyboard, D3DXMATRIX pMtx, const D3DXVECTOR3 DestPos); // �ʏ�U���X�V�֐�
	void UpdateMove(const D3DXVECTOR3 DestPos,CInputKeyboard* pInputKeyboard);   // �ړ��X�V�֐�
	void UpdateJumpAction(CInputKeyboard* pInputKeyboard, D3DXMATRIX pMtx, const D3DXVECTOR3 DestMove);

	D3DXVECTOR3 m_move;		// �ړ���
	D3DXVECTOR3 m_rotDest;  // �ړI�p

	D3DXVECTOR3 m_pos;		// �K�w�\���ݒ���W
	D3DXVECTOR3 m_posOld;	// �ߋ��̍��W���
	D3DXVECTOR3 m_rot;		// �p�x
	D3DXMATRIX m_mtxworld;	// ���[���h�}�g���b�N�X

	CModel* m_apModel[MAX_MODEL]; // �g�����f���̃|�C���^
	CMotion* m_pMotion;		// ���[�V�����̃|�C���^
	CShadow* m_pShadow;		// �e�N���X�̃|�C���^�ϐ�
	CState* m_pState;		// ��ԊǗ��N���X�̃|�C���^
	CParameter* m_pParameter; // �p�����[�^�[�N���X�|�C���^

	int m_type;				// ���[�V�����̎�ޕϐ�
	int m_nNumAll;		    // ���f������
	int m_State;		// ��ԊǗ��J�E���^�[
	int m_nIdxTexture;		// �e�N�X�`��ID

	bool m_isLanding;		// ���n����
	bool m_isJump;			// �W�����v����
	bool m_isMoving;
	bool m_isAttack;

	float m_size;
	float m_fAngle;		// ���݂̊p�x

	int m_nIdxPlayer; // �v���C���[�̎��ʔԍ�

	const char* m_pFilename; // �ǂݍ��ރt�@�C����
};

#endif