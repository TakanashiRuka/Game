//----------------------------------------------------------------------------
//
//	�Q�[���p�̋ȃX�e�[�^�X
//
//														 ����ҁF����
//                                                       �쐬���F2018/01/01
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "Texture.h"
#include "Polygon.h"
#include "SongStatus.h"
#include "SelectSongPanel.h"
#include "debug.h"
#include "number.h"
#include "game.h"
#include "Timer.h"
#include "music.h"

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;					// ���W
	int nA;								// ���l
	TEXTURE tTextureStatus;				// �e�N�X�`���f�[�^
}SONG_BOARD;							// �ȃX�e�[�^�X�w�i

typedef struct {
	D3DXVECTOR2 vPos;					// ���W
	int nA;								// ���l
	char cTitle[MAX_WORD];				// �Ȗ��̕�����
	int nFontSize;						// �����̑傫��
	int nFrame;
}SONG_TITLE;							// �Ȗ�

//---------------------------�O���[�o���ϐ�-----------------------------------
static SONG_BOARD g_SongBoard;
static SONG_BOARD g_SongStatusWord;
static SONG_BOARD g_AlbumBoard;
static SONG_BOARD g_Album;
static SONG_BOARD g_PlayPosition;

static SONG_TITLE g_SongTitle;
static SONG_LEVEL_WORD g_SongLevelWord;

static const int gc_nSongBoardInitA = 255;				// �Ȕw�i�̏������l
static const float gc_fSongTitleMaxX = 265.0f;			// �^�C�g�������̍ő咷��

static const int gc_nSongTitleMaxFontSize = 26;			// �t�H���g�̍ő�T�C�Y

static const int gc_nDrawFrameFontSize = 22;			// �t�`��`�悳����̍ő�T�C�Y

static const int gc_nMaxDigitLevel = 2;					// ���x�������̌���
static const D3DXVECTOR2 gc_vLevelNumInitPos = { 86.0f,146.0f };			// ���x�������̏������W
static const float gc_fLevelNumRageX = 10.0f;								// ���x�������̊Ԋu

static const D3DXVECTOR2 gc_vLevelWordInitPos = { 45.0f,146.0f };			// ���x�������̏������W

static const float gc_fPlayPosScale = GetTextureStatus(TEX_GAME_GAUGE_BOARD).u;
static const int gc_nPlayPosInitA = 100;

//---------------------------�v���g�^�C�v�錾---------------------------------
float GetPlayPosPersent();

//---------------------------------------------------------------
// �w�i����
//---------------------------------------------------------------
void InitSongBoard()
{
	g_SongBoard.nA = gc_nSongBoardInitA;
	g_SongBoard.tTextureStatus = GetTextureStatus(TEX_GAME_SONG_BOARD);
	g_SongBoard.vPos =
	{
		g_SongBoard.tTextureStatus.u / 2.0f - 3.0f,
		g_SongBoard.tTextureStatus.v / 2.0f - 2.0f
	};
}

void UninitSongBoard()
{

}

void UpdateSongBoard()
{

}

void DrawSongBoard()
{
	SetTexture(TEX_GAME_SONG_BOARD);
	SetPolygonColor(D3DCOLOR_ARGB(g_SongBoard.nA, 255, 255, 255));
	DrawPolygon
	(
		g_SongBoard.vPos.x,
		g_SongBoard.vPos.y,
		0,
		0,
		g_SongBoard.tTextureStatus.u,
		g_SongBoard.tTextureStatus.v
	);
}

//---------------------------------------------------------------
// �ȃX�e�[�^�X����(�����p)
//---------------------------------------------------------------
void InitSongStatusWord()
{
	g_SongStatusWord.nA = gc_nSongBoardInitA;
	g_SongStatusWord.tTextureStatus = GetTextureStatus(TEX_GAME_SONG_STATUS_WORD);
	g_SongStatusWord.vPos = { 60.0f,16.0f };
}

void UninitSongStatusWord()
{

}

void UpdateSongStatusWord()
{

}

void DrawSongStatusWord()
{
	SetTexture(TEX_GAME_SONG_STATUS_WORD);
	SetPolygonColor(D3DCOLOR_ARGB(g_SongStatusWord.nA, 255, 255, 255));
	DrawPolygon
	(
		g_SongStatusWord.vPos.x,
		g_SongStatusWord.vPos.y,
		0,
		0,
		g_SongStatusWord.tTextureStatus.u,
		g_SongStatusWord.tTextureStatus.v
	);
}

