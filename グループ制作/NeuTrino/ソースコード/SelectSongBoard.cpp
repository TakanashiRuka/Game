//----------------------------------------------------------------------------
//
//	曲の背景
//
//														 制作者：劉喆
//                                                       作成日：2017/12/26
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "SelectSongBoard.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	float fScaleY;				// 縦のスケーリング
	int nFadeInCount;			// フェードインのカウンター
}SONG_BOARD;					// 曲の背景

//---------------------------グローバル変数-----------------------------------
static SONG_BOARD g_SongBoard;

static const int gc_nMaxFadeInCount = 10;			// 曲背景フェードインカウンターの最大値
static const float gc_fInitScaleY = 0.0f;			// 縦のスケーリング(初期値)
static const float gc_fEndScaleY = 1.0f;			// 縦のスケーリング(終了値)

//---------------------------プロトタイプ宣言---------------------------------
void InitSelectSongBoard()
{
	g_SongBoard.fScaleY = gc_fInitScaleY;
	g_SongBoard.nFadeInCount = 0;
}

void UninitSelectSongBoard()
{

}

void UpdateSelectSongBoard()
{
	if (g_SongBoard.nFadeInCount < gc_nMaxFadeInCount)
	{
		g_SongBoard.nFadeInCount++;
		g_SongBoard.fScaleY += (gc_fEndScaleY - gc_fInitScaleY) / (gc_nMaxFadeInCount * 1.0f);
		if (g_SongBoard.nFadeInCount == gc_nMaxFadeInCount)
		{
			g_SongBoard.fScaleY = gc_fEndScaleY;
		}
	}
}

void DrawSelectSongBoard()
{
	SetTexture(TEX_SELECT_SONG_BOARD);
	SetPolygonColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	SetScale
	(
		1.0f,
		g_SongBoard.fScaleY,
		GetTextureStatus(TEX_SELECT_SONG_BOARD).u / 2.0f,
		GetTextureStatus(TEX_SELECT_SONG_BOARD).v / 2.0f
	);
	DrawPolygon
	(
		WINDOW_WIDTH / 2.0f,
		WINDOW_HEIGHT / 2.0f,
		0,
		0,
		GetTextureStatus(TEX_SELECT_SONG_BOARD).u,
		GetTextureStatus(TEX_SELECT_SONG_BOARD).v
	);
}