//----------------------------------------------------------------------------
//
//	判定エフェクト
//
//														 制作者：劉喆
//                                                       作成日：2017/12/29
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Texture.h"
#include "Polygon3D.h"
#include "Light.h"
#include "JudgeEffect.h"
#include "Note.h"
#include "Judge.h"
#include "Btn.h"
#include "JudgeLine.h"
#include "BillBoard.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	JEP_INIT,
	JEP_MIDDLE,
	JEP_END,
	JEP_MAX
}JUDGE_EFFECT_PARTS;
//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR3 vPos;			// 座標
	int nNoteType;				// 音符種類
	int nJudgeLevel;			// 判定レベル
	float fScale;				// 大きさ
	float fAngle;				// 角度
	int nCount;					// カウンター
	int nA;						// α値
	int nV;						// Vの値
	bool bStatus;				// 描画状態
}JUDGE_EFFECT_UNDER;			// 判定エフェクト(下)

//---------------------------グローバル変数-----------------------------------
static JUDGE_EFFECT_UNDER g_JudgeEffect[NUM_BTN];
static JUDGE_EFFECT_UNDER g_JudgeEffectUp[NUM_BTN];

static const int gc_nMaxA = 255;
static const int gc_nMinA = 0;

//---------------------------------------------------------------
// 下部分
//---------------------------------------------------------------
static const int gc_nJudgeEffectUnderDrawCountMax = 2;

static const float gc_fJudgeEffectScale[NT_MAX][JEP_MAX] =				// 音符種類ごとのサイズ
{				
	{ 0.2f,1.0f,1.2f },
	{ 0.2f,1.5f,2.0f },
	{ 1.2f,1.2f,1.2f },
	{ 0.2f,1.0f,1.2f },
	{ 1.0f,1.2f,2.0f },
};

static const int gc_nJudgeEffectScaleMaxCount[NT_MAX][JEP_MAX]=			// 音符種類ごとのサイズのカウンター
{
	{ 8,30,30 },
	{ 8,30,30 },
	{ 8,30,30 },
	{ 8,30,30 },
	{ 8,30,30 },
};

static const int gc_nJudgeEffectA[NT_MAX][JEP_MAX]=						// 音符種類ごとのα値
{
	{ 63,255,0 },
	{ 63,255,0 },
	{ 63,255,0 },
	{ 63,255,0 },
	{ 127,255,0 },
};

static const int gc_nJudgeEffectAMaxCount[NT_MAX][JEP_MAX] =			// 音符種類ごとのα値のカウンター
{
	{ 10,30,30 },
	{ 10,30,30 },
	{ 10,30,30 },
	{ 10,30,30 },
	{ 10,30,30 },
};

static const int gc_nJudgeEffectMaxCount[NT_MAX]=						// 音符種類ごとカウンターの最大値
{
	30,
	30,
	30,
	30,
	30,
};

static const float gc_vJudgeEffectInitPosX[NUM_BTN] =
{
	NOTE_X + 0 * NOTE_RANGE_X,
	NOTE_X + 1 * NOTE_RANGE_X,
	NOTE_X + 2 * NOTE_RANGE_X,
	NOTE_X + 3 * NOTE_RANGE_X,
	NOTE_FLICK_X + (4 - NUM_BTN_NORMAL) * NOTE_RANGE_X * 2.0f,
	NOTE_FLICK_X + (5 - NUM_BTN_NORMAL) * NOTE_RANGE_X * 2.0f,
};

static const float gc_fJudgeEffectRotateSpeed = 0.10f;

//---------------------------------------------------------------
// 上部分
//---------------------------------------------------------------
static const float gc_fJudgeEffectUpScale[NT_MAX][JEP_MAX] =				// 音符種類ごとのサイズ
{
	{ 0.5f,1.0f,2.0f },
	{ 0.8f,1.5f,3.0f },
	{ 1.0f,1.2f,2.0f },
	{ 0.5f,1.5f,2.0f },
	{ 0.75f,0.9f,1.0f },
};

static const int gc_nJudgeEffectUpScaleMaxCount[NT_MAX][JEP_MAX] =			// 音符種類ごとのサイズのカウンター
{
	{ 4,30,30 },
	{ 4,30,30 },
	{ 8,30,30 },
	{ 4,16,30 },
	{ 4,10,16 },
};

