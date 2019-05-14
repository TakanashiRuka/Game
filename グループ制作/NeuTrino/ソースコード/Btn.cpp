//----------------------------------------------------------------------------
//
//	ボタン
//
//														 制作者：劉喆
//                                                       作成日：2017/11/29
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Btn.h"
#include "Texture.h"
#include "Polygon3D.h"
#include "Light.h"
#include "input.h"
#include "JoyStick.h"
#include "Timer.h"
#include "debug.h"
#include "Note.h"
#include "Judge.h"

//---------------------------マクロ定義---------------------------------------
#define BTN_X								(-152.0f)
#define BTN_Y								(0.0001f)
#define BTN_Z								(-300.0f)

#define BTN_RANGE_X							(101.0f)
#define BTN_COUNT_MAX						(5)

#define BTN_RELEASE_FLAG_MAX_COUNT			(12)

#define STICK_MIN_LENGTH					(0.4f)		// スティックを反応させる最小距離
#define STICK_CHANGE_LENGTH_MIN				(0.4f)		// スティックを反応させる最小距離変化
#define STICK_CHANGE_ANGLE_MIN				(20.0f)		// スティックを反応させる最小角度変化
#define MAX_ANGLE							(360.0f)	

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------
static BTN g_Btn[NUM_BTN];
static int g_nTestBtnTriggerTime[NUM_BTN];
static int g_nTestBtnReleaseTime[NUM_BTN];
static int g_nTestBtnDirect[NUM_BTN];

static const D3DCOLOR gc_dDefaultBtnColor = D3DCOLOR_ARGB(255, 255, 255, 255);
static const D3DCOLOR gc_dBtnColorList[JL_MAX]=
{
	D3DCOLOR_ARGB(255, 253, 255, 100),
	D3DCOLOR_ARGB(255, 255, 138, 0),
	D3DCOLOR_ARGB(255, 0, 255, 30),
	D3DCOLOR_ARGB(255, 0, 108, 255),
};

static float g_fStickDirectAngle[NFD_MAX][2]=
{
	{ 315.0f, 45.0f },
	{ 135.0f, 225.0f },
	{ 225.0f, 315.0f },
	{ 45.0f, 135.0f },
};

static bool g_bAuto = false;

