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
#include "player.h"

//*********************************
// プレイヤーのベース状態管理
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

//*********************************
// 待機状態時の状態管理
//*********************************
class CPlayerStateNeutral : public CPlayerStateBase
{
public:
	CPlayerStateNeutral();
	~CPlayerStateNeutral();

	// ステートが始まるときに一度だけ呼ばれる関数
	void OnStart();

	// ステートが更新されるときに呼ばれる関数
	void OnUpdate();

	// ステートが終了する時に一度だけ呼ばれる関数
	void OnExit();

};

//*********************************
// 移動時の状態管理
//*********************************
class CPlayerStateMove : public CPlayerStateBase
{
public:
	CPlayerStateMove();
	~CPlayerStateMove();

	// ステートが始まるときに一度だけ呼ばれる関数
	void OnStart();

	// ステートが更新されるときに呼ばれる関数
	void OnUpdate();

	// ステートが終了する時に一度だけ呼ばれる関数
	void OnExit();
};

//*********************************
// 攻撃時の状態管理
//*********************************
class CPlayerStateAction : public CPlayerStateBase
{
public:
	CPlayerStateAction();
	~CPlayerStateAction();

	// ステートが始まるときに一度だけ呼ばれる関数
	void OnStart();

	// ステートが更新されるときに呼ばれる関数
	void OnUpdate();

	// ステートが終了する時に一度だけ呼ばれる関数
	void OnExit();
};

//*********************************
// ジャンプ時の状態管理
//*********************************
class CPlayerStateJump : public CPlayerStateBase
{
public:
	CPlayerStateJump();
	~CPlayerStateJump();

	// ステートが始まるときに一度だけ呼ばれる関数
	void OnStart();

	// ステートが更新されるときに呼ばれる関数
	void OnUpdate();

	// ステートが終了する時に一度だけ呼ばれる関数
	void OnExit();
};

#endif