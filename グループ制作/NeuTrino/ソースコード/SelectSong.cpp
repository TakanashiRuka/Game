//----------------------------------------------------------------------------
//
//	�ȑI��
//
//														 ����ҁF����
//                                                       �쐬���F2017/12/26
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "SelectSongBG.h"
#include "SelectSongBoard.h"
#include "SelectSongWord.h"
#include "ReadSongList.h"
#include "SelectSongPanel.h"
#include "SelectSongFinalCheck.h"
#include "Camera.h"
#include "music.h"
#include "Setting.h"
#include "Note.h"
#include "JudgeSound.h"
#include "input.h"
#include "StatusPanel.h"				//�ǉ�
#include "SelectSongWaitMember.h"	//�ǉ�
#include "data.h"					//�ǉ�
#include "BtnManual.h"
#include "ScreenMask.h"
#include "Ranking.h"				//�ǉ�

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------
typedef enum {
	SSAT_BG = 0,
	SSAT_SONG_BOARD = 15,
	SSAT_SONG_WORD = 25,
	SSAT_SONG_PANEL = 25,
	SSAT_SONG_TITLE = 25,
	SSAT_SONG_LEVEL = 30,
	SSAT_SONG_ALBUM_FRAME = 33,
	SSAT_SETTING = 40,
	SSAT_MAX
}SELECT_SONG_ANIMATION_TIME;

//---------------------------�\���̒�`---------------------------------------

//---------------------------�O���[�o���ϐ�-----------------------------------
static int g_nCountSelectSongAnimation;				// �ȑI����ʂ̃A�j���[�V�����J�E���^�[
static bool g_bFadeIn;								// ����p�t���O

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitSelectSong()
{
	ResetData();
	g_nCountSelectSongAnimation = 0;
	g_bFadeIn = false;

	if (GetTestModeMemberStatus(TMML_GUEST))
	{
		if (GetGameMode() != MODE_GAME && GetGameMode() != MODE_CONTINUE)
		{
			SetHiSpeed(INIT_SPEED);
			SetOffsetNote(INIT_OFFSET);
			SetJudgeSoundVolume(INIT_SE_VOLUME);
		}
	}

	SetCamera(MODE_SELECT_SONG);
	InitSelectSongBG();
	InitSelectSongBoard();
	InitSelectSongWord();
	InitSelectSongPanel();
	InitSelectSongFinalCheck();
	InitSongTitle();
	InitSongSubTitle();
	InitNoteDesigner();
	InitLevelWord();
	InitLevelIcon();
	InitAlbumFrame();
	InitSetting();
	InitStatusPanel();				//�ǉ�
	InitSelectSongWaitMember();		//�ǉ�
	InitBtnManual(BMP_SELECT_SONG);
	InitScreenMask();
	//SetSongPanelA(RDP_END);
	//ChangeSelectNum(3);
	InitRanking();					//�ǉ�
	CheckRanking();
}

void UninitSelectSong()
{
	UninitSelectSongBG();
	UninitSelectSongBoard();
	UninitSelectSongWord();
	UninitSelectSongPanel();
	UninitSelectSongFinalCheck();
	UninitSongTitle();
	UninitSongSubTitle();
	UninitNoteDesigner();
	UninitLevelWord();
	UninitLevelIcon();
	UninitAlbumFrame();
	UninitSetting();
	UninitStatusPanel();			//�ǉ�
	UninitSelectSongWaitMember();	//�ǉ�
	UninitBtnManual();
	UninitScreenMask();
	UninitRanking();				//�ǉ�
}

void UpdateSelectSong()
{
	if (g_nCountSelectSongAnimation < SSAT_MAX)
	{
		g_nCountSelectSongAnimation++;
	}
	if (g_nCountSelectSongAnimation == SSAT_MAX)
	{
		if (!g_bFadeIn)
		{
			g_bFadeIn = true;
		}
	}

	if (g_nCountSelectSongAnimation >= SSAT_BG)
	{
		UpdateSelectSongBG();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SONG_BOARD)
	{
		UpdateSelectSongBoard();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SONG_WORD)
	{
		UpdateSelectSongWord();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SONG_PANEL)
	{
		UpdateSelectSongPanel();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SONG_TITLE)
	{
		UpdateSongTitle();
		UpdateSongSubTitle();
		UpdateNoteDesigner();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SONG_LEVEL)
	{
		UpdateLevelWord();
		UpdateLevelIcon();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SONG_ALBUM_FRAME)
	{
		UpdateAlbumFrame();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SETTING)
	{
		UpdateSetting();
		UpdateStatusPanel();			//�ǉ�
		UpdateSelectSongWaitMember();	//�ǉ�
		UpdateSelectSongFinalCheck();
		UpdateBtnManual();
		UpdateRanking();				//�ǉ�
	}
	UpdateSendData();		//�ǉ�
	UpdateScreenMask();
}

void DrawSelectSong()
{
	if (g_nCountSelectSongAnimation >= SSAT_BG)
	{
		DrawSelectSongBG();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SONG_BOARD)
	{
		DrawSelectSongBoard();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SONG_WORD)
	{
		DrawSelectSongWord();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SONG_ALBUM_FRAME)
	{
		DrawAlbumFrame();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SONG_PANEL)
	{
		DrawSelectSongPanel();
		DrawSongAlbum();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SONG_TITLE)
	{
		DrawSongTitle();
		DrawSongSubTitle();
		DrawNoteDesigner();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SONG_LEVEL)
	{
		DrawLevelWord();
		DrawLevelNum();
		DrawLevelIcon();
	}
	if (g_nCountSelectSongAnimation >= SSAT_SETTING)
	{
		DrawStatusPanel();				//�ǉ�
		DrawSelectSongWaitMember();		//�ǉ�
		DrawSelectSongFinalCheck();
		DrawRanking();					//�ǉ�
		DrawScreenMask();
		DrawSetting();
		DrawBtnManual();
	}

	DrawDebugRSL();
	DrawSongStatus();
	MusicDebug();
	DrawRecvDataDebug();		//�ǉ�
}

bool GetSelectSongFadeInFlag()
{
	return g_bFadeIn;
}