//---------------------------プロトタイプ宣言---------------------------------
void InitBtn()
{
	g_bAuto = false;
	for (int nCountBtn = 0; nCountBtn < NUM_BTN; nCountBtn++)
	{
		g_nTestBtnTriggerTime[nCountBtn] = -1;
		g_nTestBtnReleaseTime[nCountBtn] = -1;
		g_nTestBtnDirect[nCountBtn] = -1;
		g_Btn[nCountBtn].nCount = 0;
		g_Btn[nCountBtn].nCountReleaseFlag = 0;
		g_Btn[nCountBtn].nTriggerTiming = -1;
		g_Btn[nCountBtn].nReleaseTiming = -1;
		g_Btn[nCountBtn].bStatus = false;
		g_Btn[nCountBtn].nBtnDirection = NFD_UP;
		g_Btn[nCountBtn].fJoyConAngle = 0.0f;
		g_Btn[nCountBtn].fJoyConLength = 0.0f;
		g_Btn[nCountBtn].fJoyConAngleOld = 0.0f;
		g_Btn[nCountBtn].fJoyConLengthOld = 0.0f;
		g_Btn[nCountBtn].dColor = gc_dDefaultBtnColor;
		g_Btn[nCountBtn].vPos =
		{
			BTN_X + nCountBtn * BTN_RANGE_X,
			BTN_Y,
			BTN_Z
		};
    }

	// キーボード
	g_Btn[0].nKeyNum = DIK_S;
	g_Btn[1].nKeyNum = DIK_F;
	g_Btn[2].nKeyNum = DIK_H;
	g_Btn[3].nKeyNum = DIK_K;

	g_Btn[4].nKeyNumSpecial[NFD_UP] = DIK_D;
	g_Btn[4].nKeyNumSpecial[NFD_DOWN] = DIK_X;
	g_Btn[4].nKeyNumSpecial[NFD_LEFT] = DIK_Z;
	g_Btn[4].nKeyNumSpecial[NFD_RIGHT] = DIK_C;

	g_Btn[5].nKeyNumSpecial[NFD_UP] = DIK_J;
	g_Btn[5].nKeyNumSpecial[NFD_DOWN] = DIK_M;
	g_Btn[5].nKeyNumSpecial[NFD_LEFT] = DIK_N;
	g_Btn[5].nKeyNumSpecial[NFD_RIGHT] = DIK_COMMA;

	// コントローラー
	if (!GetTestModeMemberStatus(TMML_XINPUT))
	{
		g_Btn[0].nJoyConKeyNum[JCNBN_00] = JOY_LEFT;
		g_Btn[0].nJoyConKeyNum[JCNBN_01] = JOY_DOWN;
		g_Btn[0].nJoyConKeyNum[JCNBN_02] = JOY_L1;

		g_Btn[1].nJoyConKeyNum[JCNBN_00] = JOY_RIGHT;
		g_Btn[1].nJoyConKeyNum[JCNBN_01] = JOY_UP;
		g_Btn[1].nJoyConKeyNum[JCNBN_02] = JOY_L2;

		g_Btn[2].nJoyConKeyNum[JCNBN_00] = JOY_RECT;
		g_Btn[2].nJoyConKeyNum[JCNBN_01] = JOY_TRI;
		g_Btn[2].nJoyConKeyNum[JCNBN_02] = JOY_R2;

		g_Btn[3].nJoyConKeyNum[JCNBN_00] = JOY_CROSS;
		g_Btn[3].nJoyConKeyNum[JCNBN_01] = JOY_SPHERE;
		g_Btn[3].nJoyConKeyNum[JCNBN_02] = JOY_R1;
	}
	else
	{
		g_Btn[0].nJoyConKeyNum[JCNBN_00] = JOY_LEFT;
		g_Btn[0].nJoyConKeyNum[JCNBN_01] = JOY_DOWN;
		g_Btn[0].nJoyConKeyNum[JCNBN_02] = JOY_L1;

		g_Btn[1].nJoyConKeyNum[JCNBN_00] = JOY_RIGHT;
		g_Btn[1].nJoyConKeyNum[JCNBN_01] = JOY_UP;
		g_Btn[1].nJoyConKeyNum[JCNBN_02] = JOY_SHERE;

		g_Btn[2].nJoyConKeyNum[JCNBN_00] = JOY_SPHERE;
		g_Btn[2].nJoyConKeyNum[JCNBN_01] = JOY_TRI;
		g_Btn[2].nJoyConKeyNum[JCNBN_02] = JOY_OPTION;

		g_Btn[3].nJoyConKeyNum[JCNBN_00] = JOY_RECT;
		g_Btn[3].nJoyConKeyNum[JCNBN_01] = JOY_CROSS;
		g_Btn[3].nJoyConKeyNum[JCNBN_02] = JOY_R1;
	}

	g_Btn[4].nJoyConKeyNum[JCNBN_00] = AS_LEFT;
	g_Btn[5].nJoyConKeyNum[JCNBN_00] = AS_RIGHT;
}

void UninitBtn()
{

}

