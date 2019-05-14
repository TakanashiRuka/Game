//----------------------------------------------------------------------------
//
//	後書きの背景
//
//														 制作者：劉喆
//                                                       作成日：2017/02/28
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "ContinueBG.h"
#include "sound.h"

//---------------------------マクロ定義---------------------------------------

typedef enum {
	CBP_BG,
	CBP_WORD,
	CBP_MAX,
}CONTINUE_BG_PARTS;

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;
	int nA;
	D3DXVECTOR2 vScale;
	D3DXVECTOR2 vUV;
	TEXTURE tTexture;
	int nCountFadeIn;
	int nCountAnimation;
}CONTINUE_BG;

//---------------------------グローバル変数-----------------------------------
//----------------------------------------------------------------------------
// バージョン
//----------------------------------------------------------------------------

static CONTINUE_BG g_ContinueBG[CBP_MAX];
static const float gc_fContinueWordAddY = 0.0f;

static const int gc_nContinueBGTextureNum[CBP_MAX] =													// タイトルのテクスチャ番号
{
	TEX_THANKS_BG,
	TEX_CONTINUE_WORD,
};

static const D3DXVECTOR2 gc_vContinueBGPos[CBP_MAX][RDP_MAX] =										   // タイトルの座標
{
	{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f } },
	{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + gc_fContinueWordAddY } },
};

static const int gc_nContinueBGA[CBP_MAX][RDP_MAX] =												   // タイトルのα値
{
	{ 255,255 },
	{ 0,255 },
};

static const D3DXVECTOR2 gc_vContinueBGScale[CBP_MAX][RDP_MAX] =									   // タイトルのスケーリング
{
	{ { 1.0f,0.0f },{ 1.0f,1.0f } },
	{ { 1.0f,1.0f },{ 1.0f,1.0f } },
};

static const int gc_nContinueBGFadeInCountMax[CBP_MAX] =											   // タイトルのフェードインのカウンターの最大値
{
	8,
	8,
};

static const int gc_nContinueBGFadeInStartFrame[CBP_MAX] =											  // タイトルのアニメーションの開始フレーム
{
	16,
	20,
};

// アニメーション部分
static const D3DXVECTOR2 gc_vContinueBGChangeUV[CBP_MAX] =
{
	{ 0.5f,0.0f },
	{ 0.0f,0.0f },
};

static const D3DXVECTOR2 gc_vContinueBGTargetPos[CBP_MAX] =
{
	{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },
	{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + gc_fContinueWordAddY },
};

static const D3DXVECTOR2 gc_vContinueBGTargetScale[CBP_MAX] =
{
	{ 1.0f,1.0f },
	{ 1.0f,1.0f },
};

static const int gc_nContinueBGAnimationTargetA[CBP_MAX] =
{
	255,
	255,
};

static const int gc_nContinueBGAnimationMaxFrame[CBP_MAX] =
{
	0,
	0,
};

static int gc_nContinueAlphaBlend[CBP_MAX] =
{
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_INVSRCALPHA,
};

static const int gc_nContinueBGFadeInStartFrameCountMax = 20;
static int g_nContinueBGFadeInStartFrameCount;
static bool g_bPlayStatus;

//---------------------------プロトタイプ宣言---------------------------------
void InitContinueBG()
{
	g_bPlayStatus = false;
	g_nContinueBGFadeInStartFrameCount = 0;

	for (int nCountContinueBGParts = 0; nCountContinueBGParts < CBP_MAX; nCountContinueBGParts++)
	{
		g_ContinueBG[nCountContinueBGParts].nA = gc_nContinueBGA[nCountContinueBGParts][RDP_INIT];
		g_ContinueBG[nCountContinueBGParts].nCountFadeIn = 0;
		g_ContinueBG[nCountContinueBGParts].nCountAnimation = 0;
		g_ContinueBG[nCountContinueBGParts].vPos = gc_vContinueBGPos[nCountContinueBGParts][RDP_INIT];
		g_ContinueBG[nCountContinueBGParts].vScale = gc_vContinueBGScale[nCountContinueBGParts][RDP_INIT];
		g_ContinueBG[nCountContinueBGParts].vUV = { 0.0f,0.0f };
		g_ContinueBG[nCountContinueBGParts].tTexture = GetTextureStatus(gc_nContinueBGTextureNum[nCountContinueBGParts]);
	}
}

