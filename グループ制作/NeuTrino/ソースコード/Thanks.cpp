//----------------------------------------------------------------------------
//
//	�㏑��
//
//														 ����ҁF����
//                                                       �쐬���F2017/02/28
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "SelectSongBG.h"
#include "ThanksBG.h"
#include "sound.h"
#include "FadeInOut.h"

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------

//---------------------------�O���[�o���ϐ�-----------------------------------
static const int gc_nThanksMaxCount = 6 * 60;
static int g_nThanksCount;
static bool g_bPlayBGM;

//---------------------------�v���g�^�C�v�錾---------------------------------
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