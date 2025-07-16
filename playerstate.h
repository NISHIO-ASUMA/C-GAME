//============================================
//
// プレイヤー状態管理処理 [ playerstate.h ]
// Author: Asuma Nishio
//
//=============================================

#ifndef _PLAYERSTATE_H_ // このマクロ定義がされてなかったら
#define _PLAYERSTATE_H_ // 2重インクルード防止のマクロ定義

//**************************
// インクルードファイル宣言
//**************************
#include "state.h"

//**************************
// 前方宣言
//**************************
class CPlayer;

//*********************************
// プレイヤーの状態管理
//*********************************
class CPlayerStateBase : public CStateBase
{
public:
	CPlayerStateBase();
	~CPlayerStateBase();

	void SetOwner(CPlayer* pPlayer) { m_pPlayer = pPlayer; } // ポインタセット

	// ステートが始まるときに一度だけ呼ばれる関数
	virtual void OnStart() {}

	// ステートが更新されるときに呼ばれる関数
	virtual void OnUpdate() {}

	// ステートが終了する時に一度だけ呼ばれる関数
	virtual void OnExit() {}

protected:
	CPlayer* m_pPlayer;		// プレイヤーのポインタ
};

#endif