void UninitContinueBG()
{

}

void UpdateContinueBG()
{
	if (!g_bPlayStatus && g_nContinueBGFadeInStartFrameCount == gc_nContinueBGFadeInStartFrameCountMax / 2)
	{
		g_bPlayStatus = true;
		PlaySound(SE_FINAL_CHECK);
	}
	if (g_nContinueBGFadeInStartFrameCount < gc_nContinueBGFadeInStartFrameCountMax)
	{
		g_nContinueBGFadeInStartFrameCount++;
	}
	for (int nCountContinueBGParts = 0; nCountContinueBGParts < CBP_MAX; nCountContinueBGParts++)
	{
		if (g_nContinueBGFadeInStartFrameCount >= gc_nContinueBGFadeInStartFrame[nCountContinueBGParts])
		{
			if (g_ContinueBG[nCountContinueBGParts].nCountFadeIn < gc_nContinueBGFadeInCountMax[nCountContinueBGParts])
			{
				g_ContinueBG[nCountContinueBGParts].nCountFadeIn++;

				g_ContinueBG[nCountContinueBGParts].nA += (gc_nContinueBGA[nCountContinueBGParts][RDP_END] - gc_nContinueBGA[nCountContinueBGParts][RDP_INIT]) * 1.0f /
					(gc_nContinueBGFadeInCountMax[nCountContinueBGParts] * 1.0f);

				g_ContinueBG[nCountContinueBGParts].vPos += (gc_vContinueBGPos[nCountContinueBGParts][RDP_END] - gc_vContinueBGPos[nCountContinueBGParts][RDP_INIT]) /
					(gc_nContinueBGFadeInCountMax[nCountContinueBGParts] * 1.0f);

				g_ContinueBG[nCountContinueBGParts].vScale += (gc_vContinueBGScale[nCountContinueBGParts][RDP_END] - gc_vContinueBGScale[nCountContinueBGParts][RDP_INIT]) /
					(gc_nContinueBGFadeInCountMax[nCountContinueBGParts]);

				if (g_ContinueBG[nCountContinueBGParts].nCountFadeIn == gc_nContinueBGFadeInCountMax[nCountContinueBGParts])
				{
					g_ContinueBG[nCountContinueBGParts].nA = gc_nContinueBGA[nCountContinueBGParts][RDP_END];
					g_ContinueBG[nCountContinueBGParts].vPos = gc_vContinueBGPos[nCountContinueBGParts][RDP_END];
					g_ContinueBG[nCountContinueBGParts].vScale = gc_vContinueBGScale[nCountContinueBGParts][RDP_END];
				}
			}
			else
			{
				g_ContinueBG[nCountContinueBGParts].vUV += gc_vContinueBGChangeUV[nCountContinueBGParts];
				g_ContinueBG[nCountContinueBGParts].nCountAnimation++;
				if (g_ContinueBG[nCountContinueBGParts].nCountAnimation < gc_nContinueBGAnimationMaxFrame[nCountContinueBGParts])
				{
					g_ContinueBG[nCountContinueBGParts].nA += (gc_nContinueBGAnimationTargetA[nCountContinueBGParts] - gc_nContinueBGA[nCountContinueBGParts][RDP_END]) * 1.0f /
						(gc_nContinueBGAnimationMaxFrame[nCountContinueBGParts] * 1.0f);
					g_ContinueBG[nCountContinueBGParts].vPos += (gc_vContinueBGTargetPos[nCountContinueBGParts] - gc_vContinueBGPos[nCountContinueBGParts][RDP_END]) /
						(gc_nContinueBGAnimationMaxFrame[nCountContinueBGParts] * 1.0f);
					g_ContinueBG[nCountContinueBGParts].vScale += (gc_vContinueBGTargetScale[nCountContinueBGParts] - gc_vContinueBGScale[nCountContinueBGParts][RDP_END]) /
						(gc_nContinueBGAnimationMaxFrame[nCountContinueBGParts] * 1.0f);
				}
				else
					if (g_ContinueBG[nCountContinueBGParts].nCountAnimation == gc_nContinueBGAnimationMaxFrame[nCountContinueBGParts])
					{
						g_ContinueBG[nCountContinueBGParts].nA = gc_nContinueBGAnimationTargetA[nCountContinueBGParts];
						g_ContinueBG[nCountContinueBGParts].vPos = gc_vContinueBGTargetPos[nCountContinueBGParts];
						g_ContinueBG[nCountContinueBGParts].vScale = gc_vContinueBGTargetScale[nCountContinueBGParts];
					}
					else
						if (g_ContinueBG[nCountContinueBGParts].nCountAnimation < 2 * gc_nContinueBGAnimationMaxFrame[nCountContinueBGParts])
						{
							g_ContinueBG[nCountContinueBGParts].nA += (-gc_nContinueBGAnimationTargetA[nCountContinueBGParts] + gc_nContinueBGA[nCountContinueBGParts][RDP_END]) * 1.0f /
								(gc_nContinueBGAnimationMaxFrame[nCountContinueBGParts] * 1.0f);
							g_ContinueBG[nCountContinueBGParts].vPos += (-gc_vContinueBGTargetPos[nCountContinueBGParts] + gc_vContinueBGPos[nCountContinueBGParts][RDP_END]) /
								(gc_nContinueBGAnimationMaxFrame[nCountContinueBGParts] * 1.0f);
							g_ContinueBG[nCountContinueBGParts].vScale += (-gc_vContinueBGTargetScale[nCountContinueBGParts] + gc_vContinueBGScale[nCountContinueBGParts][RDP_END]) /
								(gc_nContinueBGAnimationMaxFrame[nCountContinueBGParts] * 1.0f);
						}
						else
							if (g_ContinueBG[nCountContinueBGParts].nCountAnimation == 2 * gc_nContinueBGAnimationMaxFrame[nCountContinueBGParts])
							{
								g_ContinueBG[nCountContinueBGParts].nA = gc_nContinueBGA[nCountContinueBGParts][RDP_END];
								g_ContinueBG[nCountContinueBGParts].vPos = gc_vContinueBGPos[nCountContinueBGParts][RDP_END];
								g_ContinueBG[nCountContinueBGParts].vScale = gc_vContinueBGScale[nCountContinueBGParts][RDP_END];

								g_ContinueBG[nCountContinueBGParts].nCountAnimation = 0;
							}
				if (g_ContinueBG[nCountContinueBGParts].nA < gc_nContinueBGA[nCountContinueBGParts][RDP_END])
				{
					g_ContinueBG[nCountContinueBGParts].nA = gc_nContinueBGA[nCountContinueBGParts][RDP_END];
				}
				if (g_ContinueBG[nCountContinueBGParts].nA > gc_nContinueBGAnimationTargetA[nCountContinueBGParts])
				{
					g_ContinueBG[nCountContinueBGParts].nA = gc_nContinueBGAnimationTargetA[nCountContinueBGParts];
				}
			}
		}
	}
}

