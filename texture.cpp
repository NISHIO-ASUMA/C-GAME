//====================================
//
// �e�N�X�`������ [ texture.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "texture.h"
#include "manager.h"

//**********************
// �ÓI�����o�ϐ�
//**********************
int CTexture::m_nNumAll = NULL;	// �����Ǘ�

//===============================
// �R���X�g���N�^
//===============================
CTexture::CTexture()
{
	// �l�̃N���A
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		m_apTexture[nCnt] = nullptr;
	}
}
//===============================
// �f�X�g���N�^
//===============================
CTexture::~CTexture()
{
	// ����
}
//===============================
// �e�N�X�`���ǂݍ���
//===============================
HRESULT CTexture::Load(void)
{
#if 1
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���ׂẴe�N�X�`������
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		// �t�@�C�����������Ȃ�X�L�b�v
		if (TexName[nCnt] == nullptr)
			continue;

		// �񋓌^�̕��ǂݍ���
		if (FAILED(D3DXCreateTextureFromFile(pDevice, TexName[nCnt], &m_apTexture[nCnt])))
		{
			// �x���\��
			MessageBox(NULL, "�e�N�X�`���p�X�����݂��܂���", "�e�N�X�`���ǂݍ��݃G���[", MB_OK);

			return E_FAIL;
		}

		// ���Z����
		m_nNumAll++;
	}
#endif

	// ���ʂ�Ԃ�
	return S_OK;

}
//===============================
// �e�N�X�`���j��
//===============================
void CTexture::UnLoad(void)
{
	// ���ׂẴe�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}
//===============================
// �e�N�X�`���ԍ���o�^����
//===============================
int CTexture::Register(const char* pFileName)
{
	// �C���f�b�N�X�p�̕ϐ�
	int nIdx = -1;

	// �C���f�b�N�X�ԍ������Z
	nIdx++;

	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		// nullptr�`�F�b�N
		if (TexName[nCnt] != nullptr)
		{
			// �t�@�C���p�X������v���Ă�����
			if (strcmp(pFileName, TexName[nCnt]) == 0)
			{
				// �J�E���g�ԍ�����
				nIdx = nCnt;

				// for��������
				break;
			}
		}
	}

	// �C���f�b�N�X�ԍ���Ԃ�
	return nIdx;
}
//===============================
//�e�N�X�`���ԍ��擾
//===============================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	// ��O����
	if (nIdx < 0 || nIdx >= MAX_TEXTURE) return nullptr;

	// �e�N�X�`���ԍ����擾
	return m_apTexture[nIdx];
}
