//====================================
//
// �G�f�B�^�[���� [ uiedit.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _UIEDIT_H_ // ���̃}�N����`������ĂȂ�������
#define _UIEDIT_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//******************************
// �O���錾
//******************************
class CUi;

//*******************************
// UI�G�f�B�^�[�N���X���`
//*******************************
class CUiedit
{
public:
	CUiedit();
	~CUiedit();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//**********************
	// �\���̒�`
	//**********************
	struct EditInfo
	{

	};

	int m_nIdxCount;	// �z��C���f�b�N�X�ԍ�

	CUi* m_pUi;	// UI�N���X�̃|�C���^
};

#endif