static const int gc_nJudgeEffectAUp[NT_MAX][JEP_MAX] =					// 音符種類ごとのα値
{
	{ 0,255,0 },
	{ 0,255,0 },
	{ 127,255,0 },
	{ 63,100,0 },
	{ 0,63,0 },
};

static const int gc_nJudgeEffectAUpMaxCount[NT_MAX][JEP_MAX] =			// 音符種類ごとのα値のカウンター
{
	{ 8,16,30 },
	{ 8,16,30 },
	{ 8,16,30 },
	{ 8,16,30 },
	{ 4,10,16 },
};

static const int gc_nJudgeEffectUpMaxCount[NT_MAX] =					// 音符種類ごとカウンターの最大値
{
	16,
	16,
	16,
	16,
	16,
};

static const float gc_fJudgeEffectUpInitY = GetTextureStatus(TEX_GAME_JUDGE_TAP_EFFECT_UP).v / 2.0f / N;

static const float gc_fJudgeEffectUpRotateSpeed = 0.10f;

//---------------------------プロトタイプ宣言---------------------------------
void InitJudgeEffect()
{
	// 下
	for (int nCountJudgeEffect = 0; nCountJudgeEffect < NUM_BTN; nCountJudgeEffect++)
	{
		g_JudgeEffect[nCountJudgeEffect].fAngle = 0.0f;
		g_JudgeEffect[nCountJudgeEffect].fScale = 0.0f;
		g_JudgeEffect[nCountJudgeEffect].nA = 0;
		g_JudgeEffect[nCountJudgeEffect].nCount = 0;
		g_JudgeEffect[nCountJudgeEffect].nJudgeLevel = 0;
		g_JudgeEffect[nCountJudgeEffect].nNoteType = 0;
		g_JudgeEffect[nCountJudgeEffect].nV = 0;
		g_JudgeEffect[nCountJudgeEffect].vPos = { 0.0f,0.0f,0.0f };
		g_JudgeEffect[nCountJudgeEffect].bStatus = false;
	}

	// 上
	for (int nCountJudgeEffect = 0; nCountJudgeEffect < NUM_BTN; nCountJudgeEffect++)
	{
		g_JudgeEffect[nCountJudgeEffect].fAngle = 0.0f;
		g_JudgeEffect[nCountJudgeEffect].fScale = 0.0f;
		g_JudgeEffectUp[nCountJudgeEffect].nA = 0;
		g_JudgeEffectUp[nCountJudgeEffect].nCount = 0;
		g_JudgeEffectUp[nCountJudgeEffect].nJudgeLevel = 0;
		g_JudgeEffectUp[nCountJudgeEffect].nNoteType = 0;
		g_JudgeEffectUp[nCountJudgeEffect].nV = 0;
		g_JudgeEffectUp[nCountJudgeEffect].vPos = { 0.0f,0.0f,0.0f };
		g_JudgeEffectUp[nCountJudgeEffect].bStatus = false;
	}
}

void UninitJudgeEffect()
{

}

