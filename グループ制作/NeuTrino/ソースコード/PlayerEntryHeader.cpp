#include "PlayerEntryHeader.h"
#include "main.h"
#include "Polygon.h"
#include "Texture.h"

typedef struct {
	D3DXVECTOR2 vPos;			// ���W
	int nA;						// ���l
	int nFadeInCount;			// �t�F�[�h�C���J�E���^�[
}PLAYER_ENTRY_HEADER;				// �ȑI�����(�����p)

static PLAYER_ENTRY_HEADER g_PlayerEntryHeader;

static const int gc_nInitA = 0;											// ���l(�����l)
static const int gc_nMaxA = 255;										// ���l(�I���l)
static const int gc_nMaxFadeInCount = 10;								// �t�F�[�h�C���J�E���^�[�̍ő�l
static const D3DXVECTOR2 gc_vInitPos = { WINDOW_WIDTH / 2.0f,132.0f };	// ���W(�����l)

void InitPlayerEntryHeader()
{
	g_PlayerEntryHeader.nA = gc_nInitA;
	g_PlayerEntryHeader.vPos = gc_vInitPos;
	g_PlayerEntryHeader.nFadeInCount = 0;
}

void UninitPlayerEntryHeader()
{

}

void UpdatePlayerEntryHeader()
{
	if (g_PlayerEntryHeader.nFadeInCount < gc_nMaxFadeInCount)
	{
		g_PlayerEntryHeader.nFadeInCount++;

		g_PlayerEntryHeader.nA += (gc_nMaxA - gc_nInitA) * 1.0f / (gc_nMaxFadeInCount * 1.0f);
		if (g_PlayerEntryHeader.nFadeInCount == gc_nMaxFadeInCount)
		{
			g_PlayerEntryHeader.nA = gc_nMaxA;
		}
	}
}

void DrawPlayerEntryHeader()
{
	SetTexture(TEX_PLAYER_ENTRY_HEADER);
	SetPolygonColor(D3DCOLOR_ARGB(g_PlayerEntryHeader.nA, 255, 255, 255));
	DrawPolygon
	(
		g_PlayerEntryHeader.vPos.x,
		g_PlayerEntryHeader.vPos.y,
		0,
		0,
		GetTextureStatus(TEX_PLAYER_ENTRY_HEADER).u,
		GetTextureStatus(TEX_PLAYER_ENTRY_HEADER).v
	);
}