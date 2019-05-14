//----------------------------------------------------------------------------
//
//	コンティニュー
//
//														 制作者：劉喆
//                                                       作成日：2017/03/05
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Continue.h"
#include "SelectSongBG.h"
#include "ContinueBG.h"
#include "ContinueBtn.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	CA_WORD = 20,
	CA_BTN = 80,
	CA_MAX,
}CONTINUE_ANIMATION;

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------
static const int gc_nMaxCount = 80;
static int g_nConut;

//---------------------------プロトタイプ宣言---------------------------------
void InitContinue()
{
	g_nConut = 0;
	InitSelectSongBG();
	InitContinueBG();
	InitContinueBtn();
}

void UninitContinue()
{
	UninitSelectSongBG();
	UninitContinueBG();
	UninitContinueBtn();
}

void UpdateContinue()
{
	if (g_nConut < gc_nMaxCount)
	{
		g_nConut++;
	}
	UpdateSelectSongBG();
	if (g_nConut >= CA_WORD)
	{
		UpdateContinueBG();
	}
	if (g_nConut >= CA_BTN)
	{
		UpdateContinueBtn();
	}
}

void DrawContinue()
{
	DrawSelectSongBG();
	if (g_nConut >= CA_WORD)
	{
		DrawContinueBG();
	}
	if (g_nConut >= CA_BTN)
	{
		DrawContinueBtn();
	}
}