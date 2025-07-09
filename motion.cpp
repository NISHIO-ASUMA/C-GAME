//====================================
//
// ���[�V�������� [ motion.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**************************
// �C���N���[�h�t�@�C���錾
//**************************
#include "motion.h"
#include "template.h"

//==============================
// �R���X�g���N�^
//==============================
CMotion::CMotion()
{
	// �l�̃N���A
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

// �u�����h---------------------------------------
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
// �f�X�g���N�^
//==============================
CMotion::~CMotion()
{
	// ����
}
//==============================
// ���[�V�����ǂݍ��݊֐�
//==============================
CMotion* CMotion::Load(const char* pFilename,const int nMaxParts, CModel** pModel)
{
	// ���[�V�����N���X�̃C���X�^���X����
	CMotion* pMotion = new CMotion;

	// �t�@�C���ݒ�
	std::ifstream file(pFilename);

	// �t�@�C����O�`�F�b�N
	if (!file)
	{
		MessageBox(NULL, "�t�@�C���I�[�v�����s", pFilename, MB_OK);

		// �|�C���^�̔j��
		delete pMotion;

		// ���s���ʂ�Ԃ�
		return nullptr;
	}

	// ��s�ǂݍ���
	std::string line;

	// �g�p���[�J���ϐ�
	int nModel = 0;
	int nIdx = 0;
	int nCntMotion = 0;

	// �������ǂݑ�����
	while (std::getline(file, line))
	{
		// �g�[�N���ݒ�
		std::istringstream iss(line);
		std::string token;
		iss >> token;

		// "NUM_MODEL"�ǂݎ�莞
		if (token == "NUM_MODEL")
		{
			// ���f�����ݒ�
			pMotion->SetModels(iss, nModel, nMaxParts);
		}
		// "MODEL_FILENAME"�ǂݎ�莞
		else if (token == "MODEL_FILENAME")
		{
			// ���f���t�@�C���ǂݍ���
			pMotion->SetModelFile(iss, pModel, nIdx);

			// �C���f�b�N�X�J�E���g�����Z
			nIdx++;
		}
		// "PARTSSET"�ǂݎ�莞
		else if (token == "PARTSSET")
		{
			// �p�[�c���Ƃ̃Z�b�g
			pMotion->SetParts(file, pModel);
		}
		// "MOTIONSET"�ǂݎ�莞
		else if (token == "MOTIONSET")
		{
			// �p�[�c�̃��[�V�����̐ݒ�
			pMotion->SetPartsMotion(file, pMotion, nCntMotion);

			// ���[�V�����J�E���g�����Z
			nCntMotion++;
		}
		// "END_SCRIPT"�ǂݎ�莞
		else if (token == "END_SCRIPT")
		{
			break;
		}
	}

	// �t�@�C�������
	file.close();

	// �������ꂽ�|�C���^��Ԃ�
	return pMotion;
}
//=================================
// ���[�V�����Z�b�g
//=================================
void CMotion::SetMotion(int motiontype)
{
	// ������������
	if (m_motiontype == motiontype)
	{
		return;
	}

	// ���
	m_motiontype = motiontype;
	m_nKey = 0;
	m_nCounterMotion = 0;
	m_isFinishMotion = false;
}
//==============================
// ���[�V�����X�V����
//==============================
void CMotion::Update(CModel** ppModel, const int nMaxPart)
{
	// ���f�����i�[
	int nNumModel = nMaxPart;
	
	// ��O����
	if (nNumModel <= 0)
	{
		// �x���\��
		MessageBox(GetActiveWindow(), "���f�������݂��܂���", "�L�����N�^�[�G���[", MB_OK);

		// ���̏����ɓ���Ȃ��悤�ɂ���
		return;
	}

	// �ő僂�f�����ŉ�
	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// ���݃��[�V�����L�[�v�Z
		m_motiontype = Clump(m_motiontype, 0, m_nNumMotion);
		m_nNextKey = Wrap(m_nKey + 1, 0, m_aMotionInfo[m_motiontype].nNumKey -1);

		// �u�����h�̃��[�V�����ݒ�
		m_motiontypeBlend = Clump(m_motiontypeBlend, 0, 1);
		m_nNextKeyBlend = Wrap(m_nKeyBlend + 1, 0, m_aMotionInfo[m_motiontypeBlend].nNumKey - 1);

		// ���݂̃��[�V�����X�V
		UpdateCurrentMotion(ppModel, nCnt);
	}

	// �ʏ탂�[�V�����̃t���[����i�߂�
	if (m_nCounterMotion >= m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].nFrame)
	{
		// ���[�V�����J�E���g��������
		m_nCounterMotion = 0;

		// �A�N�V�������[�V�����̎�,�Ō�̃L�[�Ŏ~�߂�
		if (m_motiontype == TYPE_ACTION || m_motiontype == TYPE_JUMPATTACK)
		{
			if (m_nKey < m_aMotionInfo[m_motiontype].nNumKey - 1)
			{
				// �L�[�����Z
				m_nKey++;
			}
			else
			{
				// �L�[���ő吔�ɐݒ�
				m_nKey = m_aMotionInfo[m_motiontype].nNumKey - 2;

				// ���[�V�����J�E���g���Ō�̃t���[���̒l�ɐݒ�
				m_nCounterMotion = m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].nFrame;
			}
		}
		else
		{
			// �ʏ�̓��[�v
			m_nCounterMotion = 0;

			// �L�[�������b�v
			m_nKey = Wrap(m_nKey + 1, 0, m_aMotionInfo[m_motiontype].nNumKey - 1);
		}
	}
	else
	{
		// ���[�V�����J�E���g�����Z
		m_nCounterMotion++;
	}

	// �A�N�V�����Ȃ�
	if (m_motiontype == TYPE_ACTION || m_motiontype == TYPE_JUMPATTACK)
	{
		m_isFinishMotion = true;

		// �L�[���ő吔�ɐݒ�
		m_nKey = m_aMotionInfo[m_motiontype].nNumKey - 2;

		// ���[�V�����J�E���g���Ō�̃t���[���̒l�ɐݒ�
		m_nCounterMotion = m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].nFrame;

		return;
	}

	// Loop ��false ���� �L�[������������
	if (m_aMotionInfo[m_motiontype].bLoop == false && m_aMotionInfo[m_motiontype].nNumKey - 1 <= m_nKey && m_motiontype != TYPE_ACTION)
	{
		// �j���[�g�������[�V�����ɕύX
		m_motiontype = TYPE_NEUTRAL;

		// �L�[����������
		m_nKey = 0;

		// ���[�V�����J�E���g��������
		m_nCounterMotion = 0;
	}
}
//======================================
// ���݂̃��[�V�����̍X�V�֐�
//======================================
void CMotion::UpdateCurrentMotion(CModel** ppModel, int nModelCount)
{
	// �L�[��񂩂�ʒu�ƌ������Z�o
	D3DXVECTOR3 posMotion, rotMotion;

	// ���W�̍������v�Z����
	posMotion.x = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fPosX - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosX;
	posMotion.y = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fPosY - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosY;
	posMotion.z = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fPosZ - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosZ;

	// �p�x�̍������v�Z
	rotMotion.x = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fRotX - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotX;
	rotMotion.y = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fRotY - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotY;
	rotMotion.z = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fRotZ - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotZ;

	// ���߂�l��ۑ�����ϐ���錾
	D3DXVECTOR3 Pos, Rot;

	// ��ԌW�����v�Z
	float fDis = (float)m_nCounterMotion / m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].nFrame;

	// ���W�v�Z
	Pos.x = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosX + posMotion.x * fDis);
	Pos.y = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosY + posMotion.y * fDis);
	Pos.z = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosZ + posMotion.z * fDis);

	// �p�x�v�Z
	Rot.x = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotX + rotMotion.x * fDis);
	Rot.y = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotY + rotMotion.y * fDis);
	Rot.z = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotZ + rotMotion.z * fDis);

	// ���f���̃p�[�c�ɐݒ�
	ppModel[nModelCount]->SetPos(Pos);
	ppModel[nModelCount]->SetRot(Rot);
}
//======================================
// �u�����h���[�V�����̍X�V�֐�
//======================================
void CMotion::UpdateBlend(CModel** ppModel, int nModelCount)
{
// ���݃L�[�̍X�V�֌W---------------------------------------------------------------------------------------
	// �L�[��񂩂�ʒu�ƌ������Z�o
	D3DXVECTOR3 posMotion, rotMotion;

	// ���W�̍������v�Z����
	posMotion.x = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fPosX - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosX;
	posMotion.y = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fPosY - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosY;
	posMotion.z = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fPosZ - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosZ;

	// �p�x�̍������v�Z
	rotMotion.x = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fRotX - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotX;
	rotMotion.y = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fRotY - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotY;
	rotMotion.z = m_aMotionInfo[m_motiontype].aKeyInfo[m_nNextKey].aKey[nModelCount].fRotZ - m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotZ;

	// ���߂�l��ۑ�����ϐ���錾
	D3DXVECTOR3 Pos, Rot;

	// ��ԌW�����v�Z
	float fDis = (float)m_nCounterMotion / m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].nFrame;

	// ���W�v�Z
	Pos.x = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosX + posMotion.x * fDis);
	Pos.y = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosY + posMotion.y * fDis);
	Pos.z = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fPosZ + posMotion.z * fDis);

	// �p�x�v�Z
	Rot.x = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotX + rotMotion.x * fDis);
	Rot.y = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotY + rotMotion.y * fDis);
	Rot.z = (m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].aKey[nModelCount].fRotZ + rotMotion.z * fDis);

