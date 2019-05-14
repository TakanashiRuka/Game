//----------------------------------------------------------------------------
//
//	�Ȃ̔w�i
//
//														 ����ҁF����
//                                                       �쐬���F2017/12/26
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "SelectSongBoard.h"

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------
typedef struct {
	float fScaleY;				// �c�̃X�P�[�����O
	int nFadeInCount;			// �t�F�[�h�C���̃J�E���^�[
}SONG_BOARD;					// �Ȃ̔w�i

//---------------------------�O���[�o���ϐ�-----------------------------------
static SONG_BOARD g_SongBoard;

static const int gc_nMaxFadeInCount = 10;			// �Ȕw�i�t�F�[�h�C���J�E���^�[�̍ő�l
static const float gc_fInitScaleY = 0.0f;			// �c�̃X�P�[�����O(�����l)
static const float gc_fEndScaleY = 1.0f;			// �c�̃X�P�[�����O(�I���l)

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitSelectSongBoard()
{
	g_SongBoard.fScaleY = gc_fInitScaleY;
	g_SongBoard.nFadeInCount = 0;
}

void UninitSelectSongBoard()
{

}

void UpdateSelectSongBoard()
{
	if (g_SongBoard.nFadeInCount < gc_nMaxFadeInCount)
	{
		g_SongBoard.nFadeInCount++;
		g_SongBoard.fScaleY += (gc_fEndScaleY - gc_fInitScaleY) / (gc_nMaxFadeInCount * 1.0f);
		if (g_SongBoard.nFadeInCount == gc_nMaxFadeInCount)
		{
			g_SongBoard.fScaleY = gc_fEndScaleY;
		}
	}
}

void DrawSelectSongBoard()
{
	SetTexture(TEX_SELECT_SONG_BOARD);
	SetPolygonColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	SetScale
	(
		1.0f,
		g_SongBoard.fScaleY,
		GetTextureStatus(TEX_SELECT_SONG_BOARD).u / 2.0f,
		GetTextureStatus(TEX_SELECT_SONG_BOARD).v / 2.0f
	);
	DrawPolygon
	(
		WINDOW_WIDTH / 2.0f,
		WINDOW_HEIGHT / 2.0f,
		0,
		0,
		GetTextureStatus(TEX_SELECT_SONG_BOARD).u,
		GetTextureStatus(TEX_SELECT_SONG_BOARD).v
	);
}