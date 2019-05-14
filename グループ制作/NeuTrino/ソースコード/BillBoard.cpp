#include "main.h"
#include "BillBoard.h"
#include "Camera.h"
#include "Texture.h"
#include <d3dx9.h>

#define FVF_VERTEX3D_TEX             (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)

// 逆行列		D3DXMatrixInverse(出力先アドレス, NULL, 入力先アドレス);
// 転置行列		D3DXMatrixTranspose(出力先アドレス, 入力先アドレス);

static D3DXMATRIX g_MtxWorld;
static D3DXMATRIX g_MtxTrans;
static D3DXMATRIX g_MtxScale;
static D3DXMATRIX g_MtxRotX;
static D3DXMATRIX g_MtxRotY;
static D3DXMATRIX g_MtxRotZ;
static D3DXMATRIX g_MtxViewInverse;
static D3DXMATRIX g_MtxView;
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static D3DCOLOR color = 0xffffffff;

static float g_fScaleX = 1.0f;
static float g_fScaleY = 1.0f;
static float g_fScaleZ = 1.0f;

static float g_fRotX = 0.0f;
static float g_fRotY = 0.0f;
static float g_fRotZ = 0.0f;

static bool g_bScale = false;

static D3DXVECTOR2 g_fBillboardCenterPos = { 0.5f,0.5f };
static bool g_bSetCenter = false;

void InitBillboard()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	//ワールド
	D3DXMatrixIdentity(&g_MtxWorld);

	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX3DTEX) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX3D_TEX,
		D3DPOOL_MANAGED,
		&g_pVertexBuffer,
		NULL
	);

}

void UninitBillboard()
{
	if (g_pVertexBuffer != NULL)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}

void UpdateBillboard()
{

}

void SetBillBoardColor(D3DCOLOR dColor)
{
	color = dColor;
}

void SetBillboardScale(float x, float y, float z)
{
	g_fScaleX = x;
	g_fScaleY = y;
	g_fScaleZ = z;

	g_bScale = true;
}

void SetBillboardRot(float x, float y, float z)
{
	g_fRotX = x;
	g_fRotY = y;
	g_fRotZ = z;
}

void SetBillboardCenterPos(float x, float y)
{
	g_fBillboardCenterPos.x = x;
	g_fBillboardCenterPos.y = 1.0f - y;
	g_bSetCenter = true;
}

void DrawBillboard(float x, float y, float z, int nSU, int nSV, int nWU, int nHV)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	TEXTURE Texture = GetTextureStatus(GetTextureNum());

	VERTEX3DTEX v[4];

	float wd = Texture.u / N;
	float wh = Texture.v / N;
	int tw = Texture.width;
	int th = Texture.height;

	float u0 = (float)nSU / tw;
	float v0 = (float)nSV / th;
	float u1 = (float)(nSU + nWU) / tw;
	float v1 = (float)(nSV + nHV) / th;

	if (g_bSetCenter)
	{
		v[0].vPos = { -g_fBillboardCenterPos.x * wd, (1.0f - g_fBillboardCenterPos.y) * wh, 0.0f };
		v[1].vPos = { (1.0f - g_fBillboardCenterPos.x) * wd, (1.0f - g_fBillboardCenterPos.y) * wh, 0.0f };
		v[2].vPos = { -g_fBillboardCenterPos.x * wd, -g_fBillboardCenterPos.y * wh,0.0f };
		v[3].vPos = { (1.0f - g_fBillboardCenterPos.x) * wd, -g_fBillboardCenterPos.y * wh,0.0f };
	}
	else
	{
		v[0].vPos = { -0.5f * wd, 0.5f * wh, 0.0f };
		v[1].vPos = { 0.5f * wd,  0.5f * wh, 0.0f };
		v[2].vPos = { -0.5f * wd, -0.5f * wh,0.0f };
		v[3].vPos = { 0.5f * wd,  -0.5f * wh,0.0f };
	}

	v[0].vNormal = { 0.0f, 0.0f, 1.0f };
	v[1].vNormal = { 0.0f, 0.0f, 1.0f };
	v[2].vNormal = { 0.0f, 0.0f, 1.0f };
	v[3].vNormal = { 0.0f, 0.0f, 1.0f };

	v[0].texcoord = { u0, v0 };
	v[1].texcoord = { u1, v0 };
	v[2].texcoord = { u0, v1 };
	v[3].texcoord = { u1, v1 };

	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	D3DXMatrixIdentity(&g_MtxWorld);

	if (g_bScale)
	{
		D3DXMatrixScaling(&g_MtxScale, g_fScaleX, g_fScaleY, g_fScaleZ);
		D3DXMatrixMultiply(&g_MtxWorld, &g_MtxWorld, &g_MtxScale);

		g_bScale = false;
	}
	else
	{
		g_fScaleX = g_fScaleY = g_fScaleZ = 1.0f;
	}

	if (g_fRotX != 0.0f)
	{
		D3DXMatrixRotationX(&g_MtxRotX, g_fRotX);
		D3DXMatrixMultiply(&g_MtxWorld, &g_MtxWorld, &g_MtxRotX);
		g_fRotX = 0.0f;
	}
	if (g_fRotY != 0.0f)
	{
		D3DXMatrixRotationY(&g_MtxRotY, g_fRotY);
		D3DXMatrixMultiply(&g_MtxWorld, &g_MtxWorld, &g_MtxRotY);
		g_fRotY = 0.0f;
	}
	if (g_fRotZ != 0.0f)
	{
		D3DXMatrixRotationZ(&g_MtxRotZ, g_fRotZ);
		D3DXMatrixMultiply(&g_MtxWorld, &g_MtxWorld, &g_MtxRotZ);
		g_fRotZ = 0.0f;
	}

	g_MtxView = GetCameraView();

	g_MtxView._41 = 0.0f;
	g_MtxView._42 = 0.0f;
	g_MtxView._43 = 0.0f;
	g_MtxView._14 = 0.0f;
	g_MtxView._24 = 0.0f;
	g_MtxView._34 = 0.0f;

	//D3DXMatrixInverse(&g_MtxViewInverse, NULL, &g_MtxView);
	D3DXMatrixTranspose(&g_MtxViewInverse, &g_MtxView);


	D3DXMatrixMultiply(&g_MtxWorld, &g_MtxWorld, &g_MtxViewInverse);

	D3DXMatrixTranslation(&g_MtxTrans, x / N, y, z / N);
	D3DXMatrixMultiply(&g_MtxWorld, &g_MtxWorld, &g_MtxTrans);

	VERTEX3DTEX* pV;

	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
	memcpy(&pV[0], &v[0], sizeof(VERTEX3DTEX) * 4);
	g_pVertexBuffer->Unlock();

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3DTEX));
	pDevice->SetFVF(FVF_VERTEX3D_TEX);

	//各種行列の設定
	pDevice->SetTransform
	(
		D3DTS_WORLD,
		&g_MtxWorld
	);

	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		2
	);

}