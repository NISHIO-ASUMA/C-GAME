//======================================================
//
// �e�̃I�u�W�F�N�g�z�[�~���O���� [ bullethorming.cpp ]
// Author: Asuma Nishio
//
//======================================================

//**************************
// �C���N���[�h�t�@�C���錾
//**************************
#include "bullethorming.h"
#include "manager.h"

//==================================
// �R���X�g���N�^
//==================================
CBulletHorming::CBulletHorming()
{

}
//==================================
// �f�X�g���N�^
//==================================
CBulletHorming::~CBulletHorming()
{
	// ����
}
//==================================
// ��������
//==================================
CBulletHorming* CBulletHorming::Create(const char * pFileName,D3DXVECTOR3 pos)
{
	// �C���X�^���X����
	CBulletHorming* pBulletHorming = new CBulletHorming;

	// null�`�F�b�N
	if (pBulletHorming == nullptr)
		return nullptr;

	// �I�u�W�F�N�g�ݒ�
	pBulletHorming->SetFilePass(pFileName);
	pBulletHorming->SetPos(pos);

	// ���������s��
	if (FAILED(pBulletHorming->Init()))
	{
		return nullptr;
	}

	// �������ꂽ�|�C���^��Ԃ�
	return pBulletHorming;
}
//==================================
// ����������
//==================================
HRESULT CBulletHorming::Init(void)
{
	// �I�u�W�F�N�g����������
	CObjectX::Init();

	// ���������ʂ�Ԃ�
	return S_OK;
}
//==================================
// �I������
//==================================
void CBulletHorming::Uninit(void)
{
	// �I�u�W�F�N�g�I������
	CObjectX::Uninit();
}
//==================================
// �X�V����
//==================================
void CBulletHorming::Update(void)
{
	// ����
}
//==================================
// �`�揈��
//==================================
void CBulletHorming::Draw(void)
{
	// �I�u�W�F�N�g�`�揈��
	CObjectX::Draw();
}

