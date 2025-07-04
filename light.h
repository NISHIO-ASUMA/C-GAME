//====================================
//
// ���C�g���� [ light.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _LIGHT_H_ // ���̃}�N����`������ĂȂ�������
#define _LIGHT_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**********************
// �C���N���[�h�t�@�C��
//**********************


//**********************
// �}�N����`
//**********************
#define NUM_LIGHT (3)

//**********************
// ���C�g�N���X���`
//**********************
class CLight
{
public:
	CLight();
	~CLight();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	D3DLIGHT9 m_aLight[NUM_LIGHT]; // ���C�g��
	D3DXVECTOR3 m_vecDir[NUM_LIGHT];	// �x�N�g��
};

#endif