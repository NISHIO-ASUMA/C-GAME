//====================================
//
// �^�C�g������ [ title.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _TITLE_H_ // ���̃}�N����`������ĂȂ�������
#define _TITLE_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "scene.h"

//****************************
// �^�C�g���N���X���`
//****************************
class CTitle :public CScene
{
public:
	CTitle();
	~CTitle();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTitle* Create(void);

private:

};
#endif
