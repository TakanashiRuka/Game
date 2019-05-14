#include "PlayerEntry.h"
#include "main.h"
#include "SelectSongBG.h"
#include "SelectSongBoard.h"
#include "PlayerEntryHeader.h"
#include "PlayerEntryWordBoard.h"
#include "PlayerEntryArrow.h"
#include "PlayerEntryNameBG.h"
#include "Camera.h"
#include "Polygon.h"
#include "Texture.h"
#include "sound.h"
#include "name.h"
#include "ErrorList.h"
#include "ScreenMask.h"
#include "debug.h"
#include "BtnManual.h"
#include "FadeInOut.h"

#include <string>

typedef enum {
	PEAT_BG = 0,
	PEAT_BOARD = 15,
	PEAT_HEADER = 25,
	PEAT_WORD_BOARD = 25,
	PEAT_ARROW = 25,
	PEAT_NAME_BG = 30,
	PEAT_MAX
}PLAYER_ENTRY_ANIMATION_TIME;

static const int gc_nMaxWord = 37;
static const int gc_nMaxNameSize = 7;
static const char gc_cSelectWord[gc_nMaxWord] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
static const int gc_nFontSizeCenter = 256;
static const int gc_nFontSizeSide = 196;
static const float gc_fSideAlfa = 0.7f;
static const int gc_nFontSizeName = 40;
static const int gc_nRepeatControl = 6;

static int g_nCountPlayerEntryAnimation;				// 曲選択画面のアニメーションカウンター
static bool g_bFadeIn;								// 制御用フラグ
static std::string g_sPlayerName;
static int g_nSelectWord;
static int g_nRepeatCnt;

void InitPlayerEntry()
{
	g_nCountPlayerEntryAnimation = 0;
	g_bFadeIn = false;
	g_sPlayerName = "";
	g_nSelectWord = 0;
	g_nRepeatCnt = 0;

	SetCamera(MODE_PLAYER_ENTRY);
	InitSelectSongBG();
	InitSelectSongBoard();
	InitPlayerEntryHeader();
	InitPlayerEntryWordBoard();
	InitPlayerEntryArrow();
	InitPlayerEntryNameBG();

	InitText(F_PLAYER_ENTRY_CENTER, gc_nFontSizeCenter, "MS コジック");
	InitText(F_PLAYER_ENTRY_SIDE, gc_nFontSizeSide, "MS コジック");
	InitText(F_PLAYER_ENTRY_NAME, gc_nFontSizeName, "MS コジック");
	InitBtnManual(BMP_PLAYER_ENTRY);
	InitScreenMask();
	PlaySound(BGM_PLAYER_ENTRY);
	StopSound(BGM_PLAYER_ENTRY);
}

void UninitPlayerEntry()
{
	UninitSelectSongBG();
	UninitSelectSongBoard();
	UninitPlayerEntryHeader();
	UninitPlayerEntryWordBoard();
	UninitPlayerEntryArrow();
	UninitPlayerEntryNameBG();

	UninitText(F_PLAYER_ENTRY_CENTER);
	UninitText(F_PLAYER_ENTRY_SIDE);
	UninitText(F_PLAYER_ENTRY_NAME);
	
	UninitScreenMask();
	UninitBtnManual();
	StopSound(BGM_PLAYER_ENTRY);

	SetName(g_sPlayerName);
}

