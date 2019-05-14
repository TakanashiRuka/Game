#include "main.h"
#include "Timer.h"
#include "debug.h"
#include "game.h"
#include "music.h"
#include "Result.h"

// �O���[�o���ϐ�
static DWORD TimeFirst; 
static DWORD TimeGet;
static DWORD TimeGetOld;
static int Time = 0;
static int g_TimeMiss;

// �^�C�}�[�̏�����
void InitTimer()
{
	Time = 0;
	TimeFirst =
	TimeGetOld =
	TimeGet = timeGetTime();
}

// �^�C�}�[�̍X�V
void UpdateTimer()
{
	g_TimeMiss = TimeGet - TimeGetOld;
	TimeGetOld = TimeGet;
	TimeGet = timeGetTime();
	Time = TimeGet - TimeFirst;
}

void DrawTimer()
{
	DebugText(200, 22, "TimeMiss:%d", g_TimeMiss);
	DebugText(0, 20, "Time:%d", Time);
}

// �^�C�}�[�̎擾
int GetTimer()
{
	return Time;
}

void SetTimer(int nTime)
{
	Time = nTime;
	TimeGet = timeGetTime();
	TimeFirst = TimeGet - Time;
}
