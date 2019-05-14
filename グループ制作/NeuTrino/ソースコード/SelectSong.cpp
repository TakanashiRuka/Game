//----------------------------------------------------------------------------
//
//	曲選択
//
//														 制作者：劉喆
//                                                       作成日：2017/12/26
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
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
#include "StatusPanel.h"				//追加
#include "SelectSongWaitMember.h"	//追加
#include "data.h"					//追加
#include "BtnManual.h"
#include "ScreenMask.h"
#include "Ranking.h"				//追加

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
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

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------
static int g_nCountSelectSongAnimation;				// 曲選択画面のアニメーションカウンター
static bool g_bFadeIn;								// 制御用フラグ

//---------------------------プロトタイプ宣言---------------------------------
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
	InitStatusPanel();				//追加
	InitSelectSongWaitMember();		//追加
	InitBtnManual(BMP_SELECT_SONG);
	InitScreenMask();
	//SetSongPanelA(RDP_END);
	//ChangeSelectNum(3);
	InitRanking();					//追加
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
	UninitStatusPanel();			//追加
	UninitSelectSongWaitMember();	//追加
	UninitBtnManual();
	UninitScreenMask();
	UninitRanking();				//追加
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
		UpdateStatusPanel();			//追加
		UpdateSelectSongWaitMember();	//追加
		UpdateSelectSongFinalCheck();
		UpdateBtnManual();
		UpdateRanking();				//追加
	}
	UpdateSendData();		//追加
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
		DrawStatusPanel();				//追加
		DrawSelectSongWaitMember();		//追加
		DrawSelectSongFinalCheck();
		DrawRanking();					//追加
		DrawScreenMask();
		DrawSetting();
		DrawBtnManual();
	}

	DrawDebugRSL();
	DrawSongStatus();
	MusicDebug();
	DrawRecvDataDebug();		//追加
}

bool GetSelectSongFadeInFlag()
{
	return g_bFadeIn;
}