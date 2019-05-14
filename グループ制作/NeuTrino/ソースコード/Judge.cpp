//----------------------------------------------------------------------------
//
//	判定ライン
//
//														 制作者：劉喆
//                                                       作成日：2017/11/30
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Judge.h"
#include "Texture.h"
#include "Polygon3D.h"
#include "Light.h"
#include "Btn.h"
#include "Note.h"
#include "Timer.h"
#include "EditMode.h"
#include "JudgeSound.h"
#include "sound.h"
#include "decision.h"
#include "score.h"
#include "Gauge.h"
#include "Combo.h"
#include "JudgeEffect.h"
#include "Camera.h"

//---------------------------マクロ定義---------------------------------------
#define MAX_SLIDE_MISS_COUNT			(10 * 1000 / 1000)		// スライドの判定遅延値

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	int nJudgeData[JL_MAX];		// 判定の詳細結果
	int nJudgeTime[JL_MAX];		// 判定のタイミング
}JUDGE;							// 判定

static int g_nJudgeTime[JL_MAX]=		// 判定のタイミング
{
	40,									// PERFECT JUST
	60,									// PERFECT
	120,								// GOOD
	120									// BAD
};

static int g_nJudgeTimeFlick[JL_MAX]=	// フリック音符判定のタイミング
{
	130,								// PERFECT JUST
	130,								// PERFECT
	130,								// GOOD
	130									// BAD
};

//---------------------------グローバル変数-----------------------------------
static JUDGE g_Judge;
static bool g_bSlideNoneStatus[NUM_BTN] = { false };		// スライドの頭判定フラグ
static bool g_bSlideMiddleStatus[NUM_BTN] = { false };		// スライドの中間判定フラグ
static int g_nSlideNoneCount[NUM_BTN] = { 0 };				// スライドの遅延判定カウンター

static int g_nJudgeDetailPerType[NT_MAX];					// 各種判定の回数

static const float gc_nSlideMaxChangeAngle = 5.0f;
static const float gc_fSlideMaxChangeY = 0.5f;

static const float gc_nChangeAngleSlideMaxCount[RDP_MAX] = { 15.0f, 45.0f };
static const float gc_fChangeAngleN = 15.0f;

static float g_fChangeAngleNMemo[NUM_BTN];

//---------------------------プロトタイプ宣言---------------------------------
void UpdateJudgeTap(int nCountBtn);
void UpdateJudgeBreak(int nCountBtn);
void UpdateJudgeHold(int nCountBtn);
void UpdateJudgeFlick(int nCountBtn);
void UpdateJudgeSlide(int nCountBtn, int nJudgeNote);

void SetJudgeDetailPerType(int nType, int nNum);

void InitJudge()
{
	for (int nCountType = 0; nCountType < NT_MAX; nCountType++)
	{
		g_nJudgeDetailPerType[nCountType] = 0;
	}
	for (int nCountJudgeLevel = 0; nCountJudgeLevel < JL_MAX; nCountJudgeLevel++)
	{
		g_Judge.nJudgeData[nCountJudgeLevel] = 0;
		g_Judge.nJudgeTime[nCountJudgeLevel] = g_nJudgeTime[nCountJudgeLevel];
	}
	for (int nCountBtn = 0; nCountBtn < NUM_BTN; nCountBtn++)
	{
		g_nSlideNoneCount[nCountBtn] = 0;
		g_fChangeAngleNMemo[nCountBtn] = 0.0f;
		g_bSlideNoneStatus[nCountBtn] = false;
		g_bSlideMiddleStatus[nCountBtn] = false;
	}
}

void UninitJudge()
{

}

void UpdateJudge()
{
	for (int nCountBtn = 0; nCountBtn < NUM_BTN; nCountBtn++)
	{
		if (GetJudgeNoteNum(nCountBtn) < GetNoteMaxNum(nCountBtn))
		{
			switch (GetNote(nCountBtn, GetJudgeNoteNum(nCountBtn)).nType)
			{
			case NT_TAP:
			{
				UpdateJudgeTap(nCountBtn);
				break;
			}
			case NT_BREAK:
			{
				UpdateJudgeBreak(nCountBtn);
				break;
			}
			case NT_HOLD:
			{
				UpdateJudgeHold(nCountBtn);
				break;
			}
			case NT_FLICK:
			{
				UpdateJudgeFlick(nCountBtn);
				break;
			}
			case NT_SLIDE:
			{
				UpdateJudgeSlide(nCountBtn,GetJudgeNoteNum(nCountBtn));
				break;
			}
			}
		}
		SetBtnTriggerTime(nCountBtn, -1);
		//SetBtnDirect(nCountBtn, -1);
	}
	if (GetEditMode())
	{
		//InitJudge();
	}
}

