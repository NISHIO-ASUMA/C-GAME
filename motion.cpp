//====================================
//
// モーション処理 [ motion.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**************************
// インクルードファイル宣言
//**************************
#include "motion.h"
#include "template.h"

//==============================
// コンストラクタ
//==============================
CMotion::CMotion()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_aMotionInfo[nCnt] = {};
	}

	m_isLoopMotion = false;
	m_nCounterMotion = NULL;
	m_nKey = NULL;
	m_nNumKey = NULL;
	m_nNumMotion = NULL;
	m_motiontype = NULL;
	m_type = NULL;
	m_nNextKey = NULL;

// ブレンド---------------------------------------
	m_motiontypeBlend = NULL;
	m_nFrameBlend = NULL;
	m_nCounterBlend = NULL;
	m_nKeyBlend = NULL;
	m_nNextKeyBlend = NULL;

	m_isBlendMotion = false;
	m_isFinishMotion = false;
	m_isFirstMotion = false;
}
//==============================
// デストラクタ
//==============================
CMotion::~CMotion()
{
	// 無し
}
//==============================
// モーション読み込み関数
//==============================
CMotion* CMotion::Load(const char* pFilename,const int nMaxParts, CModel** pModel)
{
	// モーションクラスのインスタンス生成
	CMotion* pMotion = new CMotion;

	// ファイル設定
	std::ifstream file(pFilename);

	// ファイル例外チェック
	if (!file)
	{
		MessageBox(NULL, "ファイルオープン失敗", pFilename, MB_OK);

		// ポインタの破棄
		delete pMotion;

		// 失敗結果を返す
		return nullptr;
	}

	// 一行読み込む
	std::string line;

	// 使用ローカル変数
	int nModel = 0;
	int nIdx = 0;
	int nCntMotion = 0;

	// 文字列を読み続ける
	while (std::getline(file, line))
	{
		// トークン設定
		std::istringstream iss(line);
		std::string token;
		iss >> token;

		// "NUM_MODEL"読み取り時
		if (token == "NUM_MODEL")
		{
			// モデル数設定
			pMotion->SetModels(iss, nModel, nMaxParts);
		}
		// "MODEL_FILENAME"読み取り時
		else if (token == "MODEL_FILENAME")
		{
			// モデルファイル読み込み
			pMotion->SetModelFile(iss, pModel, nIdx);

			// インデックスカウントを加算
			nIdx++;
		}
		// "PARTSSET"読み取り時
		else if (token == "PARTSSET")
		{
			// パーツごとのセット
			pMotion->SetParts(file, pModel);
		}
		// "MOTIONSET"読み取り時
		else if (token == "MOTIONSET")
		{
			// パーツのモーションの設定
			pMotion->SetPartsMotion(file, pMotion, nCntMotion);

			// モーションカウントを加算
			nCntMotion++;
		}
		// "END_SCRIPT"読み取り時
		else if (token == "END_SCRIPT")
		{
			break;
		}
	}

	// ファイルを閉じる
	file.close();

	// 生成されたポインタを返す
	return pMotion;
}
//=================================
// モーションセット
//=================================
void CMotion::SetMotion(int motiontype)
{
	// 同じだったら
	if (m_motiontype == motiontype)
	{
		return;
	}

	// 代入
	m_motiontype = motiontype;
	m_nKey = 0;
	m_nCounterMotion = 0;
	m_isFinishMotion = false;
}
//==============================
// モーション更新処理
//==============================
void CMotion::Update(CModel** ppModel, const int nMaxPart)
{
	// モデル数格納
	int nNumModel = nMaxPart;
	
	// 例外処理
	if (nNumModel <= 0)
	{
		// 警告表示
		MessageBox(GetActiveWindow(), "モデルが存在しません", "キャラクターエラー", MB_OK);

		// 下の処理に入らないようにする
		return;
	}

	// 最大モデル数で回す
	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// 現在モーションキー計算
		m_motiontype = Clump(m_motiontype, 0, m_nNumMotion);
		m_nNextKey = Wrap(m_nKey + 1, 0, m_aMotionInfo[m_motiontype].nNumKey -1);

		// ブレンドのモーション設定
		m_motiontypeBlend = Clump(m_motiontypeBlend, 0, 1);
		m_nNextKeyBlend = Wrap(m_nKeyBlend + 1, 0, m_aMotionInfo[m_motiontypeBlend].nNumKey - 1);

		// 現在のモーション更新
		UpdateCurrentMotion(ppModel, nCnt);
	}

	// 通常モーションのフレームを進める
	if (m_nCounterMotion >= m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].nFrame)
	{
		// モーションカウントを初期化
		m_nCounterMotion = 0;

		// アクションモーションの時,最後のキーで止める
		if (m_motiontype == TYPE_ACTION || m_motiontype == TYPE_JUMPATTACK)
		{
			if (m_nKey < m_aMotionInfo[m_motiontype].nNumKey - 1)
			{
				// キー数加算
				m_nKey++;
			}
			else
			{
				// キーを最大数に設定
				m_nKey = m_aMotionInfo[m_motiontype].nNumKey - 2;

				// モーションカウントを最後のフレームの値に設定
				m_nCounterMotion = m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].nFrame;
			}
		}
		else
		{
			// 通常はループ
			m_nCounterMotion = 0;

			// キー数をラップ
			m_nKey = Wrap(m_nKey + 1, 0, m_aMotionInfo[m_motiontype].nNumKey - 1);
		}
	}
	else
	{
		// モーションカウントを加算
		m_nCounterMotion++;
	}

	// アクションなら
	if (m_motiontype == TYPE_ACTION || m_motiontype == TYPE_JUMPATTACK)
	{
		m_isFinishMotion = true;

		// キーを最大数に設定
		m_nKey = m_aMotionInfo[m_motiontype].nNumKey - 2;

		// モーションカウントを最後のフレームの値に設定
		m_nCounterMotion = m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].nFrame;

		return;
	}

	// Loop がfalse かつ キー数が超えたら
	if (m_aMotionInfo[m_motiontype].bLoop == false && m_aMotionInfo[m_motiontype].nNumKey - 1 <= m_nKey && m_motiontype != TYPE_ACTION)
	{
		// ニュートラルモーションに変更
		m_motiontype = TYPE_NEUTRAL;

		// キー数を初期化
		m_nKey = 0;

		// モーションカウントを初期化
		m_nCounterMotion = 0;
	}
}
//======================================
// 現在のモーションの更新関数
//======================================
void CMotion::UpdateCurrentMotion(CModel** ppModel, int nModelCount)
{
	// キー情報から位置と向きを算出
	D3DXVECTOR3 posMotion, rotMotion;

	// 座標の差分を計算する
	posMotion.x = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fPosX - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosX;
	posMotion.y = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fPosY - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosY;
	posMotion.z = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fPosZ - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosZ;

	// 角度の差分を計算
	rotMotion.x = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fRotX - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotX;
	rotMotion.y = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fRotY - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotY;
	rotMotion.z = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fRotZ - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotZ;

	// 求める値を保存する変数を宣言
	D3DXVECTOR3 Pos, Rot;

	// 補間係数を計算
	float fDis = (float)m_nCounterMotion / m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].nFrame;

	// 座標計算
	Pos.x = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosX + posMotion.x * fDis);
	Pos.y = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosY + posMotion.y * fDis);
	Pos.z = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosZ + posMotion.z * fDis);

	// 角度計算
	Rot.x = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotX + rotMotion.x * fDis);
	Rot.y = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotY + rotMotion.y * fDis);
	Rot.z = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotZ + rotMotion.z * fDis);

	// モデルのパーツに設定
	ppModel[nModelCount]->SetPos(Pos);
	ppModel[nModelCount]->SetRot(Rot);
}
//======================================
// ブレンドモーションの更新関数
//======================================
void CMotion::UpdateBlend(CModel** ppModel, int nModelCount)
{
// 現在キーの更新関係---------------------------------------------------------------------------------------
	// キー情報から位置と向きを算出
	D3DXVECTOR3 posMotion, rotMotion;

	// 座標の差分を計算する
	posMotion.x = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fPosX - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosX;
	posMotion.y = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fPosY - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosY;
	posMotion.z = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fPosZ - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosZ;

	// 角度の差分を計算
	rotMotion.x = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fRotX - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotX;
	rotMotion.y = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fRotY - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotY;
	rotMotion.z = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fRotZ - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotZ;

	// 求める値を保存する変数を宣言
	D3DXVECTOR3 Pos, Rot;

	// 補間係数を計算
	float fDis = (float)m_nCounterMotion / m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].nFrame;

	// 座標計算
	Pos.x = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosX + posMotion.x * fDis);
	Pos.y = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosY + posMotion.y * fDis);
	Pos.z = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosZ + posMotion.z * fDis);

	// 角度計算
	Rot.x = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotX + rotMotion.x * fDis);
	Rot.y = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotY + rotMotion.y * fDis);
	Rot.z = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotZ + rotMotion.z * fDis);

