//----------------------------------------------------------------------------
//
//	タイトルの背景部分
//
//														 制作者：劉喆
//                                                       作成日：2017/01/27
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "sound.h"
#include "TitleBG.h"
#include "debug.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	TBP_BG,
	TBP_SQUARE_SMALL,
	TBP_SQUARE_BIG,
	TBP_SQUARE_BIG_ALPHA_BLEND,
	TBP_FLOOR,
	TBP_HEXANGONAL,
	TBP_HEXANGONAL_ALPHA_BLEND,
	TBP_LOGO,
	TBP_MAX,
}TITLE_BG_PARTS;

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;
	int nA;
	D3DXVECTOR2 vScale;
	D3DXVECTOR2 vUV;
	TEXTURE tTexture;
	int nCountFadeIn;
	int nCountAnimation;
}TITLE_BG;

//---------------------------グローバル変数-----------------------------------
//----------------------------------------------------------------------------
// バージョン
//----------------------------------------------------------------------------
static const char gc_cVersion[] = "Ver 1.20.20";

static TITLE_BG g_TitleBG[TBP_MAX];

static const int gc_nTitleBGTextureNum[TBP_MAX] =													// タイトルのテクスチャ番号
{
	TEX_TITLE_BG,
	TEX_TITLE_BG_SQUARE_SMALL,
	TEX_TITLE_BG_SQUARE_BIG,
	TEX_TITLE_BG_SQUARE_BIG,
	TEX_TITLE_BG_FLOOR,
	TEX_TITLE_BG_HEXANGONAL,
	TEX_TITLE_BG_HEXANGONAL,
	TEX_TITLE_BG_LOGO,
};

static const D3DXVECTOR2 gc_vTitleBGPos[TBP_MAX][RDP_MAX]=										   // タイトルの座標
{
	{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f } },
	{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f } },
	{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f } },
	{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f } },
	{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT + 100.0f },{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 80.0f } },
	{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f } },
	{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f } },
	{ { WINDOW_WIDTH / 2.0f - 20.0f, WINDOW_HEIGHT / 2.0f - 20.0f },{ WINDOW_WIDTH / 2.0f - 20.0f, WINDOW_HEIGHT / 2.0f - 20.0f } },
};

static const int gc_nTitleBGA[TBP_MAX][RDP_MAX]=												   // タイトルのα値
{
	{ 0,255 },
	{ 0,255 },
	{ 0,255 },
	{ 0,0 },
	{ 255,255 },
	{ 255,255 },
	{ 0,0 },
	{ 0,255 },
};

static const D3DXVECTOR2 gc_vTitleBGScale[TBP_MAX][RDP_MAX] =									   // タイトルのスケーリング
{
	{ { 1.0f,1.0f },{ 1.0f,1.0f } },
	{ { 1.0f,1.0f },{ 1.0f,1.0f } },
	{ { 1.0f,1.0f },{ 1.0f,1.0f } },
	{ { 1.0f,1.0f },{ 1.0f,1.0f } },
	{ { 1.0f,1.0f },{ 1.0f,1.0f } },
	{ { 2.0f,2.0f },{ 1.0f,1.0f } },
	{ { 2.0f,2.0f },{ 1.0f,1.0f } },
	{ { 2.0f,2.0f },{ 1.0f,1.0f } },
};

static const int gc_nTitleBGFadeInCountMax[TBP_MAX] =											   // タイトルのフェードインのカウンターの最大値
{
	40,
	12,
	12,
	12,
	12,
	4,
	4,
	12,
};

static const int gc_nTitleBGFadeInStartFrame[TBP_MAX] =											  // タイトルのアニメーションの開始フレーム
{
	20,
	0,
	20,
	20,
	40,
	60,
	60,
	48,
};

