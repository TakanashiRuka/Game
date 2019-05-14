//----------------------------------------------------------------------------
//
//	タイトルの操作説明
//
//														 制作者：劉喆
//                                                       作成日：2017/01/30
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;
	int nA;
	int nV;
	int nCount;
}TITLE_BTN_MANUAL;				// タイトルの操作説明

//---------------------------グローバル変数-----------------------------------
static TITLE_BTN_MANUAL g_TitleBtnManual;

static const D3DXVECTOR2 gc_vTitleBtnManualInitPos = { WINDOW_WIDTH / 2.0f,550.0f };	// タイトルの操作説明の初期座標
static const D3DXVECTOR2 gc_vTitleBtnManualPosRange = { 0.0f,115.0f };					// タイトルの操作説明の初期座標
static const D3DXVECTOR2 gc_vTitleBtnManualPosRangeTutorial = { 350.0f,-440.0f };					// タイトルの操作説明の初期座標
static const int gc_nTitleBtnManualA[AFF_MAX] = { 127, 255, 255, 127 };					// タイトルの操作説明のα値
static const int gc_nTitleAnimationMaxCount[CFF_MAX] = { 10,60,10 };

//---------------------------プロトタイプ宣言---------------------------------
void InitTitleBtnManual()
{
	g_TitleBtnManual.nA = gc_nTitleBtnManualA[AFF_FADEIN];
	g_TitleBtnManual.nCount = 0;
	g_TitleBtnManual.vPos = gc_vTitleBtnManualInitPos;
	if (!GetJSDev())
	{
		g_TitleBtnManual.nV = 0;
	}
	else
	{
		g_TitleBtnManual.nV = 1;
	}
}

void UninitTitleBtnManual()
{

}

void UpdateTitleBtnManual()
{
	g_TitleBtnManual.nCount++;
	for (int nCountAnimationPart = 0; nCountAnimationPart < CFF_MAX; nCountAnimationPart++)
	{
		int nCounter = 0;
		for (int nCountAPSave = 0; nCountAPSave <= nCountAnimationPart; nCountAPSave++)
		{
			nCounter += gc_nTitleAnimationMaxCount[nCountAPSave];
		}
		if (g_TitleBtnManual.nCount < nCounter)
		{
			g_TitleBtnManual.nA +=
				(gc_nTitleBtnManualA[nCountAnimationPart + 1] -
					gc_nTitleBtnManualA[nCountAnimationPart]) * 1.0f /
					(gc_nTitleAnimationMaxCount[nCountAnimationPart] * 1.0f);

			if (g_TitleBtnManual.nA < gc_nTitleBtnManualA[AFF_FADEIN])
			{
				g_TitleBtnManual.nA = gc_nTitleBtnManualA[AFF_FADEIN];
			}
			if (g_TitleBtnManual.nA > gc_nTitleBtnManualA[AFF_MIDDLE_START])
			{
				g_TitleBtnManual.nA = gc_nTitleBtnManualA[AFF_MIDDLE_START];
			}
			break;
		}
		if (g_TitleBtnManual.nCount == nCounter)
		{
			g_TitleBtnManual.nA = gc_nTitleBtnManualA[nCountAnimationPart + 1];
			if (nCountAnimationPart == CFF_MAX - 1)
			{
				g_TitleBtnManual.nCount = 0;
				g_TitleBtnManual.nA = gc_nTitleBtnManualA[AFF_FADEIN];
			}
			break;
		}
	}
}

void DrawTitleBtnManual()
{
	SetZBuffer(FALSE);
	TEXTURE tTexture;
	switch (GetGameMode())
	{
	case MODE_TITLE:
	case MODE_DEMO:
	{
		SetTexture(TEX_TITLE_BTN_MANUAL);
		tTexture = GetTextureStatus(TEX_TITLE_BTN_MANUAL);
		break;
	}
	case MODE_TUTORIAL:
	{
		SetTexture(TEX_TUTORIAL_BTN);
		tTexture = GetTextureStatus(TEX_TUTORIAL_BTN);
		g_TitleBtnManual.vPos = gc_vTitleBtnManualInitPos + gc_vTitleBtnManualPosRangeTutorial;
		break;
	}
	case MODE_RESULT:
	{
		SetTexture(TEX_RESULT_BTN);
		tTexture = GetTextureStatus(TEX_RESULT_BTN);
		g_TitleBtnManual.vPos = gc_vTitleBtnManualInitPos + gc_vTitleBtnManualPosRange;
		break;
	}
	}
	SetPolygonColor(D3DCOLOR_ARGB(g_TitleBtnManual.nA, 255, 255, 255));
	DrawPolygon
	(
		g_TitleBtnManual.vPos.x,
		g_TitleBtnManual.vPos.y,
		0,
		tTexture.v * g_TitleBtnManual.nV,
		tTexture.u,
		tTexture.v
	);
	SetZBuffer(TRUE);
}