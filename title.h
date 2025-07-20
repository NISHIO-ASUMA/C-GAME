//====================================
//
// タイトル処理 [ title.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _TITLE_H_ // このマクロ定義がされてなかったら
#define _TITLE_H_ // 2重インクルード防止のマクロ定義

//****************************
// インクルードファイル宣言
//****************************
#include "scene.h"

//****************************
// タイトルクラスを定義
//****************************
class CTitle :public CScene
{
public:
	CTitle();
	~CTitle();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTitle* Create(void);

private:

};
#endif