// アニメーション部分
static const D3DXVECTOR2 gc_vTitleBGChangeUV[TBP_MAX] =
{
	{ 0.0f,0.0f },
	{ 0.0f,0.5f },
	{ 0.0f,2.0f },
	{ 0.0f,2.0f },
	{ 0.0f,0.0f },
	{ 0.0f,0.0f },
	{ 0.0f,0.0f },
	{ 0.0f,0.0f },
};

static const D3DXVECTOR2 gc_vTitleBGTargetPos[TBP_MAX] =
{
	{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },
	{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },
	{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },
	{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },
	{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 70.0f },
	{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },
	{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },
	{ WINDOW_WIDTH / 2.0f - 20.0f, WINDOW_HEIGHT / 2.0f - 20.0f },
};

static const D3DXVECTOR2 gc_vTitleBGTargetScale[TBP_MAX] =
{
	{ 1.0f,1.0f },
	{ 1.0f,1.0f },
	{ 1.0f,1.0f },
	{ 1.0f,1.0f },
	{ 1.0f,1.0f },
	{ 1.02f,1.02f },
	{ 1.1f,1.1f },
	{ 1.0f,1.0f },
};

static const int gc_nTitleBGAnimationTargetA[TBP_MAX] =
{
	255,
	255,
	255,
	255,
	255,
	255,
	127,
	255,
};

static const int gc_nTitleBGAnimationMaxFrame[TBP_MAX] =
{
	0,
	0,
	0,
	20,
	60,
	60,
	60,
	0,
};

static int gc_nTitleAlphaBlend[TBP_MAX]=
{
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_BLENDFACTOR,
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_BLENDFACTOR,
	D3DBLEND_INVSRCALPHA,
};

static const int gc_nTitleBGFadeInStartFrameCountMax = 60;
static int g_nTitleBGFadeInStartFrameCount;

//-------------------------------------------------------------------
// タイトルマスク
//-------------------------------------------------------------------
static const int gc_nTitleMaskA[RDP_MAX] = { 0,48 };
static const int gc_nTitleMaskMaxCount[RDP_MAX] = { 6,36 };

static int g_nTitleMaskA;
static int g_nTitleMaskCount;

//---------------------------プロトタイプ宣言---------------------------------
void InitTitleBG()
{
	g_nTitleBGFadeInStartFrameCount = 0;

	for (int nCountTitleBGParts = 0; nCountTitleBGParts < TBP_MAX; nCountTitleBGParts++)
	{
		g_TitleBG[nCountTitleBGParts].nA = gc_nTitleBGA[nCountTitleBGParts][RDP_INIT];
		g_TitleBG[nCountTitleBGParts].nCountFadeIn = 0;
		g_TitleBG[nCountTitleBGParts].nCountAnimation = 0;
		g_TitleBG[nCountTitleBGParts].vPos = gc_vTitleBGPos[nCountTitleBGParts][RDP_INIT];
		g_TitleBG[nCountTitleBGParts].vScale = gc_vTitleBGScale[nCountTitleBGParts][RDP_INIT];
		g_TitleBG[nCountTitleBGParts].vUV = { 0.0f,0.0f };
		g_TitleBG[nCountTitleBGParts].tTexture = GetTextureStatus(gc_nTitleBGTextureNum[nCountTitleBGParts]);
	}
	PlaySound(BGM_TITLE_P2);
	StopSound(BGM_TITLE_P2);
	PlaySound(BGM_TITLE_P1);
}

void UninitTitleBG()
{
	StopSound(BGM_TITLE_P1);
	StopSound(BGM_TITLE_P2);
}

