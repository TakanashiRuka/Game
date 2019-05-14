//----------------------------------------------------------------------------
//
//	�R���e�B�j���[
//
//														 ����ҁF����
//                                                       �쐬���F2017/03/05
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "Continue.h"
#include "SelectSongBG.h"
#include "ContinueBG.h"
#include "ContinueBtn.h"

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------
typedef enum {
	CA_WORD = 20,
	CA_BTN = 80,
	CA_MAX,
}CONTINUE_ANIMATION;

//---------------------------�\���̒�`---------------------------------------

//---------------------------�O���[�o���ϐ�-----------------------------------
static const int gc_nMaxCount = 80;
static int g_nConut;

//---------------------------�v���g�^�C�v�錾---------------------------------
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