void UpdateBtnHoldFlag()
{
	int nTime = GetTimer();
	LPDIRECTINPUTDEVICE8 pJoyConDevice = GetJSDev();
	for (int nCountBtn = 0; nCountBtn < NUM_BTN_NORMAL; nCountBtn++)
	{
		if (!pJoyConDevice)
		{
			if (GetKeyboardPress(g_Btn[nCountBtn].nKeyNum))
			{
				g_Btn[nCountBtn].nCountReleaseFlag = 0;
			}
			else
			{
				if (g_Btn[nCountBtn].nCountReleaseFlag < BTN_RELEASE_FLAG_MAX_COUNT)
				{
					g_Btn[nCountBtn].nCountReleaseFlag++;
				}
				if (g_Btn[nCountBtn].nCountReleaseFlag == BTN_RELEASE_FLAG_MAX_COUNT && !g_bAuto)
				{
					g_Btn[nCountBtn].nCountReleaseFlag++;
					g_Btn[nCountBtn].nReleaseTiming = g_nTestBtnReleaseTime[nCountBtn] = nTime;
				}
			}
		}
		else
		{
			bool bPressStatus = false;
			for (int nCountJoyConBtn = 0; nCountJoyConBtn < JCNBN_MAX; nCountJoyConBtn++)
			{
				if (g_Btn[nCountBtn].nJoyConKeyNum[nCountJoyConBtn] < JOY_POS_MIN)
				{
					if (GetJoyStickPress(g_Btn[nCountBtn].nJoyConKeyNum[nCountJoyConBtn]))
					{
						bPressStatus = true;
						g_Btn[nCountBtn].nCountReleaseFlag = 0;
					}
				}
				else
				{
					if (GetPOVPress(g_Btn[nCountBtn].nJoyConKeyNum[nCountJoyConBtn]))
					{
						bPressStatus = true;
						g_Btn[nCountBtn].nCountReleaseFlag = 0;
					}
				}
				if (bPressStatus)
				{
					break;
				}
			}
			if (!bPressStatus)
			{
				if (g_Btn[nCountBtn].nCountReleaseFlag < BTN_RELEASE_FLAG_MAX_COUNT)
				{
					g_Btn[nCountBtn].nCountReleaseFlag++;
				}
				if (g_Btn[nCountBtn].nCountReleaseFlag == BTN_RELEASE_FLAG_MAX_COUNT && !g_bAuto)
				{
					g_Btn[nCountBtn].nCountReleaseFlag++;
					g_Btn[nCountBtn].nReleaseTiming = g_nTestBtnReleaseTime[nCountBtn] = nTime;
				}
			}
		}
		if (g_Btn[nCountBtn].bStatus && (g_bAuto || !pJoyConDevice))
		{
			g_Btn[nCountBtn].nCount++;
			if (g_Btn[nCountBtn].nCount == BTN_COUNT_MAX)
			{
				if (GetKeyboardPress(g_Btn[nCountBtn].nKeyNum) && !g_bAuto)
				{
					g_Btn[nCountBtn].nCount = 0;
				}
				else
				{
					g_Btn[nCountBtn].bStatus = false;
				}
			}
		}
	}
}

