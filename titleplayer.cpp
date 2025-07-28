#include "titleplayer.h"

//============================
// �R���X�g���N�^
//============================
CTitlePlayer::CTitlePlayer(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_move = VECTOR3_NULL;
	m_rotDest = VECTOR3_NULL;
	m_nIdxPlayer = NULL;

	// ���f���̃|�C���^�̃N���A
	for (int nCnt = 0; nCnt < TITLE_MODEL; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
	}

	// �N���X�|�C���^
	m_pMotion = nullptr;
	m_pFilename = {};
}
//============================
// �f�X�g���N�^
//============================
CTitlePlayer::~CTitlePlayer()
{
	// ����
}
//============================
// ��������
//============================
CTitlePlayer* CTitlePlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, const int nIdxParson, const char* pFilename)
{
	// �C���X�^���X����
	CTitlePlayer* pTitlePlayer = new CTitlePlayer;

	// nullptr��������
	if (pTitlePlayer == nullptr) return nullptr;

	// �I�u�W�F�N�g�ݒ�
	pTitlePlayer->m_pos = pos;
	pTitlePlayer->m_rot = rot;
	pTitlePlayer->m_nIdxPlayer = nIdxParson;
	pTitlePlayer->m_pFilename = pFilename;

	// ���������s��
	if (FAILED(pTitlePlayer->Init()))
	{
		return nullptr;
	}

	// �������ꂽ�|�C���^��Ԃ�
	return pTitlePlayer;
}
//============================
// ����������
//============================
HRESULT CTitlePlayer::Init(void)
{
	// �I�u�W�F�N�g�̎�ނ��Z�b�g
	SetObjType(TYPE_PLAYER);

	// ���[�V�����̓ǂݍ���
	m_pMotion = CMotion::Load(m_pFilename, TITLE_MODEL, m_apModel, TITLEMOTION_MAX);

	// ���[�V��������ݒ�
	m_pMotion->SetMotionNum(TITLEMOTION_MAX);

	// �v���C���[���ʃ��f���t���O��ݒ�
	for (int nCnt = 0; nCnt < TITLE_MODEL; nCnt++)
	{
		// nullptr����Ȃ�������
		if (m_apModel[nCnt] != nullptr)
		{
			// �t���O��ݒ肷��
			m_apModel[nCnt]->SetIsPlayer(true);
		}
	}

	// ���������ʂ�Ԃ�
	return S_OK;
}
//============================
// �I������
//============================
void CTitlePlayer::Uninit(void)
{
	// ���g�̔j��
	CObject::Release();
}
//============================
// �X�V����
//============================
void CTitlePlayer::Update(void)
{

}
//============================
// �`�揈��
//============================
void CTitlePlayer::Draw(void)
{

}
