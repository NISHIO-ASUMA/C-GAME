//====================================
//
// �I�u�W�F�N�g���� [ object.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "object.h"
#include "object2D.h"
#include "Manager.h"
#include "debugproc.h"

//**********************
// �ÓI�����o�ϐ��錾
//**********************
CObject* CObject::m_apObject[PRIORITY_MAX][MAX_OBJECT] = {}; // �I�u�W�F�N�g�̃|�C���^
int CObject::m_nNumAll = 0;									 // �I�u�W�F�N�g����

//===============================
// �I�[�o�[���[�h�R���X�g���N�^
//===============================
CObject::CObject(int nPriority)
{
	// �S�I�u�W�F�N�g����
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		// NULL�`�F�b�N
		if (m_apObject[nPriority][nCntObj] == nullptr)
		{
			m_apObject[nPriority][nCntObj] = this; // �������g����
			m_nID = nCntObj;			// ID�ێ�
			m_nNumAll++;				// ���������Z
			m_nPriority = nPriority;	// �v���C�I���e�B��ێ�
			break;
		}
	}

	// �l�̃N���A
	m_Type = TYPE_NONE;
}
//===============================
// �f�X�g���N�^
//===============================
CObject::~CObject()
{
	// ����
}
//===============================
// �S�������
//===============================
void CObject::ReleaseAll(void)
{
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nCntPri][nCnt] != nullptr)
			{
				// nullptr����Ȃ�������
				m_apObject[nCntPri][nCnt]->Uninit();
			}
		}
	}
}
//===============================
// �S�X�V����
//===============================
void CObject::UpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nCntPri][nCnt] != nullptr)
			{
				// nullptr����Ȃ�������
				m_apObject[nCntPri][nCnt]->Update();
			}
		}
	}
}
//===============================
// �S�`�揈��
//===============================
void CObject::DrawAll(void)
{
	// �J�����̎擾
	CCamera* pCamera = CManager::GetCamera();

	// �J�����̃Z�b�g
	pCamera->SetCamera();

	// �ő啪��
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nCntPri][nCnt] != nullptr)
			{
				// nullptr����Ȃ�������
				m_apObject[nCntPri][nCnt]->Draw();
			}
		}
	}

	// �f�o�b�O�t�H���g
	CDebugproc::Print("���݂̃I�u�W�F�N�g�� : %d", m_nNumAll);
	CDebugproc::Draw(0, 120);
}
//===============================
// ��ނ��Z�b�g
//===============================
void CObject::SetObjType(TYPE type)
{
	m_Type = type;
}
//===============================
// ��ނ��擾
//===============================
CObject::TYPE CObject::GetObjType(void)
{
	return m_Type;
}
//===============================
// �I�u�W�F�N�g���擾
//===============================
CObject* CObject::GetObject(int nPriority,int nIdx)
{
	// nullptr�`�F�b�N
	if (nIdx < 0 || nIdx >= m_nNumAll)
	{
		// �����Ō��ʂ�Ԃ�
		return nullptr;
	}

	// nullptr����Ȃ�������
	return m_apObject[nPriority][nIdx];
}
//===============================
// �I�u�W�F�N�g�̔j��
//===============================
void CObject::Release(void)
{
	// �������g��ID����
	int nId = this->m_nID;

	// �������g�̃v���C�I���e�B�ԍ�����
	int nPrio = this->m_nPriority;

	// nullptr����Ȃ�������
	if (m_apObject[nPrio][nId] != nullptr)
	{
		// �j��
		delete m_apObject[nPrio][nId];

		// nullptr�ɂ���
		m_apObject[nPrio][nId] = nullptr;

		// �������f�N�������g
		m_nNumAll--;
	}
}