// �u�����h�֘A�̍X�V����---------------------------------------------------------------------------------------

	// �L�[�ϐ�
	KEY KeyLastSet,DiffBlendKey,BlendKey;
	KEY NextBlendKey, NowBlendKey,DiffBlend;

	// �t���[���v�Z
	float fRateMotion, fRateBlend;
	
	// �u�����h�̑��Βl�v�Z
	fRateMotion = (float)m_nCounterBlend / (float)m_aMotionInfo[m_motiontypeBlend].aKeyInfo[m_nKeyBlend].nFrame;

	// �u�����h�̃t���[��
	fRateBlend = (float)m_nCounterBlend / (float)m_nFrameBlend;

	// �L�[���
	NextBlendKey = m_aMotionInfo[m_motiontypeBlend].aKeyInfo[m_nNextKeyBlend].aKey[nModelCount];
	NowBlendKey = m_aMotionInfo[m_motiontypeBlend].aKeyInfo[m_nKeyBlend].aKey[nModelCount];

// �����v�Z-----------------------------
	DiffBlendKey.fRotX = NextBlendKey.fRotX - NowBlendKey.fRotX;
	DiffBlendKey.fRotY = NextBlendKey.fRotY - NowBlendKey.fRotY;
	DiffBlendKey.fRotZ = NextBlendKey.fRotZ - NowBlendKey.fRotZ;