void UpdateJudgeEffect()
{
	// 下
	for (int nCountJudgeEffect = 0; nCountJudgeEffect < NUM_BTN; nCountJudgeEffect++)
	{
		if (g_JudgeEffect[nCountJudgeEffect].bStatus)
		{
			switch (g_JudgeEffect[nCountJudgeEffect].nNoteType)
			{
			case NT_TAP:
			case NT_BREAK:
			case NT_HOLD:
			case NT_FLICK:
			{
				g_JudgeEffect[nCountJudgeEffect].nCount++;
				
				// 角度
				g_JudgeEffect[nCountJudgeEffect].fAngle += gc_fJudgeEffectRotateSpeed;
				
				// サイズ
				if (g_JudgeEffect[nCountJudgeEffect].nCount <= gc_nJudgeEffectScaleMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT])
				{
					g_JudgeEffect[nCountJudgeEffect].fScale += (gc_fJudgeEffectScale[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE] -
						gc_fJudgeEffectScale[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT]) /
						(gc_nJudgeEffectScaleMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT] * 1.0f);
				}
				else
					if (g_JudgeEffect[nCountJudgeEffect].nCount <= gc_nJudgeEffectScaleMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE])
					{
						g_JudgeEffect[nCountJudgeEffect].fScale += (gc_fJudgeEffectScale[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_END] -
							gc_fJudgeEffectScale[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE]) /
							(gc_nJudgeEffectScaleMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE] * 1.0f);
					}

				// α値
				if (g_JudgeEffect[nCountJudgeEffect].nCount <= gc_nJudgeEffectAUpMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT])
				{
					g_JudgeEffect[nCountJudgeEffect].nA += (gc_nJudgeEffectAUp[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE] -
						gc_nJudgeEffectAUp[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT]) * 1.0f /
						(gc_nJudgeEffectAUpMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT] * 1.0f);
				}
				else
					if (g_JudgeEffect[nCountJudgeEffect].nCount <= gc_nJudgeEffectAMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE])
					{
						g_JudgeEffect[nCountJudgeEffect].nA += (gc_nJudgeEffectA[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_END] -
							gc_nJudgeEffectA[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE]) * 1.0f /
							((gc_nJudgeEffectAMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE]
								- gc_nJudgeEffectAMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT]) * 1.0f);
					}
				if (g_JudgeEffect[nCountJudgeEffect].nA > gc_nMaxA)
				{
					g_JudgeEffect[nCountJudgeEffect].nA = gc_nMaxA;
				}
				if (g_JudgeEffect[nCountJudgeEffect].nA < gc_nMinA)
				{
					g_JudgeEffect[nCountJudgeEffect].nA = gc_nMinA;
				}

				if (g_JudgeEffect[nCountJudgeEffect].nCount == gc_nJudgeEffectUpMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType])
				{
					g_JudgeEffect[nCountJudgeEffect].bStatus = false;
				}
				break;
			}
			case NT_SLIDE:
			{
				g_JudgeEffect[nCountJudgeEffect].vPos = GetSlidePos(nCountJudgeEffect);
				g_JudgeEffect[nCountJudgeEffect].vPos.y /= N;
				g_JudgeEffect[nCountJudgeEffect].vPos.z = JUDGE_LINE_Z;

				g_JudgeEffect[nCountJudgeEffect].nCount++;

				// 角度
				//g_JudgeEffect[nCountJudgeEffect].fAngle += gc_fJudgeEffectRotateSpeed;

				// サイズ
				if (g_JudgeEffect[nCountJudgeEffect].nCount <= gc_nJudgeEffectScaleMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT])
				{
					g_JudgeEffect[nCountJudgeEffect].fScale += (gc_fJudgeEffectScale[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE] -
						gc_fJudgeEffectScale[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT]) /
						(gc_nJudgeEffectScaleMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT] * 1.0f);
				}
				else
					if (g_JudgeEffect[nCountJudgeEffect].nCount <= gc_nJudgeEffectScaleMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE])
					{
						g_JudgeEffect[nCountJudgeEffect].fScale += (gc_fJudgeEffectScale[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_END] -
							gc_fJudgeEffectScale[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE]) /
							(gc_nJudgeEffectScaleMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE] * 1.0f);
					}

				// α値
				if (g_JudgeEffect[nCountJudgeEffect].nCount <= gc_nJudgeEffectAUpMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT])
				{
					g_JudgeEffect[nCountJudgeEffect].nA += (gc_nJudgeEffectAUp[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE] -
						gc_nJudgeEffectAUp[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT]) * 1.0f /
						(gc_nJudgeEffectAUpMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT] * 1.0f);
				}
				else
					if (g_JudgeEffect[nCountJudgeEffect].nCount <= gc_nJudgeEffectAMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE])
					{
						g_JudgeEffect[nCountJudgeEffect].nA += (gc_nJudgeEffectA[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_END] -
							gc_nJudgeEffectA[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE]) * 1.0f /
							((gc_nJudgeEffectAMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_MIDDLE]
								- gc_nJudgeEffectAMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType][JEP_INIT]) * 1.0f);
					}
				if (g_JudgeEffect[nCountJudgeEffect].nA > gc_nMaxA)
				{
					g_JudgeEffect[nCountJudgeEffect].nA = gc_nMaxA;
				}
				if (g_JudgeEffect[nCountJudgeEffect].nA < gc_nMinA)
				{
					g_JudgeEffect[nCountJudgeEffect].nA = gc_nMinA;
				}

				if (g_JudgeEffect[nCountJudgeEffect].nCount == gc_nJudgeEffectUpMaxCount[g_JudgeEffect[nCountJudgeEffect].nNoteType])
				{
					g_JudgeEffect[nCountJudgeEffect].bStatus = false;
				}
				break;
			}
			}
		}
	}

	// 上
	for (int nCountJudgeEffect = 0; nCountJudgeEffect < NUM_BTN; nCountJudgeEffect++)
	{
		if (g_JudgeEffectUp[nCountJudgeEffect].bStatus)
		{
			switch (g_JudgeEffectUp[nCountJudgeEffect].nNoteType)
			{
			case NT_TAP:
			case NT_BREAK:
			case NT_HOLD:
			{
				g_JudgeEffectUp[nCountJudgeEffect].nCount++;

				// 角度
				//g_JudgeEffectUp[nCountJudgeEffect].fAngle += gc_fJudgeEffectUpRotateSpeed;

				// サイズ
				if (g_JudgeEffectUp[nCountJudgeEffect].nCount <= gc_nJudgeEffectUpScaleMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT])
				{
					g_JudgeEffectUp[nCountJudgeEffect].fScale += (gc_fJudgeEffectUpScale[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE] -
						gc_fJudgeEffectUpScale[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT]) /
						(gc_nJudgeEffectUpScaleMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT] * 1.0f);
				}
				else
					if (g_JudgeEffectUp[nCountJudgeEffect].nCount <= gc_nJudgeEffectUpScaleMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE])
					{
						g_JudgeEffectUp[nCountJudgeEffect].fScale += (gc_fJudgeEffectUpScale[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_END] -
							gc_fJudgeEffectUpScale[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE]) /
							(gc_nJudgeEffectUpScaleMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE] * 1.0f);
					}

				// α値
				if (g_JudgeEffectUp[nCountJudgeEffect].nCount <= gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT])
				{
					g_JudgeEffectUp[nCountJudgeEffect].nA += (gc_nJudgeEffectAUp[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE] -
						gc_nJudgeEffectAUp[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT]) * 1.0f /
						(gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT] * 1.0f);
				}
				else
					if (g_JudgeEffectUp[nCountJudgeEffect].nCount <= gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE])
					{
						g_JudgeEffectUp[nCountJudgeEffect].nA += (gc_nJudgeEffectAUp[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_END] -
							gc_nJudgeEffectAUp[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE]) * 1.0f /
							((gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE] - 
								gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT]) * 1.0f);
					}
				if (g_JudgeEffectUp[nCountJudgeEffect].nA > gc_nMaxA)
				{
					g_JudgeEffectUp[nCountJudgeEffect].nA = gc_nMaxA;
				}
				if (g_JudgeEffectUp[nCountJudgeEffect].nA < gc_nMinA)
				{
					g_JudgeEffectUp[nCountJudgeEffect].nA = gc_nMinA;
				}

				if (g_JudgeEffectUp[nCountJudgeEffect].nCount == gc_nJudgeEffectMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType])
				{
					g_JudgeEffectUp[nCountJudgeEffect].bStatus = false;
				}
				break;
			}
			//case NT_HOLD:
			//{
			//	break;
			//}
			case NT_FLICK:
			{
				g_JudgeEffectUp[nCountJudgeEffect].nCount++;

				// 角度
				//g_JudgeEffectUp[nCountJudgeEffect].fAngle += gc_fJudgeEffectUpRotateSpeed;

				// サイズ
				if (g_JudgeEffectUp[nCountJudgeEffect].nCount <= gc_nJudgeEffectUpScaleMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT])
				{
					g_JudgeEffectUp[nCountJudgeEffect].fScale += (gc_fJudgeEffectUpScale[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE] -
						gc_fJudgeEffectUpScale[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT]) /
						(gc_nJudgeEffectUpScaleMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT] * 1.0f);
				}
				else
					if (g_JudgeEffectUp[nCountJudgeEffect].nCount <= gc_nJudgeEffectUpScaleMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE])
					{
						g_JudgeEffectUp[nCountJudgeEffect].fScale += (gc_fJudgeEffectUpScale[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_END] -
							gc_fJudgeEffectUpScale[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE]) /
							(gc_nJudgeEffectUpScaleMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE] * 1.0f);
					}

				// α値
				if (g_JudgeEffectUp[nCountJudgeEffect].nCount <= gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT])
				{
					g_JudgeEffectUp[nCountJudgeEffect].nA += (gc_nJudgeEffectAUp[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE] -
						gc_nJudgeEffectAUp[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT]) * 1.0f /
						(gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT] * 1.0f);
				}
				else
					if (g_JudgeEffectUp[nCountJudgeEffect].nCount <= gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE])
					{
						g_JudgeEffectUp[nCountJudgeEffect].nA += (gc_nJudgeEffectAUp[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_END] -
							gc_nJudgeEffectAUp[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE]) * 1.0f /
							((gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE] -
								gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT]) * 1.0f);
					}
				if (g_JudgeEffectUp[nCountJudgeEffect].nA > gc_nMaxA)
				{
					g_JudgeEffectUp[nCountJudgeEffect].nA = gc_nMaxA;
				}
				if (g_JudgeEffectUp[nCountJudgeEffect].nA < gc_nMinA)
				{
					g_JudgeEffectUp[nCountJudgeEffect].nA = gc_nMinA;
				}

				if (g_JudgeEffectUp[nCountJudgeEffect].nCount == gc_nJudgeEffectMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType])
				{
					g_JudgeEffectUp[nCountJudgeEffect].bStatus = false;
				}
				break;
			}
			case NT_SLIDE:
			{
				g_JudgeEffectUp[nCountJudgeEffect].nCount++;

				// 角度
				g_JudgeEffectUp[nCountJudgeEffect].fAngle += gc_fJudgeEffectUpRotateSpeed;

				// サイズ
				if (g_JudgeEffectUp[nCountJudgeEffect].nCount <= gc_nJudgeEffectUpScaleMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT])
				{
					g_JudgeEffectUp[nCountJudgeEffect].fScale += (gc_fJudgeEffectUpScale[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE] -
						gc_fJudgeEffectUpScale[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT]) /
						(gc_nJudgeEffectUpScaleMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT] * 1.0f);
				}
				else
					if (g_JudgeEffectUp[nCountJudgeEffect].nCount <= gc_nJudgeEffectUpScaleMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE])
					{
						g_JudgeEffectUp[nCountJudgeEffect].fScale += (gc_fJudgeEffectUpScale[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_END] -
							gc_fJudgeEffectUpScale[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE]) /
							(gc_nJudgeEffectUpScaleMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE] * 1.0f);
					}

				// α値
				if (g_JudgeEffectUp[nCountJudgeEffect].nCount <= gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT])
				{
					g_JudgeEffectUp[nCountJudgeEffect].nA += (gc_nJudgeEffectAUp[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE] -
						gc_nJudgeEffectAUp[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT]) * 1.0f /
						(gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT] * 1.0f);
				}
				else
					if (g_JudgeEffectUp[nCountJudgeEffect].nCount <= gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE])
					{
						g_JudgeEffectUp[nCountJudgeEffect].nA += (gc_nJudgeEffectAUp[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_END] -
							gc_nJudgeEffectAUp[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE]) * 1.0f /
							((gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_MIDDLE]
								- gc_nJudgeEffectAUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType][JEP_INIT]) * 1.0f);
					}
				if (g_JudgeEffectUp[nCountJudgeEffect].nA > gc_nMaxA)
				{
					g_JudgeEffectUp[nCountJudgeEffect].nA = gc_nMaxA;
				}
				if (g_JudgeEffectUp[nCountJudgeEffect].nA < gc_nMinA)
				{
					g_JudgeEffectUp[nCountJudgeEffect].nA = gc_nMinA;
				}

				if (g_JudgeEffectUp[nCountJudgeEffect].nCount == gc_nJudgeEffectUpMaxCount[g_JudgeEffectUp[nCountJudgeEffect].nNoteType])
				{
					g_JudgeEffectUp[nCountJudgeEffect].bStatus = false;
				}
				break;
			}
			}
		}
	}
}

