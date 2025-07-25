//=========================================
//
// タイトル管理処理 [ titlemanager.cpp ]
// Author: Asuma Nishio
//
// TODO : タイトルで行う操作や配置物の管理全般をする
// 
//==========================================

#ifndef _TITLEMANAGER_H_ // このマクロ定義がされてなかったら
#define _TITLEMANAGER_H_ // 2重インクルード防止のマクロ定義

//**************************************
// タイトルマネージャー管理クラスを定義
//**************************************
class CTitleManager
{
public:
	CTitleManager();
	~CTitleManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nIdx;		// 選択番号

};

#endif

