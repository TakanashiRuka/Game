//----------------------------------------------------------------------------
//
//	判定音
//
//														 制作者：劉喆
//                                                       作成日：2017/12/14
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Timer.h"
#include "debug.h"
#include "Judge.h"
#include "sound.h"
#include "JudgeSound.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	int nSoundNum;				// サンウドの番号
	int nCountPlay;				// 再生からの時間カウンター
}JUDGE_SOUND;					// 判定音

//---------------------------グローバル変数-----------------------------------
static JUDGE_SOUND g_JudgeSound[JSL_MAX];
static int g_nJudgeSoundVolume = INIT_SE_VOLUME;

static const int g_nSoundNum[JSL_MAX]=			// 判定音の番号
{
	SE_TAP,
	SE_BREAK,
	SE_TAP,
	SE_TAP,
	SE_TAP,
	SE_FLICK,
	SE_TAP,
	SE_TAP,
	SE_TAP,
};

static const int g_nMaxPlayCount = 2;			// 再生カウンターの最大値

//---------------------------プロトタイプ宣言---------------------------------
void InitJudgeSound()
{
	for (int nCountJudgeSound = 0; nCountJudgeSound < JSL_MAX; nCountJudgeSound++)
	{
		g_JudgeSound[nCountJudgeSound].nCountPlay = g_nMaxPlayCount;
		g_JudgeSound[nCountJudgeSound].nSoundNum = g_nSoundNum[nCountJudgeSound];
	}
	SetJudgeSoundVolume(g_nJudgeSoundVolume);
}

void UninitJudgeSound()
{

}

void UpdateJudgeSound()
{
	for (int nCountJudgeSound = 0; nCountJudgeSound < JSL_MAX; nCountJudgeSound++)
	{
		if (g_JudgeSound[nCountJudgeSound].nCountPlay < g_nMaxPlayCount)
		{
			g_JudgeSound[nCountJudgeSound].nCountPlay++;
		}
	}
}

void SetJudgeSound(int nJudgeSound, bool bStatus)
{
	if (bStatus)
	{
		if (g_JudgeSound[nJudgeSound].nCountPlay == g_nMaxPlayCount)
		{
			if (GetPlayStatus((SOUND_LABEL)g_JudgeSound[nJudgeSound].nSoundNum))
			{
				StopSound((SOUND_LABEL)g_JudgeSound[nJudgeSound].nSoundNum);
			}
			PlaySound((SOUND_LABEL)g_JudgeSound[nJudgeSound].nSoundNum);
			g_JudgeSound[nJudgeSound].nCountPlay = 0;
		}
	}
	else
	{
		StopSound((SOUND_LABEL)g_JudgeSound[nJudgeSound].nSoundNum);
	}
}

void SetJudgeSoundVolume(int nAdd)
{
	g_nJudgeSoundVolume = nAdd;
	
	for (int nCountJudgeSound = 0; nCountJudgeSound < JSL_MAX; nCountJudgeSound++)
	{
		SetSoundVolume((SOUND_LABEL)g_nSoundNum[nCountJudgeSound], g_nJudgeSoundVolume * 0.01f);
	}
}

int GetJudgeSoundVolume()
{
	return g_nJudgeSoundVolume;
}
