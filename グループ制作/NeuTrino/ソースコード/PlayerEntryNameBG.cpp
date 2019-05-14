#include "PlayerEntryNameBG.h"
#include "main.h"
#include "Polygon.h"
#include "Texture.h"

typedef struct {
	D3DXVECTOR2 vPos;							// ���W
	int nA;										// ���l
	int	nFadeInCount;							// �t�F�[�h�C���J�E���^�[
}PE_NAME_BG;									// ��Փx(����)

static PE_NAME_BG g_PE_Name_BG;

static const int gc_nFontInitA = 0;											// �t�H���g�̃��l(�����l)
static const int gc_nFontEndA = 255;										// �t�H���g�̃��l(�I���l)
static const int gc_nPanelFadeInCountMax = 8;								// �p�l���̃t�F�[�h�C���J�E���^�[�̍ő�l
static const D3DXVECTOR2 gc_vPE_Name_BGInitPos = { 640.0f,587.0f };			// ��Փx(����)�̏������W

void InitPlayerEntryNameBG()
{
	g_PE_Name_BG.nA = gc_nFontInitA;
	g_PE_Name_BG.nFadeInCount = 0;
	g_PE_Name_BG.vPos = gc_vPE_Name_BGInitPos;
}

void UninitPlayerEntryNameBG()
{

}

void UpdatePlayerEntryNameBG()
{
	if (g_PE_Name_BG.nFadeInCount < gc_nPanelFadeInCountMax)
	{
		g_PE_Name_BG.nFadeInCount++;
		g_PE_Name_BG.nA += (gc_nFontEndA - gc_nFontInitA) * 1.0f / (gc_nPanelFadeInCountMax * 1.0f);
		if (g_PE_Name_BG.nFadeInCount == gc_nPanelFadeInCountMax)
		{
			g_PE_Name_BG.nA = gc_nFontEndA;
		}
	}
}

void DrawPlayerEntryNameBG()
{
	SetTexture(TEX_PLAYER_ENTRY_NAME_BG);
	TEXTURE tPE_Name_BG = GetTextureStatus(TEX_PLAYER_ENTRY_NAME_BG);
	SetPolygonColor(D3DCOLOR_ARGB(g_PE_Name_BG.nA, 255, 255, 255));
	DrawPolygon
	(
		g_PE_Name_BG.vPos.x,
		g_PE_Name_BG.vPos.y,
		0,
		0,
		tPE_Name_BG.u,
		tPE_Name_BG.v
	);
}