void UpdatePlayerEntry()
{
	if (!GetFadeFlag())
	{
		if (!GetPlayStatus(BGM_PLAYER_ENTRY))
		{
			PlaySound(BGM_PLAYER_ENTRY);
		}
	}
	if (g_nCountPlayerEntryAnimation < PEAT_MAX)
	{
		g_nCountPlayerEntryAnimation++;
	}
	if (g_nCountPlayerEntryAnimation == PEAT_MAX)
	{
		if (!g_bFadeIn)
		{
			g_bFadeIn = true;
		}
	}

	if (g_nCountPlayerEntryAnimation >= PEAT_BG)
	{
		UpdateSelectSongBG();
	}
	if (g_nCountPlayerEntryAnimation >= PEAT_BOARD)
	{
		UpdateSelectSongBoard();
	}
	if (g_nCountPlayerEntryAnimation >= PEAT_HEADER)
	{
		UpdatePlayerEntryHeader();
	}
	if (g_nCountPlayerEntryAnimation >= PEAT_WORD_BOARD)
	{
		UpdatePlayerEntryWordBoard();
	}
	if (g_nCountPlayerEntryAnimation >= PEAT_ARROW)
	{
		UpdatePlayerEntryArrow();
	}
	if (g_nCountPlayerEntryAnimation >= PEAT_NAME_BG)
	{
		UpdatePlayerEntryNameBG();
		UpdateBtnManual();
	}
	if (g_bFadeIn && !GetErrorListStatus())
	{
		if (g_nRepeatCnt < gc_nRepeatControl)
		{
			g_nRepeatCnt++;
		}
		if ((GetKeyboardTrigger(DIK_LEFT) || GetPOVTrigger(JOY_LEFT) || GetKeyboardTrigger(DIK_RIGHT) || GetPOVTrigger(JOY_RIGHT)) && !GetFadeFlag())
		{
			g_nRepeatCnt == gc_nRepeatControl;
		}

		if ((GetKeyboardRepeat(DIK_LEFT) || GetPOVRepeat(JOY_LEFT)) && !GetFadeFlag())
		{
			if (g_nRepeatCnt == gc_nRepeatControl)
			{
				if (GetPlayStatus(SE_SELECT_SONG))
				{
					StopSound(SE_SELECT_SONG);
				}
				PlaySound(SE_SELECT_SONG);
				--g_nSelectWord;
				g_nRepeatCnt = 0;
			}
		}
		if ((GetKeyboardRepeat(DIK_RIGHT) || GetPOVRepeat(JOY_RIGHT)) && !GetFadeFlag())
		{
			if (g_nRepeatCnt == gc_nRepeatControl)
			{
				if (GetPlayStatus(SE_SELECT_SONG))
				{
					StopSound(SE_SELECT_SONG);
				}
				PlaySound(SE_SELECT_SONG);
				++g_nSelectWord;
				g_nRepeatCnt = 0;
			}
		}

		if (g_nSelectWord < 0)
		{
			g_nSelectWord += gc_nMaxWord - 1;
		}
		else if (g_nSelectWord >= (gc_nMaxWord - 1))
		{
			g_nSelectWord -= gc_nMaxWord - 1;
		}

		if ((GetKeyboardTrigger(DIK_RETURN) || GetJoyStickTrigger(JOY_SPHERE)) && !GetFadeFlag())
		{
			if (g_sPlayerName.size() < gc_nMaxNameSize)
			{
				g_sPlayerName += gc_cSelectWord[g_nSelectWord];
				if (GetPlayStatus(SE_PLAYER_ENTRY_INPUT))
				{
					StopSound(SE_PLAYER_ENTRY_INPUT);
				}
				PlaySound(SE_PLAYER_ENTRY_INPUT);
			}
			else
				SetErrorList(ELW_NAME_SIZE_OVER, RDP_END);
		}
		if ((GetKeyboardTrigger(DIK_ESCAPE) || GetJoyStickTrigger(JOY_CROSS)) && !GetFadeFlag())
		{
			if (g_sPlayerName.size() > 0)
			{
				g_sPlayerName.erase(g_sPlayerName.begin() + g_sPlayerName.size() - 1);
				if (GetPlayStatus(SE_PLAYER_ENTRY_CANCEL))
				{
					StopSound(SE_PLAYER_ENTRY_CANCEL);
				}
				PlaySound(SE_PLAYER_ENTRY_CANCEL);
			}
		}
		if ((GetKeyboardTrigger(DIK_E) || GetJoyStickTrigger(JOY_OPTION)) && !GetFadeFlag())
		{
			if (g_sPlayerName.size() == 0)
			{
				SetErrorList(ELW_NAME_SIZE_ZERO, RDP_END);
			}
			else
			{
				PlaySound(SE_ENTER);
				SetGameModeTarget(MODE_SELECT_SONG);
				SetFadeWaitFrame(0);
				SetFadeInOut(FIOF_FADEIN);
			}
		}
	}
	UpdateScreenMask();
}

