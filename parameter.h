//====================================
//
// パラメータ処理 [ boss.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _PARAMETER_H_ // このマクロ定義がされてなかったら
#define _PARAMETER_H_ // 2重インクルード防止のマクロ定義

//******************************
// 前方宣言
//******************************
class CObject;

//******************************
// パラメータークラスを定義
//******************************
class CParameter
{
public:
	CParameter();
	~CParameter();

	void HitDamage(int nDamage);

	int GetHp()const { return m_nHp; }

	void SetHp(int nHp) { m_nHp = nHp; }
	void AddHp(int nValue) { m_nHp += nValue; }
	
private:
	int m_nHp; // 体力
	int m_nDamage; // ダメージ数
	D3DXVECTOR3 m_move; // 移動量

};

#endif