// �����v�Z-----------------------------
	DiffBlendKey.fPosX = NextBlendKey.fPosX - NowBlendKey.fPosX;
	DiffBlendKey.fPosY = NextBlendKey.fPosY - NowBlendKey.fPosY;
	DiffBlendKey.fPosZ = NextBlendKey.fPosZ - NowBlendKey.fPosZ;

// �u�����h�̍���-----------------------
	BlendKey.fRotX = NowBlendKey.fRotX + (DiffBlendKey.fRotX * fRateMotion);
	BlendKey.fRotY = NowBlendKey.fRotY + (DiffBlendKey.fRotY * fRateMotion);
	BlendKey.fRotZ = NowBlendKey.fRotZ + (DiffBlendKey.fRotZ * fRateMotion);

// �u�����h�̍���-----------------------
	BlendKey.fPosX = NowBlendKey.fPosX + (DiffBlendKey.fPosX * fRateMotion);
	BlendKey.fPosY = NowBlendKey.fPosY + (DiffBlendKey.fPosY * fRateMotion);
	BlendKey.fPosZ = NowBlendKey.fPosZ + (DiffBlendKey.fPosZ * fRateMotion);

// �S�̂̍����v�Z-----------------------
	DiffBlend.fRotX = BlendKey.fRotX - Rot.x;
	DiffBlend.fRotY = BlendKey.fRotY - Rot.y;
	DiffBlend.fRotZ = BlendKey.fRotZ - Rot.z;

	DiffBlend.fPosX = BlendKey.fPosX - Pos.x;
	DiffBlend.fPosY = BlendKey.fPosY - Pos.y;
	DiffBlend.fPosZ = BlendKey.fPosZ - Pos.z;

// �l�����߂�---------------------------
	KeyLastSet.fRotX = Rot.x + (DiffBlend.fRotX * fRateBlend);
	KeyLastSet.fRotY = Rot.y + (DiffBlend.fRotY * fRateBlend);
	KeyLastSet.fRotZ = Rot.z + (DiffBlend.fRotZ * fRateBlend);

	KeyLastSet.fPosX = Pos.x + (DiffBlend.fPosX * fRateBlend);
	KeyLastSet.fPosY = Pos.y + (DiffBlend.fPosY * fRateBlend);
	KeyLastSet.fPosZ = Pos.z + (DiffBlend.fPosZ * fRateBlend);

// �����A���W��ݒ�---------------------
	ppModel[nModelCount]->SetPos(D3DXVECTOR3(KeyLastSet.fRotX, KeyLastSet.fRotY, KeyLastSet.fRotZ));
	ppModel[nModelCount]->SetRot(D3DXVECTOR3(KeyLastSet.fPosX, KeyLastSet.fPosY, KeyLastSet.fPosZ));
}

