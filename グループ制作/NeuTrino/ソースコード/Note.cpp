//----------------------------------------------------------------------------
//
//	ノーツ
//
//														 制作者：劉喆
//                                                       作成日：2017/11/30
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Note.h"
#include "Btn.h"
#include "Texture.h"
#include "Polygon3D.h"
#include "Light.h"
#include "Timer.h"
#include "JudgeLine.h"
#include "Line.h"

//---------------------------マクロ定義---------------------------------------
#define NOTE_FLICK_ADD_Y        (30.0f)

#define HOLD_BLEND_FACTOR_TIME	(5)				// ホールドの加算合成のループ時間
#define NOTE_FLICK_ARROW_TIME	(18)				// フリックアローのループ時間

#define SLIDE_SHADOW_A			(63)				// 影のα値

#define SLIDE_ARROW_A_JUDGE		(255)				// スライド音符矢印のα値(判定直前)
#define SLIDE_ARROW_A_UNJUDGED	(255)				// スライド音符矢印のα値(判定しない方)

#define DARW_SP_JUDGE_LINE_TIME	(1500)				// スペシャル判定バーの描画時間

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	int nU;						// uの値
	int nV;						// vの値
	unsigned int nCount;					// ループカウンター
	unsigned int nTexNum;				// テクスチャ番号
}NOTE_FLICK_ARROW;				// フリックアローのアニメーション

//---------------------------グローバル変数-----------------------------------
static NOTE g_Note[NUM_BTN][MAX_NOTE_PER_LANE];
static int g_nJudgeNoteNum[NUM_BTN];					// 判定すべき音符番号
static int g_nNoteMaxNum[NUM_BTN];						// レーンごとに音符の最大数
static float g_fHiSpeed = INIT_SPEED;							// スクロールスピード
static int g_nOffset = INIT_OFFSET;

static int g_nMaxNote;									// 音符の最大数
static int g_nMaxNotePerType[NT_MAX];					// 各種音符の最大数

static const float gc_fFlickUVScale = 1.0f;

static int g_nHoldTexList[NDS_MAX] =						// ホールドのテクスチャリスト
{
	TEX_HOLD_START,
	TEX_HOLD_MIDDLE,
	TEX_HOLD_END
};

static int g_nSlideTexList[NDS_MAX] =						// スライドのテクスチャリスト
{
	TEX_SLIDE_START,
	TEX_SLIDE_MIDDLE,
	TEX_SLIDE_END
};

static D3DXVECTOR2 g_vSlidePosList[NSL_MAX][NSP_MAX]=
{
	{
		{ NOTE_X ,NOTE_SLIDE_Y_MAX },
		{ NOTE_X + NOTE_RANGE_X * 1.0f ,NOTE_SLIDE_Y_MAX },
		{ NOTE_X ,NOTE_SLIDE_Y_MIN },
		{ NOTE_X + NOTE_RANGE_X * 1.0f ,NOTE_SLIDE_Y_MIN }
	},
	{
		{ NOTE_X + 2.0f * NOTE_RANGE_X ,NOTE_SLIDE_Y_MAX },
		{ NOTE_X + 3.0f * NOTE_RANGE_X ,NOTE_SLIDE_Y_MAX },
		{ NOTE_X + 2.0f *NOTE_RANGE_X ,NOTE_SLIDE_Y_MIN },
		{ NOTE_X + 3.0f *NOTE_RANGE_X ,NOTE_SLIDE_Y_MIN }
	}
};

static D3DXVECTOR3 g_vSlideVertexChange = { GetTextureStatus(TEX_SLIDE_MIDDLE).u * 0.5f,0.0f,0.0f };

static NOTE_FLICK_ARROW g_NoteFlickArrow[NFD_MAX];

static const int gc_nBreakEffectCountMax = 8;
static const int gc_nBreakEffectMinA = 0;
static const int gc_nBreakEffectMaxA = 127;

static int g_nBreakEffectA;
static int g_nBreakEffectCount;

//---------------------------プロトタイプ宣言---------------------------------
void UpdateHoldBlendFactor(int nNum, int nNoteNum);

void InitNote()
{
	g_nBreakEffectA = gc_nBreakEffectMinA;
	g_nBreakEffectCount = 0;

	// 音符の初期化
	for (int nCountLane = 0; nCountLane < NUM_BTN; nCountLane++)
	{
		g_nNoteMaxNum[nCountLane] = 0;
		g_nJudgeNoteNum[nCountLane] = 0;
		for (int nCountNote = 0; nCountNote < MAX_NOTE_PER_LANE; nCountNote++)
		{
			if (nCountLane < NUM_BTN_NORMAL)
			{
				for (int nCountDrawPart = 0; nCountDrawPart < NDS_MAX; nCountDrawPart++)
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[nCountDrawPart] = false;
					g_Note[nCountLane][nCountNote].nTexNum[nCountDrawPart] = TEX_TAP;
					g_Note[nCountLane][nCountNote].vPos[nCountDrawPart] =
					{
						NOTE_X + nCountLane * NOTE_RANGE_X,
						NOTE_Y + 0.005f,
						NOTE_Z
					};
					g_Note[nCountLane][nCountNote].fHoldH[nCountDrawPart] = 1.0f;
				}
				for (int nCountJudgePart = 0; nCountJudgePart < NJS_MAX; nCountJudgePart++)
				{
					g_Note[nCountLane][nCountNote].bJudgeStatus[nCountJudgePart] = false;
					g_Note[nCountLane][nCountNote].nTiming[nCountJudgePart] = 0;
					g_Note[nCountLane][nCountNote].nNumOfLine[nCountJudgePart] = 0;
					g_Note[nCountLane][nCountNote].nNumOfNote[nCountJudgePart] = 0;
				}

				g_Note[nCountLane][nCountNote].nType = NT_TAP;
				g_Note[nCountLane][nCountNote].nTimingHoldMiddle = 0;
				g_Note[nCountLane][nCountNote].nTimingHoldMiddleMax = 0;
				g_Note[nCountLane][nCountNote].nHoldNum = 0;
				g_Note[nCountLane][nCountNote].nHoldA = HOLD_MAX_A;
				g_Note[nCountLane][nCountNote].nHoldBlendFactorA = HOLD_BLEND_FACTOR_MIN_A;
				g_Note[nCountLane][nCountNote].bHoldBlendFactor = false;
				g_Note[nCountLane][nCountNote].nCountHoldBlendFactor = 0;
				g_Note[nCountLane][nCountNote].nFlickDirection = NFD_UP;
				g_Note[nCountLane][nCountNote].bDrawShadow = false;
				g_Note[nCountLane][nCountNote].bSlideArrowDrawStatus = false;
				g_Note[nCountLane][nCountNote].nSlideNum = -1;
				for (int nCountVertex = 0; nCountVertex < NSP_MAX; nCountVertex++)
				{
					g_Note[nCountLane][nCountNote].vSlideMiddlePos[nCountVertex] =
						g_Note[nCountLane][nCountNote].vSlideShadowPos[nCountVertex] =
					{ 0.0f,0.0f,0.0f };
				}
			}
			else
			{
				for (int nCountDrawPart = 0; nCountDrawPart < NDS_MAX; nCountDrawPart++)
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[nCountDrawPart] = false;
					g_Note[nCountLane][nCountNote].nTexNum[nCountDrawPart] = TEX_FLICK;
					g_Note[nCountLane][nCountNote].vPos[nCountDrawPart] =
					{
						NOTE_FLICK_X + (nCountLane - NUM_BTN_NORMAL) * NOTE_RANGE_X * 2.0f,
						NOTE_Y,
						NOTE_Z
					};
					g_Note[nCountLane][nCountNote].fHoldH[nCountDrawPart] = 1.0f;
				}
				for (int nCountJudgePart = 0; nCountJudgePart < NJS_MAX; nCountJudgePart++)
				{
					g_Note[nCountLane][nCountNote].bJudgeStatus[nCountJudgePart] = false;
					g_Note[nCountLane][nCountNote].nTiming[nCountJudgePart] = 0;
					g_Note[nCountLane][nCountNote].nSlidePointPosNum[nCountJudgePart] = NSP_LEFT_UP;
				}

				g_Note[nCountLane][nCountNote].nType = NT_FLICK;
				g_Note[nCountLane][nCountNote].nTimingHoldMiddle = 0;
				g_Note[nCountLane][nCountNote].nTimingHoldMiddleMax = 0;
				g_Note[nCountLane][nCountNote].nHoldNum = 0;
				g_Note[nCountLane][nCountNote].nHoldA = HOLD_MAX_A;
				g_Note[nCountLane][nCountNote].nHoldBlendFactorA = HOLD_BLEND_FACTOR_MIN_A;
				g_Note[nCountLane][nCountNote].bHoldBlendFactor = false;
				g_Note[nCountLane][nCountNote].nCountHoldBlendFactor = 0;
				g_Note[nCountLane][nCountNote].nFlickDirection = NFD_UP;
				g_Note[nCountLane][nCountNote].bDrawShadow = false;
				g_Note[nCountLane][nCountNote].bSlideArrowDrawStatus = false;
				for (int nCountVertex = 0; nCountVertex < NSP_MAX; nCountVertex++)
				{
					g_Note[nCountLane][nCountNote].vSlideMiddlePos[nCountVertex] = 
						g_Note[nCountLane][nCountNote].vSlideShadowPos[nCountVertex] = 
					{ 0.0f,0.0f,0.0f };
				}
			}
		}
	}
}

