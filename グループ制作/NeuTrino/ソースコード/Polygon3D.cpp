#include "main.h"
#include "Polygon3D.h"
#include "Texture.h"


typedef enum {
	VPN_1,
	VPN_2,
	VPN_3,
	VPN_4,
	VPN_MAX
}VERTEX_POS_NUM;

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static D3DCOLOR color = 0xffffffff;

static D3DXMATRIX g_MtxWorld;
static D3DXMATRIX g_MtxTrans;
static D3DXMATRIX g_MtxScale;
static D3DXMATRIX g_MtxRotX;
static D3DXMATRIX g_MtxRotY;
static D3DXMATRIX g_MtxRotZ;

static float g_fScaleX = 1.0f;
static float g_fScaleY = 1.0f;
static float g_fScaleZ = 1.0f;

static float g_fRotX = 0.0f;
static float g_fRotY = 0.0f;
static float g_fRotZ = 0.0f;

static D3DXVECTOR3 g_vVertex[VPN_MAX]=
{
	{0.0f, 0.0f, 0.0f}
};

static bool g_bVertex = false;

void InitPolygon3D()
{
	D3DXMatrixIdentity(&g_MtxWorld);

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	HRESULT hr;

	hr = pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX3DTEX) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVertexBuffer,
		NULL
	);

	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点バッファが作れなかった", "エラー", MB_OK);
	}

}

void UninitPolygon3D()
{
	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}

void SetPolygonColor3D(D3DCOLOR d_color)
{
	color = d_color;
}

void SetScale3D(float fX, float fY, float fZ)
{
	g_fScaleX = fX;
	g_fScaleY = fY;
	g_fScaleZ = fZ;
}

void SetRot3D(float fX, float fY, float fZ)
{
	g_fRotX = fX;
	g_fRotY = fY;
	g_fRotZ = fZ;
}

void SetPolygon3DVertex(D3DXVECTOR3 vPos1, D3DXVECTOR3 vPos2, D3DXVECTOR3 vPos3, D3DXVECTOR3 vPos4)
{
	g_vVertex[VPN_1] = vPos1 / N;
	g_vVertex[VPN_2] = vPos2 / N;
	g_vVertex[VPN_3] = vPos3 / N;
	g_vVertex[VPN_4] = vPos4 / N;
	g_bVertex = true;
}

void DrawPolygon3D(float fX, float fY, float fZ, int nSU, int nSV, int nWU, int nHV)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	TEXTURE Texture = GetTextureStatus(GetTextureNum());

	VERTEX3DTEX* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	float wd = Texture.u / N;
	float wh = Texture.v / N;
	int tw = Texture.width;
	int th = Texture.height;

	float u0 = (float)nSU / tw;
	float v0 = (float)nSV / th;
	float u1 = (float)(nSU + nWU) / tw;
	float v1 = (float)(nSV + nHV) / th;

	if (g_bVertex)
	{
		pV[0].vPos = g_vVertex[VPN_1];
		pV[1].vPos = g_vVertex[VPN_2];
		pV[2].vPos = g_vVertex[VPN_3];
		pV[3].vPos = g_vVertex[VPN_4];

		g_bVertex = false;
	}
	else
	{
		pV[0].vPos = { -0.5f * wd, 0.0f , 0.5f * wh };
		pV[1].vPos = { 0.5f * wd,  0.0f , 0.5f * wh };
		pV[2].vPos = { -0.5f * wd, 0.0f , -0.5f * wh };
		pV[3].vPos = { 0.5f * wd,  0.0f , -0.5f * wh };
	}

	pV[0].color = color;
	pV[1].color = color;
	pV[2].color = color;
	pV[3].color = color;

	pV[0].texcoord = { u0,v0 };
	pV[1].texcoord = { u1,v0 };
	pV[2].texcoord = { u0,v1 };
	pV[3].texcoord = { u1,v1 };

	pV[0].vNormal = { 0.0f, 1.0f, 0.0f };
	pV[1].vNormal = { 0.0f, 1.0f, 0.0f };
	pV[2].vNormal = { 0.0f, 1.0f, 0.0f };
	pV[3].vNormal = { 0.0f, 1.0f, 0.0f };

	g_pVertexBuffer->Unlock();

	D3DXMatrixIdentity(&g_MtxWorld);

	if (g_fScaleX != 1.0f || g_fScaleY != 1.0f || g_fScaleZ != 1.0f)
	{
		D3DXMatrixScaling(&g_MtxScale, g_fScaleX, g_fScaleY, g_fScaleZ);
		D3DXMatrixMultiply(&g_MtxWorld, &g_MtxWorld, &g_MtxScale);
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

	D3DXMatrixTranslation(&g_MtxTrans, fX / N, fY, fZ / N);

	D3DXMatrixMultiply(&g_MtxWorld, &g_MtxWorld, &g_MtxTrans);

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3DTEX));
	pDevice->SetFVF(FVF_VERTEX3D_TEX);
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