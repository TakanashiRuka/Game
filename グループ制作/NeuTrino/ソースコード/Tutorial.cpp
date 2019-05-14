//----------------------------------------------------------------------------
//
//	ゲーム説明
//
//														 制作者：劉喆
//                                                       作成日：2017/02/21
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "game.h"
#include "Tutorial.h"
#include "Demo.h"
#include "Btn.h"
#include "Timer.h"
#include "Preface.h"
#include "SelectSongPanel.h"
#include "ReadSongList.h"
#include "Note.h"
#include "JudgeSound.h"
#include "TitleBtnManual.h"
#include "GamePreface.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------
static SAVE_GAME_CONFIG g_SaveGameConfig;

//---------------------------プロトタイプ宣言---------------------------------
void InitTutorial()
{
	// 前準備
	g_SaveGameConfig.nSelectNum = GetSelectNum();
	g_SaveGameConfig.nLevel = GetLevel();
	g_SaveGameConfig.fSpeed = GetHiSpeed();
	g_SaveGameConfig.nOffset = GetOffsetNote();
	g_SaveGameConfig.nSE = GetJudgeSoundVolume();

	SetLevel(0);
	SetHiSpeed(INIT_SPEED);
	SetOffsetNote(INIT_OFFSET);
	SetJudgeSoundVolume(INIT_SE_VOLUME);

	InitGame();
	InitTitleBtnManual();
}

void UninitTutorial()
{
	UninitGame();
	UninitTitleBtnManual();
	SetLevel(g_SaveGameConfig.nLevel);
	SetSelectNum(g_SaveGameConfig.nSelectNum);
	SetHiSpeed(g_SaveGameConfig.fSpeed);
	SetOffsetNote(g_SaveGameConfig.nOffset);
	SetJudgeSoundVolume(g_SaveGameConfig.nSE);
}

void UpdateTutorial()
{
	UpdateGame();
	UpdateTitleBtnManual();
}

void DrawTutorial()
{
	DrawGame();
	DrawTitleBtnManual();
}
