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
#include "ThanksBG.h"

//---------------------------マクロ定義---------------------------------------

typedef enum {
	TSBP_BG,
	TSBP_WORD,
	TSBP_MASK,
	TSBP_MAX,
}THANKS_BG_PARTS;

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;
	int nA;
	D3DXVECTOR2 vScale;
	D3DXVECTOR2 vUV;
	TEXTURE tTexture;
	int nCountFadeIn;
	int nCountAnimation;
}THANKS_BG;

//---------------------------グローバル変数-----------------------------------
//----------------------------------------------------------------------------
// バージョン
//----------------------------------------------------------------------------

static THANKS_BG g_ThanksBG[TSBP_MAX];
static const float gc_fThanksWordAddY = 12.5f;

static const int gc_nThanksBGTextureNum[TSBP_MAX] =													// タイトルのテクスチャ番号
{
	TEX_THANKS_BG,
	TEX_THANKS_WORD,
	TEX_THANKS_MASK,
};

static const D3DXVECTOR2 gc_vThanksBGPos[TSBP_MAX][RDP_MAX] =										   // タイトルの座標
{
	{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f } },
	{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + gc_fThanksWordAddY } },
	{ { 0.0f, 0.0f },{ 0.0f, 0.0f } },
};

static const int gc_nThanksBGA[TSBP_MAX][RDP_MAX] =												   // タイトルのα値
{
	{ 255,255 },
	{ 0,255 },
	{ 0,255 },
};

static const D3DXVECTOR2 gc_vThanksBGScale[TSBP_MAX][RDP_MAX] =									   // タイトルのスケーリング
{
	{ { 1.0f,0.0f },{ 1.0f,1.0f } },
	{ { 1.0f,1.0f },{ 1.0f,1.0f } },
	{ { 1280.0f,720.0f },{ 1280.0f,720.0f } },
};

static const int gc_nThanksBGFadeInCountMax[TSBP_MAX] =											   // タイトルのフェードインのカウンターの最大値
{
	8,
	8,
	60,
};

static const int gc_nThanksBGFadeInStartFrame[TSBP_MAX] =											  // タイトルのアニメーションの開始フレーム
{
	16,
	20,
	60 * 5,
};

// アニメーション部分
static const D3DXVECTOR2 gc_vThanksBGChangeUV[TSBP_MAX] =
{
	{ 0.5f,0.0f },
	{ 0.0f,0.0f },
	{ 0.0f,0.0f },
};

static const D3DXVECTOR2 gc_vThanksBGTargetPos[TSBP_MAX] =
{
	{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },
	{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + gc_fThanksWordAddY },
	{ 0.0f, 0.0f },
};

static const D3DXVECTOR2 gc_vThanksBGTargetScale[TSBP_MAX] =
{
	{ 1.0f,1.0f },
	{ 1.0f,1.0f },
	{ 1280.0f,720.0f },
};

static const int gc_nThanksBGAnimationTargetA[TSBP_MAX] =
{
	255,
	255,
	255,
};

static const int gc_nThanksBGAnimationMaxFrame[TSBP_MAX] =
{
	0,
	0,
	0,
};

static int gc_nThanksAlphaBlend[TSBP_MAX] =
{
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_INVSRCALPHA,
};

static const int gc_nThanksBGFadeInStartFrameCountMax = 6 * 60;
static int g_nThanksBGFadeInStartFrameCount;

//---------------------------プロトタイプ宣言---------------------------------
void InitThanksBG()
{
	g_nThanksBGFadeInStartFrameCount = 0;

	for (int nCountThanksBGParts = 0; nCountThanksBGParts < TSBP_MAX; nCountThanksBGParts++)
	{
		g_ThanksBG[nCountThanksBGParts].nA = gc_nThanksBGA[nCountThanksBGParts][RDP_INIT];
		g_ThanksBG[nCountThanksBGParts].nCountFadeIn = 0;
		g_ThanksBG[nCountThanksBGParts].nCountAnimation = 0;
		g_ThanksBG[nCountThanksBGParts].vPos = gc_vThanksBGPos[nCountThanksBGParts][RDP_INIT];
		g_ThanksBG[nCountThanksBGParts].vScale = gc_vThanksBGScale[nCountThanksBGParts][RDP_INIT];
		g_ThanksBG[nCountThanksBGParts].vUV = { 0.0f,0.0f };
		g_ThanksBG[nCountThanksBGParts].tTexture = GetTextureStatus(gc_nThanksBGTextureNum[nCountThanksBGParts]);
	}
	//PlaySound(BGM_Thanks_P2);
	//StopSound(BGM_Thanks_P2);
	//PlaySound(BGM_Thanks_P1);
}