void UpdateBtn()
{
	int nTime = GetTimer();
	LPDIRECTINPUTDEVICE8 pJoyConDevice = GetJSDev();

	// ボタンの更新(1-4)
	for (int nCountBtn = 0; nCountBtn < NUM_BTN_NORMAL; nCountBtn++)
	{
		if (!pJoyConDevice)
		{
			if (GetKeyboardTrigger(g_Btn[nCountBtn].nKeyNum))
			{
				g_Btn[nCountBtn].bStatus = true;
				g_Btn[nCountBtn].dColor = gc_dDefaultBtnColor;
				g_Btn[nCountBtn].nCount = 0;
				g_Btn[nCountBtn].nTriggerTiming = g_nTestBtnTriggerTime[nCountBtn] = nTime;
				g_Btn[nCountBtn].nReleaseTiming = g_nTestBtnReleaseTime[nCountBtn] = 0;
			}
		}
		else
		{
			bool bTriggerStatus = false;
			bool bPressStatus = false;
			for (int nCountJoyConBtn = 0; nCountJoyConBtn < JCNBN_MAX; nCountJoyConBtn++)
			{
				if (g_Btn[nCountBtn].nJoyConKeyNum[nCountJoyConBtn] < JOY_POS_MIN)
				{
					if (GetJoyStickTrigger(g_Btn[nCountBtn].nJoyConKeyNum[nCountJoyConBtn]))
					{
						bTriggerStatus = true;
						g_Btn[nCountBtn].bStatus = true;
						g_Btn[nCountBtn].dColor = gc_dDefaultBtnColor;
						g_Btn[nCountBtn].nCount = 0;
						g_Btn[nCountBtn].nTriggerTiming = g_nTestBtnTriggerTime[nCountBtn] = nTime;
						g_Btn[nCountBtn].nReleaseTiming = g_nTestBtnReleaseTime[nCountBtn] = 0;
					}
				}
				else
				{
					if (GetPOVTrigger(g_Btn[nCountBtn].nJoyConKeyNum[nCountJoyConBtn]))
					{
						bTriggerStatus = true;
						g_Btn[nCountBtn].bStatus = true;
						g_Btn[nCountBtn].dColor = gc_dDefaultBtnColor;
						g_Btn[nCountBtn].nCount = 0;
						g_Btn[nCountBtn].nTriggerTiming = g_nTestBtnTriggerTime[nCountBtn] = nTime;
						g_Btn[nCountBtn].nReleaseTiming = g_nTestBtnReleaseTime[nCountBtn] = 0;
					}
				}
				if (bTriggerStatus)
				{
					break;
				}
			}
			if (g_Btn[nCountBtn].bStatus)
			{
				g_Btn[nCountBtn].nCount++;
				if (g_Btn[nCountBtn].nCount == BTN_COUNT_MAX)
				{
					for (int nCountJoyConBtn = 0; nCountJoyConBtn < JCNBN_MAX; nCountJoyConBtn++)
					{
						if (g_Btn[nCountBtn].nJoyConKeyNum[nCountJoyConBtn] < JOY_POS_MIN)
						{
							if (GetJoyStickPress(g_Btn[nCountBtn].nJoyConKeyNum[nCountJoyConBtn]))
							{
								bPressStatus = true;
								g_Btn[nCountBtn].nCount = 0;
							}
						}
						else
						{
							if (GetPOVPress(g_Btn[nCountBtn].nJoyConKeyNum[nCountJoyConBtn]))
							{
								bPressStatus = true;
								g_Btn[nCountBtn].nCount = 0;
							}
						}
						if (bPressStatus)
						{
							break;
						}
					}
					if (!bPressStatus)
					{
						g_Btn[nCountBtn].bStatus = false;
					}
				}
			}
		}
	}

	// ボタンの更新(5-6)
	for (int nCountBtn = NUM_BTN_NORMAL; nCountBtn < NUM_BTN; nCountBtn++)
	{
		if (!pJoyConDevice)
		{
			for (int nCountBtnSpecial = 0; nCountBtnSpecial < NFD_MAX; nCountBtnSpecial++)
			{
				if (GetKeyboardTrigger(g_Btn[nCountBtn].nKeyNumSpecial[nCountBtnSpecial]))
				{
					g_Btn[nCountBtn].nTriggerTiming = g_nTestBtnTriggerTime[nCountBtn] = nTime;
					g_Btn[nCountBtn].nReleaseTiming = g_nTestBtnReleaseTime[nCountBtn] = 0;
					g_Btn[nCountBtn].nBtnDirection = g_nTestBtnDirect[nCountBtn] = nCountBtnSpecial;
					break;
				}
			}

			bool bTrigger = false;
			for (int nCountBtnSpecial = 0; nCountBtnSpecial < NFD_MAX; nCountBtnSpecial++)
			{
				if (GetKeyboardPress(g_Btn[nCountBtn].nKeyNumSpecial[nCountBtnSpecial]))
				{
					bTrigger = true;
					break;
				}
			}
			if (!bTrigger)
			{
				g_Btn[nCountBtn].nBtnDirection = g_nTestBtnDirect[nCountBtn] = -1;
			}
		}
		else
		{
			g_Btn[nCountBtn].fJoyConAngle = GetStickAngle(g_Btn[nCountBtn].nJoyConKeyNum[JCNBN_00]);
			g_Btn[nCountBtn].fJoyConLength = GetStickLength(g_Btn[nCountBtn].nJoyConKeyNum[JCNBN_00]);
			
			if (g_Btn[nCountBtn].fJoyConLength >= STICK_MIN_LENGTH)
			{
				for (int nCountDirect = 0; nCountDirect < NFD_MAX; nCountDirect++)
				{
					switch (nCountDirect)
					{
					case NFD_UP:
					{
						if (g_Btn[nCountBtn].fJoyConAngle >= g_fStickDirectAngle[nCountDirect][0] || g_Btn[nCountBtn].fJoyConAngle < g_fStickDirectAngle[nCountDirect][1])
						{
							g_Btn[nCountBtn].nBtnDirection = g_nTestBtnDirect[nCountBtn] = nCountDirect;
						}
						break;
					}
					case NFD_DOWN:
					case NFD_LEFT:
					case NFD_RIGHT:
					{
						if (g_Btn[nCountBtn].fJoyConAngle >= g_fStickDirectAngle[nCountDirect][0] && g_Btn[nCountBtn].fJoyConAngle < g_fStickDirectAngle[nCountDirect][1])
						{
							g_Btn[nCountBtn].nBtnDirection = g_nTestBtnDirect[nCountBtn] = nCountDirect;
						}
						break;
					}
					}
				}
			}

			if (g_Btn[nCountBtn].fJoyConLength < STICK_MIN_LENGTH)
			{
				g_Btn[nCountBtn].nBtnDirection = g_nTestBtnDirect[nCountBtn] = -1;
				g_Btn[nCountBtn].fJoyConLengthOld = g_Btn[nCountBtn].fJoyConLength = STICK_MIN_LENGTH;
			}
			else
			{
				bool bGetTimeStatus = false;
				if (g_Btn[nCountBtn].fJoyConLength - g_Btn[nCountBtn].fJoyConLengthOld >= STICK_CHANGE_LENGTH_MIN &&
					g_Btn[nCountBtn].fJoyConLengthOld == STICK_MIN_LENGTH)
				{
					bGetTimeStatus = true;
					g_Btn[nCountBtn].fJoyConLengthOld = g_Btn[nCountBtn].fJoyConLength;
				}
				if ((g_Btn[nCountBtn].fJoyConAngle - g_Btn[nCountBtn].fJoyConAngleOld >= STICK_CHANGE_ANGLE_MIN ||
					g_Btn[nCountBtn].fJoyConAngle - g_Btn[nCountBtn].fJoyConAngleOld <= -STICK_CHANGE_ANGLE_MIN)&&
					(g_Btn[nCountBtn].fJoyConLength >= STICK_MIN_LENGTH + STICK_CHANGE_LENGTH_MIN))
				{
					if (g_Btn[nCountBtn].fJoyConAngle < STICK_CHANGE_ANGLE_MIN && g_Btn[nCountBtn].fJoyConAngleOld > MAX_ANGLE - STICK_CHANGE_ANGLE_MIN)
					{
						if (g_Btn[nCountBtn].fJoyConAngle + MAX_ANGLE - g_Btn[nCountBtn].fJoyConAngleOld >= STICK_CHANGE_ANGLE_MIN)
						{
							bGetTimeStatus = true;
							g_Btn[nCountBtn].fJoyConAngleOld = g_Btn[nCountBtn].fJoyConAngle;
						}
					}
					else
						if (g_Btn[nCountBtn].fJoyConAngle > MAX_ANGLE - STICK_CHANGE_ANGLE_MIN && g_Btn[nCountBtn].fJoyConAngleOld < STICK_CHANGE_ANGLE_MIN)
						{
							if (MAX_ANGLE + g_Btn[nCountBtn].fJoyConAngleOld - g_Btn[nCountBtn].fJoyConAngle >= STICK_CHANGE_ANGLE_MIN)
							{
								bGetTimeStatus = true;
								g_Btn[nCountBtn].fJoyConAngleOld = g_Btn[nCountBtn].fJoyConAngle;
							}
						}
						else
						{
							bGetTimeStatus = true;
							g_Btn[nCountBtn].fJoyConAngleOld = g_Btn[nCountBtn].fJoyConAngle;
						}
				}
				if (bGetTimeStatus)
				{
					g_Btn[nCountBtn].nTriggerTiming = g_nTestBtnTriggerTime[nCountBtn] = nTime;
					g_Btn[nCountBtn].nReleaseTiming = g_nTestBtnReleaseTime[nCountBtn] = 0;

				}
			}
		}
	}
}

