#ifndef STRUCT_H
#define STRUCT_H

#include <d3dx9.h>

typedef struct {
	D3DXVECTOR3 vPos;
	D3DCOLOR color;
}VERTEX3D;

typedef struct {
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
}VERTEX3DTEX;

#endif // !STRUCT_H
