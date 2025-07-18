//===================================================
//
// �e�̃I�u�W�F�N�g�z�[�~���O���� [ bullethorming.h ]
// Author: Asuma Nishio
//
//===================================================

#ifndef _BULLETHORMING_H_ // ���̃}�N����`������ĂȂ�������
#define _BULLETHORMING_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**************************
// �C���N���[�h�t�@�C���錾
//**************************
#include "objectX.h"

//********************************
// �e�̃z�[�~���O�N���X���`
//********************************
class CBulletHorming : public CObjectX
{
public:
	CBulletHorming();
	~CBulletHorming();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBulletHorming* Create(const char * pFileNama,D3DXVECTOR3 pos);

private:

};

#endif