void SetNote()
{
	// 音符の設定(1-4)
	for (int nCountLane = 0; nCountLane < NUM_BTN_NORMAL; nCountLane++)
	{
		for (int nCountNote = 0; nCountNote < g_nNoteMaxNum[nCountLane]; nCountNote++)
		{
			switch (g_Note[nCountLane][nCountNote].nType)
			{
			case NT_TAP:
			{
				g_Note[nCountLane][nCountNote].nTexNum[NDS_START] = TEX_TAP;
				break;
			}
			case NT_BREAK:
			{
				g_Note[nCountLane][nCountNote].nTexNum[NDS_START] = TEX_BREAK;
				break;
			}
			case NT_HOLD:
			{
				for (int nCountHoldTexList = 0; nCountHoldTexList < NDS_MAX; nCountHoldTexList++)
				{
					g_Note[nCountLane][nCountNote].nTexNum[nCountHoldTexList] = g_nHoldTexList[nCountHoldTexList];
				}
				g_Note[nCountLane][nCountNote].vPos[NDS_START].y += 0.005f;
				g_Note[nCountLane][nCountNote].nHoldNum =
					(g_Note[nCountLane][nCountNote].nTiming[NJS_END] -
						g_Note[nCountLane][nCountNote].nTiming[NJS_START]) / TIME_PER_HOLD + 1;
				g_Note[nCountLane][nCountNote].nTimingHoldMiddle = g_Note[nCountLane][nCountNote].nTiming[NJS_START] + TIME_PER_HOLD;
				g_Note[nCountLane][nCountNote].nTimingHoldMiddleMax = g_Note[nCountLane][nCountNote].nTiming[NJS_START] + TIME_PER_HOLD * (g_Note[nCountLane][nCountNote].nHoldNum);

				break;
			}
			}
		}
	}

	// 音符の設定(5-6)
	for (int nCountLane = NUM_BTN_NORMAL; nCountLane < NUM_BTN; nCountLane++)
	{
		for (int nCountNote = 0; nCountNote < g_nNoteMaxNum[nCountLane]; nCountNote++)
		{
			switch (g_Note[nCountLane][nCountNote].nType)
			{
			case NT_FLICK:
			{
				g_Note[nCountLane][nCountNote].nTexNum[NDS_START] = TEX_FLICK;
				break;
			}
			case NT_SLIDE:
			{
				//テクスチャの設定
				for (int nCountSlideTexList = 0; nCountSlideTexList < NDS_MAX; nCountSlideTexList++)
				{
					g_Note[nCountLane][nCountNote].nTexNum[nCountSlideTexList] = g_nSlideTexList[nCountSlideTexList];
				}

				//音符数の設定
				g_Note[nCountLane][nCountNote].nHoldNum =
					(g_Note[nCountLane][nCountNote].nTiming[NJS_END] -
						g_Note[nCountLane][nCountNote].nTiming[NJS_START]) / TIME_PER_HOLD + 1;

				g_Note[nCountLane][nCountNote].nTimingHoldMiddle = g_Note[nCountLane][nCountNote].nTiming[NJS_START] + TIME_PER_HOLD;
				g_Note[nCountLane][nCountNote].nTimingHoldMiddleMax = g_Note[nCountLane][nCountNote].nTiming[NJS_START] + TIME_PER_HOLD * (g_Note[nCountLane][nCountNote].nHoldNum);


				//スライド座標の設定
				g_Note[nCountLane][nCountNote].vPos[NDS_START].x = g_vSlidePosList[nCountLane - (NUM_BTN - NSL_MAX)][g_Note[nCountLane][nCountNote].nSlidePointPosNum[NJS_START]].x;
				g_Note[nCountLane][nCountNote].vPos[NDS_START].y = g_vSlidePosList[nCountLane - (NUM_BTN - NSL_MAX)][g_Note[nCountLane][nCountNote].nSlidePointPosNum[NJS_START]].y;
				g_Note[nCountLane][nCountNote].vPos[NDS_END].x = g_vSlidePosList[nCountLane - (NUM_BTN - NSL_MAX)][g_Note[nCountLane][nCountNote].nSlidePointPosNum[NJS_END]].x;
				g_Note[nCountLane][nCountNote].vPos[NDS_END].y = g_vSlidePosList[nCountLane - (NUM_BTN - NSL_MAX)][g_Note[nCountLane][nCountNote].nSlidePointPosNum[NJS_END]].y;

				//操作方向の設定
				D3DXVECTOR2 vSlidePos = g_vSlidePosList[nCountLane - (NUM_BTN - NSL_MAX)][g_Note[nCountLane][nCountNote].nSlidePointPosNum[NJS_END]] -
					g_vSlidePosList[nCountLane - (NUM_BTN - NSL_MAX)][g_Note[nCountLane][nCountNote].nSlidePointPosNum[NJS_START]];
				if (vSlidePos.x == 0 && vSlidePos.y == 0)
				{
					g_Note[nCountLane][nCountNote].nFlickDirection = -1;
				}
				else
					if (vSlidePos.x > 0 && vSlidePos.y == 0)
					{
						g_Note[nCountLane][nCountNote].nFlickDirection = NFD_RIGHT;
					}
					else
						if (vSlidePos.x < 0 && vSlidePos.y == 0)
						{
							g_Note[nCountLane][nCountNote].nFlickDirection = NFD_LEFT;
						}
						else
							if (vSlidePos.x == 0 && vSlidePos.y > 0)
							{
								g_Note[nCountLane][nCountNote].nFlickDirection = NFD_UP;
							}
							else
								if (vSlidePos.x == 0 && vSlidePos.y < 0)
								{
									g_Note[nCountLane][nCountNote].nFlickDirection = NFD_DOWN;
								}

				if (g_Note[nCountLane][nCountNote].vPos[NDS_START].y > NOTE_SLIDE_Y_MIN || g_Note[nCountLane][nCountNote].vPos[NDS_END].y > NOTE_SLIDE_Y_MIN)
				{
					g_Note[nCountLane][nCountNote].bDrawShadow = true;
				}

				switch (g_Note[nCountLane][nCountNote].nFlickDirection)
				{
				case NFD_LEFT:
				case NFD_RIGHT:
				{
					if (g_Note[nCountLane][nCountNote].nTiming[NJS_END] == g_Note[nCountLane][nCountNote].nTiming[NJS_START])
					{
						g_Note[nCountLane][nCountNote].bSlideArrowDrawStatus = true;
					}
					break;
				}
				case NFD_UP:
				case NFD_DOWN:
				{
					//if (g_Note[nCountLane][nCountNote].nTiming[NJS_END] == g_Note[nCountLane][nCountNote].nTiming[NJS_START])
					{
						g_Note[nCountLane][nCountNote].bSlideArrowDrawStatus = true;
					}
					break;
				}
				}
				break;
			}
			}
		}
	}

}

