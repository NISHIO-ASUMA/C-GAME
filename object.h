//====================================
//
// �I�u�W�F�N�g���� [ object.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _OBJECT_H_ // ���̃}�N����`������ĂȂ�������
#define _OBJECT_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**********************
// �}�N����`
//**********************
#define MAX_OBJECT (512)		// �ő�I�u�W�F�N�g��

//***************************
// �I�u�W�F�N�g�N���X���`
//***************************
class CObject
{
public:
	//*************************
	// �I�u�W�F�N�g�̎�ޒ�`
	//*************************
	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_BULLET,
		TYPE_BOSS,
		TYPE_BLOCK,
		TYPE_MAX
	};

	//*************************
	// �`��ԍ���`
	//*************************
	enum PRIORITY
	{
		NONE, // �������
		MESH, // ���b�V��
		BLOCK, // �u���b�N
		BULLET,// �e
		BOSS, // �{�X
		SHADOW,//�e
		IMPACT, // �Ռ��g
		PLAYER, // �v���C���[
		UI,  // 2DUI�֌W
		PRIORITY_MAX // �ő吔
	};

	CObject(int nPriority = NONE);
	virtual ~CObject();

	// �������z�֐�
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	void SetObjType(TYPE type);
	TYPE GetObjType(void);

	static CObject* GetObject(int nPriority,int nIdx);

	static int GetNumAll(void) { return m_nNumAll; }

protected:
	void Release(void);

private:
	static CObject* m_apObject[PRIORITY_MAX][MAX_OBJECT]; // �I�u�W�F�N�g�̃|�C���^
	static int m_nNumAll;		// �����Ǘ�
	int m_nID;					// �������g��ID
	TYPE m_Type;				// �I�u�W�F�N�g�̎�ޕϐ�
	int m_nPriority;			// �D�揇�ʔԍ�
};
#endif