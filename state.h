//============================================
//
// 状態管理処理 [ state.h ]
// Author: Asuma Nishio
//
//=============================================

#ifndef _STATE_H_ // このマクロ定義がされてなかったら
#define _STATE_H_ // 2重インクルード防止のマクロ定義

//**********************
// 状態管理クラスを定義
//**********************
class CState
{
public:
	//***********************
	// 状態の種類
	//***********************
	enum STATE
	{
		STATE_NONE,
		STATE_NORMAL,
		STATE_DAMAGE,
		STATE_INVINCIBLE,
		STATE_DEATH,
		STATE_MAX
	};

	CState();
	~CState();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static CState* Create(void);

	void SetState(int state) { m_state = state; }
	int GetState(void) { return m_state; }

private:
	int m_state;		// 状態管理変数
	int m_StateCount;	// カウンター
};

//*********************************
// ステートベースクラス
//*********************************
class CStateBase
{
public:
	CStateBase();
	virtual ~CStateBase();

	// ステートが始まるときに一度だけ呼ばれる関数
	virtual void OnStart() {}

	// ステートが更新されるときに呼ばれる関数
	virtual void OnUpdate() {}

	// ステートが終了する時に一度だけ呼ばれる関数
	virtual void OnExit() {}
};

//*********************************
// ステートマシンクラス
//*********************************
class CStateMachine
{
public:
	CStateMachine();
	~CStateMachine();

	void Update(void);						 // 更新処理
	void ChangeState(CStateBase* pNewState); // ステート変更

private:
	CStateBase* m_pNowState;	// 基底クラスのステートポインタ
};



#endif
