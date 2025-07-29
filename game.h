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
// 前方宣言
//****************************
class CGameManager;

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

	static CPauseManager* GetPause(void) { return m_pPausemanager; }

private:
	static CPauseManager* m_pPausemanager;		// ポーズマネージャーポインタ
	CGameManager* m_pGameManager;					// ゲームマネージャーポインタ
};

#endif