void UpdateBtnAuto()
{
	int nTime = GetTimer();

	// ボタンの更新(1-4)
	for (int nCountBtn = 0; nCountBtn < NUM_BTN_NORMAL; nCountBtn++)
	{
		NOTE Note = GetNote(nCountBtn, GetJudgeNoteNum(nCountBtn));
		if (nTime >= Note.nTiming[NJS_START] && !Note.bJudgeStatus[NJS_START] && GetJudgeNoteNum(nCountBtn) < GetNoteMaxNum(nCountBtn))
		{
			g_Btn[nCountBtn].bStatus = true;
			g_Btn[nCountBtn].dColor = gc_dDefaultBtnColor;
			g_Btn[nCountBtn].nCount = 0;
			g_Btn[nCountBtn].nTriggerTiming = g_nTestBtnTriggerTime[nCountBtn] = nTime;
			g_Btn[nCountBtn].nReleaseTiming = g_nTestBtnReleaseTime[nCountBtn] = 0;
		}
	}

	// ボタンの更新(5-6)
	for (int nCountBtn = NUM_BTN_NORMAL; nCountBtn < NUM_BTN; nCountBtn++)
	{
		NOTE Note = GetNote(nCountBtn, GetJudgeNoteNum(nCountBtn));

		switch (Note.nType)
		{
		case NT_FLICK:
		{
			if (nTime >= Note.nTiming[NJS_START] && !Note.bJudgeStatus[NJS_START] && GetJudgeNoteNum(nCountBtn) < GetNoteMaxNum(nCountBtn))
			{
				g_Btn[nCountBtn].nTriggerTiming = g_nTestBtnTriggerTime[nCountBtn] = nTime;
				g_Btn[nCountBtn].nReleaseTiming = g_nTestBtnReleaseTime[nCountBtn] = 0;
				g_Btn[nCountBtn].nBtnDirection = g_nTestBtnDirect[nCountBtn] = Note.nFlickDirection;
			}
			break;
		}
		case NT_SLIDE:
		{
			if (nTime >= Note.nTiming[NJS_START] && !Note.bJudgeStatus[NJS_START] && GetJudgeNoteNum(nCountBtn) < GetNoteMaxNum(nCountBtn))
			{
				g_Btn[nCountBtn].nTriggerTiming = g_nTestBtnTriggerTime[nCountBtn] = nTime;
				g_Btn[nCountBtn].nReleaseTiming = g_nTestBtnReleaseTime[nCountBtn] = 0;
			}
			if (nTime >= Note.nTiming[NJS_START] && GetJudgeNoteNum(nCountBtn) < GetNoteMaxNum(nCountBtn))
			{
				g_Btn[nCountBtn].nBtnDirection = g_nTestBtnDirect[nCountBtn] = Note.nFlickDirection;
			}
			break;
		}
		}
	}
}