void UpdateJudgeTap(int nCountBtn)
{
	int nBtnTime = GetBtn(nCountBtn).nTriggerTiming;
	int nJudgeNoteNum = GetJudgeNoteNum(nCountBtn);
	int nNoteTime = GetNote(nCountBtn, nJudgeNoteNum).nTiming[NJS_START];
	int nTime = GetTimer();
	bool bJudgeStart = false;

	for (int nCountJudgeLevel = 0; nCountJudgeLevel < JL_MAX; nCountJudgeLevel++)
	{
		if (nBtnTime - nNoteTime <= g_Judge.nJudgeTime[nCountJudgeLevel] &&
			nBtnTime - nNoteTime >= -g_Judge.nJudgeTime[nCountJudgeLevel])
		{
			if (nJudgeNoteNum < GetNoteMaxNum(nCountBtn))
			{
				g_Judge.nJudgeData[nCountJudgeLevel]++;
				CreateDecision(nCountBtn, nCountJudgeLevel);
				if (nCountJudgeLevel == JL_PERFECT)
				{
					if (nBtnTime < nNoteTime)
					{
						CreateDecisionEarlyLate(nCountBtn, JELT_EARLY);
					}
					else
					{
						CreateDecisionEarlyLate(nCountBtn, JELT_LATE);
					}
				}
				SetBtnColor(nCountBtn, nCountJudgeLevel);
				SetJudgeEffect(nCountBtn, NT_TAP, nCountJudgeLevel, JES_UNDER);
				SetJudgeEffect(nCountBtn, NT_TAP, nCountJudgeLevel, JES_UP);
				SetGaugeNum(NT_TAP, nCountJudgeLevel);
				AddScore(NT_TAP, nCountJudgeLevel);
				SetCombo(CC_ADD);
				SetJudgeDetailPerType(NT_TAP, 1);
				SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_START, true);
				SetJudgeNoteNum(nCountBtn, 1);
				bJudgeStart = true;
				SetJudgeSound(JSL_TAP, true);
			}
			break;
		}
	}

	if (!bJudgeStart)
	{
		if (nTime - nNoteTime > g_Judge.nJudgeTime[JL_BAD])
		{
			if (nJudgeNoteNum < GetNoteMaxNum(nCountBtn))
			{
				g_Judge.nJudgeData[JL_BAD]++;
				SetGaugeNum(NT_TAP, JL_BAD);
				//SetBtnColor(nCountBtn, JL_BAD);
				CreateDecision(nCountBtn, JL_BAD);
				SetCombo(CC_RESET);
				SetJudgeDetailPerType(NT_TAP, 1);
				SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_START, true);
				SetJudgeNoteNum(nCountBtn, 1);
			}
		}
	}
}

void UpdateJudgeBreak(int nCountBtn)
{
	int nBtnTime = GetBtn(nCountBtn).nTriggerTiming;
	int nJudgeNoteNum = GetJudgeNoteNum(nCountBtn);
	int nNoteTime = GetNote(nCountBtn, nJudgeNoteNum).nTiming[NJS_START];
	int nTime = GetTimer();
	bool bJudgeStart = false;

	for (int nCountJudgeLevel = 0; nCountJudgeLevel < JL_MAX; nCountJudgeLevel++)
	{
		if (nBtnTime - nNoteTime <= g_Judge.nJudgeTime[nCountJudgeLevel] &&
			nBtnTime - nNoteTime >= -g_Judge.nJudgeTime[nCountJudgeLevel])
		{
			if (nJudgeNoteNum < GetNoteMaxNum(nCountBtn))
			{
				g_Judge.nJudgeData[nCountJudgeLevel]++;
				CreateDecision(nCountBtn, nCountJudgeLevel);
				if (nCountJudgeLevel == JL_PERFECT)
				{
					if (nBtnTime < nNoteTime)
					{
						CreateDecisionEarlyLate(nCountBtn, JELT_EARLY);
					}
					else
					{
						CreateDecisionEarlyLate(nCountBtn, JELT_LATE);
					}
				}
				SetBtnColor(nCountBtn, nCountJudgeLevel);
				SetJudgeEffect(nCountBtn, NT_BREAK, nCountJudgeLevel, JES_UNDER);
				SetJudgeEffect(nCountBtn, NT_BREAK, nCountJudgeLevel, JES_UP);
				SetGaugeNum(NT_BREAK, nCountJudgeLevel);
				AddScore(NT_BREAK, nCountJudgeLevel);
				SetJudgeDetailPerType(NT_BREAK, 1);
				SetCombo(CC_ADD);
				SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_START, true);
				SetJudgeNoteNum(nCountBtn, 1);
				bJudgeStart = true;
				SetJudgeSound(JSL_BREAK, true);
			}
			break;
		}
	}

	if (!bJudgeStart)
	{
		if (nTime - nNoteTime > g_Judge.nJudgeTime[JL_BAD])
		{
			if (nJudgeNoteNum < GetNoteMaxNum(nCountBtn))
			{
				g_Judge.nJudgeData[JL_BAD]++;
				CreateDecision(nCountBtn, JL_BAD);
				SetGaugeNum(NT_BREAK, JL_BAD);
				SetCombo(CC_RESET);
				SetJudgeDetailPerType(NT_BREAK, 1);
				SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_START, true);
				SetJudgeNoteNum(nCountBtn, 1);
			}
		}
	}
}

