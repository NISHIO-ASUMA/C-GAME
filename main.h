//====================================
//
// ���C���֐� [ main.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _MAIN_H_ // ���̃}�N����`������ĂȂ�������
#define _MAIN_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//****************************
// �}�N����`
//****************************
#define CLASS_NAME "WindowClass"		// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME "C++3D"				// �E�C���h�E�̖��O(�L���v�V�����ɕ\��)

//*****************************
// ���C���v���Z�X�N���X���`
//*****************************
class CMainProc
{
public:
	CMainProc();
	~CMainProc();

	void ToggleFullScreen(HWND hWnd);

private:
	RECT m_Windowrect;	// �E�B���h�E�̕�

};

#endif
