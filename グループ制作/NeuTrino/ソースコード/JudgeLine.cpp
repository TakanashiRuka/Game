//----------------------------------------------------------------------------
//
//	判定ライン
//
//														 制作者：劉喆
//                                                       作成日：2017/11/30
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "JudgeLine.h"
#include "Texture.h"
#include "Polygon3D.h"
#include "Light.h"
#include "Note.h"
#include "Btn.h"

//---------------------------マクロ定義---------------------------------------
#define JUDGE_LINE_X					(0.0f)
#define JUDGE_LINE_Y					(0.0002f)
#define JUDGE_LINE_SP_A_MIN				(0)
#define JUDGE_LINE_SP_A_MAX				(127)
#define JUDGE_LINE_SP_LOOP_TIME			(3)

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR3 vPos;		// 座標
}JUDGE_LINE;				// 判定ライン

//---------------------------グローバル変数-----------------------------------
static JUDGE_LINE g_JudgeLine;
static int g_nJudgeLineSpA;
static int g_nJudgeLineSpCount;

//---------------------------プロトタイプ宣言---------------------------------
void InitJudgeLine()
{
	g_JudgeLine.vPos = { JUDGE_LINE_X,JUDGE_LINE_Y,JUDGE_LINE_Z };
	g_nJudgeLineSpA = JUDGE_LINE_SP_A_MIN;
	g_nJudgeLineSpCount = 0;
}

void UninitJudgeLine()
{

}

void UpdateJudgeLine()
{

}

void DrawJudgeLine()
{
	SetLight(FALSE);
	SetTexture(TEX_JUDGE_LINE);
	SetPolygonColor3D(D3DCOLOR_ARGB(255, 255, 255, 255));
	DrawPolygon3D
	(
		g_JudgeLine.vPos.x,
		g_JudgeLine.vPos.y,
		g_JudgeLine.vPos.z,
		0,
		0,
		GetTextureStatus(TEX_JUDGE_LINE).u,
		GetTextureStatus(TEX_JUDGE_LINE).v
	);
}

void UpdateJudgeLineSpecial()
{
	g_nJudgeLineSpCount++;
	if (g_nJudgeLineSpCount < JUDGE_LINE_SP_LOOP_TIME)
	{
		g_nJudgeLineSpA += (float)(JUDGE_LINE_SP_A_MAX - JUDGE_LINE_SP_A_MIN) / JUDGE_LINE_SP_LOOP_TIME;
	}
	if (g_nJudgeLineSpCount == JUDGE_LINE_SP_LOOP_TIME)
	{
		g_nJudgeLineSpA = JUDGE_LINE_SP_A_MAX;
	}
	if (g_nJudgeLineSpCount > JUDGE_LINE_SP_LOOP_TIME && g_nJudgeLineSpCount <= JUDGE_LINE_SP_LOOP_TIME * 2)
	{																									
		g_nJudgeLineSpA += (float)(-JUDGE_LINE_SP_A_MAX + JUDGE_LINE_SP_A_MIN) / JUDGE_LINE_SP_LOOP_TIME;
	}
	if (g_nJudgeLineSpCount == JUDGE_LINE_SP_LOOP_TIME * 2)
	{
		g_nJudgeLineSpA = JUDGE_LINE_SP_A_MIN;
		g_nJudgeLineSpCount = 0;
	}

}

void DrawJudgeLineSpecial()
{
	SetLight(FALSE);
	SetTexture(TEX_JUDGE_LINE_SP);
	for (int nCountJudgeLine = NSP_MAX; nCountJudgeLine < NUM_BTN; nCountJudgeLine++)
	{
		float fY = GetSlidePos(nCountJudgeLine).y;

		if (fY != -1.0f)
		{
			if (fY > NOTE_SLIDE_Y_MAX)
			{
				fY = NOTE_SLIDE_Y_MAX;
			}
			SetPolygonColor3D(D3DCOLOR_ARGB(255, 255, 255, 255));
			DrawPolygon3D
			(
				g_JudgeLine.vPos.x - NOTE_RANGE_X + (nCountJudgeLine - NSP_MAX) * NOTE_RANGE_X * 2.0f,
				fY / N,
				g_JudgeLine.vPos.z,
				0,
				0,
				GetTextureStatus(TEX_JUDGE_LINE).u,
				GetTextureStatus(TEX_JUDGE_LINE).v
			);
			SetBlend(D3DBLEND_BLENDFACTOR);
			SetPolygonColor3D(D3DCOLOR_ARGB(g_nJudgeLineSpA, 255, 255, 255));
			DrawPolygon3D
			(
				g_JudgeLine.vPos.x - NOTE_RANGE_X + (nCountJudgeLine - NSP_MAX) * NOTE_RANGE_X * 2.0f,
				fY / N,
				g_JudgeLine.vPos.z,
				0,
				0,
				GetTextureStatus(TEX_JUDGE_LINE).u,
				GetTextureStatus(TEX_JUDGE_LINE).v
			);
			SetBlend(D3DBLEND_INVSRCALPHA);
		}
	}
}