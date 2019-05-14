#include "PlayerEntryWordBoard.h"
#include "main.h"
#include "Polygon.h"
#include "Texture.h"

typedef struct {
	D3DXVECTOR2 vPos;			// 座標
	int nA;						// α値
	int nFadeInCount;			// フェードインカウンター
}PLAYER_ENTRY_WORD_BOARD;				// 曲選択画面(説明用)

static PLAYER_ENTRY_WORD_BOARD g_PlayerEntryWordBoard;

static const int gc_nInitA = 0;											// α値(初期値)
static const int gc_nMaxA = 255;										// α値(終了値)
static const int gc_nMaxFadeInCount = 10;								// フェードインカウンターの最大値
static const D3DXVECTOR2 gc_vInitPos = { WINDOW_WIDTH / 2.0f,360.0f };	// 座標(初期値)

void InitPlayerEntryWordBoard()
{
	g_PlayerEntryWordBoard.nA = gc_nInitA;
	g_PlayerEntryWordBoard.vPos = gc_vInitPos;
	g_PlayerEntryWordBoard.nFadeInCount = 0;
}

void UninitPlayerEntryWordBoard()
{

}

void UpdatePlayerEntryWordBoard()
{
	if (g_PlayerEntryWordBoard.nFadeInCount < gc_nMaxFadeInCount)
	{
		g_PlayerEntryWordBoard.nFadeInCount++;

		g_PlayerEntryWordBoard.nA += (gc_nMaxA - gc_nInitA) * 1.0f / (gc_nMaxFadeInCount * 1.0f);
		if (g_PlayerEntryWordBoard.nFadeInCount == gc_nMaxFadeInCount)
		{
			g_PlayerEntryWordBoard.nA = gc_nMaxA;
		}
	}
}

void DrawPlayerEntryWordBoard()
{
	SetTexture(TEX_PLAYER_ENTRY_WORD_BORAD);
	SetPolygonColor(D3DCOLOR_ARGB(g_PlayerEntryWordBoard.nA, 255, 255, 255));
	DrawPolygon
	(
		g_PlayerEntryWordBoard.vPos.x,
		g_PlayerEntryWordBoard.vPos.y,
		0,
		0,
		GetTextureStatus(TEX_PLAYER_ENTRY_WORD_BORAD).u,
		GetTextureStatus(TEX_PLAYER_ENTRY_WORD_BORAD).v
	);
}