void DrawBtn()
{
	SetBlend(D3DBLEND_BLENDFACTOR);
	for (int nCountBtn = 0; nCountBtn < NUM_BTN_NORMAL; nCountBtn++)
	{
		DebugText(0, 44 + 22 * nCountBtn, "Lane %d Time: %d, %d", nCountBtn + 1, g_nTestBtnTriggerTime[nCountBtn], g_nTestBtnReleaseTime[nCountBtn]);
		SetLight(FALSE);
		SetTexture(TEX_BTN_LIGHT);
		if (g_Btn[nCountBtn].bStatus)
		{
			SetPolygonColor3D(g_Btn[nCountBtn].dColor);
			DrawPolygon3D
			(
				g_Btn[nCountBtn].vPos.x,
				g_Btn[nCountBtn].vPos.y,
				g_Btn[nCountBtn].vPos.z,
				0,
				0,
				GetTextureStatus(TEX_BTN_LIGHT).u,
				GetTextureStatus(TEX_BTN_LIGHT).v
			);
		}
	}
	SetBlend(D3DBLEND_INVSRCALPHA);
	for (int nCountBtn = NUM_BTN_NORMAL; nCountBtn < NUM_BTN; nCountBtn++)
	{
		DebugText(0, 44 + 22 * nCountBtn, "Lane %d Time: %d, %d Direct: %d", nCountBtn + 1, g_nTestBtnTriggerTime[nCountBtn], g_nTestBtnReleaseTime[nCountBtn], g_nTestBtnDirect[nCountBtn]);
	}
}

BTN GetBtn(int nNum)
{
	return g_Btn[nNum];
}

void SetBtnTriggerTime(int nNum, int nTime)
{
	g_Btn[nNum].nTriggerTiming = nTime;
}

void SetBtnReleaseTime(int nNum, int nTime)
{
	g_Btn[nNum].nReleaseTiming = nTime;
}

void SetBtnDirect(int nNum, int nDirect)
{
	g_Btn[nNum].nBtnDirection = nDirect;
}

void SetAutoPlay(bool bStatus)
{
	g_bAuto = bStatus;
}

bool GetAutoPlay()
{
	return g_bAuto;
}

void SetBtnColor(int nBtn, int nJudgeLevel)
{
	g_Btn[nBtn].dColor = gc_dBtnColorList[nJudgeLevel];
}