void UpdateJudgeHold(int nCountBtn)
{
	BTN Btn = GetBtn(nCountBtn);
	int nJudgeNoteNum = GetJudgeNoteNum(nCountBtn);
	NOTE Note = GetNote(nCountBtn, nJudgeNoteNum);
	int nTime = GetTimer();

	if (!Note.bJudgeStatus[NJS_START])
	{
		for (int nCountJudgeLevel = 0; nCountJudgeLevel < JL_MAX; nCountJudgeLevel++)
		{
			if (Btn.nTriggerTiming - Note.nTiming[NJS_START] <= g_Judge.nJudgeTime[nCountJudgeLevel] &&
				Btn.nTriggerTiming - Note.nTiming[NJS_START] >= -g_Judge.nJudgeTime[nCountJudgeLevel])
			{
				if (nJudgeNoteNum < GetNoteMaxNum(nCountBtn))
				{
					g_Judge.nJudgeData[nCountJudgeLevel]++;
					CreateDecision(nCountBtn, nCountJudgeLevel);
					if (nCountJudgeLevel == JL_PERFECT)
					{
						if (Btn.nTriggerTiming < Note.nTiming[NJS_START])
						{
							CreateDecisionEarlyLate(nCountBtn, JELT_EARLY);
						}
						else
						{
							CreateDecisionEarlyLate(nCountBtn, JELT_LATE);
						}
					}
					SetBtnColor(nCountBtn, nCountJudgeLevel);
					SetJudgeEffect(nCountBtn, NT_HOLD, nCountJudgeLevel, JES_UNDER);
					SetJudgeEffect(nCountBtn, NT_HOLD, nCountJudgeLevel, JES_UP);
					AddScore(NT_HOLD, nCountJudgeLevel);
					SetGaugeNum(NT_HOLD, nCountJudgeLevel);
					SetJudgeDetailPerType(NT_HOLD, 1);
					SetCombo(CC_ADD);
					SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_START, true);
					SetJudgeSound(JSL_HOLD_START, true);
				}
				if (nCountJudgeLevel != JL_BAD)
				{
					if (!Note.bHoldBlendFactor)
					{
						SetHoldBlendFactor(nCountBtn, nJudgeNoteNum, true);
					}
				}
				break;
			}
		}
	}

	Note = GetNote(nCountBtn, nJudgeNoteNum);
	if (!Note.bJudgeStatus[NJS_START])
	{
		if (nTime - Note.nTiming[NJS_START] > g_Judge.nJudgeTime[JL_BAD])
		{
			if (nJudgeNoteNum < GetNoteMaxNum(nCountBtn))
			{
				g_Judge.nJudgeData[JL_BAD]++;
				CreateDecision(nCountBtn, JL_BAD);
				SetGaugeNum(NT_HOLD, JL_BAD);
				SetCombo(CC_RESET);
				SetJudgeDetailPerType(NT_HOLD, 1);
				SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_START, true);
				SetBtnReleaseTime(nCountBtn, -1);
			}
		}
	}

	Note = GetNote(nCountBtn, nJudgeNoteNum);
	if (Note.bJudgeStatus[NJS_START] && !Note.bJudgeStatus[NJS_END])
	{										
		if (nTime >= Note.nTimingHoldMiddle)
		{
			if (Btn.nReleaseTiming == 0 && Note.nTimingHoldMiddle < Note.nTimingHoldMiddleMax)
			{
				g_Judge.nJudgeData[JL_PERFECT_JUST]++;
				CreateDecision(nCountBtn, JL_PERFECT_JUST);
				SetJudgeEffect(nCountBtn, NT_HOLD, JL_PERFECT_JUST, JES_UNDER);
				SetJudgeEffect(nCountBtn, NT_HOLD, JL_PERFECT_JUST, JES_UP);
				SetGaugeNum(NT_HOLD, JL_PERFECT_JUST);
				SetCombo(CC_ADD);
				AddScore(NT_HOLD, JL_PERFECT_JUST);
				SetJudgeDetailPerType(NT_HOLD, 1);
				if (Note.nHoldA != HOLD_MAX_A)
				{
					SetHoldAlpha(nCountBtn, nJudgeNoteNum, HOLD_MAX_A);
				}
				if (!Note.bHoldBlendFactor)
				{
					SetHoldBlendFactor(nCountBtn, nJudgeNoteNum, true);
				}
			}
			else
			if (Btn.nReleaseTiming != 0 && Note.nTimingHoldMiddle < Note.nTimingHoldMiddleMax)
			{
				g_Judge.nJudgeData[JL_BAD]++;
				CreateDecision(nCountBtn, JL_BAD);
				SetGaugeNum(NT_HOLD, JL_BAD);
				SetCombo(CC_RESET);
				SetJudgeDetailPerType(NT_HOLD, 1);
				if (Note.nHoldA != HOLD_MIN_A)
				{
					SetHoldAlpha(nCountBtn, nJudgeNoteNum, HOLD_MIN_A);
				}
				if (Note.bHoldBlendFactor)
				{
					SetHoldBlendFactor(nCountBtn, nJudgeNoteNum, false);
				}
			}
			SetHoldMiddleTime(nCountBtn, nJudgeNoteNum, TIME_PER_HOLD);
		}
		Note = GetNote(nCountBtn, nJudgeNoteNum);
		if (nTime >= Note.nTiming[NJS_END] && Note.nTimingHoldMiddle >= Note.nTimingHoldMiddleMax)
		{
			SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_END, true);
			SetJudgeNoteNum(nCountBtn, 1);
			if (Btn.nReleaseTiming == 0)
			{
				SetJudgeSound(JSL_HOLD_END, true);
			}
		}
	}
}

