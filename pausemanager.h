//====================================
//
// ポーズ管理処理 [ pausemanager.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _PAUSEMANAGER_H_ // このマクロ定義がされてなかったら
#define _PAUSEMANAGER_H_ // 2重インクルード防止のマクロ定義

//*********************************
// ポーズメニュー管理クラスを定義
//*********************************
class CPauseManager
{
public:
	CPauseManager();
	~CPauseManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nSelectIdx;	// セレクト番号
};

#endif
