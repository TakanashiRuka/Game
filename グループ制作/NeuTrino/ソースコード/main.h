#ifndef MAIN_H
#define MAIN_H
#define _WINSOCKAPI_		//�ǉ�

#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#include "input.h"
#include "JoyStick.h"
#include "Struct.h"
#include "Define.h"
#include "debug.h"
#include "RenderState.h"
#include "TestMode.h"
#include "FadeInOut.h"
#include "ErrorList.h"

#define WINDOW_WIDTH    (1280)
#define WINDOW_HEIGHT   (720)

// �񋓌^��`
typedef enum {
	MODE_PREFACE,
	MODE_TITLE,
	MODE_DEMO,
	MODE_PLAYER_ENTRY,
	MODE_SELECT_SONG,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT,
	MODE_CONTINUE,
	MODE_THANKS,
	MODE_TEST,
	MODE_MAX
}GAMEMODE;

LPDIRECT3DDEVICE9 GetD3DDevice(void);

//------------------------------------------------------------------------------
//	�֐���:	SetMode
//	����:	MODE_XXXX
//	�߂�l:	�Ȃ�
//	����:	�Q�[�����[�h�̐ݒ�
//------------------------------------------------------------------------------
void SetMode(int nMode);

//------------------------------------------------------------------------------
//	�֐���:	GetGameMode
//	����:	�Ȃ�
//	�߂�l:	�񋓌^�Q�[�����[�h�̔ԍ�
//	����:	�Q�[�����[�h�̎擾
//------------------------------------------------------------------------------
int GetGameMode();

void SetGameModeTarget(int nMode);
int GetGameModeTarget();
void ChangeGameMode();

bool GetMultiPlayCancel();
bool GetOnline();
bool GetSetAuto();

HWND GethWnd();

#endif