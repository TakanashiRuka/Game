#include "main.h"
#include "score.h"
#include "Gauge.h"
#include "Combo.h"
#include "JudgeEffect.h"
#include "SongStatus.h"
#include "Result.h"
#include "Camera.h"
#include "ResultBoard.h"
#include "ResultScore.h"
#include "ResultJudgeDetail.h"
#include "ResultRank.h"
#include "ResultAchievement.h"
#include "SelectSongBG.h"
#include "sound.h"
#include "GamePlayerBoard.h"
#include "TitleBtnManual.h"
#include "Ranking.h"
#include "SelectSongWaitMember.h"
#include "data.h"
#include "Polygon.h"
#include "Texture.h"

typedef enum {
	RAT_RESULT_BG = 0,
	RAT_RESULT_BOARD = 10,
	RAT_RESULT_CLEAR_FAILED = 15,
	RAT_RESULT_SCORE = 20,
	RAT_RESULT_JUDGE_DETAIL =20,
	RAT_RESULT_RANK = 32,
	RAT_RESULT_ACHIEVEMENT = 40,
	RAT_MAX = 90
}RESULT_ANIMATION_TIME;

static int g_nCountResultAnimation;
static bool g_bResultFadeIn;

void InitResult()
{
	g_nCountResultAnimation = 0;
	g_bResultFadeIn = false;

	SetCamera(MODE_RESULT);
	InitGameSongTitle();
	InitResultBoard();
	InitResultScore();
	InitResultJudgeDetail();
	InitResultRankBoard();
	InitRank();
	InitResultClearFailed();
	InitResultAchievement();
	InitSelectSongBG();
	InitTitleBtnManual();
	if (!GetSetAuto())
	{
		SetRanking();		//追加
	}
	InitRanking();
	//SetRoomCreate(false);
	//SetRoomEntry(false);
	InitText(FONTLIST::F_GAME_PLAYER_NAME, PLAYER_NAME_FONT_SIZE, "MS コジック");
	InitText(FONTLIST::F_GAME_RIVAL_NAME, RIVAL_NAME_FONT_SIZE, "MS コジック");
	PlaySound(BGM_RESULT);
}

void UninitResult()
{
	UninitGameSongTitle();
	UninitResultBoard();
	UninitResultScore();
	UninitResultJudgeDetail();
	UninitResultRankBoard();
	UninitRank();
	UninitResultClearFailed();
	UninitResultAchievement();
	UninitSelectSongBG();
	UninitTitleBtnManual();
	UninitRanking();
	UninitText(FONTLIST::F_GAME_PLAYER_NAME);
	UninitText(FONTLIST::F_GAME_RIVAL_NAME);
	StopSound(BGM_RESULT);
	//ResetData();
}

void UpdateResult()
{
	if (g_nCountResultAnimation < RAT_MAX)
	{
		g_nCountResultAnimation++;
	}
	else
	{
		g_bResultFadeIn = true;
	}
	if (g_nCountResultAnimation >= RAT_RESULT_BG)
	{
		UpdateSelectSongBG();
	}
	if (g_nCountResultAnimation >= RAT_RESULT_BOARD)
	{
		UpdateResultBoard();
	}
	if (g_nCountResultAnimation >= RAT_RESULT_CLEAR_FAILED)
	{
		UpdateResultClearFailed();
	}
	if (g_nCountResultAnimation >= RAT_RESULT_SCORE)
	{
		UpdateResultScore();
	}
	if (g_nCountResultAnimation >= RAT_RESULT_JUDGE_DETAIL)
	{
		UpdateResultJudgeDetail();
		UpdateRanking();
	}
	if (g_nCountResultAnimation >= RAT_RESULT_RANK)
	{
		UpdateResultRankBoard();
		UpdateRank();
	}
	if (g_nCountResultAnimation >= RAT_RESULT_ACHIEVEMENT)
	{
		UpdateResultAchievement();
	}
	if (g_nCountResultAnimation >= RAT_MAX)
	{
		UpdateTitleBtnManual();
	}
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoyStickTrigger(JOY_SPHERE))
	{
		if (GetResultFadeInStatus() && !GetFadeFlag())
		{
			StopSound(BGM_RESULT);
			if (GetPlayStatus(SE_ENTER))
			{
				StopSound(SE_ENTER);
			}
			PlaySound(SE_ENTER);
			if (!GetTestModeMemberStatus(TMML_GUEST))
			{
				SetGameModeTarget(MODE_SELECT_SONG);
				SetFadeWaitFrame(0);
			}
			else
			{
				SetGameModeTarget(MODE_CONTINUE);
				//SetGameModeTarget(MODE_THANKS);
				SetFadeWaitFrame(30);
			}
			SetFadeInOut(FIOF_FADEIN);
			//ChangeGameMode();
		}
	}
}

void DrawResult()
{
	if (g_nCountResultAnimation >= RAT_RESULT_BG)
	{
		DrawSelectSongBG();
		if (!GetClearStatus())
		{
			SetZBuffer(FALSE);
			SetTexture(TEX_SLIDE_SHADOW);
			TEXTURE tTexture = GetTextureStatus(TEX_SLIDE_SHADOW);
			SetScale(WINDOW_WIDTH, WINDOW_HEIGHT);
			SetPolygonColor(D3DCOLOR_ARGB(63, 30, 0, 0));
			DrawPolygon(0, 0, 0, 0, tTexture.u, tTexture.v);
			SetZBuffer(TRUE);
		}
	}
	if (g_nCountResultAnimation >= RAT_RESULT_BOARD)
	{
		DrawResultBoard();
	}
	if (g_nCountResultAnimation >= RAT_RESULT_SCORE)
	{
		DrawResultScore();
	}
	if (g_nCountResultAnimation >= RAT_RESULT_JUDGE_DETAIL)
	{
		DrawResultJudgeDetail();
		DrawRanking();
	}
	if (g_nCountResultAnimation >= RAT_RESULT_RANK)
	{
		DrawResultRankBoard();
		DrawRank();
	}
	if (g_nCountResultAnimation >= RAT_RESULT_ACHIEVEMENT)
	{
		DrawResultAchievement();
	}
	if (g_nCountResultAnimation >= RAT_MAX)
	{
		DrawTitleBtnManual();
	}
	DrawScoreBoard();
	DrawScoreWord();
	DrawScore();
	DrawMaxComboWord();
	DrawMaxComboNum();
	DrawGaugeBoard();
	DrawGaugeBG();
	DrawGauge();
	DrawGaugeClearLine();
	if (g_nCountResultAnimation >= RAT_RESULT_CLEAR_FAILED)
	{
		DrawResultClearFailed();
	}
	DrawSongBoard();
	DrawSongStatusWord();
	DrawGameAlbumBoard();
	DrawGameAlbum();
	DrawGameSongTitle();
	DrawGameLevelWord();
	DrawGameLevelNum();
	DrawGamePlayerBoard();
}

bool GetResultFadeInStatus()
{
	return g_bResultFadeIn;
}