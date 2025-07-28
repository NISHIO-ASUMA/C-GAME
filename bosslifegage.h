//=========================================
//
// �{�X�̗͏��� [ bosslifegage.h ]
// Author: Asuma Nishio
//
//=========================================

#ifndef _BOSSLIFEGAGE_H_ // ���̃}�N����`������ĂȂ�������
#define _BOSSLIFEGAGE_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "object.h"
#include "object2D.h"

//**********************
// �O���錾
//**********************
class CBoss;

//***************************
// �{�X�̗͊Ǘ��N���X���`
//***************************
class CBossLifeGage : public CObject2D
{
public:
	CBossLifeGage(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CBossLifeGage();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBossLifeGage* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);

private:
	int m_nIdxTex;	// �e�N�X�`���C���f�b�N�X
	CBoss* m_pBoss; // �{�X�̃|�C���^

};

#endif