void DrawJudgeEffect()
{
	SetBlend(D3DBLEND_BLENDFACTOR);
	SetZBuffer(FALSE);

	// 下
	for (int nCountJudgeEffect = 0; nCountJudgeEffect < NUM_BTN; nCountJudgeEffect++)
	{
		if (g_JudgeEffect[nCountJudgeEffect].bStatus)
		{
			switch (g_JudgeEffect[nCountJudgeEffect].nNoteType)
			{
			case NT_TAP:
			case NT_BREAK:
			case NT_HOLD:
			case NT_FLICK:
			{
				TEXTURE tJudgeTapEffectUnder = GetTextureStatus(TEX_GAME_JUDGE_TAP_EFFECT_UNDER);
				SetTexture(TEX_GAME_JUDGE_TAP_EFFECT_UNDER);

				for (int nCountDraw = 0; nCountDraw < gc_nJudgeEffectUnderDrawCountMax; nCountDraw++)
				{
					SetPolygonColor3D(D3DCOLOR_ARGB(g_JudgeEffect[nCountJudgeEffect].nA, 255, 255, 255));
					SetScale3D(g_JudgeEffect[nCountJudgeEffect].fScale, 1.0f, g_JudgeEffect[nCountJudgeEffect].fScale);
					SetRot3D(0.0f, g_JudgeEffect[nCountJudgeEffect].fAngle, 0.0f);
					DrawPolygon3D
					(
						g_JudgeEffect[nCountJudgeEffect].vPos.x,
						g_JudgeEffect[nCountJudgeEffect].vPos.y,
						g_JudgeEffect[nCountJudgeEffect].vPos.z,
						0,
						tJudgeTapEffectUnder.v * g_JudgeEffect[nCountJudgeEffect].nV,
						tJudgeTapEffectUnder.u,
						tJudgeTapEffectUnder.v
					);
				}
				break;
			}
			case NT_SLIDE:
			{
				TEXTURE tJudgeFlickEffect = GetTextureStatus(TEX_GAME_JUDGE_SLIDE_EFFECT_UNDER);
				SetTexture(TEX_GAME_JUDGE_SLIDE_EFFECT_UNDER);
				for (int nCountDraw = 0; nCountDraw < gc_nJudgeEffectUnderDrawCountMax; nCountDraw++)
				{
					SetBillBoardColor(D3DCOLOR_ARGB(g_JudgeEffect[nCountJudgeEffect].nA, 255, 255, 255));
					SetBillboardScale(g_JudgeEffect[nCountJudgeEffect].fScale, g_JudgeEffect[nCountJudgeEffect].fScale, 1.0f);
					//SetBillboardRot(0.0f, 0.0f, g_JudgeEffect[nCountJudgeEffect].fAngle);
					SetBillboardCenterPos(0.5f, 0.5f);
					DrawBillboard
					(
						g_JudgeEffect[nCountJudgeEffect].vPos.x,
						g_JudgeEffect[nCountJudgeEffect].vPos.y,
						g_JudgeEffect[nCountJudgeEffect].vPos.z,
						0,
						tJudgeFlickEffect.v * g_JudgeEffect[nCountJudgeEffect].nV,
						tJudgeFlickEffect.u,
						tJudgeFlickEffect.v
					);
				}
				break;
			}
			}
		}
	}

	// 上
	for (int nCountJudgeEffect = 0; nCountJudgeEffect < NUM_BTN; nCountJudgeEffect++)
	{
		if (g_JudgeEffectUp[nCountJudgeEffect].bStatus)
		{
			switch (g_JudgeEffectUp[nCountJudgeEffect].nNoteType)
			{
			case NT_TAP:
			case NT_BREAK:
			case NT_HOLD:
			{
				TEXTURE tJudgeTapEffectUp = GetTextureStatus(TEX_GAME_JUDGE_TAP_EFFECT_UP);
				SetTexture(TEX_GAME_JUDGE_TAP_EFFECT_UP);

				for (int nCountDraw = 0; nCountDraw < gc_nJudgeEffectUnderDrawCountMax; nCountDraw++)
				{
					SetBillBoardColor(D3DCOLOR_ARGB(g_JudgeEffectUp[nCountJudgeEffect].nA, 255, 255, 255));
					SetBillboardScale(g_JudgeEffectUp[nCountJudgeEffect].fScale, g_JudgeEffectUp[nCountJudgeEffect].fScale, 1.0f);
					SetBillboardRot(0.0f, g_JudgeEffectUp[nCountJudgeEffect].fAngle, 0.0f);
					SetBillboardCenterPos(0.5f, 0.5f);
					DrawBillboard
					(
						g_JudgeEffectUp[nCountJudgeEffect].vPos.x,
						g_JudgeEffectUp[nCountJudgeEffect].vPos.y,
						g_JudgeEffectUp[nCountJudgeEffect].vPos.z,
						0,
						tJudgeTapEffectUp.v * g_JudgeEffectUp[nCountJudgeEffect].nV,
						tJudgeTapEffectUp.u,
						tJudgeTapEffectUp.v
					);
				}
				break;
			}
			//case NT_HOLD:
			//{
			//	break;
			//}
			case NT_FLICK:
			{
				TEXTURE tJudgeFlickEffectUp = GetTextureStatus(TEX_GAME_JUDGE_FLICK_EFFECT_UP);
				SetTexture(TEX_GAME_JUDGE_FLICK_EFFECT_UP);

				for (int nCountDraw = 0; nCountDraw < gc_nJudgeEffectUnderDrawCountMax; nCountDraw++)
				{
					SetBillBoardColor(D3DCOLOR_ARGB(g_JudgeEffectUp[nCountJudgeEffect].nA, 255, 255, 255));
					SetBillboardScale(g_JudgeEffectUp[nCountJudgeEffect].fScale, 1.0f, 1.0f);
					SetBillboardRot(0.0f, 0.0f, g_JudgeEffectUp[nCountJudgeEffect].fAngle);
					SetBillboardCenterPos(0.5f, 0.5f);
					DrawBillboard
					(
						g_JudgeEffectUp[nCountJudgeEffect].vPos.x,
						g_JudgeEffectUp[nCountJudgeEffect].vPos.y,
						g_JudgeEffectUp[nCountJudgeEffect].vPos.z,
						0,
						tJudgeFlickEffectUp.v * g_JudgeEffectUp[nCountJudgeEffect].nV,
						tJudgeFlickEffectUp.u,
						tJudgeFlickEffectUp.v
					);
				}
				break;
			}
			case NT_SLIDE:
			{
				TEXTURE tJudgeTapEffectUnder = GetTextureStatus(TEX_GAME_JUDGE_SLIDE_EFFECT_UP);
				SetTexture(TEX_GAME_JUDGE_SLIDE_EFFECT_UP);

				for (int nCountDraw = 0; nCountDraw < gc_nJudgeEffectUnderDrawCountMax; nCountDraw++)
				{
					SetPolygonColor3D(D3DCOLOR_ARGB(g_JudgeEffectUp[nCountJudgeEffect].nA, 255, 255, 255));
					SetScale3D(g_JudgeEffectUp[nCountJudgeEffect].fScale, 1.0f, g_JudgeEffectUp[nCountJudgeEffect].fScale);
					SetRot3D(0.0f, g_JudgeEffectUp[nCountJudgeEffect].fAngle, 0.0f);
					DrawPolygon3D
					(
						g_JudgeEffectUp[nCountJudgeEffect].vPos.x,
						g_JudgeEffectUp[nCountJudgeEffect].vPos.y,
						g_JudgeEffectUp[nCountJudgeEffect].vPos.z,
						0,
						tJudgeTapEffectUnder.v * g_JudgeEffectUp[nCountJudgeEffect].nV,
						tJudgeTapEffectUnder.u,
						tJudgeTapEffectUnder.v
					);
				}
				break;
			}
			}
		}
	}
	SetBlend(D3DBLEND_INVSRCALPHA);
	SetZBuffer(TRUE);
}

