//====================================
//
// ���b�V���h�[������ [ meshdome.h ]
// Author: Asuma Nishio
//
//====================================

#ifndef _MESHDONE_H_ // ���̃}�N����`������ĂȂ�������
#define _MESHDONE_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "object.h"

//**********************************
// ���b�V���h�[���N���X���`
//**********************************
class CMeshDome : public CObject
{
public:
	CMeshDome(int nPrio = static_cast<int>(CObject::PRIORITY::MESH));
	~CMeshDome();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshDome* Create(D3DXVECTOR3 pos, float nRadius);

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	void SetTexture(void);
private:
	LPDIRECT3DINDEXBUFFER9 m_pIdx; // �C���f�b�N�X�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9 m_pVtx;	// ���_�o�b�t�@

	D3DXVECTOR3 m_pos;		// ���W
	D3DXVECTOR3 m_rot;		// �p�x
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
	float m_nRadius;		// ���a
	int m_nTexIdx;
};


#endif