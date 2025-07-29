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

//**************
// 前方宣言
//**************
class CTitleUi;

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

private:
	static constexpr int TITLE_MENU = 2; // タイトルui数
	static constexpr float DIGITPOS = 180.0f; // uiの幅
	static constexpr float UIWIDTH = 300.0f;		// uiの横幅
	static constexpr float UIHEIGHT = 60.0f;		// uiの高さ
	static constexpr float FIELDWIDTH = 1000.0f;	// 地面の広さ

	int m_nIdx;		// 選択番号
	CTitleUi* m_pTitleui[TITLE_MENU]; // クラスポインタ
};

#endif

