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

#endif
