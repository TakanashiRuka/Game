#include "main.h"
#include "polygon.h"
#include "polygon3D.h"
#include "number.h"
#include "Texture.h"

//------------------------------------------------------------------------------
//	�֐���:	DrawNumber
//	����:	�e�N�X�`���ԍ�, U, V, ����, X���W, Y���W, �����x
//	�߂�l:	�Ȃ�
//	����:	������\��������
//------------------------------------------------------------------------------
void DrawNumber(int tex, int u, int v, int n, float x, float y, int a)
{
	SetTexture(tex);
	SetPolygonColor(D3DCOLOR_ARGB(a, 255, 255, 255));
	DrawPolygon(x, y, u * n, v, u, v);
}

//------------------------------------------------------------------------------
//	�֐���:	DrawNumber3D
//	����:	�e�N�X�`���ԍ�, U, V, ����, X���W, Y���W, Z���W, �����x
//	�߂�l:	�Ȃ�
//	����:	������\��������
//------------------------------------------------------------------------------
void DrawNumber3D(int tex, int u, int v, int n, float x, float y, float z, int a)
{
	SetTexture(tex);
	SetPolygonColor3D(D3DCOLOR_ARGB(a, 255, 255, 255));
	DrawPolygon3D(x, y, z, u * n, v, u, v);
}