void UninitNote()
{

}

void UpdateNote()
{			
	int nTime = GetTimer();
	UpdateLine(nTime);
	for (int nCountLane = 0; nCountLane < NUM_BTN; nCountLane++)
	{
		for (int nCountNote = g_nJudgeNoteNum[nCountLane]; nCountNote < g_nNoteMaxNum[nCountLane]; nCountNote++)
		{
			switch (g_Note[nCountLane][nCountNote].nType)
			{
			case NT_TAP:
			case NT_BREAK:
			case NT_FLICK:
			{
				g_Note[nCountLane][nCountNote].vPos[NDS_START].z = 
					JUDGE_LINE_Z + (-nTime + g_Note[nCountLane][nCountNote].nTiming[NJS_START])
					* g_fHiSpeed * NOTE_SPEED_N/* * g_fBPM*/;

				if (g_Note[nCountLane][nCountNote].vPos[NDS_START].z <= DRAW_NOTE_START
					&&	g_Note[nCountLane][nCountNote].vPos[NDS_START].z > DRAW_NOTE_END
					&& !g_Note[nCountLane][nCountNote].bJudgeStatus[NJS_START])
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_START] = true;
				}
				else
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_START] = false;
				}
				if (g_Note[nCountLane][nCountNote].vPos[NDS_START].z > DRAW_NOTE_START)
				{
					break;
				}
				break;
			}
			case NT_HOLD:
			{
				UpdateHoldBlendFactor(nCountLane, nCountNote);

				g_Note[nCountLane][nCountNote].vPos[NDS_START].z =
					JUDGE_LINE_Z + (-nTime + g_Note[nCountLane][nCountNote].nTiming[NJS_START])
					* g_fHiSpeed * NOTE_SPEED_N/* * g_fBPM*/;

				g_Note[nCountLane][nCountNote].vPos[NDS_END].z =
					JUDGE_LINE_Z + (-nTime + g_Note[nCountLane][nCountNote].nTiming[NJS_END])
					* g_fHiSpeed * NOTE_SPEED_N/* * g_fBPM*/;

				// 長押し描画の長さを調整
				if (g_Note[nCountLane][nCountNote].bHoldBlendFactor&& nTime >= g_Note[nCountLane][nCountNote].nTiming[NJS_START])
				{
					g_Note[nCountLane][nCountNote].vPos[NDS_MIDDLE].z =
						(g_Note[nCountLane][nCountNote].vPos[NDS_END].z -
							JUDGE_LINE_Z) / 2.0f + JUDGE_LINE_Z;

					g_Note[nCountLane][nCountNote].fHoldH[NDS_MIDDLE] =
						g_Note[nCountLane][nCountNote].vPos[NDS_END].z -
						JUDGE_LINE_Z;
				}
				else
				{
					g_Note[nCountLane][nCountNote].vPos[NDS_MIDDLE].z =
						(g_Note[nCountLane][nCountNote].vPos[NDS_END].z -
							g_Note[nCountLane][nCountNote].vPos[NDS_START].z) / 2.0f +
						g_Note[nCountLane][nCountNote].vPos[NDS_START].z;

					g_Note[nCountLane][nCountNote].fHoldH[NDS_MIDDLE] =
						g_Note[nCountLane][nCountNote].vPos[NDS_END].z -
						g_Note[nCountLane][nCountNote].vPos[NDS_START].z;
				}

				if (g_Note[nCountLane][nCountNote].vPos[NDS_START].z <= DRAW_NOTE_START
					&&	g_Note[nCountLane][nCountNote].vPos[NDS_START].z > DRAW_NOTE_END
					&& !g_Note[nCountLane][nCountNote].bJudgeStatus[NJS_START])
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_START] = true;
				}
				else
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_START] = false;
				}
				if (!g_Note[nCountLane][nCountNote].bJudgeStatus[NJS_END]&&
					(g_Note[nCountLane][nCountNote].bDrawStatus[NDS_START]||
						g_Note[nCountLane][nCountNote].bJudgeStatus[NJS_START]))
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_MIDDLE] = true;
				}
				if (g_Note[nCountLane][nCountNote].vPos[NDS_END].z <= DRAW_NOTE_START
					&&	g_Note[nCountLane][nCountNote].vPos[NDS_END].z > DRAW_NOTE_END
					&& !g_Note[nCountLane][nCountNote].bJudgeStatus[NJS_END])
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_END] = true;
				}
				else
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_END] = false;
				}
				if (!g_Note[nCountLane][nCountNote].bDrawStatus[NDS_END] &&
					g_Note[nCountLane][nCountNote].bJudgeStatus[NJS_END])
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_END] = false;
				}

				if (g_Note[nCountLane][nCountNote].vPos[NDS_END].z > DRAW_NOTE_START)
				{
					break;
				}
				break;
			}
			case NT_SLIDE:
			{
				if (nCountNote == g_nJudgeNoteNum[nCountLane])
				{
					UpdateHoldBlendFactor(nCountLane, nCountNote);
				}
				else
				{
					if (g_Note[nCountLane][nCountNote].nSlideNum == g_Note[nCountLane][g_nJudgeNoteNum[nCountLane]].nSlideNum)
					{
						g_Note[nCountLane][nCountNote].nHoldA = g_Note[nCountLane][g_nJudgeNoteNum[nCountLane]].nHoldA;
						g_Note[nCountLane][nCountNote].nHoldBlendFactorA = g_Note[nCountLane][g_nJudgeNoteNum[nCountLane]].nHoldBlendFactorA;
						g_Note[nCountLane][nCountNote].bHoldBlendFactor = g_Note[nCountLane][g_nJudgeNoteNum[nCountLane]].bHoldBlendFactor;
						g_Note[nCountLane][nCountNote].nCountHoldBlendFactor = g_Note[nCountLane][g_nJudgeNoteNum[nCountLane]].nCountHoldBlendFactor;
					}
				}

				g_Note[nCountLane][nCountNote].vPos[NDS_START].z =
					JUDGE_LINE_Z + (-nTime + g_Note[nCountLane][nCountNote].nTiming[NJS_START])
					* g_fHiSpeed * NOTE_SPEED_N/* * g_fBPM*/;

				g_Note[nCountLane][nCountNote].vPos[NDS_END].z =
					JUDGE_LINE_Z + (-nTime + g_Note[nCountLane][nCountNote].nTiming[NJS_END])
					* g_fHiSpeed * NOTE_SPEED_N/* * g_fBPM*/;

				//中間部分
				g_Note[nCountLane][nCountNote].vPos[NDS_MIDDLE].z =
					(g_Note[nCountLane][nCountNote].vPos[NDS_END].z -
						g_Note[nCountLane][nCountNote].vPos[NDS_START].z) / 2.0f +
					g_Note[nCountLane][nCountNote].vPos[NDS_START].z;

				g_Note[nCountLane][nCountNote].fHoldH[NDS_MIDDLE] =
					g_Note[nCountLane][nCountNote].vPos[NDS_END].z -
					g_Note[nCountLane][nCountNote].vPos[NDS_START].z;

				// スライドの頂点座標の計算
				g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_LEFT_UP] = g_Note[nCountLane][nCountNote].vPos[NDS_END] - g_vSlideVertexChange;
				g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_RIGHT_UP] = g_Note[nCountLane][nCountNote].vPos[NDS_END] + g_vSlideVertexChange;
				// 長押し描画の長さを調整
				if (g_Note[nCountLane][nCountNote].bHoldBlendFactor && nTime >= g_Note[nCountLane][nCountNote].nTiming[NJS_START])
				{
					D3DXVECTOR3 vEndPos = g_Note[nCountLane][nCountNote].vPos[NDS_START];
					float fN = (float)(nTime - g_Note[nCountLane][nCountNote].nTiming[NJS_START]) /
						(g_Note[nCountLane][nCountNote].nTiming[NJS_END] - g_Note[nCountLane][nCountNote].nTiming[NJS_START]);
					vEndPos += (g_Note[nCountLane][nCountNote].vPos[NDS_END] - g_Note[nCountLane][nCountNote].vPos[NDS_START])*fN;
					g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_LEFT_DOWN] = vEndPos - g_vSlideVertexChange;
					g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_RIGHT_DOWN] = vEndPos + g_vSlideVertexChange;
				}
				else
				{
					g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_LEFT_DOWN] = g_Note[nCountLane][nCountNote].vPos[NDS_START] - g_vSlideVertexChange;
					g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_RIGHT_DOWN] = g_Note[nCountLane][nCountNote].vPos[NDS_START] + g_vSlideVertexChange;
				}

				if (g_Note[nCountLane][nCountNote].bDrawShadow)
				{
					for (int nCountVertex = 0; nCountVertex < NSP_MAX; nCountVertex++)
					{
						g_Note[nCountLane][nCountNote].vSlideShadowPos[nCountVertex] = g_Note[nCountLane][nCountNote].vSlideMiddlePos[nCountVertex];
						g_Note[nCountLane][nCountNote].vSlideShadowPos[nCountVertex].y = 0.0f;
					}
					//g_Note[nCountLane][nCountNote].vSlideShadowPos[NSP_LEFT_UP].x += 25.0f;
					//g_Note[nCountLane][nCountNote].vSlideShadowPos[NSP_RIGHT_UP].x += -25.0f;
					//g_Note[nCountLane][nCountNote].vSlideShadowPos[NSP_LEFT_DOWN].x += 25.0f;
					//g_Note[nCountLane][nCountNote].vSlideShadowPos[NSP_RIGHT_DOWN].x += -25.0f;
				}

				if (g_Note[nCountLane][nCountNote].vPos[NDS_START].z <= DRAW_NOTE_START
					&&	g_Note[nCountLane][nCountNote].vPos[NDS_START].z > DRAW_NOTE_END
					&& !g_Note[nCountLane][nCountNote].bJudgeStatus[NJS_START])
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_START] = true;
				}
				else
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_START] = false;
				}
				if (!g_Note[nCountLane][nCountNote].bJudgeStatus[NJS_END] &&
					(g_Note[nCountLane][nCountNote].bDrawStatus[NDS_START] ||
						g_Note[nCountLane][nCountNote].bJudgeStatus[NJS_START]))
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_MIDDLE] = true;
				}
				if (g_Note[nCountLane][nCountNote].vPos[NDS_END].z <= DRAW_NOTE_START
					&&	g_Note[nCountLane][nCountNote].vPos[NDS_END].z > DRAW_NOTE_END
					&& !g_Note[nCountLane][nCountNote].bJudgeStatus[NJS_END])
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_END] = true;
				}
				else
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_END] = false;
				}
				if (!g_Note[nCountLane][nCountNote].bDrawStatus[NDS_END] &&
					g_Note[nCountLane][nCountNote].bJudgeStatus[NJS_END])
				{
					g_Note[nCountLane][nCountNote].bDrawStatus[NDS_END] = false;
				}

				if (g_Note[nCountLane][nCountNote].vPos[NDS_END].z > DRAW_NOTE_START)
				{
					break;
				}
				break;
			}
			}
		}
	}
}

