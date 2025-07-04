//====================================
//
// �{�X���� [ boss.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _BOSS_H_ // ���̃}�N����`������ĂȂ�������
#define _BOSS_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//******************************
// �C���N���[�h�t�@�C��
//******************************
#include "object.h"

//**********************
// �O���錾
//**********************
class CModel;
class CMotion;

//**********************
// �{�X�N���X���`
//**********************
class CBoss : public CObject
{
public:
	//***************************
	// ���[�V�����񋓌^�錾
	//***************************
	enum TYPE
	{
		TYPE_NEUTRAL = 0,	// �j���[�g����
		TYPE_MOVE,			// �ړ�
		TYPE_ACTION,		// �A�N�V����
		TYPE_JUMP,			// �W�����v
		TYPE_LANDING,		// ���n
		TYPE_MAX
	};

	CBoss(int nPriority = static_cast<int>(CObject::PRIORITY::BOSS));
	~CBoss();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBoss* Create(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPos(void) { return m_pos;}
	D3DXVECTOR3 GetRot(void) { return m_rot; }

private:
	static const int m_nNumModels = 21;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXMATRIX m_mtxworld;	// �}�g���b�N�X

	CModel* m_pModel[m_nNumModels];
	CMotion* m_pMotion;

	int m_nNumAll;		   // ���f������
	int m_type;			   // ���[�V�����̎�ޕϐ�

};

#endif
