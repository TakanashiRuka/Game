#include "PlayerEntryNameBG.h"
#include "main.h"
#include "Polygon.h"
#include "Texture.h"

typedef struct {
	D3DXVECTOR2 vPos;							// 座標
	int nA;										// α値
	int	nFadeInCount;							// フェードインカウンター
}PE_NAME_BG;									// 難易度(文字)

static PE_NAME_BG g_PE_Name_BG;

static const int gc_nFontInitA = 0;											// フォントのα値(初期値)
static const int gc_nFontEndA = 255;										// フォントのα値(終了値)
static const int gc_nPanelFadeInCountMax = 8;								// パネルのフェードインカウンターの最大値
static const D3DXVECTOR2 gc_vPE_Name_BGInitPos = { 640.0f,587.0f };			// 難易度(文字)の初期座標

void InitPlayerEntryNameBG()
{
	g_PE_Name_BG.nA = gc_nFontInitA;
	g_PE_Name_BG.nFadeInCount = 0;
	g_PE_Name_BG.vPos = gc_vPE_Name_BGInitPos;
}

void UninitPlayerEntryNameBG()
{

}

void UpdatePlayerEntryNameBG()
{
	if (g_PE_Name_BG.nFadeInCount < gc_nPanelFadeInCountMax)
	{
		g_PE_Name_BG.nFadeInCount++;
		g_PE_Name_BG.nA += (gc_nFontEndA - gc_nFontInitA) * 1.0f / (gc_nPanelFadeInCountMax * 1.0f);
		if (g_PE_Name_BG.nFadeInCount == gc_nPanelFadeInCountMax)
		{
			g_PE_Name_BG.nA = gc_nFontEndA;
		}
	}
}

void DrawPlayerEntryNameBG()
{
	SetTexture(TEX_PLAYER_ENTRY_NAME_BG);
	TEXTURE tPE_Name_BG = GetTextureStatus(TEX_PLAYER_ENTRY_NAME_BG);
	SetPolygonColor(D3DCOLOR_ARGB(g_PE_Name_BG.nA, 255, 255, 255));
	DrawPolygon
	(
		g_PE_Name_BG.vPos.x,
		g_PE_Name_BG.vPos.y,
		0,
		0,
		tPE_Name_BG.u,
		tPE_Name_BG.v
	);
}