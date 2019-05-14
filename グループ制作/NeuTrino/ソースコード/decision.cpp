//----------------------------------------------------------------------------
//
//	判定文字
//
//														 制作者：高梨　琉佳
//                                                       作成日：2017/12/22
//----------------------------------------------------------------------------

#include "decision.h"
#include"main.h"
#include"BillBoard.h"
#include"Texture.h"
#include"Note.h"
#include"JudgeLine.h"
#include"RenderState.h"
#include"Btn.h"

#define DECISION_MAX		(6)
#define DECISION_POS_Y		(0.0f)	//元の位置
#define DECISION_POS_Z		(JUDGE_LINE_Z + 100.0f)
#define DECISION_POS_UP_Y	(0.5f)	//元の位置からどれくらい上に行くか
#define DECISION_POS_UP_Z	(30.0f)	//Z
#define DECISION_COLOR_ALFA	(1.0f)	//0.0f～1.0fに上げるため
#define DECISION_EFECT		(6)	//何フレームで効果を表すか
#define DECISION_DESTROY	(30)	//30フレームで消える

struct Decision
{
	D3DXVECTOR3 pos;
	D3DXCOLOR color;
	int tcX;
	int tcY;
	int tcW;
	int tcH;
	int time;
	bool use;
	bool bEarlyLate;
	int nV;
};

static Decision decision[DECISION_MAX];
static TEXTURE g_tEarlyLate;

void InitDecision()
{
	g_tEarlyLate = GetTextureStatus(TEX_GAME_JUDGE_EARLY_LATE);
	for (int nCnt = 0; nCnt < DECISION_MAX; nCnt++)
	{
		decision[nCnt].time = 0;
		decision[nCnt].use = false;
		decision[nCnt].bEarlyLate = false;
		decision[nCnt].nV = 0;
	}
}

void UninitDecision()
{
	for (int nCnt = 0; nCnt < DECISION_MAX; nCnt++)
	{
		decision[nCnt].time = 0;
		decision[nCnt].use = false;
	}
}

void UpdateDecision()
{
	for (int nCnt = 0; nCnt < DECISION_MAX; nCnt++)
	{
		if (decision[nCnt].use == true)
		{
			if (decision[nCnt].time < DECISION_EFECT)
			{
				decision[nCnt].pos.y += DECISION_POS_UP_Y / DECISION_EFECT;
				decision[nCnt].pos.z += DECISION_POS_UP_Z / DECISION_EFECT;
				decision[nCnt].color.a += DECISION_COLOR_ALFA / DECISION_EFECT;
			}
			decision[nCnt].time++;
			if (decision[nCnt].time > DECISION_DESTROY)
			{
				decision[nCnt].time = 0;
				decision[nCnt].use = false;
				if (decision[nCnt].bEarlyLate)
				{
					decision[nCnt].bEarlyLate = false;
				}
			}
		}
	}
}

void DrawDecision()
{
	SetZBuffer(FALSE);
	SetBillboardScale(1.0f, 1.0f, 1.0f);
	for (int nCnt = 0; nCnt < DECISION_MAX; nCnt++)
	{
		if (decision[nCnt].use == true)
		{
			SetTexture(TEX_JUDGE_WORD);
			SetBillBoardColor(decision[nCnt].color);
			DrawBillboard(decision[nCnt].pos.x, decision[nCnt].pos.y, decision[nCnt].pos.z,
				decision[nCnt].tcX, decision[nCnt].tcY, decision[nCnt].tcW, decision[nCnt].tcH);  
			if (decision[nCnt].bEarlyLate)
			{
				SetTexture(TEX_GAME_JUDGE_EARLY_LATE);
				SetBillBoardColor(decision[nCnt].color);
				SetBillboardScale(0.75f, 0.75f, 1.0f);
				DrawBillboard(decision[nCnt].pos.x, decision[nCnt].pos.y - DECISION_POS_UP_Y * 0.25f, decision[nCnt].pos.z - DECISION_POS_UP_Z * 0.25f,
					0, g_tEarlyLate.v * decision[nCnt].nV, g_tEarlyLate.u, g_tEarlyLate.v);
			}
		}
	}
	SetZBuffer(TRUE);
}

void CreateDecision(int lane, int judgeLevel)
{
	TEXTURE texture = GetTextureStatus(TEX_JUDGE_WORD);
	switch (lane)
	{
	case 0:
		decision[lane].pos = D3DXVECTOR3(NOTE_X + (float)lane * NOTE_RANGE_X, DECISION_POS_Y, DECISION_POS_Z);
		break;
	case 1:
		decision[lane].pos = D3DXVECTOR3(NOTE_X + (float)lane * NOTE_RANGE_X, DECISION_POS_Y, DECISION_POS_Z);
		break;
	case 2:
		decision[lane].pos = D3DXVECTOR3(NOTE_X + (float)lane * NOTE_RANGE_X, DECISION_POS_Y, DECISION_POS_Z);
		break;
	case 3:
		decision[lane].pos = D3DXVECTOR3(NOTE_X + (float)lane * NOTE_RANGE_X, DECISION_POS_Y, DECISION_POS_Z);
		break;
	case 4:
		decision[lane].pos = D3DXVECTOR3(NOTE_FLICK_X + ((float)lane - NUM_BTN_NORMAL) * NOTE_RANGE_X * 2.0f, DECISION_POS_Y, DECISION_POS_Z);
		break;
	case 5:
		decision[lane].pos = D3DXVECTOR3(NOTE_FLICK_X + ((float)lane - NUM_BTN_NORMAL) * NOTE_RANGE_X * 2.0f, DECISION_POS_Y, DECISION_POS_Z);
		break;
	}

	decision[lane].tcX = 0;
	decision[lane].time = 0;
	switch (judgeLevel)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		decision[lane].tcY = texture.v * judgeLevel;
		break;
	}
	decision[lane].tcW = texture.u;
	decision[lane].tcH = texture.v;
	decision[lane].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	decision[lane].use = true;
	decision[lane].bEarlyLate = false;
}

void CreateDecisionEarlyLate(int lane, int nType)
{
	decision[lane].bEarlyLate = true;
	decision[lane].nV = nType;
}