//---------------------------------------------------------------
// �A���o���摜�̔w�i
//---------------------------------------------------------------
void InitGameAlbumBoard()
{
	g_AlbumBoard.nA = gc_nSongBoardInitA;
	g_AlbumBoard.tTextureStatus = GetTextureStatus(TEX_GAME_ALBUM_BOARD);
	g_AlbumBoard.vPos = { 60.0f,80.0f };
}

void UninitGameAlbumBoard()
{

}

void UpdateGameAlbumBoard()
{

}

void DrawGameAlbumBoard()
{
	SetTexture(TEX_GAME_ALBUM_BOARD);
	SetPolygonColor(D3DCOLOR_ARGB(g_AlbumBoard.nA, 255, 255, 255));
	DrawPolygon
	(
		g_AlbumBoard.vPos.x,
		g_AlbumBoard.vPos.y,
		0,
		0,
		g_AlbumBoard.tTextureStatus.u,
		g_AlbumBoard.tTextureStatus.v
	);
}

//---------------------------------------------------------------
// �A���o���摜
//---------------------------------------------------------------
void InitGameAlbum()
{
	g_Album.nA = gc_nSongBoardInitA;
	g_Album.tTextureStatus = GetTextureStatus(TEX_SELECT_ALBUM_00);
	g_Album.vPos = g_AlbumBoard.vPos;
}

void UninitGameAlbum()
{

}

void UpdateGameAlbum()
{

}

void DrawGameAlbum()
{
	SetTexture(TEX_SELECT_ALBUM_00);
	SetPolygonColor(D3DCOLOR_ARGB(g_Album.nA, 255, 255, 255));
	DrawPolygon
	(
		g_Album.vPos.x,
		g_Album.vPos.y,
		0,
		0,
		g_Album.tTextureStatus.u,
		g_Album.tTextureStatus.v
	);
}

//----------------------------------------------------------------
// �Ȗ�
//----------------------------------------------------------------
void InitGameSongTitle()
{
	g_SongTitle.nA = gc_nSongBoardInitA;
	g_SongTitle.vPos = { 122.0f,33.0f };
	float fLen = strlen(&g_SongTitle.cTitle[0]);
	g_SongTitle.nFontSize = gc_fSongTitleMaxX / fLen * 2.0f;

	if (g_SongTitle.nFontSize > gc_nSongTitleMaxFontSize)
	{
		g_SongTitle.nFontSize = gc_nSongTitleMaxFontSize;
	}
	if (g_SongTitle.nFontSize <= gc_nDrawFrameFontSize)
	{
		g_SongTitle.nFrame = 1;
	}
	else
	{
		g_SongTitle.nFrame = 0;
	}
	InitText(F_GAME_SONG_TITLE, g_SongTitle.nFontSize, "MS �R�W�b�N");
}

void UninitGameSongTitle()
{
	UninitText(F_GAME_SONG_TITLE);
}

void UpdateGameSongTitle()
{

}

void DrawGameSongTitle()
{
	SetTextStyle(DT_LEFT);
	SetTextStatus(D3DCOLOR_ARGB(g_SongTitle.nA, 222, 222, 222), D3DCOLOR_ARGB(g_SongTitle.nA, 222, 222, 222), 0/*g_SongTitle.nFrame*/);
	OutputText(F_GAME_SONG_TITLE, g_SongTitle.vPos.x, g_SongTitle.vPos.y, "%s", &g_SongTitle.cTitle[0]);
}

void SetSongTitleGame(char *cTitle)
{
	g_SongTitle.cTitle[0] = '\0';
	strcat(g_SongTitle.cTitle, cTitle);
}

//--------------------------------------------------------------
// ��Փx(����)
//--------------------------------------------------------------
void InitGameLevelWord()
{
	g_SongLevelWord.nA = gc_nSongBoardInitA;
	g_SongLevelWord.nFadeInCount = 0;
	switch (GetGameModeTarget())
	{
	case MODE_TUTORIAL:
	{
		g_SongLevelWord.nV = 0;
		break;
	}
	case MODE_DEMO:
	case MODE_GAME:
	{
		g_SongLevelWord.nV = GetLevel();
		break;
	}
	}
	g_SongLevelWord.vPos = gc_vLevelWordInitPos;
}