void DrawNote()
{

	//DebugText(0, 220, "Blend: %d", g_Note[0][0].bHoldBlendFactor);
	//DebugText(0, 242, "Range: %.2f", g_Note[0][0].fHoldH[1]);
	//DebugText(0, 264, "PosEndZ: %.2f", g_Note[0][0].vPos[2].z);
	for (int nCountType = 0; nCountType < NT_MAX; nCountType++)
	{
		DebugText(0, 22 * 13 + 22 * nCountType, "%d: %d", nCountType,g_nMaxNotePerType[nCountType]);
	}

	for (int nCountDraw = 0; nCountDraw < NDS_MAX; nCountDraw++)
	{
		//DebugText(0, 220 + 22 * nCountDraw, "SlidePos: %.2f, %.2f, %.2f　Direct: %d", g_Note[5][0].vPos[nCountDraw].x, g_Note[5][0].vPos[nCountDraw].y, g_Note[5][0].vPos[nCountDraw].z, g_Note[5][0].nFlickDirection);
	}
	//DebugText(0, 440, "SlideFlag: %d %d", g_Note[4][0].bSlideStartEndFlag[NJS_START], g_Note[4][0].bSlideStartEndFlag[NJS_END]);
	//DebugText(0, 462, "SlideTime: %d %d", g_Note[4][0].nTiming[NJS_START], g_Note[4][0].nTiming[NJS_END]);

	SetLight(FALSE);
	for (int nCountLane = 0; nCountLane < NUM_BTN; nCountLane++)
	{
		for (int nCountNote = g_nNoteMaxNum[nCountLane]; nCountNote >= g_nJudgeNoteNum[nCountLane]; nCountNote--)
		//for (int nCountNote = g_nJudgeNoteNum[nCountLane]; nCountNote < g_nNoteMaxNum[nCountLane]; nCountNote++)
		{
			switch (g_Note[nCountLane][nCountNote].nType)
			{
			case NT_TAP:
			{
				if (g_Note[nCountLane][nCountNote].bDrawStatus[NDS_START])
				{
					SetTexture(g_Note[nCountLane][nCountNote].nTexNum[NDS_START]);
					SetPolygonColor3D(D3DCOLOR_ARGB(255, 255, 255, 255));
					DrawPolygon3D
					(
						g_Note[nCountLane][nCountNote].vPos[NDS_START].x,
						g_Note[nCountLane][nCountNote].vPos[NDS_START].y,
						g_Note[nCountLane][nCountNote].vPos[NDS_START].z,
						0,
						0,
						GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[NDS_START]).u,
						GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[NDS_START]).v
					);
				}
				break;
			}
			case NT_BREAK:
			{
				if (g_Note[nCountLane][nCountNote].bDrawStatus[NDS_START])
				{
					SetTexture(g_Note[nCountLane][nCountNote].nTexNum[NDS_START]);
					SetPolygonColor3D(D3DCOLOR_ARGB(255, 255, 255, 255));
					DrawPolygon3D
					(
						g_Note[nCountLane][nCountNote].vPos[NDS_START].x,
						g_Note[nCountLane][nCountNote].vPos[NDS_START].y,
						g_Note[nCountLane][nCountNote].vPos[NDS_START].z,
						0,
						0,
						GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[NDS_START]).u,
						GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[NDS_START]).v
					);

					SetBlend(D3DBLEND_BLENDFACTOR);
					for (int nCountDraw = 0; nCountDraw < 1; nCountDraw++)
					{
						SetTexture(g_Note[nCountLane][nCountNote].nTexNum[NDS_START]);
						SetPolygonColor3D(D3DCOLOR_ARGB(g_nBreakEffectA, 255, 255, 255));
						DrawPolygon3D
						(
							g_Note[nCountLane][nCountNote].vPos[NDS_START].x,
							g_Note[nCountLane][nCountNote].vPos[NDS_START].y,
							g_Note[nCountLane][nCountNote].vPos[NDS_START].z,
							0,
							0,
							GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[NDS_START]).u,
							GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[NDS_START]).v
						);
					}
					SetBlend(D3DBLEND_INVSRCALPHA);
				}
				break;
			}
			case NT_FLICK:
			{
				if (g_Note[nCountLane][nCountNote].bDrawStatus[NDS_START])
				{
					SetTexture(g_Note[nCountLane][nCountNote].nTexNum[NDS_START]);
					SetPolygonColor3D(D3DCOLOR_ARGB(255, 255, 255, 255));
					DrawPolygon3D
					(
						g_Note[nCountLane][nCountNote].vPos[NDS_START].x,
						g_Note[nCountLane][nCountNote].vPos[NDS_START].y,
						g_Note[nCountLane][nCountNote].vPos[NDS_START].z,
						0,
						0,
						GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[NDS_START]).u,
						GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[NDS_START]).v
					);

					// 矢印
					int nU;
					int nV;
					float fY;
					switch (g_Note[nCountLane][nCountNote].nFlickDirection)
					{
					case NFD_UP:
					case NFD_DOWN:
					{
						nU = GetTextureStatus(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum).u;
						nV = GetTextureStatus(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum).v / gc_fFlickUVScale;
						fY = (NOTE_FLICK_ADD_Y + GetTextureStatus(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum).v) * 0.25f / 100.0f;
						SetScale3D(1.0f, 1.0f, 0.5f);
						break;
					}
					case NFD_LEFT:
					case NFD_RIGHT:
					{
						nU = GetTextureStatus(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum).u / gc_fFlickUVScale;
						nV = GetTextureStatus(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum).v;
						fY = (NOTE_FLICK_ADD_Y + GetTextureStatus(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum).v) * 0.5f / 100.0f;
						SetScale3D(0.5f, 1.0f, 1.0f);
						break;
					}
					}

					// 矢印の描画
					SetTexture(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum);
					SetPolygonColor3D(D3DCOLOR_ARGB(255, 255, 255, 255));
					SetRot3D(D3DXToRadian(-90), 0, 0);
					DrawPolygon3D
					(
						g_Note[nCountLane][nCountNote].vPos[NDS_START].x,
						g_Note[nCountLane][nCountNote].vPos[NDS_START].y + fY,
						g_Note[nCountLane][nCountNote].vPos[NDS_START].z,
						g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nU,
						g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nV,
						nU,
						nV
					);
				}
				break;
			}
			case NT_HOLD:
			{
				for (int nCountTexNum = 0; nCountTexNum < NDS_MAX; nCountTexNum++)
				{
					if (nCountTexNum != NDS_END)
					{
						SetZBuffer(TRUE);
					}
					else
					{
						SetZBuffer(FALSE);
					}
					if (g_Note[nCountLane][nCountNote].bDrawStatus[nCountTexNum])
					{
						SetTexture(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]);
						SetPolygonColor3D(D3DCOLOR_ARGB(g_Note[nCountLane][nCountNote].nHoldA, 255, 255, 255));
						SetScale3D(1.0f, 1.0f, g_Note[nCountLane][nCountNote].fHoldH[nCountTexNum]);
						DrawPolygon3D
						(
							g_Note[nCountLane][nCountNote].vPos[nCountTexNum].x,
							g_Note[nCountLane][nCountNote].vPos[nCountTexNum].y,
							g_Note[nCountLane][nCountNote].vPos[nCountTexNum].z,
							0,
							0,
							GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]).u,
							GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]).v
						);

						if (g_Note[nCountLane][nCountNote].bHoldBlendFactor)
						{
							SetBlend(D3DBLEND_BLENDFACTOR);
							for (int nCountDrawNum = 0; nCountDrawNum < HOLD_BLEND_FACTOR_DRAW_NUM; nCountDrawNum++)
							{
								SetPolygonColor3D(D3DCOLOR_ARGB(g_Note[nCountLane][nCountNote].nHoldBlendFactorA, 255, 255, 255));
								SetScale3D(1.0f, 1.0f, g_Note[nCountLane][nCountNote].fHoldH[nCountTexNum]);
								DrawPolygon3D
								(
									g_Note[nCountLane][nCountNote].vPos[nCountTexNum].x,
									g_Note[nCountLane][nCountNote].vPos[nCountTexNum].y,
									g_Note[nCountLane][nCountNote].vPos[nCountTexNum].z,
									0,
									0,
									GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]).u,
									GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]).v
								);
							}
							SetBlend(D3DBLEND_INVSRCALPHA);
						}
					}
				}
				break;
			}
			case NT_SLIDE:
			{
				for (int nCountTexNum = 0; nCountTexNum < NDS_MAX; nCountTexNum++)
				{
					if (nCountTexNum != NDS_END)
					{
						SetZBuffer(TRUE);
					}
					else
					{
						SetZBuffer(FALSE);
					}
					if (g_Note[nCountLane][nCountNote].bDrawStatus[nCountTexNum])
					{
						SetTexture(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]);
						if (nCountTexNum == NDS_MIDDLE)
						{
							// 影描画
							if (g_Note[nCountLane][nCountNote].bDrawShadow &&
								g_Note[nCountLane][nCountNote].nTiming[NJS_START] != g_Note[nCountLane][nCountNote].nTiming[NJS_END])
							{
								SetZBuffer(FALSE);
								SetTexture(TEX_SLIDE_SHADOW);
								SetPolygonColor3D(D3DCOLOR_ARGB(SLIDE_SHADOW_A, 0, 0, 0));
								SetPolygon3DVertex
								(
									g_Note[nCountLane][nCountNote].vSlideShadowPos[NSP_LEFT_UP],
									g_Note[nCountLane][nCountNote].vSlideShadowPos[NSP_RIGHT_UP],
									g_Note[nCountLane][nCountNote].vSlideShadowPos[NSP_LEFT_DOWN],
									g_Note[nCountLane][nCountNote].vSlideShadowPos[NSP_RIGHT_DOWN]
								);

								DrawPolygon3D
								(
									0.0f,
									0.0001f,
									0.0f,
									0,
									0,
									GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]).u,
									GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]).v
								);
								SetZBuffer(TRUE);
							}

							// スライド描画
							SetTexture(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]);
							if (g_Note[nCountLane][nCountNote].vPos[NDS_END].y < g_Note[nCountLane][nCountNote].vPos[NDS_START].y)
							{
								SetCullMode(D3DCULL_NONE);
							}
							else
							{
								SetCullMode(D3DCULL_CCW);
							}
							if ((g_Note[nCountLane][nCountNote].vPos[NDS_START].z ==
								g_Note[nCountLane][nCountNote].vPos[NDS_END].z) &&
								(g_Note[nCountLane][nCountNote].nFlickDirection == NFD_UP ||
									g_Note[nCountLane][nCountNote].nFlickDirection == NFD_DOWN))
							{
								SetPolygonColor3D(D3DCOLOR_ARGB(0, 255, 255, 255));
							}
							else
							{
								SetPolygonColor3D(D3DCOLOR_ARGB(g_Note[nCountLane][nCountNote].nHoldA, 255, 255, 255));
							}
							
							SetBlend(D3DBLEND_BLENDFACTOR);
							SetPolygon3DVertex
							(
								g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_LEFT_UP],
								g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_RIGHT_UP],
								g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_LEFT_DOWN],
								g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_RIGHT_DOWN]
							);

							DrawPolygon3D
							(
								0.0f,
								0.0001f,
								0.0f,
								0,
								0,
								GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]).u,
								GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]).v
							);
							SetBlend(D3DBLEND_INVSRCALPHA);

							// 加算合成
							if (g_Note[nCountLane][nCountNote].bHoldBlendFactor && g_Note[nCountLane][nCountNote].nTiming[NJS_START] != g_Note[nCountLane][nCountNote].nTiming[NJS_END])
							{
								SetBlend(D3DBLEND_BLENDFACTOR);
								for (int nCountDrawNum = 0; nCountDrawNum < SLIDE_BLEND_FACTOR_DRAW_NUM; nCountDrawNum++)
								{
									SetPolygonColor3D(D3DCOLOR_ARGB(g_Note[nCountLane][nCountNote].nHoldBlendFactorA, 255, 255, 255));
									SetPolygon3DVertex
									(
										g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_LEFT_UP],
										g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_RIGHT_UP],
										g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_LEFT_DOWN],
										g_Note[nCountLane][nCountNote].vSlideMiddlePos[NSP_RIGHT_DOWN]
									);

									DrawPolygon3D
									(
										0.0f,
										0.0001f,
										0.0f,
										0,
										0,
										GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]).u,
										GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]).v
									);
								}
								SetBlend(D3DBLEND_INVSRCALPHA);
							}
						}
						else
						{
							if (nCountTexNum == NDS_START)
							{
								if (g_Note[nCountLane][nCountNote].bSlideStartEndFlag[NJS_START])
								{
									SetTexture(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]);
								}
								else
								{
									SetTexture(TEX_SLIDE_S_E_MIDDLE);
									if (g_Note[nCountLane][nCountNote].bSlideArrowDrawStatus)
									{
										if (g_Note[nCountLane][nCountNote].nFlickDirection != NFD_UP&&
											g_Note[nCountLane][nCountNote].nFlickDirection != NFD_DOWN)
										{
											SetScale3D(1.0f, 1.0f, 1.0f * (g_fHiSpeed));
										}
									}
								}
							}
							if (nCountTexNum == NDS_END)
							{
								if (g_Note[nCountLane][nCountNote].bSlideStartEndFlag[NJS_END])
								{
									SetTexture(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]);
								}
								else
								{
									SetTexture(TEX_SLIDE_S_E_MIDDLE);
									if (g_Note[nCountLane][nCountNote].bSlideArrowDrawStatus)
									{
										if (g_Note[nCountLane][nCountNote].nFlickDirection != NFD_UP&&
											g_Note[nCountLane][nCountNote].nFlickDirection != NFD_DOWN)
										{
											SetScale3D(1.0f, 1.0f, 1.0f * (g_fHiSpeed));
										}
									}
								}
							}
							SetPolygonColor3D(D3DCOLOR_ARGB(g_Note[nCountLane][nCountNote].nHoldA, 255, 255, 255));
							DrawPolygon3D
							(
								g_Note[nCountLane][nCountNote].vPos[nCountTexNum].x,
								g_Note[nCountLane][nCountNote].vPos[nCountTexNum].y / N,
								g_Note[nCountLane][nCountNote].vPos[nCountTexNum].z,
								0,
								0,
								GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]).u,
								GetTextureStatus(g_Note[nCountLane][nCountNote].nTexNum[nCountTexNum]).v
							);
						}

					}
				}

				if (g_Note[nCountLane][nCountNote].bSlideArrowDrawStatus)
				{
					SetZBuffer(FALSE);
					// 矢印
					int nU;
					int nV;
					float fY;
					float fYChange;
					switch (g_Note[nCountLane][nCountNote].nFlickDirection)
					{
					case NFD_UP:
					case NFD_LEFT:
					case NFD_RIGHT:
					{
						fYChange = 1.0f;
						break;
					}
					case NFD_DOWN:
					{
						fYChange = 0.0f;
						break;
					}
					}
					switch (g_Note[nCountLane][nCountNote].nFlickDirection)
					{
					case NFD_UP:
					case NFD_DOWN:
					{
						nU = GetTextureStatus(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum).u;
						nV = GetTextureStatus(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum).v / gc_fFlickUVScale;
						fY = (NOTE_FLICK_ADD_Y + GetTextureStatus(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum).v) * 0.25f / 100.0f;
						SetScale3D(0.5f, 1.0f, 0.5f);
						break;
					}
					case NFD_LEFT:
					case NFD_RIGHT:
					{
						nU = GetTextureStatus(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum).u / gc_fFlickUVScale;
						nV = GetTextureStatus(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum).v;
						fY = (NOTE_FLICK_ADD_Y + GetTextureStatus(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum).v) * 0.5f / 100.0f;
						SetScale3D(0.5f, 1.0f, 1.0f);
						break;
					}
					}

					float fZ;
					if (g_Note[nCountLane][nCountNote].vPos[NDS_START].z >= JUDGE_LINE_Z)
					{
						fZ = g_Note[nCountLane][nCountNote].vPos[NDS_START].z;
					}
					else
					{
						fZ = JUDGE_LINE_Z;
					}
					// 矢印の描画
					SetTexture(g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nTexNum);
				
					// α値の調整
					if (nCountNote == g_nJudgeNoteNum[nCountLane] + 1 ||
						nCountNote == g_nJudgeNoteNum[nCountLane])
					{
						SetPolygonColor3D(D3DCOLOR_ARGB(SLIDE_ARROW_A_JUDGE, 255, 255, 255));
					}
					else
					{
						SetPolygonColor3D(D3DCOLOR_ARGB(SLIDE_ARROW_A_UNJUDGED, 255, 255, 255));
					}
					SetRot3D(D3DXToRadian(-90), 0, 0);
					DrawPolygon3D
					(
						g_Note[nCountLane][nCountNote].vPos[NDS_START].x,
						(g_Note[nCountLane][nCountNote].vPos[NDS_START].y / N * fYChange) + fY,
						fZ,
						g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nU,
						g_NoteFlickArrow[g_Note[nCountLane][nCountNote].nFlickDirection].nV,
						//0,
						//0,
						nU,
						nV
					);
					SetZBuffer(TRUE);
				}
				break;
			}
			}
		}
	}
	SetZBuffer(TRUE);
}

