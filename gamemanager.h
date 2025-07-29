//=========================================
//
// �Q�[���Ǘ����� [ gamemanager.h ]
// Author: Asuma Nishio
// 
//==========================================

//***************************
// �C���N���[�h�t�@�C���錾
//***************************
#include "meshcylinder.h"
#include "boss.h"
#include "block.h"
#include "time.h"

//*****************************
// �Q�[����ԊǗ��N���X���`
//*****************************
class CGameManager
{
public:
	CGameManager();
	~CGameManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static CBlock* GetBlock(void) { return m_pBlock; }
	static CMeshCylinder* GetCylinder(void) { return m_pMeshCylinder; }
	static CBoss* GetBoss(void) { return m_pBoss; }
	static CTime* GetTime(void) { return m_pTime; }

private:
	static CBlock* m_pBlock;					// �u���b�N
	static CMeshCylinder* m_pMeshCylinder;		// �V�����_�[
	static CBoss* m_pBoss;						// �{�X
	static CTime* m_pTime;						// �^�C�}�[

};

