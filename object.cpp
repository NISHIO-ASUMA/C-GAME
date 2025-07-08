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
#include "Manager.h"
#include "debugproc.h"

//**********************
// �ÓI�����o�ϐ��錾
//**********************
int CObject::m_nNumAll = 0;					// �I�u�W�F�N�g����

CObject* CObject::m_pTop[PRIORITY_MAX] = {};	// �擪�A�h���X
CObject* CObject::m_pCur[PRIORITY_MAX] = {};	// �Ō���A�h���X

//===============================
// �I�[�o�[���[�h�R���X�g���N�^
//===============================
CObject::CObject(int nPriority)
{
	m_Type = TYPE_NONE;					// �I�u�W�F�N�g��ނ�������
	m_nPriority = nPriority;			// �D��x���L�^

	m_pNext = nullptr;					// ���̃|�C���^��nullptr�ɏ�����
	m_pPrev = nullptr;					// �O�̃|�C���^��nullptr�ɏ�����

	m_pPrev = m_pCur[nPriority];		// ���݂̍Ō����O�|�C���^�ɐݒ�

	if (m_pCur[nPriority])
	{// ���łɍŌ��������Ȃ�

		m_pCur[nPriority]->m_pNext = this; // �������玩���ɂȂ���
	}
	else
	{// �擪�����Ȃ���Ύ������擪�ɂȂ�
		
		m_pTop[nPriority] = this;
	}

	m_pCur[nPriority] = this;			// �������Ō���Ƃ��ċL�^
	m_nNumAll++;					    // �������J�E���g�A�b�v
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
	// �D��ԍ�����
	for (int nPri = 0; nPri < PRIORITY_MAX; nPri++)
	{
		// �擪�̃A�h���X���擾����
		CObject* pObj = m_pTop[nPri];

		// �擾�ł�����
		while (pObj != nullptr)
		{
			// ���̃I�u�W�F�N�g��ۑ�����
			CObject* pNext = pObj->m_pNext;

			// �I�u�W�F�N�g�I������
			pObj->Uninit();

			// ���̃I�u�W�F�N�g�ɑ��
			pObj = pNext;
		}

		// �擪�̃|�C���^��nullptr�ɂ���
		m_pTop[nPri] = nullptr;

		// �Ō���̃|�C���^��nullptr�ɂ���
		m_pCur[nPri] = nullptr;
	}
}
//===============================
// �I�u�W�F�N�g�̔j��
//===============================
void CObject::Release(void)
{
	// ���g�̗D��x���擾
	int pri = m_nPriority;

	// �O�̃I�u�W�F�N�g�����݂���ꍇ�́A�����̎���O�̃I�u�W�F�N�g�ɐڑ�
	if (m_pPrev != nullptr)
	{
		m_pPrev->m_pNext = m_pNext;
	}
	else
	{
		// ���������X�g�̐擪�Ȃ�A�g�b�v�����̃I�u�W�F�N�g�ɍX�V
		m_pTop[pri] = m_pNext;
	}

	// ���̃I�u�W�F�N�g�����݂���ꍇ�́A�����̑O�����̃I�u�W�F�N�g�ɐڑ�
	if (m_pNext != nullptr)
	{
		m_pNext->m_pPrev = m_pPrev;
	}
	else
	{
		// ���������X�g�̍Ō���Ȃ�A�Ō����O�̃I�u�W�F�N�g�ɍX�V
		m_pCur[pri] = m_pPrev;
	}

	// �����̑O��|�C���^��������
	m_pNext = nullptr;
	m_pPrev = nullptr;

	// ������1���炷
	m_nNumAll--;

	// ���g������������폜
	delete this;
}
//===============================
// �S�X�V����
//===============================
void CObject::UpdateAll(void)
{
	// �D��ԍ�����
	for (int nPrio = 0; nPrio < PRIORITY_MAX; nPrio++)
	{
		// �擪�A�h���X,�D��ԍ����擾
		CObject* pObject = m_pTop[nPrio];

		// �擾�ł�����
		while (pObject != nullptr)
		{
			// ���̃I�u�W�F�N�g�ۑ�
			CObject* pObjeNext = pObject->m_pNext;

			// �I�u�W�F�N�g�X�V
			pObject->Update();

			// ���̃I�u�W�F�N�g����
			pObject = pObjeNext;
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

	// �D��ԍ�����
	for (int nPrio = 0; nPrio < PRIORITY_MAX; nPrio++)
	{
		// �擪�A�h���X,�D��ԍ����擾
		CObject* pObject = m_pTop[nPrio];

		// �擾�ł�����
		while (pObject != nullptr)
		{
			// ���̃I�u�W�F�N�g�ۑ�
			CObject* pObjeNext = pObject->m_pNext;

			// �I�u�W�F�N�g�`��
			pObject->Draw();

			// ���̃I�u�W�F�N�g����
			pObject = pObjeNext;
		}
	}

	// �f�o�b�O�t�H���g
	CDebugproc::Print("���݂̃I�u�W�F�N�g�� : %d", m_nNumAll);
	CDebugproc::Draw(0, 120);
}