// ブレンド関連の更新処理---------------------------------------------------------------------------------------

	// キー変数
	KEY KeyLastSet,DiffBlendKey,BlendKey;
	KEY NextBlendKey, NowBlendKey,DiffBlend;

	// フレーム計算
	float fRateMotion, fRateBlend;
	
	// ブレンドの相対値計算
	fRateMotion = (float)m_nCounterBlend / (float)m_aMotionInfo[m_motiontypeBlend].aKeyInfo[m_nKeyBlend].nFrame;

	// ブレンドのフレーム
	fRateBlend = (float)m_nCounterBlend / (float)m_nFrameBlend;

	// キー代入
	NextBlendKey = m_aMotionInfo[m_motiontypeBlend].aKeyInfo[m_nNextKeyBlend].aKey[nModelCount];
	NowBlendKey = m_aMotionInfo[m_motiontypeBlend].aKeyInfo[m_nKeyBlend].aKey[nModelCount];

// 差分計算-----------------------------
	DiffBlendKey.fRotX = NextBlendKey.fRotX - NowBlendKey.fRotX;
	DiffBlendKey.fRotY = NextBlendKey.fRotY - NowBlendKey.fRotY;
	DiffBlendKey.fRotZ = NextBlendKey.fRotZ - NowBlendKey.fRotZ;