void UpdateTitleBG()
{
	if (g_nTitleBGFadeInStartFrameCount < gc_nTitleBGFadeInStartFrameCountMax)
	{
		g_nTitleBGFadeInStartFrameCount++;
	}
	for (int nCountTitleBGParts = 0; nCountTitleBGParts < TBP_MAX; nCountTitleBGParts++)
	{
		if (g_nTitleBGFadeInStartFrameCount >= gc_nTitleBGFadeInStartFrame[nCountTitleBGParts])
		{
			if (g_TitleBG[nCountTitleBGParts].nCountFadeIn < gc_nTitleBGFadeInCountMax[nCountTitleBGParts])
			{
				g_TitleBG[nCountTitleBGParts].nCountFadeIn++;
				
				g_TitleBG[nCountTitleBGParts].nA += (gc_nTitleBGA[nCountTitleBGParts][RDP_END] - gc_nTitleBGA[nCountTitleBGParts][RDP_INIT]) * 1.0f /
					(gc_nTitleBGFadeInCountMax[nCountTitleBGParts] * 1.0f);

				g_TitleBG[nCountTitleBGParts].vPos += (gc_vTitleBGPos[nCountTitleBGParts][RDP_END] - gc_vTitleBGPos[nCountTitleBGParts][RDP_INIT]) /
					(gc_nTitleBGFadeInCountMax[nCountTitleBGParts] * 1.0f);

				g_TitleBG[nCountTitleBGParts].vScale += (gc_vTitleBGScale[nCountTitleBGParts][RDP_END] - gc_vTitleBGScale[nCountTitleBGParts][RDP_INIT]) /
					(gc_nTitleBGFadeInCountMax[nCountTitleBGParts]);

				if (g_TitleBG[nCountTitleBGParts].nCountFadeIn == gc_nTitleBGFadeInCountMax[nCountTitleBGParts])
				{
					g_TitleBG[nCountTitleBGParts].nA = gc_nTitleBGA[nCountTitleBGParts][RDP_END];
					g_TitleBG[nCountTitleBGParts].vPos = gc_vTitleBGPos[nCountTitleBGParts][RDP_END];
					g_TitleBG[nCountTitleBGParts].vScale = gc_vTitleBGScale[nCountTitleBGParts][RDP_END];
				}
			}
			else
			{
				g_TitleBG[nCountTitleBGParts].vUV += gc_vTitleBGChangeUV[nCountTitleBGParts];
				g_TitleBG[nCountTitleBGParts].nCountAnimation++;
				if (g_TitleBG[nCountTitleBGParts].nCountAnimation < gc_nTitleBGAnimationMaxFrame[nCountTitleBGParts])
				{
					g_TitleBG[nCountTitleBGParts].nA += (gc_nTitleBGAnimationTargetA[nCountTitleBGParts] - gc_nTitleBGA[nCountTitleBGParts][RDP_END]) * 1.0f /
						(gc_nTitleBGAnimationMaxFrame[nCountTitleBGParts] * 1.0f);
					g_TitleBG[nCountTitleBGParts].vPos += (gc_vTitleBGTargetPos[nCountTitleBGParts] - gc_vTitleBGPos[nCountTitleBGParts][RDP_END]) /
						(gc_nTitleBGAnimationMaxFrame[nCountTitleBGParts] * 1.0f);
					g_TitleBG[nCountTitleBGParts].vScale += (gc_vTitleBGTargetScale[nCountTitleBGParts] - gc_vTitleBGScale[nCountTitleBGParts][RDP_END]) /
						(gc_nTitleBGAnimationMaxFrame[nCountTitleBGParts] * 1.0f);
				}
				else
					if (g_TitleBG[nCountTitleBGParts].nCountAnimation == gc_nTitleBGAnimationMaxFrame[nCountTitleBGParts])
					{
						g_TitleBG[nCountTitleBGParts].nA = gc_nTitleBGAnimationTargetA[nCountTitleBGParts];
						g_TitleBG[nCountTitleBGParts].vPos = gc_vTitleBGTargetPos[nCountTitleBGParts];
						g_TitleBG[nCountTitleBGParts].vScale = gc_vTitleBGTargetScale[nCountTitleBGParts];
					}
					else
						if (g_TitleBG[nCountTitleBGParts].nCountAnimation < 2 * gc_nTitleBGAnimationMaxFrame[nCountTitleBGParts])
						{
							g_TitleBG[nCountTitleBGParts].nA += (-gc_nTitleBGAnimationTargetA[nCountTitleBGParts] + gc_nTitleBGA[nCountTitleBGParts][RDP_END]) * 1.0f /
								(gc_nTitleBGAnimationMaxFrame[nCountTitleBGParts] * 1.0f);
							g_TitleBG[nCountTitleBGParts].vPos += (-gc_vTitleBGTargetPos[nCountTitleBGParts] + gc_vTitleBGPos[nCountTitleBGParts][RDP_END]) /
								(gc_nTitleBGAnimationMaxFrame[nCountTitleBGParts] * 1.0f);
							g_TitleBG[nCountTitleBGParts].vScale += (-gc_vTitleBGTargetScale[nCountTitleBGParts] + gc_vTitleBGScale[nCountTitleBGParts][RDP_END]) /
								(gc_nTitleBGAnimationMaxFrame[nCountTitleBGParts] * 1.0f);
						}
						else
							if (g_TitleBG[nCountTitleBGParts].nCountAnimation == 2 * gc_nTitleBGAnimationMaxFrame[nCountTitleBGParts])
							{
								g_TitleBG[nCountTitleBGParts].nA = gc_nTitleBGA[nCountTitleBGParts][RDP_END];
								g_TitleBG[nCountTitleBGParts].vPos = gc_vTitleBGPos[nCountTitleBGParts][RDP_END];
								g_TitleBG[nCountTitleBGParts].vScale = gc_vTitleBGScale[nCountTitleBGParts][RDP_END];

								g_TitleBG[nCountTitleBGParts].nCountAnimation = 0;
							}
				if (g_TitleBG[nCountTitleBGParts].nA < gc_nTitleBGA[nCountTitleBGParts][RDP_END])
				{
					g_TitleBG[nCountTitleBGParts].nA = gc_nTitleBGA[nCountTitleBGParts][RDP_END];
				}
				if (g_TitleBG[nCountTitleBGParts].nA > gc_nTitleBGAnimationTargetA[nCountTitleBGParts])
				{
					g_TitleBG[nCountTitleBGParts].nA = gc_nTitleBGAnimationTargetA[nCountTitleBGParts];
				}
			}
		}
	}
	// BGM
	if (!GetPlayStatus(BGM_TITLE_P1) && !GetPlayStatus(BGM_TITLE_P2))
	{
		PlaySound(BGM_TITLE_P2);
	}
}

