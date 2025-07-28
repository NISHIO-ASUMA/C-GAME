//====================================
//
// メインゲーム処理 [ game.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _GAME_H_ // このマクロ定義がされてなかったら
#define _GAME_H_ // 2重インクルード防止のマクロ定義

//****************************
// インクルードファイル宣言
//****************************
#include "scene.h"
#include "meshcylinder.h"
#include "boss.h"
#include "block.h"
#include "pausemanager.h"
#include "time.h"

//****************************
// ゲームクラスを定義
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
	static CPauseManager* GetPause(void) { return m_pPausemanager; }
	static CTime* GetTime(void) { return m_pTime; }

private:
	static CBlock* m_pBlock;					// ブロック
	static CMeshCylinder* m_pMeshCylinder;		// シリンダー
	static CBoss* m_pBoss;						// ボス
	static CPauseManager* m_pPausemanager;		// マネージャーポインタ
	static CTime* m_pTime;
	bool m_bPause;
};

#endif