void UpdateJudgeFlick(int nCountBtn)
{
	BTN Btn = GetBtn(nCountBtn);
	int nJudgeNoteNum = GetJudgeNoteNum(nCountBtn);
	NOTE Note = GetNote(nCountBtn, nJudgeNoteNum);
	int nTime = GetTimer();
	for (int nCountJudgeLevel = 0; nCountJudgeLevel < JL_MAX; nCountJudgeLevel++)
	{
		if (Btn.nTriggerTiming - Note.nTiming[NJS_START] <= g_nJudgeTimeFlick[nCountJudgeLevel] &&
			Btn.nTriggerTiming - Note.nTiming[NJS_START] >= -g_nJudgeTimeFlick[nCountJudgeLevel])
		{
			if (nJudgeNoteNum < GetNoteMaxNum(nCountBtn))
			{
				if (Btn.nBtnDirection == Note.nFlickDirection/* && Btn.nBtnDirection != -1*/)
				{
					g_Judge.nJudgeData[nCountJudgeLevel]++;
					CreateDecision(nCountBtn, nCountJudgeLevel);
					SetJudgeEffect(nCountBtn, NT_FLICK, nCountJudgeLevel, JES_UNDER, true, Note.nFlickDirection);
					SetJudgeEffect(nCountBtn, NT_FLICK, nCountJudgeLevel, JES_UP, true, Note.nFlickDirection);
					AddScore(NT_FLICK, nCountJudgeLevel);
					SetGaugeNum(NT_FLICK, nCountJudgeLevel);
					SetCombo(CC_ADD);
					SetJudgeDetailPerType(NT_FLICK, 1);
					SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_START, true);
					SetJudgeNoteNum(nCountBtn, 1);
					SetJudgeSound(JSL_FLICK, true);
				}
				//else
				//	if (Btn.nBtnDirection != Note.nFlickDirection)
				//	{
				//		g_Judge.nJudgeData[JL_GOOD]++;
				//	}
			}
			break;
		}
	}

	Note = GetNote(nCountBtn, nJudgeNoteNum);
	if (!Note.bJudgeStatus[NJS_START])
	{
		if (nTime - Note.nTiming[NJS_START] > g_nJudgeTimeFlick[JL_BAD] && !Note.bJudgeStatus[NJS_START])
		{
			if (nJudgeNoteNum < GetNoteMaxNum(nCountBtn))
			{
				g_Judge.nJudgeData[JL_BAD]++;
				CreateDecision(nCountBtn, JL_BAD);
				SetCombo(CC_RESET);
				SetGaugeNum(NT_FLICK, JL_BAD);
				SetJudgeDetailPerType(NT_FLICK, 1);
				SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_START, true);
				SetJudgeNoteNum(nCountBtn, 1);
			}
		}
	}
}

