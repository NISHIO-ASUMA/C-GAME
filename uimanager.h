//======================================
//
// UI全体を管理する処理 [ uimanager.h ]
// Author: Asuma Nishio
//
//======================================

#ifndef _UIMANAGER_H_ // このマクロ定義がされてなかったら
#define _UIMANAGER_H_ // 2重インクルード防止のマクロ定義

//****************************
// インクルードファイル宣言
//****************************

//****************************
// UI管理クラスを定義
//****************************
class CUimanager
{
public:
	CUimanager();
	~CUimanager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	static int m_nIdxUi;			// UIのインデックス番号

	// NOTE : 出すuiを配列で持ってロードで適用する処理を作成

};

#endif