void SetMaxNote()
{
	g_nMaxNote = 0;
	for (int nCountLane = 0; nCountLane < NUM_BTN; nCountLane++)
	{
		for (int nCountNote = 0; nCountNote < g_nNoteMaxNum[nCountLane]; nCountNote++)
		{
			switch (g_Note[nCountLane][nCountNote].nType)
			{
			case NT_TAP:
			case NT_BREAK:
			case NT_FLICK:
			{
				g_nMaxNote++;
				break;
			}
			case NT_HOLD:
			case NT_SLIDE:
			{
				g_nMaxNote += g_Note[nCountLane][nCountNote].nHoldNum;
				break;
			}
			}
		}
	}
}

int GetMaxNote()
{
	return g_nMaxNote;
}

void DrawDebugMaxNote()
{
	DebugText(0, 198, "MaxNote: %d", g_nMaxNote);
}

int GetNoteMaxNum(int nNum)
{
	return g_nNoteMaxNum[nNum];
}

int GetJudgeNoteNum(int nNum)
{
	return g_nJudgeNoteNum[nNum];
}

void SetJudgeNoteNum(int nNum, int nChangeNum)
{
	g_nJudgeNoteNum[nNum] += nChangeNum;
}

void SetNoteJudgeStatus(int nNum, int nNoteNum, int nParts, bool bStatus)
{
	g_Note[nNum][nNoteNum].bJudgeStatus[nParts] = bStatus;
}

