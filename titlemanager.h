//=========================================
//
// �^�C�g���Ǘ����� [ titlemanager.cpp ]
// Author: Asuma Nishio
//
// TODO : �^�C�g���ōs�������z�u���̊Ǘ��S�ʂ�����
// 
//==========================================

#ifndef _TITLEMANAGER_H_ // ���̃}�N����`������ĂȂ�������
#define _TITLEMANAGER_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**************
// �O���錾
//**************
class CTitleUi;

//**************************************
// �^�C�g���}�l�[�W���[�Ǘ��N���X���`
//**************************************
class CTitleManager
{
public:
	CTitleManager();
	~CTitleManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	static constexpr int TITLE_MENU = 2; // �^�C�g��ui��
	static constexpr float DIGITPOS = 180.0f; // ui�̕�
	static constexpr float UIWIDTH = 300.0f;		// ui�̉���
	static constexpr float UIHEIGHT = 60.0f;		// ui�̍���
	static constexpr float FIELDWIDTH = 1000.0f;	// �n�ʂ̍L��

	int m_nIdx;		// �I��ԍ�
	CTitleUi* m_pTitleui[TITLE_MENU]; // �N���X�|�C���^
};

#endif