//======================================
// ���f�����ǂݍ���
//======================================
void CMotion::SetModels(std::istringstream& iss, int& nModel, int nMaxParts)
{
	// ������ݒ�
	std::string eq;

	// �ǂݍ��񂾃��f������ݒ�
	iss >> eq >> nModel;

	// ��O����
	if (nModel > nMaxParts)
	{
		MessageBox(NULL, "�ő僂�f�����𒴂��Ă��܂�", "���f�����G���[", MB_OK);
		return;
	}
}
//======================================
// ���f���t�@�C���ǂݍ���
//======================================
void CMotion::SetModelFile(std::istringstream& iss, CModel** pModel, int nCnt)
{
	// �ǂݍ��ݗp������
	std::string eq, filename;

	// �t�@�C������ǂݍ��񂾂��̂�ݒ�
	iss >> eq >> filename;

	// ���f���̐�������
	CModel* pNewModel = CModel::Create(VECTOR3_NULL, VECTOR3_NULL, filename.c_str());

	// ���f���̃|�C���^�Ɋi�[
	pModel[nCnt] = pNewModel;
}
//======================================
// �p�[�c���Ƃ̐ݒ�
//======================================
void CMotion::SetParts(std::ifstream& file, CModel** pModel)
{
	// �ǂݍ��ݕ�����
	std::string line;

	// ���f���ԍ��Ɛe���f���̐ݒ�p�ϐ�
	int nIdx = -1, nParentIdx = -1;

	// ����p���W�Ɗp�x
	D3DXVECTOR3 pos = VECTOR3_NULL;
	D3DXVECTOR3 rot = VECTOR3_NULL;

	// �ǂݍ��݃��[�v
	while (std::getline(file, line))
	{
		std::istringstream partss(line);
		std::string cmd;
		partss >> cmd;

		// "INDEX"��ǂݎ����
		if (cmd == "INDEX")
		{
			// �C���f�b�N�X�̕ϐ��ɒl����
			std::string eq; 
			partss >> eq >> nIdx;
		}
		// "PARENT"��ǂݎ����
		else if (cmd == "PARENT")
		{
			// �e���f���ݒ�p�̕ϐ��ɒl����
			std::string eq;
			partss >> eq >> nParentIdx;

			// �C���f�b�N�X�͈͓̔��Ȃ�
			if (nIdx >= 0 && pModel[nIdx])
			{
				if (nParentIdx != -1)
				{// �e���f�������݂���
					pModel[nIdx]->SetParent(pModel[nParentIdx]);
				}
				else
				{// �e���f�������݂��Ȃ�
					pModel[nIdx]->SetParent(nullptr);
				}
			}	
		}
		// "POS"��ǂݎ����
		else if (cmd == "POS")
		{
			// �ǂݎ�������W����
			std::string eq; 
			partss >> eq >> pos.x >> pos.y >> pos.z;

			// �͈͓����`�F�b�N
			if (nIdx >= 0 && pModel[nIdx])
			{
				// ���f���̃I�t�Z�b�g���W�ɒl��ݒ�
				pModel[nIdx]->OffSetPos(pos);
			}
		}
		// "ROT"��ǂݎ����
		else if (cmd == "ROT")
		{
			// �ǂݎ�����p�x����
			std::string eq; 
			partss >> eq >> rot.x >> rot.y >> rot.z;

			// �͈͓����`�F�b�N
			if (nIdx >= 0 && pModel[nIdx])
			{
				// ���f���̃I�t�Z�b�g�p�x�ɒl��ݒ�
				pModel[nIdx]->OffSetRot(rot);
			}
		}
		// "PARTTYPE"��ǂݎ����
		else if (cmd == "PARTTYPE")
		{
			// �ǂݎ������ނ�������
			std::string eq, partTypeStr;
			partss >> eq >> partTypeStr;

			// �C���f�b�N�X��0�ȏ� ���� ���f��������Ȃ�
			if (nIdx >= NULL && pModel[nIdx])
			{
				if (partTypeStr == "WEAPON") { pModel[nIdx]->SetPartType(CModel::PARTTYPE_WEAPON); }// ����̕���
				else { pModel[nIdx]->SetPartType(CModel::PARTTYPE_NONE); }							// �����Ƃ��͂���ɂ���
			}
		}
		// "END_PARTSSET"��ǂݎ����
		else if (cmd == "END_PARTSSET")
		{
			break;
		}
	}
}
//======================================
// �p�[�c���Ƃ̃��[�V�����ݒ�
//======================================
void CMotion::SetPartsMotion(std::ifstream& file, CMotion* pMotion, int nCntMotion)
{
	// �ǂݍ��ݗp1�s���̃o�b�t�@
	std::string line;

	// �L�[�Z�b�g���̃J�E���g�p�ϐ�
	int nCntKey = 0;

	// ���[�V�����ݒ�̓ǂݎ�胋�[�v
	while (std::getline(file, line))
	{
		// �s�f�[�^�𕪉����ăg�[�N�������
		std::istringstream motionss(line);
		std::string token;
		motionss >> token;

		// "LOOP"��ǂݎ����
		if (token == "LOOP")
		{
			std::string eq;
			int loopFlag;
			motionss >> eq >> loopFlag;

			// "="��ǂݎ����
			if (eq == "=")
			{
				// ���[�V�������[�v�t���O��ݒ�
				pMotion->m_aMotionInfo[nCntMotion].bLoop = loopFlag;
			}
		}
		// "NUM_KEY"��ǂݎ����
		else if (token == "NUM_KEY")
		{
			std::string eq;
			int numKeys;
			motionss >> eq >> numKeys;

			// "="��ǂݎ����
			if (eq == "=")
			{
				// �L�[�̑����ݒ�
				pMotion->m_aMotionInfo[nCntMotion].nNumKey = numKeys;

				// �w�肳�ꂽ�������L�[�Z�b�g��ǂݍ���
				while (nCntKey < numKeys)
				{
					// �L�[�Z�b�g�ǂݍ��݊֐�
					SetKey(file, pMotion, nCntMotion, nCntKey);

					// ���̃��[�V�����̃L�[��
					nCntKey++;
				}
			}
		}
		// "END_MOTIONSET"��ǂݎ����
		else if (token == "END_MOTIONSET")
		{
			nCntKey = 0;
			break;
		}
	}
}
//======================================
// ���[�V�����L�[�S�̂̐ݒ�
//======================================
void CMotion::SetKey(std::ifstream& file, CMotion* pMotion, int nCntMotion, int nCntKey)
{
	std::string line;

	// �g�p�ϐ�
	int frame = 0;
	int posKeyIndex = 0, rotKeyIndex = 0;

	// �ǂ񂾂��t���O
	bool ReadKey = false;

	// FRAME �s�̓ǂݍ���
	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		std::string cmd, eq;
		ss >> cmd >> eq >> frame;

		// �ǂݎ���v��
		if (cmd == "FRAME" && eq == "=")
		{
			pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = frame;
			break;
		}
	}

	// �ǂݍ��݃��[�v
	while (std::getline(file, line))
	{
		if (line.find("END_KEYSET") != std::string::npos)
		{
			// �L�[�Z�b�g�I��
			break;
		}
		else if (line.find("KEY") != std::string::npos)
		{
			// �L����
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
				// �e�L�[���Ƃ̐ݒ�
				SetKeyDate(ss, param, pMotion, nCntMotion, nCntKey, posKeyIndex, rotKeyIndex);
			}
			else if (param == "END_KEY")
			{
				// �����𖳌���
				ReadKey = false;
			}
		}
	}
}
//======================================
// �L�[���Ƃ̏��ݒ�
//======================================
void CMotion::SetKeyDate(std::istringstream& ss, const std::string& param, CMotion* pMotion, int nCntMotion, int nCntKey, int& posKeyIndex, int& rotKeyIndex)
{
	// �ǂݍ��ݗp1�s���̃o�b�t�@
	std::string eq;

	// ����p�ϐ�
    D3DXVECTOR3 Vec = VECTOR3_NULL;

	// ���W����
    ss >> eq >> Vec.x >> Vec.y >> Vec.z;

	// "="��������
    if (eq != "=") return;

	// �L�[���ϐ��ɑ������
    KEY_INFO& keyInfo = pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey];

	// "POS"�ǂݎ�莞
    if (param == "POS")
    {
		// �e���W�ɒl��ݒ�
        keyInfo.aKey[posKeyIndex].fPosX = Vec.x;
        keyInfo.aKey[posKeyIndex].fPosY = Vec.y;
        keyInfo.aKey[posKeyIndex].fPosZ = Vec.z;

		// �L�[�J�E���g���C���N�������g
        posKeyIndex++;
    }
	// "ROT"�ǂݎ�莞
    else if (param == "ROT")
    {
		// �e�p�x�ɒl��ݒ�
        keyInfo.aKey[rotKeyIndex].fRotX = Vec.x;
        keyInfo.aKey[rotKeyIndex].fRotY = Vec.y;
        keyInfo.aKey[rotKeyIndex].fRotZ = Vec.z;

		// �L�[�J�E���g���C���N�������g
        rotKeyIndex++;
    }
}
