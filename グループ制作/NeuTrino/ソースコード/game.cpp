#include "main.h"
#include "game.h"
#include "input.h"
#include "JoyStick.h"
#include "Lane.h"
#include "Camera.h"
#include "Btn.h"
#include "JudgeLine.h"
#include "Note.h"
#include "Line.h"
#include "Timer.h"
#include "Judge.h"
#include "ReadNoteFile.h"
#include "music.h"
#include "EditMode.h"
#include "JudgeSound.h"
#include "decision.h"
#include "score.h"
#include "Gauge.h"
#include "Combo.h"
#include "JudgeEffect.h"
#include "SongStatus.h"
#include "Result.h"
#include "GameAchievement.h"
#include "GameBG.h"
#include "data.h"		//í«â¡
#include "SelectSongWaitMember.h"
#include "GamePlayerBoard.h"
#include "ScreenMask.h"
#include "GamePreface.h"
#include "Texture.h"

static bool g_bMusic;
static int g_nPlayMusicOffset = 1000;				// âπäyÇçƒê∂Ç≥ÇπÇÈéûä‘(ms)
static char g_cSongURL[MAX_WORD];
static int g_nStartDrawTime;
static const int gc_nStartDrawTimeMax = 2;
static bool g_bJudgeFCAP;
static bool g_bInitGame;

// ÉQÅ[ÉÄÇÃèâä˙âª
void InitGame()
{
	g_bInitGame = false;
	g_nStartDrawTime = 0;
	g_bMusic = false;
	SetCamera(MODE_GAME);
	g_bJudgeFCAP = false;

	switch (GetGameModeTarget())
	{
	case MODE_TUTORIAL:
	{
		SetTextureAddress(TEX_SELECT_ALBUM_00, "Data/SongList/Tutorial.png");
		SetSongTitleGame("TUTORIAL");
		SetNoteFileURL("Data/SongList/Tutorial.neu");
		SetSongURL("Data/SongList/Tutorial.wav");
		break;
	}
	}

	InitGameBG();
	InitLane();
	InitLaneEffect();
	InitBtn();
	InitJudgeLine();
	InitNote();
	InitNoteFlickArrow();
	InitJudge();
	InitReadNote();
	InitDecision();
	SetReadNote();
	SetNote();
	SetMaxNote();
	SetMaxNotePerType();
	InitCombo();
	InitMaxComboWord();
	InitMaxComboNum();
	InitComboWord();
	InitScoreBoard();
	InitScoreWord();
	InitScore();
	InitGaugeBoard();
	InitGaugeBG();
	InitGauge();
	InitGaugeClearLine();
	InitEditMode();
	InitLine();
	InitJudgeSound();
	InitJudgeEffect();
	InitSongBoard();
	InitSongStatusWord();
	InitGameAlbumBoard();
	InitGameAlbum();
	InitGameSongTitle();
	InitGameLevelWord();
	InitPlayPosition();
	InitGameAchievementClearFailed();
	InitGameAchievementFCAP();
	InitScreenMask();
	InitGamePreface();
	Data::g_player.score = 0.0f;
	Data::g_player.display_score = 0.0f;
	InitGamePlayerBoard();

	int nOffset = -GetOffsetNote();

	SetNoteOffset(nOffset);
	SetLineOffset(nOffset);
	SetLine();

	SetMusicM(g_cSongURL, 0);
	InitMusic(GethWnd());
	InitTimer();
	SetGamePreface();
	g_bInitGame = true;
}

void UninitGame()
{
	UninitGameBG();
	UninitLane();
	UninitLaneEffect();
	UninitBtn();
	UninitJudgeLine();
	UninitNote();
	UninitDecision();
	UninitGaugeBoard();
	UninitGaugeBG();
	UninitGaugeClearLine();
	UninitJudge();
	UninitMusic();
	UninitEditMode();
	UninitLine();
	UninitJudgeSound();
	UninitScore();
	UninitScoreBoard();
	UninitScoreWord();
	UninitGauge();
	UninitCombo();
	UninitMaxComboWord();
	UninitMaxComboNum();
	UninitComboWord();
	UninitJudgeEffect();
	UninitSongBoard();
	UninitSongStatusWord();
	UninitGameAlbumBoard();
	UninitGameAlbum();
	UninitGameSongTitle();
	UninitGameLevelWord();
	UninitPlayPosition();
	UninitGameAchievementClearFailed();
	UninitGameAchievementFCAP();
	UninitGamePlayerBoard();
	UninitScreenMask();
	UninitGamePreface();
}

