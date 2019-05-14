//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	BGM_TITLE_P1,
	BGM_TITLE_P2,
	BGM_RESULT,
	BGM_THANKS,
	BGM_PLAYER_ENTRY,
	SE_TAP,
	SE_BREAK,
	SE_FLICK,
	SE_SELECT_LEVEL,
	SE_SELECT_SONG,
	SE_ENTER,
	SE_SETTING_OPEN,
	SE_SETTING_CLOSE,
	SE_FC_AP,
	SE_RESULT_DETAIL,
	SE_GAME_FC_AP,
	SE_GAME_CLEAR,
	SE_GAME_FAILED,
	SE_FADEIN_START,
	SE_FADEIN_CLOSE,
	SE_FINAL_CHECK,
	SE_ERROR,
	SE_JOIN,
	SE_MESSAGE,
	SE_READY,
	SE_CREATE,
	SE_PLAYER_ENTRY_INPUT,
	SE_PLAYER_ENTRY_CANCEL,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

//------------------------------------------------------------------------------
//	�֐���:	GetPlayStatus
//	����:	SOUND_LABEL	
//	�߂�l:	true false
//	����:	�Đ���Ԃ̎擾
//------------------------------------------------------------------------------
bool GetPlayStatus(SOUND_LABEL label);

void SetSoundVolume(SOUND_LABEL label, float fVolume);

void SetSoundVolumeAll(float fVolume);

#endif