void DrawTitleBG()
{
	SetZBuffer(FALSE);
	for (int nCountTitleBGParts = 0; nCountTitleBGParts < TBP_MAX; nCountTitleBGParts++)
	{
		if (g_nTitleBGFadeInStartFrameCount >= gc_nTitleBGFadeInStartFrame[nCountTitleBGParts])
		{
			SetBlend(gc_nTitleAlphaBlend[nCountTitleBGParts]);
			SetTexture(gc_nTitleBGTextureNum[nCountTitleBGParts]);
			SetPolygonColor(D3DCOLOR_ARGB(g_TitleBG[nCountTitleBGParts].nA, 255, 255, 255));
			SetScale
			(
				g_TitleBG[nCountTitleBGParts].vScale.x,
				g_TitleBG[nCountTitleBGParts].vScale.y,
				g_TitleBG[nCountTitleBGParts].tTexture.u / 2.0f,
				g_TitleBG[nCountTitleBGParts].tTexture.v / 2.0f
			);
			DrawPolygon
			(
				g_TitleBG[nCountTitleBGParts].vPos.x,
				g_TitleBG[nCountTitleBGParts].vPos.y,
				g_TitleBG[nCountTitleBGParts].vUV.x,
				g_TitleBG[nCountTitleBGParts].vUV.y,
				g_TitleBG[nCountTitleBGParts].tTexture.u,
				g_TitleBG[nCountTitleBGParts].tTexture.v
			);
		}
	}
	SetBlend(D3DBLEND_INVSRCALPHA);
	SetZBuffer(TRUE);
}