// ÉQÅ[ÉÄÇÃçXêV
void UpdateGame()
{
	int nTime = GetTimer();

	if (nTime >= g_nPlayMusicOffset && !g_bMusic)
	{
		int nOffset = nTime - g_nPlayMusicOffset;
		int nTimePlayMusicBefore = GetTimer();
		PlayMusic(MUSIC);
		int nTimePlayMusicAfter = GetTimer();
		SetNoteOffset(nOffset + nTimePlayMusicAfter - nTimePlayMusicBefore);
		SetLineOffset(nOffset + nTimePlayMusicAfter - nTimePlayMusicBefore);
		SetLine();
		g_bMusic = true;
	}
	if (g_nStartDrawTime < gc_nStartDrawTimeMax)
	{
		g_nStartDrawTime++;
	}
	//UpdateTimer();
	if (!GetFadeFlag())
	{
		UpdateGamePreface();
	}
	ResetCameraAngleTarget();
	SetChangeAngleCountMax(30);
	ResetCameraY();
	UpdateGameBG();
	UpdateLane();
	UpdateLaneEffect();
	UpdateJudgeSound();
	UpdateBtnHoldFlag();
	UpdateJudgeLine();
	UpdateJudgeLineSpecial();
	UpdateJudge();

	ChangeCameraAngle();
	ChangeCameraY();

	UpdateDecision();
	UpdateJudgeEffect();
	UpdateScore();
	UpdateGamePlayerBoard();
	UpdateScoreBoard();
	UpdateScoreWord();
	UpdateGaugeBoard();
	UpdateGaugeBG();
	UpdateGauge();
	UpdateGaugeClearLine();
	UpdateCombo();
	UpdateMaxComboWord();
	UpdateMaxComboNum();
	UpdateComboWord();
	if (GetEditMode())
	{
		UpdateEditMode();
	}
	UpdateNote();
	UpdateNoteFlickArrow();
	UpdateBreakEffect();
	UpdateSongBoard();
	UpdateSongStatusWord();
	UpdateGameAlbumBoard();
	UpdateGameAlbum();
	UpdateGameSongTitle();
	UpdateGameLevelWord();
	UpdatePlayPosition();
	UpdateGameAchievementClearFailed();
	UpdateGameAchievementFCAP();

	switch (GetGameMode())
	{
	case MODE_TUTORIAL:
	case MODE_GAME:
	{
		if (!GetFadeFlag())
		{
			if (GetJudgeLevelAll() == GetMaxNote() && !g_bJudgeFCAP)
			{
				if (GetMaxNote() != 0)
				{
					SetFCAP();
				}
				g_bJudgeFCAP = true;
			}
			if (!GetClearFailedStatus() && g_bJudgeFCAP && !GetFCAP())
			{
				if (nTime >= g_nPlayMusicOffset && !GetPlayStatusM(MUSIC))
				{
					SetClearFailedStatus();
				}
			}
		}
		break;
	}
	}
	UpdateSendData();		//í«â¡
	UpdateScreenMask();
}

// ÉQÅ[ÉÄÇÃï`âÊ
void DrawGame()
{
	if (g_nStartDrawTime == gc_nStartDrawTimeMax)
	{
		DrawGameBG();
		DrawLane();
		DrawLaneEffect();
		DrawJudgeLine();
		DrawJudgeLineSpecial();
		DrawBtn();
		DrawJudgeEffect();
		DrawLine();
		DrawNote();
		DrawComboWord();
		DrawCombo();
		DrawJudge();
		DrawDecision();
		DrawScoreBoard();
		DrawMaxComboWord();
		DrawMaxComboNum();
		DrawScoreWord();
		DrawScore();
		DrawGaugeBoard();
		DrawGaugeBG();
		DrawGauge();
		DrawGaugeClearLine();
		DrawSongBoard();
		DrawSongStatusWord();
		DrawGameAlbumBoard();
		DrawGameAlbum();
		DrawGameSongTitle();
		DrawGameLevelWord();
		DrawGameLevelNum();
		DrawPlayPosition();
		DrawEditMode();
		DrawGamePlayerBoard();

		DrawTimer();
		DrawDebugMaxNote();
		MusicDebug();
		DrawReadNoteDebug();

		DrawRecvDataDebug();		//í«â¡

		DrawScreenMask();

		DrawGamePreface();
		DrawGameAchievementClearFailed();
		DrawGameAchievementFCAP();
		DebugText(200, 267, "PlayPosition:%f", ((GetTimer() - g_nPlayMusicOffset) * 1.0f / GetMusicLength(MUSIC)));
	}
}

int GetPlayMusicOffset()
{
	return g_nPlayMusicOffset;
}

void SetSongURL(char *cURL)
{
	g_cSongURL[0] = '\0';
	strcat(g_cSongURL, cURL);
}

bool GetInitGame()
{
	return g_bInitGame;
}

void SetGamePlayMusicMemo(bool bStatus)
{
	g_bMusic = bStatus;
}
