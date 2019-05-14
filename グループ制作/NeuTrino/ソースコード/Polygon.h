//--------------------------------------------------------------------------------------------------
//
// ƒ|ƒŠƒSƒ“‚Ì•`‰æ
//																				    2017.06.05 RYU
//--------------------------------------------------------------------------------------------------

#ifndef POLYGON_H
#define POLYGON_H

#include <d3d9.h>
#include <d3dx9.h>

bool InitPolygon();

void SetPolygonColor(D3DCOLOR nColor);

void SetScale(float x, float y , int cx = 0, int cy = 0);
void UninitPolygon();

void UpdatePolygon();
void DrawPolygon(float dx, float dy, int tcx, int tcy, int tcw, int tch, bool bAffine = false);


#endif