void SetNoteDrawStatus(int nNum, int nNoteNum, int nParts, bool bStatus)
{
	g_Note[nNum][nNoteNum].bDrawStatus[nParts] = bStatus;
}

NOTE GetNote(int nNum, int nNoteNum)
{
	return g_Note[nNum][nNoteNum];
}

void SetHoldMiddleTime(int nNum, int nNoteNum, int nTimeChange)
{
	g_Note[nNum][nNoteNum].nTimingHoldMiddle += nTimeChange;
}

void SetHoldAlpha(int nNum, int nNoteNum, int nA)
{
	g_Note[nNum][nNoteNum].nHoldA = nA;
}

void SetHoldBlendFactor(int nNum, int nNoteNum, bool bStatus)
{
	g_Note[nNum][nNoteNum].bHoldBlendFactor = bStatus;
	if (g_Note[nNum][nNoteNum].bHoldBlendFactor)
	{
		g_Note[nNum][nNoteNum].nHoldBlendFactorA = HOLD_BLEND_FACTOR_MIN_A;
		g_Note[nNum][nNoteNum].nCountHoldBlendFactor = 0;
	}
}

void UpdateHoldBlendFactor(int nNum, int nNoteNum)
{
	// α加算合成
	if (g_Note[nNum][nNoteNum].bHoldBlendFactor)
	{
		g_Note[nNum][nNoteNum].nCountHoldBlendFactor++;
		if (g_Note[nNum][nNoteNum].nCountHoldBlendFactor < HOLD_BLEND_FACTOR_TIME)
		{
			g_Note[nNum][nNoteNum].nHoldBlendFactorA += (float)(HOLD_BLEND_FACTOR_MAX_A - HOLD_BLEND_FACTOR_MIN_A) / HOLD_BLEND_FACTOR_TIME;
		}
		if (g_Note[nNum][nNoteNum].nCountHoldBlendFactor == HOLD_BLEND_FACTOR_TIME)
		{
			g_Note[nNum][nNoteNum].nHoldBlendFactorA = HOLD_BLEND_FACTOR_MAX_A;
		}
		if (g_Note[nNum][nNoteNum].nCountHoldBlendFactor > HOLD_BLEND_FACTOR_TIME&&
			g_Note[nNum][nNoteNum].nCountHoldBlendFactor < HOLD_BLEND_FACTOR_TIME * 2)
		{
			g_Note[nNum][nNoteNum].nHoldBlendFactorA += (float)(-HOLD_BLEND_FACTOR_MAX_A + HOLD_BLEND_FACTOR_MIN_A) / HOLD_BLEND_FACTOR_TIME;
		}
		if (g_Note[nNum][nNoteNum].nCountHoldBlendFactor == HOLD_BLEND_FACTOR_TIME * 2)
		{
			g_Note[nNum][nNoteNum].nHoldBlendFactorA = HOLD_BLEND_FACTOR_MIN_A;
			g_Note[nNum][nNoteNum].nCountHoldBlendFactor = 0;
		}
	}
}

