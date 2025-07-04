//================================
//
// 汎用処理関数 [ template.h ]
// Author : Asuma Nishio
//
//================================

#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

//*********************************
// マクロ定義
//*********************************
#define VALUE_FLOAT (2.0f)

//*********************************
// 範囲内計算関数
//*********************************
template <class T>T Clump(const T t, const T min, const T max)
{
	if (t > max) return max; // 上限超えたら上限設定
	if (t < min) return min; // 下限以下なら下限設定
	return t;
}

//*********************************
// ループ値計算関数
//*********************************
template <class T>T Wrap(const T t, const T min, const T max)
{
	if (t > max) return min; // 上限超えたら最小値へ
	if (t < min) return max; // 下限以下なら最大値へ
	return t;
}

//*********************************
// 角度正規化計算
//*********************************
template <class T> T NormalAngle(T Angle)
{
	// 角度の正規化処理
	if (Angle > D3DX_PI) Angle -= D3DX_PI * VALUE_FLOAT;
	if (Angle < D3DX_PI) Angle += D3DX_PI * VALUE_FLOAT;
	return Angle;
}
#endif // !_TEMPLETE_H_
