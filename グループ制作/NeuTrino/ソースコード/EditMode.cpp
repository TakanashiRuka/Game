//----------------------------------------------------------------------------
//
//	エディトモード
//
//														 制作者：劉喆
//                                                       作成日：2017/12/11
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "game.h"
#include "input.h"
#include "JoyStick.h"
#include "Timer.h"
#include "debug.h"
#include "ReadNoteFile.h"
#include "music.h"
#include "Note.h"
#include "Judge.h"
#include "score.h"
#include "Combo.h"
#include "Gauge.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------
static int g_nSaveTime;
static bool g_bEdit = false;
static int g_nMaxLine;
static int g_nRecordLine;

//---------------------------プロトタイプ宣言---------------------------------
void InitEditMode()
{
	g_nSaveTime = 0;
	g_bEdit = false;
	g_nMaxLine = GetMaxLineNum();
	g_nRecordLine = 0;
	InitText(F_EDIT, 36, "MS コジック");
}

void UninitEditMode()
{
	UninitText(F_EDIT);
}

void UpdateEditMode()
{
	if (g_bEdit)
	{
		if (GetPlayStatusM(MUSIC))
		{
			StopMusic();
		}
		SetTimer(g_nSaveTime);
		if (GetKeyboardRepeat(DIK_DOWN) || GetKeyboardRepeat(DIK_UP))
		{
			bool bChange = false;
			if (GetKeyboardRepeat(DIK_UP))
			{
				if (g_nRecordLine > 0)
				{
					g_nRecordLine--;
					bChange = true;
				}
				if (g_nRecordLine < 0)
				{
					g_nRecordLine = 0;
				}
			}
			if (GetKeyboardRepeat(DIK_DOWN))
			{
				if (g_nRecordLine < g_nMaxLine - 1)
				{
					g_nRecordLine++;
					bChange = true;
				}
				if (g_nRecordLine > g_nMaxLine - 1)
				{
					g_nRecordLine = g_nMaxLine - 1;
				}
			}
			if (bChange)
			{
				g_nSaveTime = GetLine(g_nRecordLine).nStartTime + GetNoteOffsetFromFile();
				SetTimer(g_nSaveTime);
				ResetNoteStatus();
				ResetScore();
				SetCombo(CC_RESET);
				ResetGauge();
				InitJudge();
				if (g_nSaveTime < GetPlayMusicOffset())
				{
					SetGamePlayMusicMemo(false);
				}
				else
				{
					SetGamePlayMusicMemo(true);
				}
			}
		}
	}
}

void DrawEditMode()
{
	if (g_bEdit)
	{
		SetTextStatus(D3DCOLOR_ARGB(255, 255, 255, 255), D3DCOLOR_ARGB(255, 0, 0, 0), 2);
		SetTextStyle(DT_CENTER);
		OutputText(F_EDIT, 0, 360, "%d / %d", g_nRecordLine, g_nMaxLine);
	}
}

void SetEditMode(bool bStatus)
{
	g_bEdit = bStatus;
	if (g_bEdit)
	{
		g_nSaveTime = GetTimer();
		StopMusic();
		SetTimer(g_nSaveTime);
		if (g_nSaveTime >= GetNoteOffsetFromFile())
		{
			g_nRecordLine = GetRecordLine(g_nSaveTime);
		}
		else
		{
			g_nRecordLine = 0;
		}
	}
	else
	{
		SetTimer(g_nSaveTime);
		if (g_nSaveTime - GetPlayMusicOffset() >= 0)
		{
			PlayMusic(MUSIC, g_nSaveTime - GetPlayMusicOffset());
		}
	}
}

bool GetEditMode()
{
	return g_bEdit;
}