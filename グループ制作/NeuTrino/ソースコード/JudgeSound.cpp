//----------------------------------------------------------------------------
//
//	���艹
//
//														 ����ҁF����
//                                                       �쐬���F2017/12/14
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "Timer.h"
#include "debug.h"
#include "Judge.h"
#include "sound.h"
#include "JudgeSound.h"

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------
typedef struct {
	int nSoundNum;				// �T���E�h�̔ԍ�
	int nCountPlay;				// �Đ�����̎��ԃJ�E���^�[
}JUDGE_SOUND;					// ���艹

//---------------------------�O���[�o���ϐ�-----------------------------------
static JUDGE_SOUND g_JudgeSound[JSL_MAX];
static int g_nJudgeSoundVolume = INIT_SE_VOLUME;

static const int g_nSoundNum[JSL_MAX]=			// ���艹�̔ԍ�
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

static const int g_nMaxPlayCount = 2;			// �Đ��J�E���^�[�̍ő�l

//---------------------------�v���g�^�C�v�錾---------------------------------
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
