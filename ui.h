//====================================
//
// UI���� [ ui.h ]
// Author: Asuma Nishio
//
//=====================================
#ifndef _UI_H_ // ���̃}�N����`������ĂȂ�������
#define _UI_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "object2D.h"

//****************************
// UI�N���X���`
//****************************
class CUi : public CObject2D
{
public:
	CUi(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CUi();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CUi* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

	void SetTexture(void);
	void SetBasePos(D3DXVECTOR3 pos) { m_BasePos = pos; }

private:
	D3DXVECTOR3 m_BasePos;    // �𑜓x�Ɉˑ����Ȃ�����W
	float m_BaseWidth;  // ��T�C�Y�̉���
	float m_BaseHeight; // ��T�C�Y�̍���
	bool m_bUseBasePos; // ����W�ݒ�

};

#endif