// 差分計算-----------------------------
	DiffBlendKey.fPosX = NextBlendKey.fPosX - NowBlendKey.fPosX;
	DiffBlendKey.fPosY = NextBlendKey.fPosY - NowBlendKey.fPosY;
	DiffBlendKey.fPosZ = NextBlendKey.fPosZ - NowBlendKey.fPosZ;

// ブレンドの差分-----------------------
	BlendKey.fRotX = NowBlendKey.fRotX + (DiffBlendKey.fRotX * fRateMotion);
	BlendKey.fRotY = NowBlendKey.fRotY + (DiffBlendKey.fRotY * fRateMotion);
	BlendKey.fRotZ = NowBlendKey.fRotZ + (DiffBlendKey.fRotZ * fRateMotion);

// ブレンドの差分-----------------------
	BlendKey.fPosX = NowBlendKey.fPosX + (DiffBlendKey.fPosX * fRateMotion);
	BlendKey.fPosY = NowBlendKey.fPosY + (DiffBlendKey.fPosY * fRateMotion);
	BlendKey.fPosZ = NowBlendKey.fPosZ + (DiffBlendKey.fPosZ * fRateMotion);

// 全体の差分計算-----------------------
	DiffBlend.fRotX = BlendKey.fRotX - Rot.x;
	DiffBlend.fRotY = BlendKey.fRotY - Rot.y;
	DiffBlend.fRotZ = BlendKey.fRotZ - Rot.z;

	DiffBlend.fPosX = BlendKey.fPosX - Pos.x;
	DiffBlend.fPosY = BlendKey.fPosY - Pos.y;
	DiffBlend.fPosZ = BlendKey.fPosZ - Pos.z;

// 値を求める---------------------------
	KeyLastSet.fRotX = Rot.x + (DiffBlend.fRotX * fRateBlend);
	KeyLastSet.fRotY = Rot.y + (DiffBlend.fRotY * fRateBlend);
	KeyLastSet.fRotZ = Rot.z + (DiffBlend.fRotZ * fRateBlend);

	KeyLastSet.fPosX = Pos.x + (DiffBlend.fPosX * fRateBlend);
	KeyLastSet.fPosY = Pos.y + (DiffBlend.fPosY * fRateBlend);
	KeyLastSet.fPosZ = Pos.z + (DiffBlend.fPosZ * fRateBlend);