//---------------------------------------------------------------
// 著作権
//---------------------------------------------------------------
void DrawTitleAllRightReserved()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_TITLE_ALL_RIGHTS_RESERVED);
	TEXTURE tTexture = GetTextureStatus(TEX_TITLE_ALL_RIGHTS_RESERVED);
	SetPolygonColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	DrawPolygon
	(
		WINDOW_WIDTH / 2.0f,
		WINDOW_HEIGHT - tTexture.v  / 2.0f,
		0,
		0,
		tTexture.u,
		tTexture.v
	);
	SetZBuffer(TRUE);
}

//-----------------------------------------------------------------
// バージョン
//-----------------------------------------------------------------
void InitVersion()
{
	InitText(F_VERSION, 22, "MS コジック");
}

void UninitVersion()
{
	UninitText(F_VERSION);
}

void UpdateVersion()
{

}

void DrawVersion()
{
	SetTextStyle(DT_LEFT);
	SetTextStatus(D3DCOLOR_ARGB(255, 255, 255, 255), D3DCOLOR_ARGB(255, 0, 0, 0), 1);
	OutputText(F_VERSION, 4, 2, "%s", &gc_cVersion);
}

//------------------------------------------------------------------
// タイトルマスク
//------------------------------------------------------------------
void InitTitleMask()
{
	g_nTitleMaskA = gc_nTitleMaskA[RDP_INIT];
	g_nTitleMaskCount = 0;
}

void UninitTitleMask()
{

}

void UpdateTitleMask()
{
	if (g_nTitleMaskCount < gc_nTitleMaskMaxCount[RDP_INIT] + gc_nTitleMaskMaxCount[RDP_END])
	{
		g_nTitleMaskCount++;
		if (g_nTitleMaskCount < gc_nTitleMaskMaxCount[RDP_INIT])
		{
			g_nTitleMaskA += (gc_nTitleMaskA[RDP_END] - gc_nTitleMaskA[RDP_INIT]) * 1.0f / (gc_nTitleMaskMaxCount[RDP_INIT] * 1.0f);
		}
		else
			if (g_nTitleMaskCount == gc_nTitleMaskMaxCount[RDP_INIT])
			{
				g_nTitleMaskA = gc_nTitleMaskA[RDP_END];
			}
			else
				if (g_nTitleMaskCount < gc_nTitleMaskMaxCount[RDP_INIT] + gc_nTitleMaskMaxCount[RDP_END])
				{
					g_nTitleMaskA += (-gc_nTitleMaskA[RDP_END] + gc_nTitleMaskA[RDP_INIT]) * 1.0f / (gc_nTitleMaskMaxCount[RDP_END] * 1.0f);
				}
				else
					if (g_nTitleMaskCount == gc_nTitleMaskMaxCount[RDP_INIT] + gc_nTitleMaskMaxCount[RDP_END])
					{
						g_nTitleMaskA = gc_nTitleMaskA[RDP_INIT];
					}
		if (g_nTitleMaskA < gc_nTitleMaskA[RDP_INIT])
		{
			g_nTitleMaskA = gc_nTitleMaskA[RDP_INIT];
		}
		if (g_nTitleMaskA > gc_nTitleMaskA[RDP_END])
		{
			g_nTitleMaskA = gc_nTitleMaskA[RDP_END];
		}
	}
}

void DrawTitleMask()
{
	SetTexture(TEX_TITLE_MASK);
	SetBlend(D3DBLEND_BLENDFACTOR);
	SetZBuffer(FALSE);
	TEXTURE tTexture = GetTextureStatus(TEX_TITLE_MASK);

	SetPolygonColor(D3DCOLOR_ARGB(g_nTitleMaskA, 255, 255, 255));
	DrawPolygon
	(
		WINDOW_WIDTH / 2.0f,
		WINDOW_HEIGHT / 2.0f,
		0,
		0,
		tTexture.u,
		tTexture.v
	);

	SetBlend(D3DBLEND_INVSRCALPHA);
	SetZBuffer(TRUE);
}