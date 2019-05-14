//----------------------------------------------------------------------------
//
//	リザルト(詳細結果)ランク
//
//														 制作者：劉喆
//                                                       作成日：2018/01/15
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "ResultRank.h"
#include "Score.h"
#include "debug.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	RRIS_F,
	RRIS_E,
	RRIS_D,
	RRIS_C,
	RRIS_B,
	RRIS_A,
	RRIS_AA,
	RRIS_AAA,
	RRIS_S,
	RRIS_SS,
	RRIS_SSS,
	RRIS_MAX
}RESULT_RANK_IN_SCORE;

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;				// 座標
	int nA;							// α値
	int nCount;						// カウンター
}RESULT_RANK_BOARD;					// ランクの背景

typedef struct {
	D3DXVECTOR2 vPos;				// 座標
	int nA;							// α値
	int nV;							// Vの値
	int nCount;						// カウンター
	float fScale;					// スケーリング
}RESULT_RANK;						// ランク

//---------------------------グローバル変数-----------------------------------
static RESULT_RANK_BOARD g_ResultRankBoard;
static RESULT_RANK g_ResultRank;

static const D3DXVECTOR2 gc_vResultRankBoardInitPos = { 813.0f,480.0f };
static const int gc_nResultRankBoardA[RDP_MAX] =
{
	0,255
};
static const int gc_nResultRankBoardFadeInMaxCount = 60;
static const int gc_nResultRankFadeInMaxCount = 8;

static const float gc_fResultRankInScore[RRIS_MAX] =
{
	0.0f,
	400000.0f,
	600000.0f,
	750000.0f,
	850000.0f,
	900000.0f,
	930000.0f,
	960000.0f,
	980000.0f,
	1000000.0f,
	1007000.0f,
};

static const float gc_fResultRankScale[RDP_MAX] =
{
	0.0f,1.0f
};

//---------------------------プロトタイプ宣言---------------------------------
void InitResultRankBoard()
{
	g_ResultRankBoard.vPos = gc_vResultRankBoardInitPos;
	g_ResultRankBoard.nCount = 0;
	g_ResultRankBoard.nA = gc_nResultRankBoardA[RDP_INIT];
}

void UninitResultRankBoard()
{

}

void UpdateResultRankBoard()
{
	if (g_ResultRank.nV >= RRIS_A)
	{
		g_ResultRankBoard.nCount++;
		if (g_ResultRankBoard.nCount < gc_nResultRankBoardFadeInMaxCount)
		{
			g_ResultRankBoard.nA += (gc_nResultRankBoardA[RDP_END] - gc_nResultRankBoardA[RDP_INIT]) * 1.0f / (gc_nResultRankBoardFadeInMaxCount * 1.0f);
		}
		else
			if (g_ResultRankBoard.nCount == gc_nResultRankBoardFadeInMaxCount)
			{
				g_ResultRankBoard.nA = gc_nResultRankBoardA[RDP_END];
			}
			else
				if (g_ResultRankBoard.nCount < gc_nResultRankBoardFadeInMaxCount * 2)
				{
					g_ResultRankBoard.nA += (-gc_nResultRankBoardA[RDP_END] + gc_nResultRankBoardA[RDP_INIT]) * 1.0f / (gc_nResultRankBoardFadeInMaxCount * 1.0f);
				}
				else
					if (g_ResultRankBoard.nCount == gc_nResultRankBoardFadeInMaxCount * 2)
					{
						g_ResultRankBoard.nA = gc_nResultRankBoardA[RDP_INIT];
						g_ResultRankBoard.nCount = 0;
					}
		if (g_ResultRankBoard.nA > gc_nResultRankBoardA[RDP_END])
		{
			g_ResultRankBoard.nA = gc_nResultRankBoardA[RDP_END];
		}
		if (g_ResultRankBoard.nA < gc_nResultRankBoardA[RDP_INIT])
		{
			g_ResultRankBoard.nA = gc_nResultRankBoardA[RDP_INIT];
		}
	}
}

void DrawResultRankBoard()
{
	if (g_ResultRank.nV >= RRIS_A)
	{
		SetBlend(D3DBLEND_BLENDFACTOR);
		SetTexture(TEX_RESULT_RANK_BOARD);
		TEXTURE tResultRankBoard = GetTextureStatus(TEX_RESULT_RANK_BOARD);
		SetPolygonColor(D3DCOLOR_ARGB(g_ResultRankBoard.nA, 255, 255, 255));
		SetScale
		(
			0.75f,
			0.75f,
			tResultRankBoard.u / 2.0f,
			tResultRankBoard.v / 2.0f
		);
		DrawPolygon
		(
			g_ResultRankBoard.vPos.x,
			g_ResultRankBoard.vPos.y,
			0,
			0,
			tResultRankBoard.u,
			tResultRankBoard.v
		);
		SetBlend(D3DBLEND_INVSRCALPHA);
	}
}

//-----------------------------------------------------------------
// ランク
//-----------------------------------------------------------------
void InitRank()
{
	g_ResultRank.vPos = gc_vResultRankBoardInitPos;
	g_ResultRank.nA = gc_nResultRankBoardA[RDP_INIT];
	g_ResultRank.fScale = gc_fResultRankScale[RDP_INIT];
	g_ResultRank.nCount = 0;
	float fScore = GetScore();
	for (int nCountRankScore = RRIS_MAX - 1; nCountRankScore >= 0; nCountRankScore--)
	{
		if (fScore >= gc_fResultRankInScore[nCountRankScore])
		{
			g_ResultRank.nV = nCountRankScore;
			break;
		}
	}
}

void UninitRank()
{

}

void UpdateRank()
{
	if (g_ResultRank.nCount < gc_nResultRankFadeInMaxCount)
	{
		g_ResultRank.nCount++;
		g_ResultRank.nA += (gc_nResultRankBoardA[RDP_END] - gc_nResultRankBoardA[RDP_INIT]) * 1.0f / (gc_nResultRankFadeInMaxCount * 1.0f);
		g_ResultRank.fScale += (gc_fResultRankScale[RDP_END] - gc_fResultRankScale[RDP_INIT]) / (gc_nResultRankFadeInMaxCount * 1.0f);
		if (g_ResultRank.nCount == gc_nResultRankFadeInMaxCount)
		{
			g_ResultRank.nA = gc_nResultRankBoardA[RDP_END];
		}
	}
}

void DrawRank()
{
	DebugText(0, 22, "Rank: %d", g_ResultRank.nV);
	SetTexture(TEX_RESULT_RANK);
	TEXTURE tResultRank = GetTextureStatus(TEX_RESULT_RANK);
	SetPolygonColor(D3DCOLOR_ARGB(g_ResultRank.nA, 255, 255, 255));
	SetScale
	(
		g_ResultRank.fScale,
		g_ResultRank.fScale,
		tResultRank.u / 2.0f,
		tResultRank.v / 2.0f
		);
	DrawPolygon
	(
		g_ResultRank.vPos.x,
		g_ResultRank.vPos.y,
		0,
		tResultRank.v * g_ResultRank.nV,
		tResultRank.u,
		tResultRank.v
	);
}