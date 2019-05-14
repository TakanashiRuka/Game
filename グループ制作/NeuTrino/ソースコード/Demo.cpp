//----------------------------------------------------------------------------
//
//	�f�����[�h
//
//														 ����ҁF����
//                                                       �쐬���F2018/01/22
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "Demo.h"
#include "game.h"
#include "Btn.h"
#include "Timer.h"
#include "Preface.h"
#include "SelectSongPanel.h"
#include "ReadSongList.h"
#include "Note.h"
#include "JudgeSound.h"
#include "TitleBtnManual.h"
#include "GamePreface.h"

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------

//---------------------------�O���[�o���ϐ�-----------------------------------
static SAVE_GAME_CONFIG g_SaveGameConfig;
static const int gc_nCountFrameMax = 75 * 60;
static int g_nCountFrame;

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitDemo()
{
	g_nCountFrame = 0;

	// �O����
	g_SaveGameConfig.nSelectNum = GetSelectNum();
	g_SaveGameConfig.nLevel = GetLevel();
	g_SaveGameConfig.fSpeed = GetHiSpeed();
	g_SaveGameConfig.nOffset = GetOffsetNote();
	g_SaveGameConfig.nSE = GetJudgeSoundVolume();
	
	SetLevel(rand() % (MAX_LEVEL - 1));
	int nSelectNum = rand() % (GetMaxFileNum());
	do
	{
		nSelectNum = rand() % (GetMaxFileNum());
	} while (!GetSSSongStatus(nSelectNum).bDemo);
	SetSelectNum(nSelectNum);
	SetHiSpeed(INIT_SPEED);
	SetOffsetNote(INIT_OFFSET);
	SetJudgeSoundVolume(INIT_SE_VOLUME);

	UninitSelectSongPanel();
	InitGame();
	SetAutoPlay(true);
	InitTimer();
	InitTitleBtnManual();
}

void UninitDemo()
{
	UninitGame();
	UninitTitleBtnManual();
	SetLevel(g_SaveGameConfig.nLevel);
	SetSelectNum(g_SaveGameConfig.nSelectNum);
	SetHiSpeed(g_SaveGameConfig.fSpeed);
	SetOffsetNote(g_SaveGameConfig.nOffset);
	SetJudgeSoundVolume(g_SaveGameConfig.nSE);
	SetAutoPlay(false);
}

void UpdateDemo()
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoyStickTrigger(JOY_SPHERE))
	{
		if (GetPlayStatus(SE_ENTER))
		{
			StopSound(SE_ENTER);
		}
		PlaySound(SE_ENTER);
		SetGameModeTarget(MODE_TITLE);
		ChangeGameMode();
	}
	if (g_nCountFrame < gc_nCountFrameMax)
	{
		g_nCountFrame++;
		if (g_nCountFrame == gc_nCountFrameMax)
		{
			UninitDemo();
			InitPreface();
			SetMode(MODE_PREFACE);
		}
	}
	if (!GetFadeFlag() && !GetGamePreface())
	{
		UpdateTimer();
	}
	UpdateGame();
	UpdateTitleBtnManual();
}

void DrawDemo()
{
	DrawGame();
	DrawTitleBtnManual();
}