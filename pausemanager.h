//====================================
//
// �|�[�Y�Ǘ����� [ pausemanager.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _PAUSEMANAGER_H_ // ���̃}�N����`������ĂȂ�������
#define _PAUSEMANAGER_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//*********************************
// �|�[�Y���j���[�Ǘ��N���X���`
//*********************************
class CPauseManager
{
public:
	CPauseManager();
	~CPauseManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nSelectIdx;	// �Z���N�g�ԍ�
};

#endif
