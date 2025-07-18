//======================================================
//
// 弾のオブジェクトホーミング処理 [ bullethorming.cpp ]
// Author: Asuma Nishio
//
//======================================================

//**************************
// インクルードファイル宣言
//**************************
#include "bullethorming.h"
#include "manager.h"

//==================================
// コンストラクタ
//==================================
CBulletHorming::CBulletHorming()
{

}
//==================================
// デストラクタ
//==================================
CBulletHorming::~CBulletHorming()
{
	// 無し
}
//==================================
// 生成処理
//==================================
CBulletHorming* CBulletHorming::Create(const char * pFileName,D3DXVECTOR3 pos)
{
	// インスタンス生成
	CBulletHorming* pBulletHorming = new CBulletHorming;

	// nullチェック
	if (pBulletHorming == nullptr)
		return nullptr;

	// オブジェクト設定
	pBulletHorming->SetFilePass(pFileName);
	pBulletHorming->SetPos(pos);

	// 初期化失敗時
	if (FAILED(pBulletHorming->Init()))
	{
		return nullptr;
	}

	// 生成されたポインタを返す
	return pBulletHorming;
}
//==================================
// 初期化処理
//==================================
HRESULT CBulletHorming::Init(void)
{
	// オブジェクト初期化処理
	CObjectX::Init();

	// 初期化結果を返す
	return S_OK;
}
//==================================
// 終了処理
//==================================
void CBulletHorming::Uninit(void)
{
	// オブジェクト終了処理
	CObjectX::Uninit();
}
//==================================
// 更新処理
//==================================
void CBulletHorming::Update(void)
{
	// 無し
}
//==================================
// 描画処理
//==================================
void CBulletHorming::Draw(void)
{
	// オブジェクト描画処理
	CObjectX::Draw();
}