void UpdateJudgeSlide(int nCountBtn, int nJudgeNote)
{
	BTN Btn = GetBtn(nCountBtn);
	int nJudgeNoteNum = nJudgeNote;
	NOTE Note = GetNote(nCountBtn, nJudgeNoteNum);
	int nTime = GetTimer();

	// 頭部分の判定
	if (!Note.bJudgeStatus[NJS_START])
	{
		if (Note.bSlideStartEndFlag[NJS_START])
		{
			g_bSlideNoneStatus[nCountBtn] = true;
			g_nSlideNoneCount[nCountBtn] = 0;
			g_bSlideMiddleStatus[nCountBtn] = true;
		}

		if (Note.nFlickDirection != -1)
		{

			for (int nCountJudgeLevel = 0; nCountJudgeLevel < JL_MAX; nCountJudgeLevel++)
			{
				if (Btn.nTriggerTiming - Note.nTiming[NJS_START] <= g_nJudgeTimeFlick[nCountJudgeLevel] &&
					Btn.nTriggerTiming - Note.nTiming[NJS_START] >= -g_nJudgeTimeFlick[nCountJudgeLevel])
				{
					if (nJudgeNoteNum < GetNoteMaxNum(nCountBtn))
					{
						//if (Note.nFlickDirection == Btn.nBtnDirection)
						if (Note.nTiming[NJS_START] == Note.nTiming[NJS_END])
						{
							if (Note.nFlickDirection == Btn.nBtnDirection)
							{
								if (!g_bSlideNoneStatus[nCountBtn])
								{
									g_bSlideNoneStatus[nCountBtn] = true;
									g_bSlideMiddleStatus[nCountBtn] = true;
									g_nSlideNoneCount[nCountBtn] = 0;
								}
								g_Judge.nJudgeData[nCountJudgeLevel]++;
								CreateDecision(nCountBtn, nCountJudgeLevel);
								if (nJudgeNoteNum == GetJudgeNoteNum(nCountBtn))
								{
									SetJudgeEffect(nCountBtn, NT_SLIDE, nCountJudgeLevel, JES_UNDER, true);
								}
								SetGaugeNum(NT_SLIDE, nCountJudgeLevel);
								SetCombo(CC_ADD);
								AddScore(NT_SLIDE, nCountJudgeLevel);
								SetJudgeDetailPerType(NT_SLIDE, 1);
								SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_START, true);
								SetJudgeSound(JSL_FLICK, true);
								if (!Note.bSlideStartEndFlag[NJS_END])
								{
									g_Judge.nJudgeData[nCountJudgeLevel]++;
									CreateDecision(nCountBtn, nCountJudgeLevel);
									if (nJudgeNoteNum == GetJudgeNoteNum(nCountBtn))
									{
										SetJudgeEffect(nCountBtn, NT_SLIDE, nCountJudgeLevel, JES_UNDER, true);
									}
									SetGaugeNum(NT_SLIDE, nCountJudgeLevel);
									SetCombo(CC_ADD);
									AddScore(NT_SLIDE, nCountJudgeLevel);
									SetJudgeDetailPerType(NT_SLIDE, 1);
									SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum + 1, NJS_START, true);
									if (!Note.bHoldBlendFactor)
									{
										SetHoldBlendFactor(nCountBtn, nJudgeNoteNum + 1, true);
									}
									if (Note.nHoldA != HOLD_MAX_A)
									{
										SetHoldAlpha(nCountBtn, nJudgeNoteNum + 1, HOLD_MAX_A);
									}
								}
							}
						}
						else
						{
							if (Note.bSlideStartEndFlag[NJS_START])
							{
								if (Note.nFlickDirection == Btn.nBtnDirection)
								{
									if (!g_bSlideNoneStatus[nCountBtn])
									{
										g_bSlideNoneStatus[nCountBtn] = true;
										g_bSlideMiddleStatus[nCountBtn] = true;
										g_nSlideNoneCount[nCountBtn] = 0;
									}
									g_Judge.nJudgeData[nCountJudgeLevel]++;
									CreateDecision(nCountBtn, nCountJudgeLevel);
									if (nJudgeNoteNum == GetJudgeNoteNum(nCountBtn))
									{
										SetJudgeEffect(nCountBtn, NT_SLIDE, nCountJudgeLevel, JES_UNDER, true);
									}
									AddScore(NT_SLIDE, nCountJudgeLevel);
									SetCombo(CC_ADD);
									SetGaugeNum(NT_SLIDE, nCountJudgeLevel);
									SetJudgeDetailPerType(NT_SLIDE, 1);
									SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_START, true);
									if (Note.bSlideStartEndFlag[NJS_START])
									{
										SetJudgeSound(JSL_SLIDE_START, true);
									}
								}
							}
							else
							{
								if (!g_bSlideNoneStatus[nCountBtn] && g_bSlideMiddleStatus[nCountBtn])
								{
									g_bSlideNoneStatus[nCountBtn] = true;
									g_bSlideMiddleStatus[nCountBtn] = true;
									g_nSlideNoneCount[nCountBtn] = 0;
								}
								g_Judge.nJudgeData[nCountJudgeLevel]++;
								CreateDecision(nCountBtn, nCountJudgeLevel);
								if (nJudgeNoteNum == GetJudgeNoteNum(nCountBtn))
								{
									SetJudgeEffect(nCountBtn, NT_SLIDE, nCountJudgeLevel, JES_UNDER, true);
								}
								AddScore(NT_SLIDE, nCountJudgeLevel);
								SetCombo(CC_ADD);
								SetGaugeNum(NT_SLIDE, nCountJudgeLevel);
								SetJudgeDetailPerType(NT_SLIDE, 1);
								SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_START, true);
								if (Note.bSlideStartEndFlag[NJS_START])
								{
									SetJudgeSound(JSL_SLIDE_START, true);
								}
							}
						}
					}
					if (nCountJudgeLevel != JL_BAD)
					{
						if (!Note.bHoldBlendFactor)
						{
							SetHoldBlendFactor(nCountBtn, nJudgeNoteNum, true);
						}
						if (Note.nHoldA != HOLD_MAX_A)
						{
							SetHoldAlpha(nCountBtn, nJudgeNoteNum, HOLD_MAX_A);
						}
					}
					break;
				}
			}
		}
		else
		{
			if (Note.nTiming[NJS_START] <= nTime && g_bSlideNoneStatus[nCountBtn])
			{
				if (nJudgeNoteNum < GetNoteMaxNum(nCountBtn))
				{
					if (g_bSlideNoneStatus[nCountBtn])
					{
						g_bSlideMiddleStatus[nCountBtn] = true;
						g_nSlideNoneCount[nCountBtn] = 0;
						g_Judge.nJudgeData[JL_PERFECT_JUST]++;
						CreateDecision(nCountBtn, JL_PERFECT_JUST);
						if (nJudgeNoteNum == GetJudgeNoteNum(nCountBtn))
						{
							SetJudgeEffect(nCountBtn, NT_SLIDE, JL_PERFECT_JUST, JES_UNDER, true);
						}
						AddScore(NT_SLIDE, JL_PERFECT_JUST);
						SetCombo(CC_ADD);
						SetGaugeNum(NT_SLIDE, JL_PERFECT_JUST);
						SetJudgeDetailPerType(NT_SLIDE, 1);
						SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_START, true);
						if (Note.bSlideStartEndFlag[NJS_START])
						{
							SetJudgeSound(JSL_SLIDE_START, true);
						}
						if (!Note.bHoldBlendFactor)
						{
							SetHoldBlendFactor(nCountBtn, nJudgeNoteNum, true);
						}
					}
				}
			}
		}
	}

	Note = GetNote(nCountBtn, nJudgeNoteNum);
	if (!Note.bJudgeStatus[NJS_START])
	{
		if (nTime - Note.nTiming[NJS_START] > g_nJudgeTimeFlick[JL_BAD])
		{
			if (nJudgeNoteNum < GetNoteMaxNum(nCountBtn))
			{
				if (g_bSlideNoneStatus[nCountBtn])
				{
					g_bSlideNoneStatus[nCountBtn] = false;
				}
				g_Judge.nJudgeData[JL_BAD]++;
				CreateDecision(nCountBtn, JL_BAD);
				SetJudgeEffect(nCountBtn, NT_SLIDE, JL_BAD, JES_UNDER, false);
				SetCombo(CC_RESET);
				SetGaugeNum(NT_SLIDE, JL_BAD);
				SetJudgeDetailPerType(NT_SLIDE, 1);
				if (Note.bHoldBlendFactor)
				{
					SetHoldBlendFactor(nCountBtn, nJudgeNoteNum, false);
				}
				if (Note.nHoldA == HOLD_MAX_A)
				{
					SetHoldAlpha(nCountBtn, nJudgeNoteNum, HOLD_MIN_A);
				}
				SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_START, true);
				SetBtnReleaseTime(nCountBtn, -1);
			}
		}
	}

	// カメラの傾き
	g_fChangeAngleNMemo[nCountBtn] = 0.0f;
	if (nTime >= Note.nTiming[NJS_START] && nTime < Note.nTiming[NJS_END] && nJudgeNoteNum == GetJudgeNoteNum(nCountBtn)
		&& g_bSlideNoneStatus[nCountBtn] && g_bSlideMiddleStatus[nCountBtn])
	{
		if (Note.nTiming[NJS_START] != Note.nTiming[NJS_END])
		{
			bool bChange = false;
			if (Note.bSlideStartEndFlag[NJS_START] && Note.bJudgeStatus[NJS_START])
			{
				bChange = true;
			}
			else
				if (Note.bSlideStartEndFlag[NJS_START] && !Note.bJudgeStatus[NJS_START])
				{
					bChange = false;
				}
				else
					if (!Note.bSlideStartEndFlag[NJS_START])
					{
						bChange = true;
					}
			if (bChange)
			{
				float fX = GetSlidePos(nCountBtn).x;
				float fY = GetSlidePos(nCountBtn).y;

				fX /= NOTE_RANGE_X;
				fY /= NOTE_SLIDE_Y_MAX;
				//if (nCountBtn == NUM_BTN_NORMAL)
				//{
				//	fX += 0.0f;
				//}
				//else
				//{
				//	fX -= 0.0f;
				//}
				float fAngle = D3DXToRadian(fX * gc_nSlideMaxChangeAngle);
				g_fChangeAngleNMemo[nCountBtn] = gc_fChangeAngleN * (1000.0f / (Note.nTiming[NJS_END] - Note.nTiming[NJS_START]));
				if (g_fChangeAngleNMemo[nCountBtn] < gc_nChangeAngleSlideMaxCount[RDP_INIT])
				{
					g_fChangeAngleNMemo[nCountBtn] = gc_nChangeAngleSlideMaxCount[RDP_INIT];
				}
				if (g_fChangeAngleNMemo[nCountBtn] > gc_nChangeAngleSlideMaxCount[RDP_END])
				{
					g_fChangeAngleNMemo[nCountBtn] = gc_nChangeAngleSlideMaxCount[RDP_END];
				}
				SetChangeAngleCountMax(g_fChangeAngleNMemo[nCountBtn]);
				if (nCountBtn == NUM_BTN - 1)
				{
					if (g_fChangeAngleNMemo[nCountBtn - 1] != 0.0f)
					{
						SetChangeAngleCountMax((g_fChangeAngleNMemo[nCountBtn - 1] + g_fChangeAngleNMemo[nCountBtn]) / 2.0f);
					}
				}
				SetCameraAngleTarget(fAngle);
				SetCameraYTarget(fY * gc_fSlideMaxChangeY);
			}
		}
	}

	// 中間部分の判定

	if (Note.nFlickDirection == -1 && !g_bSlideNoneStatus[nCountBtn] && nJudgeNoteNum == GetJudgeNoteNum(nCountBtn))
	{
		if (Btn.nTriggerTiming != -1 && Btn.nBtnDirection != -1)
		{
			g_bSlideNoneStatus[nCountBtn] = true;

			g_bSlideMiddleStatus[nCountBtn] = true;
			g_nSlideNoneCount[nCountBtn] = 0;
		}
	}

	if (Note.nFlickDirection != -1)
	{
		if (Note.nFlickDirection == Btn.nBtnDirection && nJudgeNoteNum == GetJudgeNoteNum(nCountBtn))
		{
			g_bSlideMiddleStatus[nCountBtn] = true;
			g_nSlideNoneCount[nCountBtn] = 0;
		}
		else
			if(Note.nFlickDirection != Btn.nBtnDirection && nJudgeNoteNum == GetJudgeNoteNum(nCountBtn))
		{
			if (g_nSlideNoneCount[nCountBtn] < MAX_SLIDE_MISS_COUNT)
			{
				g_nSlideNoneCount[nCountBtn]++;
			}
			if (g_nSlideNoneCount[nCountBtn] == MAX_SLIDE_MISS_COUNT)
			{
				g_bSlideMiddleStatus[nCountBtn] = false;
			}
		}
	}

	Note = GetNote(nCountBtn, nJudgeNoteNum);
	if (Note.bJudgeStatus[NJS_START] && !Note.bJudgeStatus[NJS_END] && nJudgeNoteNum == GetJudgeNoteNum(nCountBtn))
	{
		if (nTime >= Note.nTimingHoldMiddle && Note.nTimingHoldMiddle < Note.nTimingHoldMiddleMax)
		{
			if (Note.nFlickDirection != -1)
			{
				if (Btn.nReleaseTiming == 0 && g_bSlideMiddleStatus[nCountBtn])
				{
					if (!g_bSlideNoneStatus[nCountBtn])
					{
						g_bSlideNoneStatus[nCountBtn] = true;
					}
					g_Judge.nJudgeData[JL_PERFECT_JUST]++;
					CreateDecision(nCountBtn, JL_PERFECT_JUST);
					SetJudgeEffect(nCountBtn, NT_SLIDE, JL_PERFECT_JUST, JES_UNDER);
					AddScore(NT_SLIDE, JL_PERFECT_JUST);
					SetCombo(CC_ADD);
					SetGaugeNum(NT_SLIDE, JL_PERFECT_JUST);
					SetJudgeDetailPerType(NT_SLIDE, 1);
					if (Note.nHoldA != HOLD_MAX_A)
					{
						SetHoldAlpha(nCountBtn, nJudgeNoteNum, HOLD_MAX_A);
					}
					if (!Note.bHoldBlendFactor)
					{
						SetHoldBlendFactor(nCountBtn, nJudgeNoteNum, true);
					}
				}
				else
				{
					if (g_bSlideNoneStatus[nCountBtn])
					{
						g_bSlideNoneStatus[nCountBtn] = false;
					}
					g_Judge.nJudgeData[JL_BAD]++;
					CreateDecision(nCountBtn, JL_BAD);
					SetJudgeEffect(nCountBtn, NT_SLIDE, JL_BAD, JES_UNDER, false);
					SetCombo(CC_RESET);
					SetGaugeNum(NT_SLIDE, JL_BAD);
					SetJudgeDetailPerType(NT_SLIDE, 1);
					if (Note.nHoldA != HOLD_MIN_A)
					{
						SetHoldAlpha(nCountBtn, nJudgeNoteNum, HOLD_MIN_A);
					}
					if (Note.bHoldBlendFactor)
					{
						SetHoldBlendFactor(nCountBtn, nJudgeNoteNum, false);
					}
				}
			}
			else
			{
				if (g_bSlideNoneStatus[nCountBtn] && Note.nTimingHoldMiddle < Note.nTimingHoldMiddleMax)
				{
					g_Judge.nJudgeData[JL_PERFECT_JUST]++;
					CreateDecision(nCountBtn, JL_PERFECT_JUST);
					SetJudgeEffect(nCountBtn, NT_SLIDE, JL_PERFECT_JUST, JES_UNDER);
					SetGaugeNum(NT_SLIDE, JL_PERFECT_JUST);
					SetCombo(CC_ADD);
					AddScore(NT_SLIDE, JL_PERFECT_JUST);
					SetJudgeDetailPerType(NT_SLIDE, 1);
					if (Note.nHoldA != HOLD_MAX_A)
					{
						SetHoldAlpha(nCountBtn, nJudgeNoteNum, HOLD_MAX_A);
					}
					if (!Note.bHoldBlendFactor)
					{
						SetHoldBlendFactor(nCountBtn, nJudgeNoteNum, true);
					}
				}
				else
				{
					g_Judge.nJudgeData[JL_BAD]++;
					CreateDecision(nCountBtn, JL_BAD);
					SetJudgeEffect(nCountBtn, NT_SLIDE, JL_BAD, JES_UNDER, false);
					SetGaugeNum(NT_SLIDE, JL_BAD);
					SetCombo(CC_RESET);
					SetJudgeDetailPerType(NT_SLIDE, 1);
					if (Note.nHoldA != HOLD_MIN_A)
					{
						SetHoldAlpha(nCountBtn, nJudgeNoteNum, HOLD_MIN_A);
					}
					if (Note.bHoldBlendFactor)
					{
						SetHoldBlendFactor(nCountBtn, nJudgeNoteNum, false);
					}
				}
			}
			SetHoldMiddleTime(nCountBtn, nJudgeNoteNum, TIME_PER_HOLD);
		}

		// 終了部分
		Note = GetNote(nCountBtn, nJudgeNoteNum);
		if (nTime >= Note.nTiming[NJS_END] && Note.nTimingHoldMiddle >= Note.nTimingHoldMiddleMax)
		{
			SetNoteJudgeStatus(nCountBtn, nJudgeNoteNum, NJS_END, true);
			if (g_bSlideMiddleStatus[nCountBtn] && g_bSlideNoneStatus[nCountBtn])
			{
				SetJudgeEffect(nCountBtn, NT_SLIDE, JL_PERFECT_JUST, JES_UP);
				SetJudgeEffect(nCountBtn, NT_SLIDE, JL_BAD, JES_UNDER);
			}
			if (Note.bSlideStartEndFlag[NJS_END])
			{
				SetJudgeEffect(nCountBtn, NT_SLIDE, JL_BAD, JES_UNDER, false);
				SetCameraAngleTarget(0.0f);
				SetCameraYTarget(0.0f);
			}
			SetJudgeNoteNum(nCountBtn, 1);
			//if (g_bSlideMiddleStatus[nCountBtn])
			//{
			//	g_nSlideNoneCount[nCountBtn] = 0;
			//}
			if (g_bSlideMiddleStatus[nCountBtn] && g_bSlideNoneStatus[nCountBtn] && (Note.nTiming[NJS_START] != Note.nTiming[NJS_END]))
			{
				SetJudgeSound(JSL_SLIDE_END, true);
			}
		}
		if (!Note.bSlideStartEndFlag[NJS_END])
		{
			UpdateJudgeSlide(nCountBtn, nJudgeNoteNum + 1);
		}
	}
}

void DrawJudge()
{
	DebugText(0, 176, "JudgeDetail: %d, %d, %d, %d", g_Judge.nJudgeData[JL_PERFECT_JUST], g_Judge.nJudgeData[JL_PERFECT], g_Judge.nJudgeData[JL_GOOD], g_Judge.nJudgeData[JL_BAD]);
	for (int nCountType = 0; nCountType < NT_MAX; nCountType++)
	{
		DebugText(100, 22 * 13 + 22 * nCountType, "%d: %d", nCountType, g_nJudgeDetailPerType[nCountType]);
	}
}

void SetJudgeDetailPerType(int nType, int nNum)
{
	g_nJudgeDetailPerType[nType] += nNum;
}

int GetJudgeLevelDetail(int nJudgeLevel)
{
	return g_Judge.nJudgeData[nJudgeLevel];
}

int GetJudgeLevelAll()
{
	int nJudgeAll = 0;
	for (int nCount = 0; nCount < JL_MAX; nCount++)
	{
		nJudgeAll += g_Judge.nJudgeData[nCount];
	}
	return nJudgeAll;
}