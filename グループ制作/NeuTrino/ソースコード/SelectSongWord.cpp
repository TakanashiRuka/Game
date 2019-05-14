//----------------------------------------------------------------------------
//
//	�ȑI�����(�������ȑI����ʂ������Ă���)
//
//														 ����ҁF����
//                                                       �쐬���F2017/12/26
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "SelectSongWord.h"

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;			// ���W
	int nA;						// ���l
	int nFadeInCount;			// �t�F�[�h�C���J�E���^�[
}SELECT_SONG_WORD;				// �ȑI�����(�����p)

//---------------------------�O���[�o���ϐ�-----------------------------------
static SELECT_SONG_WORD g_SelectSongWord;

static const int gc_nInitA = 0;											// ���l(�����l)
static const int gc_nMaxA = 255;										// ���l(�I���l)
static const int gc_nMaxFadeInCount = 10;								// �t�F�[�h�C���J�E���^�[�̍ő�l
static const D3DXVECTOR2 gc_vInitPos = { WINDOW_WIDTH / 2.0f,132.0f };	// ���W(�����l)

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitSelectSongWord()
{
	g_SelectSongWord.nA = gc_nInitA;
	g_SelectSongWord.vPos = gc_vInitPos;
	g_SelectSongWord.nFadeInCount = 0;
}

void UninitSelectSongWord()
{

}

void UpdateSelectSongWord()
{
	if (g_SelectSongWord.nFadeInCount < gc_nMaxFadeInCount)
	{
		g_SelectSongWord.nFadeInCount++;

		g_SelectSongWord.nA += (gc_nMaxA - gc_nInitA) * 1.0f / (gc_nMaxFadeInCount * 1.0f);
		if (g_SelectSongWord.nFadeInCount == gc_nMaxFadeInCount)
		{
			g_SelectSongWord.nA = gc_nMaxA;
		}
	}
}

void DrawSelectSongWord()
{
	SetTexture(TEX_SELECT_SONG_WORD);
	SetPolygonColor(D3DCOLOR_ARGB(g_SelectSongWord.nA, 255, 255, 255));
	DrawPolygon
	(
		g_SelectSongWord.vPos.x,
		g_SelectSongWord.vPos.y,
		0,
		0,
		GetTextureStatus(TEX_SELECT_SONG_WORD).u,
		GetTextureStatus(TEX_SELECT_SONG_WORD).v
	);
}