void InitNoteFlickArrow()
{
	for (int nCountArrow = 0; nCountArrow < NFD_MAX; nCountArrow++)
	{
		g_NoteFlickArrow[nCountArrow].nCount = 0;
		g_NoteFlickArrow[nCountArrow].nU = 0;
		g_NoteFlickArrow[nCountArrow].nV = 0;
	}
	g_NoteFlickArrow[NFD_UP].nTexNum = TEX_FLICK_ARROW_UP;
	g_NoteFlickArrow[NFD_DOWN].nTexNum = TEX_FLICK_ARROW_DOWN;
	g_NoteFlickArrow[NFD_LEFT].nTexNum = TEX_FLICK_ARROW_LEFT;
	g_NoteFlickArrow[NFD_RIGHT].nTexNum = TEX_FLICK_ARROW_RIGHT;
}

void UpdateNoteFlickArrow()
{
	for (int nCountArrow = 0; nCountArrow < NFD_MAX; nCountArrow++)
	{
		D3DXVECTOR2 vUV;
			
		switch (nCountArrow)
		{
		case NFD_UP:
		{
			vUV.x = 0.0f;
			vUV.y = 1.0f;
			break;
		}
		case NFD_DOWN:
		{
			vUV.x = 0.0f;
			vUV.y = -1.0f;
			break;
		}
		case NFD_LEFT:
		{
			vUV.x = 1.0f;
			vUV.y = 0.0f;
			break;
		}
		case NFD_RIGHT:
		{
			vUV.x = -1.0f;
			vUV.y = 0.0f;
			break;
		}
		}
		g_NoteFlickArrow[nCountArrow].nCount++;
		if (g_NoteFlickArrow[nCountArrow].nCount < NOTE_FLICK_ARROW_TIME)
		{
			g_NoteFlickArrow[nCountArrow].nU += vUV.x *
				(GetTextureStatus(g_NoteFlickArrow[nCountArrow].nTexNum).u * 1.0f / (NOTE_FLICK_ARROW_TIME * 1.0f));
			g_NoteFlickArrow[nCountArrow].nV += vUV.y *
				(GetTextureStatus(g_NoteFlickArrow[nCountArrow].nTexNum).v * 1.0f / (NOTE_FLICK_ARROW_TIME * 1.0f));
		}
		if (g_NoteFlickArrow[nCountArrow].nCount == NOTE_FLICK_ARROW_TIME)
		{
			g_NoteFlickArrow[nCountArrow].nCount = 0;
			g_NoteFlickArrow[nCountArrow].nU += vUV.x * GetTextureStatus(g_NoteFlickArrow[nCountArrow].nTexNum).u;
			g_NoteFlickArrow[nCountArrow].nV += vUV.y * GetTextureStatus(g_NoteFlickArrow[nCountArrow].nTexNum).v;
		}
	}
}

