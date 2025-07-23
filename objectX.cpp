//====================================
//
// X�t�@�C������ [ objectX.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "objectX.h"
#include "manager.h"

//=============================
// �R���X�g���N�^
//=============================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_dwNumMat = NULL;
	m_mtxWorld = {};
	m_nIdxTexture = NULL;
	m_pBuffMat = nullptr;
	m_pMesh = nullptr;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_pTexture = nullptr;
	m_pFileName = {};

	// �N�H�[�^�j�I���p�ϐ��N���A
	m_isUseQaut = false;
	m_mtxRot = {};
	fValueRot = NULL;
	m_VecAxis = VECTOR3_NULL;
	m_quat = {};
	m_posOld = m_pos;
	m_theta = 0.0f;
	m_radius = 5.0f;
}
//=============================
// �f�X�g���N�^
//=============================
CObjectX::~CObjectX()
{
	// ����
}
//=============================
// ����������
//=============================
HRESULT CObjectX::Init(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(m_pFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL* pMat;

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �e�N�X�`���C���f�b�N�X�z��̓��I�m��
	m_pTexture = new int[m_dwNumMat];

	// �}�e���A����������
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// �e�N�X�`�����ǂݍ��߂���
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// �e�N�X�`���|�C���^�擾
			CTexture* pTexture = CManager::GetTexture();

			// �e�N�X�`���Z�b�g
			m_pTexture[nCntMat] = pTexture->Register(pMat[nCntMat].pTextureFilename);
		}
		else
		{
			// �e�N�X�`���Ȃ�
			m_pTexture[nCntMat] = -1; 
		}
	}

	// ���ʂ�Ԃ�
	return S_OK;
}
//=============================
// �I������
//=============================
void CObjectX::Uninit(void)
{
	// ���b�V���̔j��
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	// �}�e���A���̔j��
	if (m_pBuffMat != nullptr)
	{	
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}

	// null����Ȃ�������
	if (m_pTexture != nullptr)
	{
		// �|�C���^�̔j��
		delete m_pTexture;

		// nullptr�ɂ���
		m_pTexture = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
//=============================
// �X�V����
//=============================
void CObjectX::Update(void)
{
	// �N�H�[�^�j�I���Ȃ�
	if (m_isUseQaut)
	{
		// �~�^���̊p�x�X�V
		m_theta += 0.01f;

		if (m_theta > D3DX_PI * 2.0f)
			m_theta -= D3DX_PI * 2.0f;

		// �O��ʒu��ۑ�
		m_posOld = m_pos;

		// �~��̐V�����ʒu���v�Z
		m_pos.x = cosf(m_theta) * 5.0f;
		m_pos.z = sinf(m_theta) * 5.0f;

		// �i�s�����x�N�g���i���݈ʒu - �O��ʒu�j
		D3DXVECTOR3 vDir = m_pos - m_posOld;
		D3DXVec3Normalize(&vDir, &vDir);

		// ���f���̑O�����i+Z���Ɖ���j
		D3DXVECTOR3 vFront = { 0.0f, 0.0f, 1.0f };

		// ��]�� = front �~ dir
		D3DXVec3Cross(&m_VecAxis, &vFront, &vDir);
		D3DXVec3Normalize(&m_VecAxis, &m_VecAxis);

		m_VecAxis = { 0.0f, 1.0f, 0.0f }; // Y���Œ�
		fValueRot += 0.01f;
	}
}
//=============================
// �`�揈��
//=============================
void CObjectX::Draw(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �v�Z�p�̃}�g���b�N�X��錾
	D3DXMATRIX mtxRot, mtxTrans;

	// ���݂̃}�e���A����ۑ�
	D3DMATERIAL9 matDef;

	// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL* pMat;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �N�H�[�^�j�I����
	if (m_isUseQaut)
	{
		// �N�H�[�^�j�I������
		D3DXQuaternionRotationAxis(&m_quat, &m_VecAxis, fValueRot);

		// ��]�}�g���b�N�X�𐶐�
		D3DXMatrixRotationQuaternion(&m_mtxRot, &m_quat);

		// �����𔽉f
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);
	}
	else
	{
		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		// �����𔽉f7
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}


	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�g���b�N�X�̎擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �}�e���A����������
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// �}�e���A���ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �C���f�b�N�X�ɉ����ĕύX����
		if (m_pTexture[nCntMat] != -1)
		{
			// �e�N�X�`���擾
			CTexture* pTexture = CManager::GetTexture();

			// �e�N�X�`���Z�b�g
			pDevice->SetTexture(0, pTexture->GetAddress(m_pTexture[nCntMat]));
		}
		else
		{
			pDevice->SetTexture(0, NULL); // �e�N�X�`���Ȃ�
		}

		// ���f��(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}
//=============================
// ��������
//=============================
CObjectX* CObjectX::Create(const char* pFileName,D3DXVECTOR3 pos)
{
	// �C���X�^���X����
	CObjectX* pObjX = new CObjectX;

	// ���W���
	pObjX->m_pos = pos;

	// �t�@�C���p�X
	pObjX->m_pFileName = pFileName;

	// ����������
	if (FAILED(pObjX->Init()))
	{
		// �j��
		delete pObjX;

		// nullptr���
		pObjX = nullptr;

		// nullptr��Ԃ�
		return nullptr;
	}

	// �|�C���^��Ԃ�
	return pObjX;
}
