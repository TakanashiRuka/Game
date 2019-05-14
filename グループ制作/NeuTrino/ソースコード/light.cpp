#include "main.h"
#include "Light.h"

static D3DMATERIAL9 g_mat;
static D3DLIGHT9 g_light;
static D3DXVECTOR3 g_vDir(1.0f, 1.0f, 1.0f);

void InitLight()
{
	ZeroMemory(&g_mat, sizeof(g_mat));

	g_mat.Diffuse.r = 1.0f;
	g_mat.Diffuse.g = 1.0f;
	g_mat.Diffuse.b = 1.0f;
	g_mat.Diffuse.a = 1.0f;

	g_mat.Ambient = { 1.0f,1.0f,1.0f,1.0f };

	ZeroMemory(&g_light, sizeof(g_light));
	g_light.Type = D3DLIGHT_DIRECTIONAL;

	D3DXVec3Normalize((D3DXVECTOR3*)&g_light.Direction, &g_vDir);

	g_light.Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	g_light.Ambient = { 1.0f,1.0f,1.0f,1.0f };
}

void UninitLight()
{

}

void SetLight(bool bStatus)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	if (bStatus)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		pDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f));
		pDevice->SetMaterial(&g_mat);
		pDevice->SetLight(0, &g_light);
		pDevice->LightEnable(0, TRUE);
	}
	else
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
}