void UninitThanksBG()
{
	//StopSound(BGM_Thanks_P1);
	//StopSound(BGM_Thanks_P2);
}

void UpdateThanksBG()
{
	if (g_nThanksBGFadeInStartFrameCount < gc_nThanksBGFadeInStartFrameCountMax)
	{
		g_nThanksBGFadeInStartFrameCount++;
	}
	for (int nCountThanksBGParts = 0; nCountThanksBGParts < TSBP_MAX; nCountThanksBGParts++)
	{
		if (g_nThanksBGFadeInStartFrameCount >= gc_nThanksBGFadeInStartFrame[nCountThanksBGParts])
		{
			if (g_ThanksBG[nCountThanksBGParts].nCountFadeIn < gc_nThanksBGFadeInCountMax[nCountThanksBGParts])
			{
				g_ThanksBG[nCountThanksBGParts].nCountFadeIn++;

				g_ThanksBG[nCountThanksBGParts].nA += (gc_nThanksBGA[nCountThanksBGParts][RDP_END] - gc_nThanksBGA[nCountThanksBGParts][RDP_INIT]) * 1.0f /
					(gc_nThanksBGFadeInCountMax[nCountThanksBGParts] * 1.0f);

				g_ThanksBG[nCountThanksBGParts].vPos += (gc_vThanksBGPos[nCountThanksBGParts][RDP_END] - gc_vThanksBGPos[nCountThanksBGParts][RDP_INIT]) /
					(gc_nThanksBGFadeInCountMax[nCountThanksBGParts] * 1.0f);

				g_ThanksBG[nCountThanksBGParts].vScale += (gc_vThanksBGScale[nCountThanksBGParts][RDP_END] - gc_vThanksBGScale[nCountThanksBGParts][RDP_INIT]) /
					(gc_nThanksBGFadeInCountMax[nCountThanksBGParts]);

				if (g_ThanksBG[nCountThanksBGParts].nCountFadeIn == gc_nThanksBGFadeInCountMax[nCountThanksBGParts])
				{
					g_ThanksBG[nCountThanksBGParts].nA = gc_nThanksBGA[nCountThanksBGParts][RDP_END];
					g_ThanksBG[nCountThanksBGParts].vPos = gc_vThanksBGPos[nCountThanksBGParts][RDP_END];
					g_ThanksBG[nCountThanksBGParts].vScale = gc_vThanksBGScale[nCountThanksBGParts][RDP_END];
				}
			}
			else
			{
				g_ThanksBG[nCountThanksBGParts].vUV += gc_vThanksBGChangeUV[nCountThanksBGParts];
				g_ThanksBG[nCountThanksBGParts].nCountAnimation++;
				if (g_ThanksBG[nCountThanksBGParts].nCountAnimation < gc_nThanksBGAnimationMaxFrame[nCountThanksBGParts])
				{
					g_ThanksBG[nCountThanksBGParts].nA += (gc_nThanksBGAnimationTargetA[nCountThanksBGParts] - gc_nThanksBGA[nCountThanksBGParts][RDP_END]) * 1.0f /
						(gc_nThanksBGAnimationMaxFrame[nCountThanksBGParts] * 1.0f);
					g_ThanksBG[nCountThanksBGParts].vPos += (gc_vThanksBGTargetPos[nCountThanksBGParts] - gc_vThanksBGPos[nCountThanksBGParts][RDP_END]) /
						(gc_nThanksBGAnimationMaxFrame[nCountThanksBGParts] * 1.0f);
					g_ThanksBG[nCountThanksBGParts].vScale += (gc_vThanksBGTargetScale[nCountThanksBGParts] - gc_vThanksBGScale[nCountThanksBGParts][RDP_END]) /
						(gc_nThanksBGAnimationMaxFrame[nCountThanksBGParts] * 1.0f);
				}
				else
					if (g_ThanksBG[nCountThanksBGParts].nCountAnimation == gc_nThanksBGAnimationMaxFrame[nCountThanksBGParts])
					{
						g_ThanksBG[nCountThanksBGParts].nA = gc_nThanksBGAnimationTargetA[nCountThanksBGParts];
						g_ThanksBG[nCountThanksBGParts].vPos = gc_vThanksBGTargetPos[nCountThanksBGParts];
						g_ThanksBG[nCountThanksBGParts].vScale = gc_vThanksBGTargetScale[nCountThanksBGParts];
					}
					else
						if (g_ThanksBG[nCountThanksBGParts].nCountAnimation < 2 * gc_nThanksBGAnimationMaxFrame[nCountThanksBGParts])
						{
							g_ThanksBG[nCountThanksBGParts].nA += (-gc_nThanksBGAnimationTargetA[nCountThanksBGParts] + gc_nThanksBGA[nCountThanksBGParts][RDP_END]) * 1.0f /
								(gc_nThanksBGAnimationMaxFrame[nCountThanksBGParts] * 1.0f);
							g_ThanksBG[nCountThanksBGParts].vPos += (-gc_vThanksBGTargetPos[nCountThanksBGParts] + gc_vThanksBGPos[nCountThanksBGParts][RDP_END]) /
								(gc_nThanksBGAnimationMaxFrame[nCountThanksBGParts] * 1.0f);
							g_ThanksBG[nCountThanksBGParts].vScale += (-gc_vThanksBGTargetScale[nCountThanksBGParts] + gc_vThanksBGScale[nCountThanksBGParts][RDP_END]) /
								(gc_nThanksBGAnimationMaxFrame[nCountThanksBGParts] * 1.0f);
						}
						else
							if (g_ThanksBG[nCountThanksBGParts].nCountAnimation == 2 * gc_nThanksBGAnimationMaxFrame[nCountThanksBGParts])
							{
								g_ThanksBG[nCountThanksBGParts].nA = gc_nThanksBGA[nCountThanksBGParts][RDP_END];
								g_ThanksBG[nCountThanksBGParts].vPos = gc_vThanksBGPos[nCountThanksBGParts][RDP_END];
								g_ThanksBG[nCountThanksBGParts].vScale = gc_vThanksBGScale[nCountThanksBGParts][RDP_END];

								g_ThanksBG[nCountThanksBGParts].nCountAnimation = 0;
							}
				if (g_ThanksBG[nCountThanksBGParts].nA < gc_nThanksBGA[nCountThanksBGParts][RDP_END])
				{
					g_ThanksBG[nCountThanksBGParts].nA = gc_nThanksBGA[nCountThanksBGParts][RDP_END];
				}
				if (g_ThanksBG[nCountThanksBGParts].nA > gc_nThanksBGAnimationTargetA[nCountThanksBGParts])
				{
					g_ThanksBG[nCountThanksBGParts].nA = gc_nThanksBGAnimationTargetA[nCountThanksBGParts];
				}
			}
		}
	}
	// BGM
	//if (!GetPlayStatus(BGM_Thanks_P1) && !GetPlayStatus(BGM_Thanks_P2))
	//{
	//	PlaySound(BGM_Thanks_P2);
	//}
}

