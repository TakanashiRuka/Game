#include "main.h"
#include "title.h"
#include "Camera.h"
#include "Demo.h"
#include "TestMode.h"
#include "TitleBG.h"
#include "TitleBtnManual.h"
#include "sound.h"
#include "name.h"
#include "SelectSongPanel.h"
#include <fstream>

typedef enum {
	TAT_TITLE_BG = 0,
	TAT_TITLE_MASK = 60,
	TAT_MAX
}TITLE_ANIMATION_TIME;

static const int gc_nCountFrameMax = 15 * 60;
static int g_nCountFrame;

static const int gc_nTitleFadeInCountMax = 80;

static bool g_bTitleFadeIn;

static const std::string g_sPlayerName = "GUEST";

// タイトルの初期化
void InitTitle()
{
	g_bTitleFadeIn = false;
	g_nCountFrame = 0;
	SetCamera(MODE_TITLE);
	InitTitleBG();
	InitTitleMask();
	InitTitleBtnManual();
	InitVersion();
}

// タイトルの終了
void UninitTitle()
{
	UninitTitleBG();
	UninitTitleMask();
	UninitTitleBtnManual();
	UninitVersion();
}

// タイトルの更新
void UpdateTitle()
{
	if (g_nCountFrame >= TAT_TITLE_BG)
	{
		UpdateTitleBG();
	}
	if (g_nCountFrame >= TAT_TITLE_MASK)
	{
		UpdateTitleMask();
	}
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoyStickTrigger(JOY_SPHERE))
	{
		if (GetTitleFadeInStatus() && !GetFadeFlag())
		{
			if (GetPlayStatus(SE_ENTER))
			{
				StopSound(SE_ENTER);
			}
			PlaySound(SE_ENTER);
			std::ifstream load_file(PLAYERNAME_FILE);
			if (GetTestModeMemberStatus(TMML_GUEST) || (!GetTestModeMemberStatus(TMML_GUEST) && load_file.fail()))
			{
				SetGameModeTarget(MODE_PLAYER_ENTRY);
				SetFadeWaitFrame(30);
				SetFadeInOut(FIOF_FADEIN);
			}
			else
			{
				SetGameModeTarget(MODE_SELECT_SONG);
				SetFadeWaitFrame(0);
				SetFadeInOut(FIOF_FADEIN);
			}
			//ChangeGameMode();
		}
	}
	if (g_bTitleFadeIn)
	{
		UpdateTitleBtnManual();
	}
	if (g_nCountFrame < gc_nCountFrameMax)
	{
		g_nCountFrame++;
		if (g_nCountFrame == gc_nTitleFadeInCountMax)
		{
			g_bTitleFadeIn = true;
		}
		if (g_nCountFrame == gc_nCountFrameMax && GetTestModeMemberStatus(TMML_DEMO) && GetIsDemo())
		{
			SetGameModeTarget(MODE_DEMO);
			SetFadeWaitFrame(30);
			SetFadeInOut(FIOF_FADEIN);
			if (GetTestModeMemberStatus(TMML_GUEST))
			{
				SetName(g_sPlayerName);
			}
			//ChangeGameMode();
		}
	}
	UpdateVersion();
}	 

// タイトルの描画
void DrawTitle()
{
	if (g_nCountFrame >= TAT_TITLE_BG)
	{
		DrawTitleBG();
	}
	if (g_nCountFrame >= TAT_TITLE_MASK)
	{
		DrawTitleMask();
	}
	if (g_bTitleFadeIn)
	{
		DrawTitleBtnManual();
	}
	DrawTitleAllRightReserved();
	DrawVersion();
}

bool GetTitleFadeInStatus()
{
	return g_bTitleFadeIn;
}
