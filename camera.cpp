//====================================
//
// �J�������� [ camera.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "camera.h"
#include "Manager.h"
#include "input.h"
#include "player.h"
#include "debugproc.h"
#include "object.h"
#include "boss.h"

//**********************
// �}�N����`
//**********************
#define MAX_VIEWUP (3.0f)   // �J�����̐���
#define MAX_VIEWDOWN (0.1f) // �J�����̐���

//=================================
// �R���X�g���N�^
//=================================
CCamera::CCamera()
{
	// �l�̃N���A
	m_pCamera.mtxprojection = {};
	m_pCamera.mtxView = {};
	m_pCamera.posR = VECTOR3_NULL;
	m_pCamera.posV = VECTOR3_NULL;
	m_pCamera.rot = VECTOR3_NULL;
	m_pCamera.vecU = VECTOR3_NULL;
	m_pCamera.posRDest = VECTOR3_NULL;
	m_pCamera.fDistance = NULL;
	m_pCamera.nMode = MODE_NONE;
}
//=================================
// �f�X�g���N�^
//=================================
CCamera::~CCamera()
{
	// ����
}
//=================================
// ����������
//=================================
HRESULT CCamera::Init(void)
{
	// �����l��ݒ肷��
	m_pCamera.posV = D3DXVECTOR3(0.0f, 500.0f, -250.0f);		// �J�����̈ʒu
	m_pCamera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̌��Ă���ʒu
	m_pCamera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// ������x�N�g��
	m_pCamera.rot = D3DXVECTOR3(D3DX_PI * 0.55f, 0.0f, 0.0f);	// �p�x

	// �������v�Z
	float fRotx = m_pCamera.posV.x - m_pCamera.posR.x;
	float fRoty = m_pCamera.posV.y - m_pCamera.posR.y;
	float fRotz = m_pCamera.posV.z - m_pCamera.posR.z;

	// ���_���璍���_�܂ł̋���
	m_pCamera.fDistance = sqrtf((fRotx * fRotx) + (fRoty * fRoty) + (fRotz * fRotz));

	// �������[�h�Z�b�g
	m_pCamera.nMode = MODE_NONE;

	return S_OK;
}
//=================================
// �I������
//=================================
void CCamera::Uninit(void)
{
	// ����
}
//=================================
// �X�V����
//=================================
void CCamera::Update(void)
{
	// ���͏����擾
	CInputMouse* pMouse = CManager::GetMouse();
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// �{�X�擾
	CBoss* pBoss = CManager::GetBoss();

	// �v���C���[�擾
	CPlayer* pPlayer = CPlayer::GetIdxPlayer(0);
	CPlayer* pPlayerSub = CPlayer::GetIdxPlayer(1);

	// nullptr�`�F�b�N
	if (pPlayer == nullptr || pPlayerSub == nullptr)
	{
		// �����ŏ�����Ԃ�
		return;
	}

#ifdef _DEBUG
	// �J�������[�h�ύX
	if (pInput->GetTrigger(DIK_N))
	{
		m_pCamera.nMode = MODE_LOCKON;
	}
	if (pInput->GetTrigger(DIK_M))
	{
		m_pCamera.nMode = MODE_PLAYER;
	}
	if (pInput->GetTrigger(DIK_B))
	{
		m_pCamera.nMode = MODE_NONE;
	}
#endif

	switch (m_pCamera.nMode)
	{
	case MODE_NONE:

		// �}�E�X���_�ړ�
		MouseView(pMouse);

		break;

	case MODE_PLAYER:
		// �Ǐ]�J�����p�ɐݒ�
		m_pCamera.posRDest.x = pPlayer->GetPos().x + sinf(pPlayer->GetRotDest().y) * 1.0f;
		m_pCamera.posRDest.y = pPlayer->GetPos().y + cosf(pPlayer->GetRotDest().y) * 1.0f;
		m_pCamera.posRDest.z = pPlayer->GetPos().z + cosf(pPlayer->GetRotDest().y) * 1.0f;

		m_pCamera.posR.x += ((m_pCamera.posRDest.x - m_pCamera.posR.x) * 0.3f);
		m_pCamera.posR.y += ((m_pCamera.posRDest.y - m_pCamera.posR.y) * 0.3f);
		m_pCamera.posR.z += ((m_pCamera.posRDest.z - m_pCamera.posR.z) * 0.3f);

		// �J�����̎��_�̏��
		m_pCamera.posV.x = m_pCamera.posR.x - sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
		m_pCamera.posV.y = m_pCamera.posR.y - cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
		m_pCamera.posV.z = m_pCamera.posR.z - sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;

		break;

	case MODE_LOCKON:
	{
		// MAIN�v���C���[���W,SUB�v���C���[���W,�{�X���W���擾
		D3DXVECTOR3 playerPos = pPlayer->GetPos();				// MAIN���W
		D3DXVECTOR3 subPlayerPos = pPlayerSub->GetPos();		// SUB���W
		D3DXVECTOR3 bossPos = pBoss->GetPos();					// �{�X���W

		// MAIN�v���C���[�����v�Z
		D3DXVECTOR3 VecToBoss = bossPos - playerPos;

		// �����͖���
		VecToBoss.y = 0.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&VecToBoss, &VecToBoss);

		// �{�X�ւ̊p�x���v�Z
		float fAngleToBoss = atan2f(VecToBoss.x, VecToBoss.z);

		// �v���C���[�̖ړI�p�ɐݒ肷��
		pPlayer->SetRotDest(D3DXVECTOR3(0.0f, fAngleToBoss, 0.0f));

		// SUB�v���C���[�̌����v�Z
		D3DXVECTOR3 VecSubToBoss = bossPos - subPlayerPos;

		// �����͖���
		VecSubToBoss.y = 0.0f;

		// �x�N�g���𐳋K������
		D3DXVec3Normalize(&VecSubToBoss, &VecSubToBoss);

		// �{�X�ւ̊p�x���v�Z
		float fAngleSubToBoss = atan2f(-VecSubToBoss.x, -VecSubToBoss.z);

		// SUB�v���C���[�̖ړI�p�x��ݒ�
		pPlayerSub->SetRotDest(D3DXVECTOR3(0.0f, fAngleSubToBoss, 0.0f));

		// �J�����ʒu��MAIN�v���C���[�̌����
		D3DXVECTOR3 camOffset = -VecToBoss * 280.0f;

		// ������ݒ�
		camOffset.y = 200.0f;

		// �J�����̖ړI�ʒu
		D3DXVECTOR3 desiredPosV = playerPos + camOffset;

		// �^�[�Q�b�g���W���{�X���W�ɐݒ�
		D3DXVECTOR3 targetBoss = bossPos;

		// �������v���C���[�̍����ɍ��킹��
		targetBoss.y = playerPos.y + 50.0f;

		// �J�����ɓK�p����
		m_pCamera.posV += (desiredPosV - m_pCamera.posV) * 0.3f;
		m_pCamera.posR += (targetBoss - m_pCamera.posR) * 0.3f; 
	}
		break;

	default:
		break;
	}

	// �p�x�̐��K��
	if (m_pCamera.rot.y > D3DX_PI)
	{// D3DX_PI���傫���Ȃ�����
		m_pCamera.rot.y -= D3DX_PI * 2.0f;
	}

	// �p�x�̐��K��
	if (m_pCamera.rot.y < -D3DX_PI)
	{// D3DX_PI��菬�����Ȃ�����
		m_pCamera.rot.y += D3DX_PI * 2.0f;
	}
}
//=================================
// �J�������Z�b�g
//=================================
void CCamera::SetCamera(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_pCamera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_pCamera.mtxView,
		&m_pCamera.posV,
		&m_pCamera.posR,
		&m_pCamera.vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_pCamera.mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_pCamera.mtxprojection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_pCamera.mtxprojection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		3000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_pCamera.mtxprojection);

	CDebugproc::Print("Camera : PosV [ %.2f, %.2f, %.2f ]", m_pCamera.posV.x, m_pCamera.posV.y, m_pCamera.posV.z);
	CDebugproc::Draw(0, 100);
}
//======================================
// �}�E�X����̎��_�ړ�
//======================================
void CCamera::MouseView(CInputMouse * pMouse)
{
	// �E�N���b�N
	if (pMouse->GetPress(CInputMouse::MOUSE_LEFT))
	{
		// �}�E�X�̈ړ��ʎ擾
		D3DXVECTOR2 Move = pMouse->GetMouseVelocity();
		D3DXVECTOR2 MoveOld = pMouse->GetMouseOldVelocity();

		// ���݂̊p�x���v�Z
		D3DXVECTOR2 fAngle = Move - MoveOld;

		// ��]�ʂ��X�V
		m_pCamera.rot.y += fAngle.x * 0.01f;
		m_pCamera.rot.x += fAngle.y * 0.01f;

		// ��]�ʂ𐧌�
		if (m_pCamera.rot.x > MAX_VIEWUP)
		{
			m_pCamera.rot.x -= fAngle.y * 0.01f;
		}
		else if (m_pCamera.rot.x < MAX_VIEWDOWN)
		{
			m_pCamera.rot.x -= fAngle.y * 0.01f;
		}

		// �J�������W���X�V
		m_pCamera.posR.x = m_pCamera.posV.x + sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
		m_pCamera.posR.y = m_pCamera.posV.y + cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
		m_pCamera.posR.z = m_pCamera.posV.z + sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
	}
	// ���N���b�N
	else if (pMouse->GetPress(CInputMouse::MOUSE_RIGHT))
	{
		D3DXVECTOR2 Move = pMouse->GetMouseVelocity();
		D3DXVECTOR2 MoveOld = pMouse->GetMouseOldVelocity();

		D3DXVECTOR2 fAngle = Move - MoveOld;

		// ��]�ʂ��X�V
		m_pCamera.rot.y += fAngle.x * 0.01f;
		m_pCamera.rot.x += fAngle.y * 0.01f;

		// ��]�ʂ𐧌�
		if (m_pCamera.rot.x > MAX_VIEWUP)
		{
			m_pCamera.rot.x -= fAngle.y * 0.01f;
		}
		else if (m_pCamera.rot.x < MAX_VIEWDOWN)
		{
			m_pCamera.rot.x -= fAngle.y * 0.01f;
		}

		// �J�����̎��_�̏��
		m_pCamera.posV.x = m_pCamera.posR.x - sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
		m_pCamera.posV.y = m_pCamera.posR.y - cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
		m_pCamera.posV.z = m_pCamera.posR.z - sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
	}

	// ���K��
	if (m_pCamera.rot.y < -D3DX_PI)
	{
		m_pCamera.rot.y += D3DX_PI * 2.0f;
	}
	else if (m_pCamera.rot.y > D3DX_PI)
	{
		m_pCamera.rot.y += -D3DX_PI * 2.0f;
	}
	if (m_pCamera.rot.x < -D3DX_PI)
	{
		m_pCamera.rot.x += D3DX_PI * 2.0f;
	}
	else if (m_pCamera.rot.x > D3DX_PI)
	{
		m_pCamera.rot.x += -D3DX_PI * 2.0f;
	}

}
