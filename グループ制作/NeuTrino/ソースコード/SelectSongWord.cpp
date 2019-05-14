//----------------------------------------------------------------------------
//
//	曲選択画面(ここが曲選択画面を示している)
//
//														 制作者：劉喆
//                                                       作成日：2017/12/26
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "SelectSongWord.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;			// 座標
	int nA;						// α値
	int nFadeInCount;			// フェードインカウンター
}SELECT_SONG_WORD;				// 曲選択画面(説明用)

//---------------------------グローバル変数-----------------------------------
static SELECT_SONG_WORD g_SelectSongWord;

static const int gc_nInitA = 0;											// α値(初期値)
static const int gc_nMaxA = 255;										// α値(終了値)
static const int gc_nMaxFadeInCount = 10;								// フェードインカウンターの最大値
static const D3DXVECTOR2 gc_vInitPos = { WINDOW_WIDTH / 2.0f,132.0f };	// 座標(初期値)

//---------------------------プロトタイプ宣言---------------------------------
void InitSelectSongWord()
{
	g_SelectSongWord.nA = gc_nInitA;
	g_SelectSongWord.vPos = gc_vInitPos;
	g_SelectSongWord.nFadeInCount = 0;
}

void UninitSelectSongWord()
{

}

void UpdateSelectSongWord()
{
	if (g_SelectSongWord.nFadeInCount < gc_nMaxFadeInCount)
	{
		g_SelectSongWord.nFadeInCount++;

		g_SelectSongWord.nA += (gc_nMaxA - gc_nInitA) * 1.0f / (gc_nMaxFadeInCount * 1.0f);
		if (g_SelectSongWord.nFadeInCount == gc_nMaxFadeInCount)
		{
			g_SelectSongWord.nA = gc_nMaxA;
		}
	}
}

void DrawSelectSongWord()
{
	SetTexture(TEX_SELECT_SONG_WORD);
	SetPolygonColor(D3DCOLOR_ARGB(g_SelectSongWord.nA, 255, 255, 255));
	DrawPolygon
	(
		g_SelectSongWord.vPos.x,
		g_SelectSongWord.vPos.y,
		0,
		0,
		GetTextureStatus(TEX_SELECT_SONG_WORD).u,
		GetTextureStatus(TEX_SELECT_SONG_WORD).v
	);
}