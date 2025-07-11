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
#include "model.h"

//**********************
// �O���錾
//**********************
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
		TYPE_ACTION,		// �r�U��
		TYPE_MAX
	};

	//***************************
	// �s���p�^�[���񋓌^
	//***************************
	enum ATTACKPATTERN
	{
		PATTERN_NONE, // �����s�����Ă��Ȃ�
		PATTERN_HAND, // ����
		PATTERN_BULLET, // �e����
		PATTERN_CIRCLE, // �U��񂵍U��
		PATTERN_MAX
	};

	CBoss(int nPriority = static_cast<int>(CObject::PRIORITY::BOSS));
	~CBoss();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBoss* Create(D3DXVECTOR3 pos, float fSize);

	D3DXVECTOR3 GetPos(void) { return m_pos;}
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	float GetSize(void) { return m_fSize; }

	bool CollisionRightHand(D3DXVECTOR3* pPos);
	CModel*GetModelPartType(CModel::PARTTYPE modelpart);

private:

	static constexpr int NUMMODELS = 21; // �g�����f����

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXMATRIX m_mtxworld;	// �}�g���b�N�X

	CModel* m_pModel[NUMMODELS];
	CMotion* m_pMotion;

	int m_type;			   // ���[�V�����̎�ޕϐ�
	float m_fSize;		   // �T�C�Y

	int m_nCoolTime;	// �N�[���^�C��
	bool m_isAttacked;  // �U�����Ă��邩
};

#endif