void SetJudgeEffect(int nLane, int nNoteType, int nJudgeLevel,int nParts, bool bStatus, int nDirect)
{
	if (bStatus)
	{
		if (nParts == JES_UNDER)
		{
			// 下
			switch (nNoteType)
			{
			case NT_TAP:
			case NT_BREAK:
			case NT_HOLD:
			case NT_FLICK:
			{
				g_JudgeEffect[nLane].bStatus = true;
				g_JudgeEffect[nLane].fAngle = 0.0f;
				g_JudgeEffect[nLane].fScale = gc_fJudgeEffectScale[nNoteType][JEP_INIT];
				g_JudgeEffect[nLane].nA = gc_nJudgeEffectA[nNoteType][JEP_INIT];
				g_JudgeEffect[nLane].nCount = 0;
				g_JudgeEffect[nLane].nJudgeLevel = nJudgeLevel;
				g_JudgeEffect[nLane].nNoteType = nNoteType;
				g_JudgeEffect[nLane].nV = nJudgeLevel;
				g_JudgeEffect[nLane].vPos = { gc_vJudgeEffectInitPosX[nLane],0.0f,JUDGE_LINE_Z };
				break;
			}
			case NT_SLIDE:
			{
				g_JudgeEffect[nLane].bStatus = true;
				g_JudgeEffect[nLane].fAngle = 0.0f;
				g_JudgeEffect[nLane].fScale = gc_fJudgeEffectScale[nNoteType][JEP_INIT];
				g_JudgeEffect[nLane].nA = gc_nJudgeEffectA[nNoteType][JEP_INIT];
				g_JudgeEffect[nLane].nCount = 0;
				g_JudgeEffect[nLane].nJudgeLevel = nJudgeLevel;
				g_JudgeEffect[nLane].nNoteType = nNoteType;
				g_JudgeEffect[nLane].nV = nJudgeLevel;
				g_JudgeEffect[nLane].vPos = GetSlidePos(nLane);
				g_JudgeEffect[nLane].vPos.y /= N;
				g_JudgeEffect[nLane].vPos.z = JUDGE_LINE_Z;
				break;
			}
			}
		}

		if (nParts == JES_UP)
		{
			// 上
			switch (nNoteType)
			{
			case NT_TAP:
			case NT_BREAK:
			case NT_HOLD:
			{
				g_JudgeEffectUp[nLane].bStatus = true;
				g_JudgeEffectUp[nLane].fAngle = D3DXToRadian(0);
				g_JudgeEffectUp[nLane].fScale = gc_fJudgeEffectUpScale[nNoteType][JEP_INIT];
				g_JudgeEffectUp[nLane].nA = gc_nJudgeEffectAUp[nNoteType][JEP_INIT];
				g_JudgeEffectUp[nLane].nCount = 0;
				g_JudgeEffectUp[nLane].nJudgeLevel = nJudgeLevel;
				g_JudgeEffectUp[nLane].nNoteType = nNoteType;
				g_JudgeEffectUp[nLane].nV = nJudgeLevel;
				g_JudgeEffectUp[nLane].vPos = { gc_vJudgeEffectInitPosX[nLane], 0.0f/*gc_fJudgeEffectUpInitY*/,JUDGE_LINE_Z };
				break;
			}
			//case NT_HOLD:
			//{
			//	break;
			//}
			case NT_FLICK:
			{
				g_JudgeEffectUp[nLane].bStatus = true;
				g_JudgeEffectUp[nLane].fScale = gc_fJudgeEffectUpScale[nNoteType][JEP_INIT];
				g_JudgeEffectUp[nLane].nA = gc_nJudgeEffectAUp[nNoteType][JEP_INIT];
				g_JudgeEffectUp[nLane].nCount = 0;
				g_JudgeEffectUp[nLane].nJudgeLevel = nJudgeLevel;
				g_JudgeEffectUp[nLane].nNoteType = nNoteType;
				g_JudgeEffectUp[nLane].nV = nJudgeLevel;
				g_JudgeEffectUp[nLane].vPos = { gc_vJudgeEffectInitPosX[nLane], 0.0f,JUDGE_LINE_Z };
				switch (nDirect)
				{
				case NFD_UP:
				case NFD_DOWN:
				{
					g_JudgeEffectUp[nLane].fAngle = D3DXToRadian(90);
					break;
				}
				case NFD_LEFT:
				case NFD_RIGHT:
				{
					g_JudgeEffectUp[nLane].fAngle = D3DXToRadian(0);
					break;
				}
				default:
					break;
				}
				break;
			}
			case NT_SLIDE:
			{
				g_JudgeEffectUp[nLane].bStatus = true;
				g_JudgeEffectUp[nLane].fAngle = 0.0f;
				g_JudgeEffectUp[nLane].fScale = gc_fJudgeEffectUpScale[nNoteType][JEP_INIT];
				g_JudgeEffectUp[nLane].nA = gc_nJudgeEffectAUp[nNoteType][JEP_INIT];
				g_JudgeEffectUp[nLane].nCount = 0;
				g_JudgeEffectUp[nLane].nJudgeLevel = nJudgeLevel;
				g_JudgeEffectUp[nLane].nNoteType = nNoteType;
				g_JudgeEffectUp[nLane].nV = nJudgeLevel;
				NOTE Note = GetNote(nLane, GetJudgeNoteNum(nLane));
				g_JudgeEffectUp[nLane].vPos = { Note.vPos[NDS_END].x,Note.vPos[NDS_END].y / N,JUDGE_LINE_Z };
				break;
			}
			}
		}
	}
	else
	{
		if (nParts == JES_UNDER)
		{
			g_JudgeEffect[nLane].bStatus = false;
		}
		if (nParts == JES_UP)
		{
			g_JudgeEffectUp[nLane].bStatus = false;
		}
	}
}