void UninitGameLevelWord()
{

}

void UpdateGameLevelWord()
{

}

void DrawGameLevelWord()
{
	SetTexture(TEX_GAME_LEVEL_WORD);
	TEXTURE tLevelWord = GetTextureStatus(TEX_GAME_LEVEL_WORD);
	SetPolygonColor(D3DCOLOR_ARGB(g_SongLevelWord.nA, 255, 255, 255));
	DrawPolygon
	(
		g_SongLevelWord.vPos.x,
		g_SongLevelWord.vPos.y,
		0,
		tLevelWord.v * g_SongLevelWord.nV,
		tLevelWord.u,
		tLevelWord.v
	);
}

//---------------------------------------------------------------
// ��Փx(����)
//---------------------------------------------------------------
void DrawGameLevelNum()
{
	TEXTURE tLevelNum = GetTextureStatus(TEX_GAME_LEVEL_NUM);
	int nLevelNum;
	switch (GetGameMode())
	{
	case MODE_TUTORIAL:
	{
		nLevelNum = 1;
		break;
	}
	case MODE_DEMO:
	case MODE_GAME:
	case MODE_RESULT:
	{
		nLevelNum = GetLevelNum();
		break;
	}
	}
	int n_LvBackUp = nLevelNum;
	int nDigit = 0;

	do
	{
		n_LvBackUp /= 10;
		nDigit++;
	} while (n_LvBackUp != 0);

	n_LvBackUp = nLevelNum;
	int count;
	for (count = 0; count < nDigit; count++)
	{
		int n = n_LvBackUp % 10;
		DrawNumber(TEX_GAME_LEVEL_NUM, tLevelNum.u, tLevelNum.v, n, gc_vLevelNumInitPos.x - (count)* gc_fLevelNumRageX, gc_vLevelNumInitPos.y, g_SongLevelWord.nA);
		n_LvBackUp /= 10;
	}
	for (count; count < gc_nMaxDigitLevel; count++)
	{
		DrawNumber(TEX_GAME_LEVEL_NUM, tLevelNum.u, tLevelNum.v, 0, gc_vLevelNumInitPos.x - (count)* gc_fLevelNumRageX, gc_vLevelNumInitPos.y, g_SongLevelWord.nA);
	}
}

//--------------------------------------------------------------------
// �Đ��ʒu
//--------------------------------------------------------------------
void InitPlayPosition()
{
	g_PlayPosition.vPos = { WINDOW_WIDTH / 2.0f - gc_fPlayPosScale / 2.0f,1.0f };
	g_PlayPosition.tTextureStatus = GetTextureStatus(TEX_SLIDE_SHADOW);
	g_PlayPosition.nA = gc_nPlayPosInitA;
}

void UninitPlayPosition()
{

}

void UpdatePlayPosition()
{

}

void DrawPlayPosition()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_SLIDE_SHADOW);
	SetPolygonColor(D3DCOLOR_ARGB(g_PlayPosition.nA, 255, 255, 255));
	SetScale(gc_fPlayPosScale * GetPlayPosPersent(), 3.0f, 0, 0);
	DrawPolygon
	(
		g_PlayPosition.vPos.x,
		g_PlayPosition.vPos.y,
		0,
		0,
		g_PlayPosition.tTextureStatus.u,
		g_PlayPosition.tTextureStatus.v
	);

	//
	SetTexture(TEX_SLIDE_SHADOW);
	SetPolygonColor(D3DCOLOR_ARGB(200, 255, 255, 255));
	SetScale(4.0f, 4.0f, 0, 0);
	DrawPolygon
	(
		g_PlayPosition.vPos.x - 1.0f + gc_fPlayPosScale * GetPlayPosPersent(),
		g_PlayPosition.vPos.y + 0.0f,
		0,
		0,
		g_PlayPosition.tTextureStatus.u,
		g_PlayPosition.tTextureStatus.v
	);
	SetZBuffer(TRUE);
}

float GetPlayPosPersent()
{
	float fPersent = (GetTimer() - GetPlayMusicOffset()) * 1.0f / GetMusicLength(MUSIC);
	if (fPersent < 0.0f)
	{
		fPersent = 0.0f;
	}
	if (fPersent > 1.0f)
	{
		fPersent = 1.0f;
	}
	return fPersent;
}
