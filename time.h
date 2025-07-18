//====================================
//
// タイム処理 [ time.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _TIME_H_ // このマクロ定義がされてなかったら
#define _TIME_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************

#include "number.h"
#include "object.h"

//**********************
// マクロ定義
//**********************
#define NUM_TIME (3) // 時間の桁数

//**********************************
// タイムクラスを定義
//**********************************
class CTime : public CObject
{
public:
	CTime(int nPriority = 6);
	~CTime();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTime* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXCOLOR GetCol(void) { return m_col; }

	void SetTexture(void);

private:
	D3DXVECTOR3 m_pos;		// 座標
	D3DXCOLOR m_col;
	D3DXVECTOR3 m_rot;

	int m_nCurrentTime;		// 経過時間
	int m_nAllTime;			// 最大時間

	CNumber* m_pNumber[NUM_TIME];	// ナンバー型の静的変数

	float m_fHeight, m_fWidth;				// 横幅,高さ
	int m_nIdxTexture;
};
#endif