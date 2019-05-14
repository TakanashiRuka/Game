#include "main.h"
#include "polygon.h"
#include "polygon3D.h"
#include "number.h"
#include "Texture.h"

//------------------------------------------------------------------------------
//	関数名:	DrawNumber
//	引数:	テクスチャ番号, U, V, 数字, X座標, Y座標, 透明度
//	戻り値:	なし
//	説明:	数字を表示させる
//------------------------------------------------------------------------------
void DrawNumber(int tex, int u, int v, int n, float x, float y, int a)
{
	SetTexture(tex);
	SetPolygonColor(D3DCOLOR_ARGB(a, 255, 255, 255));
	DrawPolygon(x, y, u * n, v, u, v);
}

//------------------------------------------------------------------------------
//	関数名:	DrawNumber3D
//	引数:	テクスチャ番号, U, V, 数字, X座標, Y座標, Z座標, 透明度
//	戻り値:	なし
//	説明:	数字を表示させる
//------------------------------------------------------------------------------
void DrawNumber3D(int tex, int u, int v, int n, float x, float y, float z, int a)
{
	SetTexture(tex);
	SetPolygonColor3D(D3DCOLOR_ARGB(a, 255, 255, 255));
	DrawPolygon3D(x, y, z, u * n, v, u, v);
}