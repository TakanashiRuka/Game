#ifndef NUMBER_H
#define NUMBER_H

//------------------------------------------------------------------------------
//	関数名:	DrawNumber
//	引数:	テクスチャ番号, U, V, 数字, X座標, Y座標, 透明度
//	戻り値:	なし
//	説明:	数字を表示させる
//------------------------------------------------------------------------------

void DrawNumber(int tex, int u, int v, int n, float x, float y, int a);

void DrawNumber3D(int tex, int u, int v, int n, float x, float y, float z, int a);

#endif