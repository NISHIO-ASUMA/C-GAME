//====================================
//
// �T�E���h���� [ sound.h ]
// Author: Asuma Nishio
//
//=====================================

//**************************
// ��d�C���N���[�h�K�[�h
//**************************
#ifndef _SOUND_H_ 
#define _SOUND_H_ 

//**************************
// �C���N���[�h�t�@�C���錾
//**************************


//******************************
// �T�E���h�N���X���`
//******************************
class CSound
{
public:
	// �T�E���h�̎�ޗ񋓌^
	typedef enum
	{
		SOUND_LABEL_TITLE_BGM = 0,	// �^�C�g��BGM
		SOUND_LABEL_BREAKSUPON_SE,  // �X�|�[���j��
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	// �T�E���h���̍\���̒�`
	typedef struct
	{
		const char* pFilename;	// �t�@�C����
		int nCntLoop;			// ���[�v�J�E���g
	} SOUNDINFO;

	// �T�E���h�̏��񋓌^�錾
	SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
	{
		{"data/BGM/titlebgm.wav", -1},	 // �^�C�g��
		{"data/SE/Break.wav",0},		 // �X�|�[���j��
	};

	IXAudio2* m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice* m_pMasteringVoice;				// �}�X�^�[�{�C�X
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE* m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y
	int m_SoundCount;										// �T�E���h�̃J�E���g
	int m_SoundInterval;									// �C���^�[�o���J�E���g

};
#endif