void DrawThanksBG()
{
	SetZBuffer(FALSE);
	for (int nCountThanksBGParts = 0; nCountThanksBGParts < TSBP_MAX; nCountThanksBGParts++)
	{
		if (g_nThanksBGFadeInStartFrameCount >= gc_nThanksBGFadeInStartFrame[nCountThanksBGParts])
		{
			SetBlend(gc_nThanksAlphaBlend[nCountThanksBGParts]);
			SetTexture(gc_nThanksBGTextureNum[nCountThanksBGParts]);
			SetPolygonColor(D3DCOLOR_ARGB(g_ThanksBG[nCountThanksBGParts].nA, 255, 255, 255));
			SetScale
			(
				g_ThanksBG[nCountThanksBGParts].vScale.x,
				g_ThanksBG[nCountThanksBGParts].vScale.y,
				g_ThanksBG[nCountThanksBGParts].tTexture.u / 2.0f,
				g_ThanksBG[nCountThanksBGParts].tTexture.v / 2.0f
			);
			DrawPolygon
			(
				g_ThanksBG[nCountThanksBGParts].vPos.x,
				g_ThanksBG[nCountThanksBGParts].vPos.y,
				g_ThanksBG[nCountThanksBGParts].vUV.x,
				g_ThanksBG[nCountThanksBGParts].vUV.y,
				g_ThanksBG[nCountThanksBGParts].tTexture.u,
				g_ThanksBG[nCountThanksBGParts].tTexture.v
			);
		}
	}
	SetBlend(D3DBLEND_INVSRCALPHA);
	SetZBuffer(TRUE);
}