D3DXVECTOR3 GetSlidePos(int nNum)
{
	int nTime = GetTimer();
	if (g_Note[nNum][g_nJudgeNoteNum[nNum]].nType == NT_SLIDE &&
		g_Note[nNum][g_nJudgeNoteNum[nNum]].nTiming[NJS_START] - nTime <= DARW_SP_JUDGE_LINE_TIME)
	{
		D3DXVECTOR3 vEndPos;
		if (g_Note[nNum][g_nJudgeNoteNum[nNum]].nTiming[NJS_START] <= nTime)
		{
			vEndPos = g_Note[nNum][g_nJudgeNoteNum[nNum]].vPos[NDS_START];
			float fN = (float)(nTime - g_Note[nNum][g_nJudgeNoteNum[nNum]].nTiming[NJS_START]) /
				(g_Note[nNum][g_nJudgeNoteNum[nNum]].nTiming[NJS_END] - g_Note[nNum][g_nJudgeNoteNum[nNum]].nTiming[NJS_START]);
			vEndPos += (g_Note[nNum][g_nJudgeNoteNum[nNum]].vPos[NDS_END] - g_Note[nNum][g_nJudgeNoteNum[nNum]].vPos[NDS_START])*fN;
		}
		else
		{
			vEndPos = g_Note[nNum][g_nJudgeNoteNum[nNum]].vPos[NDS_START];
		}
		if (vEndPos.x > NOTE_X + (nNum - NUM_BTN_NORMAL) * NOTE_RANGE_X + NOTE_RANGE_X * 2.0f)
		{
			vEndPos.x = NOTE_X + (nNum - NUM_BTN_NORMAL) * NOTE_RANGE_X + NOTE_RANGE_X * 2.0f;
		}
		if (vEndPos.x < NOTE_X + (nNum - NUM_BTN_NORMAL) * NOTE_RANGE_X)
		{
			vEndPos.x = NOTE_X + (nNum - NUM_BTN_NORMAL) * NOTE_RANGE_X;
		}
		if (vEndPos.y > NOTE_SLIDE_Y_MAX)
		{
			vEndPos.y = NOTE_SLIDE_Y_MAX;
		}
		if (vEndPos.y < NOTE_SLIDE_Y_MIN)
		{
			vEndPos.y = NOTE_SLIDE_Y_MIN;
		}
		return vEndPos;
	}
	else
	{
		D3DXVECTOR3 vEndPos = { 0.0f, -100.0f, 0.0f };
		return vEndPos;
	}
}

void SetNoteLineAndNum(int nLaneNum, int nNoteNum, int nLine, int nNum, int nNJSNum)
{
	g_Note[nLaneNum][nNoteNum].nNumOfLine[nNJSNum] = nLine;
	g_Note[nLaneNum][nNoteNum].nNumOfNote[nNJSNum] = nNum;
}

void SetNoteType(int nLaneNum, int nNoteNum, int nType)
{
	g_Note[nLaneNum][nNoteNum].nType = nType;
}

void SetMaxNotePerCh(int nLaneNum, int nNoteNum)
{
	g_nNoteMaxNum[nLaneNum] = nNoteNum;
}

void SetNoteTime(int nLaneNum, int nNoteNum, int nTime, int nNJSNum)
{
	g_Note[nLaneNum][nNoteNum].nTiming[nNJSNum] = nTime;
}

void SetNoteDirect(int nLaneNum, int nNoteNum, int nDirect)
{
	g_Note[nLaneNum][nNoteNum].nFlickDirection = nDirect;
}

void SetSlidePointPosNum(int nLaneNum, int nNoteNum, int nPosNum, int nNJSNum)
{
	g_Note[nLaneNum][nNoteNum].nSlidePointPosNum[nNJSNum] = nPosNum;
}

void SetSlideStartEndFlag(int nLaneNum, int nNoteNum, bool bFlag, int nNJSNum)
{
	g_Note[nLaneNum][nNoteNum].bSlideStartEndFlag[nNJSNum] = bFlag;
}

void SetNoteOffset(int nChange)
{
	for (int nCountLane = 0; nCountLane < NUM_BTN; nCountLane++)
	{
		for (int nCountNote = 0; nCountNote < g_nNoteMaxNum[nCountLane]; nCountNote++)
		{
			for (int nCountNJS = 0; nCountNJS < NJS_MAX; nCountNJS++)
			{
				g_Note[nCountLane][nCountNote].nTiming[nCountNJS] += nChange;
				g_Note[nCountLane][nCountNote].nTimingHoldMiddle += nChange;
				g_Note[nCountLane][nCountNote].nTimingHoldMiddleMax += nChange;
			}
		}
	}
}

void ResetNoteStatus()
{
	for (int nCountLane = 0; nCountLane < NUM_BTN; nCountLane++)
	{
		g_nJudgeNoteNum[nCountLane] = 0;
		for (int nCountNote = 0; nCountNote < g_nNoteMaxNum[nCountLane]; nCountNote++)
		{
			for (int nCountNJS = 0; nCountNJS < NJS_MAX; nCountNJS++)
			{
				g_Note[nCountLane][nCountNote].bJudgeStatus[nCountNJS] = false;
			}
			g_Note[nCountLane][nCountNote].nHoldA = HOLD_MAX_A;
			g_Note[nCountLane][nCountNote].nHoldBlendFactorA = HOLD_BLEND_FACTOR_MIN_A;
			g_Note[nCountLane][nCountNote].nHoldNum =
				(g_Note[nCountLane][nCountNote].nTiming[NJS_END] -
					g_Note[nCountLane][nCountNote].nTiming[NJS_START]) / TIME_PER_HOLD + 1;

			g_Note[nCountLane][nCountNote].nTimingHoldMiddle = g_Note[nCountLane][nCountNote].nTiming[NJS_START] + TIME_PER_HOLD;
			g_Note[nCountLane][nCountNote].nTimingHoldMiddleMax = g_Note[nCountLane][nCountNote].nTiming[NJS_START] + TIME_PER_HOLD * (g_Note[nCountLane][nCountNote].nHoldNum);
		}
	}
}

void SetMaxNotePerType()
{
	for (int nCountType = 0; nCountType < NT_MAX; nCountType++)
	{
		g_nMaxNotePerType[nCountType] = 0;
	}
	for (int nCountLane = 0; nCountLane < NUM_BTN; nCountLane++)
	{
		for (int nCountNote = 0; nCountNote < g_nNoteMaxNum[nCountLane]; nCountNote++)
		{
			switch (g_Note[nCountLane][nCountNote].nType)
			{
			case NT_TAP:
			case NT_BREAK:
			case NT_FLICK:
			{
				g_nMaxNotePerType[g_Note[nCountLane][nCountNote].nType]++;
				break;												  
			}
			case NT_HOLD:
			case NT_SLIDE:
			{
				g_nMaxNotePerType[g_Note[nCountLane][nCountNote].nType] += g_Note[nCountLane][nCountNote].nHoldNum;
				break;
			}
			}
		}
	}
}

int GetNoteNumPerType(int nType)
{
	return g_nMaxNotePerType[nType];
}

float GetHiSpeed()
{
	return g_fHiSpeed;
}

void SetHiSpeed(float fAdd)
{
	g_fHiSpeed = fAdd;
}

void SetNoteSlideNum(int nLaneNum, int nNoteNum, int nSlideNum)
{
	g_Note[nLaneNum][nNoteNum].nSlideNum = nSlideNum;
}

void UpdateBreakEffect()
{
	g_nBreakEffectCount++;
	if (g_nBreakEffectCount <= gc_nBreakEffectCountMax)
	{
		g_nBreakEffectA += (gc_nBreakEffectMaxA - gc_nBreakEffectMinA) * 1.0f / (gc_nBreakEffectCountMax * 1.0f);
	}
	else
		if (g_nBreakEffectCount <= gc_nBreakEffectCountMax * 2)
		{
			g_nBreakEffectA += (-gc_nBreakEffectMaxA + gc_nBreakEffectMinA) * 1.0f / (gc_nBreakEffectCountMax * 1.0f);
		}
	if (g_nBreakEffectA > gc_nBreakEffectMaxA)
	{
		g_nBreakEffectA = gc_nBreakEffectMaxA;
	}
	if (g_nBreakEffectA < gc_nBreakEffectMinA)
	{
		g_nBreakEffectA = gc_nBreakEffectMinA;
	}
	if (g_nBreakEffectCount == gc_nBreakEffectCountMax * 2)
	{
		g_nBreakEffectCount = 0;
	}
}

int GetOffsetNote()
{
	return g_nOffset;
}

void SetOffsetNote(int nAdd)
{
	g_nOffset = nAdd;
}