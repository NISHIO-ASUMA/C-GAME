//===================================================
//
// 弾のオブジェクトホーミング処理 [ bullethorming.h ]
// Author: Asuma Nishio
//
//===================================================

#ifndef _BULLETHORMING_H_ // このマクロ定義がされてなかったら
#define _BULLETHORMING_H_ // 2重インクルード防止のマクロ定義

//**************************
// インクルードファイル宣言
//**************************
#include "objectX.h"

//********************************
// 弾のホーミングクラスを定義
//********************************
class CBulletHorming : public CObjectX
{
public:
	CBulletHorming();
	~CBulletHorming();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBulletHorming* Create(const char * pFileNama,D3DXVECTOR3 pos);

private:

};

#endif
