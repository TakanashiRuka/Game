//----------------------------------------------------------------------------
//
//	リザルト(詳細結果)の背景
//
//														 制作者：劉喆
//                                                       作成日：2018/01/12
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "ResultBoard.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	RP_RESULT_BOARD,
	RP_RESULT_WORD,
	RP_RESULT_SCORE_WORD,
	RP_RESULT_RANK_WORD,
	RP_RESULT_RANK_BOARD,
	RP_RESULT_DETAIL_BOARD_PERFECT_JUST,
	RP_RESULT_DETAIL_BOARD_PERFECT,
	RP_RESULT_DETAIL_BOARD_GREAT,
	RP_RESULT_DETAIL_BOARD_MISS,
	RP_RESULT_DETAIL_BOARD_MAX_COMBO,
	RP_MAX
}RESULT_PARTS;

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;			// 座標
	D3DXVECTOR2 vScale;			// スケーリング
	int nCountFadeIn;			// フェードインのカウンター
	int nA;						// α値
}RESULT_BOARD;					// リザルト(詳細結果)の背景

//---------------------------グローバル変数-----------------------------------
static RESULT_BOARD g_ResultBoard[RP_MAX];

static const D3DXVECTOR2 gc_vResultJudgeDetailBoardInitPos = { 467.0f,408.0f };
static const D3DXVECTOR2 gc_vResultJudgeDetailBoardAddPos = { 0.0f,38.0f };

static const int gc_nTextureNum[RP_MAX] =
{
	TEX_RESULT_BOARD,
	TEX_RESULT_WORD,
	TEX_RESULT_SCORE_WORD,
	TEX_RESULT_RANK_WORD,
	TEX_RESULT_RANK_BOARD,
	TEX_RESULT_JUDGE_DETAIL_BOARD,
	TEX_RESULT_JUDGE_DETAIL_BOARD,
	TEX_RESULT_JUDGE_DETAIL_BOARD,
	TEX_RESULT_JUDGE_DETAIL_BOARD,
	TEX_RESULT_JUDGE_DETAIL_BOARD,
};

static const D3DXVECTOR2 gc_vResultBoardPos[RP_MAX][RDP_MAX] =			// リザルト(詳細結果)の背景の座標 
{
	{ { WINDOW_WIDTH / 2.0f,400.0f },{ WINDOW_WIDTH / 2.0f,400.0f } },
	{ { WINDOW_WIDTH / 2.0f + 3.0f,215.0f },{ WINDOW_WIDTH / 2.0f + 3.0f,215.0f } },
	{ { 545.0f,250.0f },{ 535.0f,250.0f } },
	{ { 710.0f,395.0f },{ 700.0f,395.0f } },
	{ { 813.0f,480.0f },{ 813.0f,480.0f } },
	{ gc_vResultJudgeDetailBoardInitPos + 0 * gc_vResultJudgeDetailBoardAddPos, gc_vResultJudgeDetailBoardInitPos + 0 * gc_vResultJudgeDetailBoardAddPos },
	{ gc_vResultJudgeDetailBoardInitPos + 1 * gc_vResultJudgeDetailBoardAddPos, gc_vResultJudgeDetailBoardInitPos + 1 * gc_vResultJudgeDetailBoardAddPos },
	{ gc_vResultJudgeDetailBoardInitPos + 2 * gc_vResultJudgeDetailBoardAddPos, gc_vResultJudgeDetailBoardInitPos + 2 * gc_vResultJudgeDetailBoardAddPos },
	{ gc_vResultJudgeDetailBoardInitPos + 3 * gc_vResultJudgeDetailBoardAddPos, gc_vResultJudgeDetailBoardInitPos + 3 * gc_vResultJudgeDetailBoardAddPos },
	{ gc_vResultJudgeDetailBoardInitPos + 4 * gc_vResultJudgeDetailBoardAddPos, gc_vResultJudgeDetailBoardInitPos + 4 * gc_vResultJudgeDetailBoardAddPos },
};

static const int gc_nResultBoardFadeInCountMax[RP_MAX] =		   // リザルト(詳細結果)の背景のフェードインカウンターの最大値
{
	8,
	8,
	8,
	8,
	10,
	8,
	8,
	8,
	8,
	8,
};

static const int gc_nResultBoardFadeInStartTime[RP_MAX]=
{
	0,
	8,
	10,
	10,
	12,
	8,
	12,
	16,
	20,
	24,
};

