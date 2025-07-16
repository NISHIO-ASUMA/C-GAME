//====================================
//
// ���C���Q�[������ [ game.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _GAME_H_ // ���̃}�N����`������ĂȂ�������
#define _GAME_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "scene.h"
#include "meshcylinder.h"
#include "boss.h"
#include "block.h"

//****************************
// �Q�[���N���X���`
//****************************
class CGame : public CScene
{
public:
	CGame();
	~CGame();


	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGame* Create(void);

	static CBlock* GetBlock(void) { return m_pBlock; }
	static CMeshCylinder* GetCylinder(void) { return m_pMeshCylinder; }
	static CBoss* GetBoss(void) { return m_pBoss; }

private:
	static CBlock* m_pBlock;					// �u���b�N
	static CMeshCylinder* m_pMeshCylinder;		// �V�����_�[
	static CBoss* m_pBoss;						// �{�X

};

#endif