void DrawContinueBG()
{
	SetZBuffer(FALSE);
	for (int nCountContinueBGParts = 0; nCountContinueBGParts < CBP_MAX; nCountContinueBGParts++)
	{
		if (g_nContinueBGFadeInStartFrameCount >= gc_nContinueBGFadeInStartFrame[nCountContinueBGParts])
		{
			SetBlend(gc_nContinueAlphaBlend[nCountContinueBGParts]);
			SetTexture(gc_nContinueBGTextureNum[nCountContinueBGParts]);
			SetPolygonColor(D3DCOLOR_ARGB(g_ContinueBG[nCountContinueBGParts].nA, 255, 255, 255));
			SetScale
			(
				g_ContinueBG[nCountContinueBGParts].vScale.x,
				g_ContinueBG[nCountContinueBGParts].vScale.y,
				g_ContinueBG[nCountContinueBGParts].tTexture.u / 2.0f,
				g_ContinueBG[nCountContinueBGParts].tTexture.v / 2.0f
			);
			DrawPolygon
			(
				g_ContinueBG[nCountContinueBGParts].vPos.x,
				g_ContinueBG[nCountContinueBGParts].vPos.y,
				g_ContinueBG[nCountContinueBGParts].vUV.x,
				g_ContinueBG[nCountContinueBGParts].vUV.y,
				g_ContinueBG[nCountContinueBGParts].tTexture.u,
				g_ContinueBG[nCountContinueBGParts].tTexture.v
			);
		}
	}
	SetBlend(D3DBLEND_INVSRCALPHA);
	SetZBuffer(TRUE);
}