static const D3DXVECTOR2 gc_vResultBoardScale[RP_MAX][RDP_MAX] = 			   // リザルト(詳細結果)の背景の大きさ
{
	{ { 1.0f, 0.0f },{ 1.0f, 1.0f } },
	{ { 1.0f, 1.0f },{ 1.0f, 1.0f } },
	{ { 1.0f, 1.0f },{ 1.0f, 1.0f } },
	{ { 1.0f, 1.0f },{ 1.0f, 1.0f } },
	{ { 0.0f, 0.0f },{ 0.75f, 0.75f } },
	{ { 1.0f, 1.0f },{ 1.0f, 1.0f } },
	{ { 1.0f, 1.0f },{ 1.0f, 1.0f } },
	{ { 1.0f, 1.0f },{ 1.0f, 1.0f } },
	{ { 1.0f, 1.0f },{ 1.0f, 1.0f } },
	{ { 1.0f, 1.0f },{ 1.0f, 1.0f } },
};

static const int gc_nResultBoardA[RP_MAX][RDP_MAX] =
{
	{ 255, 255 },
	{ 0, 255 },
	{ 0, 255 },
	{ 0, 255 },
	{ 0, 255 },
	{ 0, 255 },
	{ 0, 255 },
	{ 0, 255 },
	{ 0, 255 },
	{ 0, 255 },
};

static int g_nReaultBoardFadeInStartTimeCount;

//---------------------------プロトタイプ宣言---------------------------------
void InitResultBoard()
{
	g_nReaultBoardFadeInStartTimeCount = 0;
	for (int nCountResultParts = 0; nCountResultParts < RP_MAX; nCountResultParts++)
	{
		g_ResultBoard[nCountResultParts].vScale = gc_vResultBoardScale[nCountResultParts][RDP_INIT];
		g_ResultBoard[nCountResultParts].nA = gc_nResultBoardA[nCountResultParts][RDP_INIT];
		g_ResultBoard[nCountResultParts].vPos = gc_vResultBoardPos[nCountResultParts][RDP_INIT];
		g_ResultBoard[nCountResultParts].nCountFadeIn = 0;
	}
}

void UninitResultBoard()
{

}

void UpdateResultBoard()
{
	if (g_nReaultBoardFadeInStartTimeCount < gc_nResultBoardFadeInStartTime[RP_MAX - 1])
	{
		g_nReaultBoardFadeInStartTimeCount++;
	}
	for (int nCountResultParts = 0; nCountResultParts < RP_MAX; nCountResultParts++)
	{
		if (g_nReaultBoardFadeInStartTimeCount >= gc_nResultBoardFadeInStartTime[nCountResultParts])
		{
			if (g_ResultBoard[nCountResultParts].nCountFadeIn < gc_nResultBoardFadeInCountMax[nCountResultParts])
			{
				g_ResultBoard[nCountResultParts].nCountFadeIn++;
				g_ResultBoard[nCountResultParts].vPos += (gc_vResultBoardPos[nCountResultParts][RDP_END] - gc_vResultBoardPos[nCountResultParts][RDP_INIT]) / (gc_nResultBoardFadeInCountMax[nCountResultParts] * 1.0f);
				g_ResultBoard[nCountResultParts].vScale += (gc_vResultBoardScale[nCountResultParts][RDP_END] - gc_vResultBoardScale[nCountResultParts][RDP_INIT]) / (gc_nResultBoardFadeInCountMax[nCountResultParts] * 1.0f);
				g_ResultBoard[nCountResultParts].nA += (gc_nResultBoardA[nCountResultParts][RDP_END] - gc_nResultBoardA[nCountResultParts][RDP_INIT]) * 1.0f / (gc_nResultBoardFadeInCountMax[nCountResultParts] * 1.0f);
				if (g_ResultBoard[nCountResultParts].nCountFadeIn == gc_nResultBoardFadeInCountMax[nCountResultParts])
				{
					g_ResultBoard[nCountResultParts].vScale = gc_vResultBoardScale[nCountResultParts][RDP_END];
					g_ResultBoard[nCountResultParts].nA = gc_nResultBoardA[nCountResultParts][RDP_END];
				}
			}
		}
	}
}

void DrawResultBoard()
{
	for (int nCountResultParts = 0; nCountResultParts < RP_MAX; nCountResultParts++)
	{
		if (g_nReaultBoardFadeInStartTimeCount >= gc_nResultBoardFadeInStartTime[nCountResultParts])
		{
			SetTexture(gc_nTextureNum[nCountResultParts]);
			TEXTURE tResultBoard = GetTextureStatus(gc_nTextureNum[nCountResultParts]);
			SetPolygonColor(D3DCOLOR_ARGB(g_ResultBoard[nCountResultParts].nA, 255, 255, 255));
			SetScale
			(
				g_ResultBoard[nCountResultParts].vScale.x,
				g_ResultBoard[nCountResultParts].vScale.y,
				tResultBoard.u / 2.0f,
				tResultBoard.v / 2.0f
			);
			DrawPolygon
			(
				g_ResultBoard[nCountResultParts].vPos.x,
				g_ResultBoard[nCountResultParts].vPos.y,
				0,
				0,
				tResultBoard.u,
				tResultBoard.v
			);
		}
	}
}
