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
#include "pausemanager.h"
#include "time.h"

//****************************
// �O���錾
//****************************
class CGameManager;

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

	static CPauseManager* GetPause(void) { return m_pPausemanager; }

private:
	static CPauseManager* m_pPausemanager;		// �|�[�Y�}�l�[�W���[�|�C���^
	CGameManager* m_pGameManager;					// �Q�[���}�l�[�W���[�|�C���^
};

#endif