void DrawPlayerEntry()
{
	if (g_nCountPlayerEntryAnimation >= PEAT_BG)
	{
		DrawSelectSongBG();
	}
	if (g_nCountPlayerEntryAnimation >= PEAT_BOARD)
	{
		DrawSelectSongBoard();
	}
	if (g_nCountPlayerEntryAnimation >= PEAT_HEADER)
	{
		DrawPlayerEntryHeader();
	}
	if (g_nCountPlayerEntryAnimation >= PEAT_WORD_BOARD)
	{
		DrawPlayerEntryWordBoard();
	}
	if (g_nCountPlayerEntryAnimation >= PEAT_ARROW)
	{
		DrawPlayerEntryArrow();
	}
	if (g_nCountPlayerEntryAnimation >= PEAT_NAME_BG)
	{
		DrawPlayerEntryNameBG();
	}
	if (g_bFadeIn)
	{
		SetTextStyle(DT_CENTER);
		SetTextStatus(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 2);
		OutputText(F_PLAYER_ENTRY_CENTER, 0, 360 - gc_nFontSizeCenter / 2, "%c", gc_cSelectWord[g_nSelectWord]);
		int nSideWord1 = g_nSelectWord - 2;
		if (nSideWord1 < 0)
			nSideWord1 += gc_nMaxWord - 1;
		else if (nSideWord1 >= (gc_nMaxWord - 1))
			nSideWord1 -= gc_nMaxWord - 1;
		SetTextStatus(D3DXCOLOR(1.0f, 1.0f, 1.0f, gc_fSideAlfa), D3DXCOLOR(0.0f, 0.0f, 0.0f, gc_fSideAlfa), 2);
		OutputText(F_PLAYER_ENTRY_SIDE, -960, 360 - gc_nFontSizeSide / 2, "%c", gc_cSelectWord[nSideWord1]);
		int nSideWord2 = g_nSelectWord - 1;
		if (nSideWord2 < 0)
			nSideWord2 += gc_nMaxWord - 1;
		else if (nSideWord2 >= (gc_nMaxWord - 1))
			nSideWord2 -= gc_nMaxWord - 1;
		SetTextStatus(D3DXCOLOR(1.0f, 1.0f, 1.0f, gc_fSideAlfa), D3DXCOLOR(0.0f, 0.0f, 0.0f, gc_fSideAlfa), 2);
		OutputText(F_PLAYER_ENTRY_SIDE, -560, 360 - gc_nFontSizeSide / 2, "%c", gc_cSelectWord[nSideWord2]);
		int nSideWord3 = g_nSelectWord + 1;
		if (nSideWord3 < 0)
			nSideWord3 += gc_nMaxWord - 1;
		else if (nSideWord3 >= (gc_nMaxWord - 1))
			nSideWord3 -= gc_nMaxWord - 1;
		SetTextStatus(D3DXCOLOR(1.0f, 1.0f, 1.0f, gc_fSideAlfa), D3DXCOLOR(0.0f, 0.0f, 0.0f, gc_fSideAlfa), 2);
		OutputText(F_PLAYER_ENTRY_SIDE, 560, 360 - gc_nFontSizeSide / 2, "%c", gc_cSelectWord[nSideWord3]);
		int nSideWord4 = g_nSelectWord + 2;
		if (nSideWord4 < 0)
			nSideWord4 += gc_nMaxWord - 1;
		else if (nSideWord4 >= (gc_nMaxWord - 1))
			nSideWord4 -= gc_nMaxWord - 1;
		SetTextStatus(D3DXCOLOR(1.0f, 1.0f, 1.0f, gc_fSideAlfa), D3DXCOLOR(0.0f, 0.0f, 0.0f, gc_fSideAlfa), 2);
		OutputText(F_PLAYER_ENTRY_SIDE, 960, 360 - gc_nFontSizeSide / 2, "%c", gc_cSelectWord[nSideWord4]);

		SetTextStyle(DT_CENTER);
		SetTextStatus(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0);
		OutputText(F_PLAYER_ENTRY_NAME, 0, 587 - gc_nFontSizeName / 2, "%s", g_sPlayerName.c_str());
	}
	DrawScreenMask();
	DrawBtnManual();
}