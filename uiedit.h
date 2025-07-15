//====================================
//
// エディター処理 [ uiedit.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _UIEDIT_H_ // このマクロ定義がされてなかったら
#define _UIEDIT_H_ // 2重インクルード防止のマクロ定義

//******************************
// 前方宣言
//******************************
class CUi;

//*******************************
// UIエディタークラスを定義
//*******************************
class CUiedit
{
public:
	CUiedit();
	~CUiedit();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//**********************
	// 構造体定義
	//**********************
	struct EditInfo
	{

	};

	int m_nIdxCount;	// 配列インデックス番号

	CUi* m_pUi;	// UIクラスのポインタ
};

#endif