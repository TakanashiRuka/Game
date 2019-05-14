#include "main.h"
#include "Timer.h"
#include "debug.h"
#include "game.h"
#include "music.h"
#include "Result.h"

// グローバル変数
static DWORD TimeFirst; 
static DWORD TimeGet;
static DWORD TimeGetOld;
static int Time = 0;
static int g_TimeMiss;

// タイマーの初期化
void InitTimer()
{
	Time = 0;
	TimeFirst =
	TimeGetOld =
	TimeGet = timeGetTime();
}

// タイマーの更新
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

// タイマーの取得
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
