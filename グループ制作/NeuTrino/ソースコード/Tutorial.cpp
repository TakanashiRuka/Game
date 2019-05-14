//----------------------------------------------------------------------------
//
//	�Q�[������
//
//														 ����ҁF����
//                                                       �쐬���F2017/02/21
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
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

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------

//---------------------------�O���[�o���ϐ�-----------------------------------
static SAVE_GAME_CONFIG g_SaveGameConfig;

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitTutorial()
{
	// �O����
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
