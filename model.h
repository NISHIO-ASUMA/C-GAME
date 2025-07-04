//====================================
//
// ���f������ [ model.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _MODEL_H_ // ���̃}�N����`������ĂȂ�������
#define _MODEL_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**********************
// �}�N����`
//**********************
#define MAX_PART (25)

//**********************
// ���f���N���X�̒�`
//**********************
class CModel
{
public:
	CModel();
	~CModel();

	static CModel* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFilename);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFilename);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetParent(CModel* pModel);
	D3DXMATRIX GetMtxWorld(void) { return m_mtxworld; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void OffSetPos(D3DXVECTOR3 offpos) { m_offPos = offpos; }
	void OffSetRot(D3DXVECTOR3 offrot) { m_offRot = offrot; }

private:
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

	//***************************
	// �L�[�\���̐錾
	//***************************
	struct KEY
	{
		float fPosX;	// �ʒuX
		float fPosY;	// �ʒuY
		float fPosZ;	// �ʒuZ
		float fRotX;	// �p�xX
		float fRotY;	// �p�xY
		float fRotZ;	// �p�xZ
	};

	//***************************
	// �L�[���̍\���̐錾
	//***************************
	struct KEY_INFO
	{
		int nFrame;				// �t���[����
		KEY aKey[MAX_PART]; 	// �e�p�[�c�̃L�[�̗v�f
	};

	//***************************
	// ���[�V�������̍\���̐錾
	//***************************
	struct MOTION_INFO
	{
		bool bLoop;						// ���[�v���邩���Ȃ���
		int nNumKey;					// �L�[�̑���
		KEY_INFO aKeyInfo[MAX_PART];	// �L�[���
		TYPE Motiontype;				// ���[�V�����̎��
	};

	MOTION_INFO m_aMotionInfo[TYPE_MAX];// ���[�V�������
	int m_nNumMotion;					// ���[�V�����̑���
	bool m_isLoopMotion;				// ���[�v���邩�ǂ���
	int m_nNumKey;						// �L�[�̑���
	int m_nKey;							// ���݂̃L�[No
	int m_nCounterMotion;				// ���[�V�����̃J�E���^�[
	TYPE m_type;

	D3DXVECTOR3 m_pos, m_rot,m_offPos,m_offRot; // ���W,�p�x

	D3DXMATRIX m_mtxworld;	  // ���[���h�}�g���b�N�X
	CModel* m_pParent;	  // �e���f��

	LPD3DXMESH m_pMesh;	   // ���b�V�����
	LPD3DXBUFFER m_pBuffMat; // �}�e���A�����
	DWORD m_dwNumMat;		// �}�e���A����
	int* m_pTexture;		// �e�N�X�`���|�C���^
};

#endif