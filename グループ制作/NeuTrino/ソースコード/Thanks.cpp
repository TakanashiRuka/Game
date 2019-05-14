//----------------------------------------------------------------------------
//
//	後書き
//
//														 制作者：劉喆
//                                                       作成日：2017/02/28
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "SelectSongBG.h"
#include "ThanksBG.h"
#include "sound.h"
#include "FadeInOut.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------
static const int gc_nThanksMaxCount = 6 * 60;
static int g_nThanksCount;
static bool g_bPlayBGM;

//---------------------------プロトタイプ宣言---------------------------------
void InitThanks()
{
	g_bPlayBGM = false;
	g_nThanksCount = 0;
	InitSelectSongBG();
	InitThanksBG();
}

void UninitThanks()
{
	UninitSelectSongBG();
	UninitThanksBG();
	StopSound(BGM_THANKS);
}

void UpdateThanks()
{
	if (!GetFadeFlag() && !g_bPlayBGM)
	{
		g_bPlayBGM = true;
		PlaySound(BGM_THANKS);
	}
	UpdateSelectSongBG();
	UpdateThanksBG();
	if (g_nThanksCount < gc_nThanksMaxCount)
	{
		g_nThanksCount++;
	}
	if (g_nThanksCount == gc_nThanksMaxCount)
	{
		g_nThanksCount++;
		SetGameModeTarget(MODE_PREFACE);
		ChangeGameMode();
	}
}

void DrawThanks()
{
	DrawSelectSongBG();
	DrawThanksBG();
}