// 向き、座標を設定---------------------
	ppModel[nModelCount]->SetPos(D3DXVECTOR3(KeyLastSet.fRotX, KeyLastSet.fRotY, KeyLastSet.fRotZ));
	ppModel[nModelCount]->SetRot(D3DXVECTOR3(KeyLastSet.fPosX, KeyLastSet.fPosY, KeyLastSet.fPosZ));
}

//======================================
// モデル数読み込み
//======================================
void CMotion::SetModels(std::istringstream& iss, int& nModel, int nMaxParts)
{
	// 文字列設定
	std::string eq;

	// 読み込んだモデル数を設定
	iss >> eq >> nModel;

	// 例外処理
	if (nModel > nMaxParts)
	{
		MessageBox(NULL, "最大モデル数を超えています", "モデル数エラー", MB_OK);
		return;
	}
}
//======================================
// モデルファイル読み込み
//======================================
void CMotion::SetModelFile(std::istringstream& iss, CModel** pModel, int nCnt)
{
	// 読み込み用文字列
	std::string eq, filename;

	// ファイルから読み込んだものを設定
	iss >> eq >> filename;

	// モデルの生成処理
	CModel* pNewModel = CModel::Create(VECTOR3_NULL, VECTOR3_NULL, filename.c_str());

	// モデルのポインタに格納
	pModel[nCnt] = pNewModel;
}
//======================================
// パーツごとの設定
//======================================
void CMotion::SetParts(std::ifstream& file, CModel** pModel)
{
	// 読み込み文字列
	std::string line;

	// モデル番号と親モデルの設定用変数
	int nIdx = -1, nParentIdx = -1;

	// 代入用座標と角度
	D3DXVECTOR3 pos = VECTOR3_NULL;
	D3DXVECTOR3 rot = VECTOR3_NULL;

	// 読み込みループ
	while (std::getline(file, line))
	{
		std::istringstream partss(line);
		std::string cmd;
		partss >> cmd;

		// "INDEX"を読み取った
		if (cmd == "INDEX")
		{
			// インデックスの変数に値を代入
			std::string eq; 
			partss >> eq >> nIdx;
		}
		// "PARENT"を読み取った
		else if (cmd == "PARENT")
		{
			// 親モデル設定用の変数に値を代入
			std::string eq;
			partss >> eq >> nParentIdx;

			// インデックスの範囲内なら
			if (nIdx >= 0 && pModel[nIdx])
			{
				if (nParentIdx != -1)
				{// 親モデルが存在する
					pModel[nIdx]->SetParent(pModel[nParentIdx]);
				}
				else
				{// 親モデルが存在しない
					pModel[nIdx]->SetParent(nullptr);
				}
			}	
		}
		// "POS"を読み取った
		else if (cmd == "POS")
		{
			// 読み取った座標を代入
			std::string eq; 
			partss >> eq >> pos.x >> pos.y >> pos.z;

			// 範囲内かチェック
			if (nIdx >= 0 && pModel[nIdx])
			{
				// モデルのオフセット座標に値を設定
				pModel[nIdx]->OffSetPos(pos);
			}
		}
		// "ROT"を読み取った
		else if (cmd == "ROT")
		{
			// 読み取った角度を代入
			std::string eq; 
			partss >> eq >> rot.x >> rot.y >> rot.z;

			// 範囲内かチェック
			if (nIdx >= 0 && pModel[nIdx])
			{
				// モデルのオフセット角度に値を設定
				pModel[nIdx]->OffSetRot(rot);
			}
		}
		// "PARTTYPE"を読み取った
		else if (cmd == "PARTTYPE")
		{
			// 読み取った種類を代入する
			std::string eq, partTypeStr;
			partss >> eq >> partTypeStr;

			// インデックスが0以上 かつ モデルがあるなら
			if (nIdx >= NULL && pModel[nIdx])
			{
				if (partTypeStr == "WEAPON") { pModel[nIdx]->SetPartType(CModel::PARTTYPE_WEAPON); }// 武器の部位
				else { pModel[nIdx]->SetPartType(CModel::PARTTYPE_NONE); }							// 無いときはこれにする
			}
		}
		// "END_PARTSSET"を読み取った
		else if (cmd == "END_PARTSSET")
		{
			break;
		}
	}
}
//======================================
// パーツごとのモーション設定
//======================================
void CMotion::SetPartsMotion(std::ifstream& file, CMotion* pMotion, int nCntMotion)
{
	// 読み込み用1行分のバッファ
	std::string line;

	// キーセット数のカウント用変数
	int nCntKey = 0;

	// モーション設定の読み取りループ
	while (std::getline(file, line))
	{
		// 行データを分解してトークンを解析
		std::istringstream motionss(line);
		std::string token;
		motionss >> token;

		// "LOOP"を読み取った
		if (token == "LOOP")
		{
			std::string eq;
			int loopFlag;
			motionss >> eq >> loopFlag;

			// "="を読み取った
			if (eq == "=")
			{
				// モーションループフラグを設定
				pMotion->m_aMotionInfo[nCntMotion].bLoop = loopFlag;
			}
		}
		// "NUM_KEY"を読み取った
		else if (token == "NUM_KEY")
		{
			std::string eq;
			int numKeys;
			motionss >> eq >> numKeys;

			// "="を読み取った
			if (eq == "=")
			{
				// キーの総数設定
				pMotion->m_aMotionInfo[nCntMotion].nNumKey = numKeys;

				// 指定された数だけキーセットを読み込む
				while (nCntKey < numKeys)
				{
					// キーセット読み込み関数
					SetKey(file, pMotion, nCntMotion, nCntKey);

					// 次のモーションのキーへ
					nCntKey++;
				}
			}
		}
		// "END_MOTIONSET"を読み取った
		else if (token == "END_MOTIONSET")
		{
			nCntKey = 0;
			break;
		}
	}
}
//======================================
// モーションキー全体の設定
//======================================
void CMotion::SetKey(std::ifstream& file, CMotion* pMotion, int nCntMotion, int nCntKey)
{
	std::string line;

	// 使用変数
	int frame = 0;
	int posKeyIndex = 0, rotKeyIndex = 0;

	// 読んだかフラグ
	bool ReadKey = false;

	// FRAME 行の読み込み
	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		std::string cmd, eq;
		ss >> cmd >> eq >> frame;

		// 読み取り一致時
		if (cmd == "FRAME" && eq == "=")
		{
			pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = frame;
			break;
		}
	}

	// 読み込みループ
	while (std::getline(file, line))
	{
		if (line.find("END_KEYSET") != std::string::npos)
		{
			// キーセット終了
			break;
		}
		else if (line.find("KEY") != std::string::npos)
		{
			// 有効化
			ReadKey = true;

			continue;
		}
		else if (ReadKey)
		{
			std::istringstream ss(line);
			std::string param;
			ss >> param;

			if (param == "POS" || param == "ROT")
			{
				// 各キーごとの設定
				SetKeyDate(ss, param, pMotion, nCntMotion, nCntKey, posKeyIndex, rotKeyIndex);
			}
			else if (param == "END_KEY")
			{
				// 条件を無効化
				ReadKey = false;
			}
		}
	}
}
//======================================
// キーごとの情報設定
//======================================
void CMotion::SetKeyDate(std::istringstream& ss, const std::string& param, CMotion* pMotion, int nCntMotion, int nCntKey, int& posKeyIndex, int& rotKeyIndex)
{
	// 読み込み用1行分のバッファ
	std::string eq;

	// 代入用変数
    D3DXVECTOR3 Vec = VECTOR3_NULL;

	// 座標を代入
    ss >> eq >> Vec.x >> Vec.y >> Vec.z;

	// "="が来たら
    if (eq != "=") return;

	// キー情報変数に代入する
    KEY_INFO& keyInfo = pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey];

	// "POS"読み取り時
    if (param == "POS")
    {
		// 各座標に値を設定
        keyInfo.aKey[posKeyIndex].fPosX = Vec.x;
        keyInfo.aKey[posKeyIndex].fPosY = Vec.y;
        keyInfo.aKey[posKeyIndex].fPosZ = Vec.z;

		// キーカウントをインクリメント
        posKeyIndex++;
    }
	// "ROT"読み取り時
    else if (param == "ROT")
    {
		// 各角度に値を設定
        keyInfo.aKey[rotKeyIndex].fRotX = Vec.x;
        keyInfo.aKey[rotKeyIndex].fRotY = Vec.y;
        keyInfo.aKey[rotKeyIndex].fRotZ = Vec.z;

		// キーカウントをインクリメント
        rotKeyIndex++;
    }
}
