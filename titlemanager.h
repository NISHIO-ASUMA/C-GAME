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
	void Draw(void);

private:
	int m_nIdx;		// �I��ԍ�

};

#endif

