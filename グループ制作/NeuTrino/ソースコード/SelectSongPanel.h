//----------------------------------------------------------------------------
//
//	�ȃp�l��
//
//														 ����ҁF����
//                                                       �쐬���F2017/12/27
//----------------------------------------------------------------------------

#ifndef SELECT_SONG_PANEL_H
#define SELECT_SONG_PANEL_H

//---------------------------�C���N���[�h-------------------------------------

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------
typedef struct {
	char c_NoteFileAddress[MAX_WORD];			// ���ʃt�@�C����URL
	char c_SongFileAddress[MAX_WORD];			// �ȃt�@�C����URL
	char c_AlbumFileAddress[MAX_WORD];			// �ȃT���l�t�@�C����URL
	unsigned int n_SampleBeginTime;				// �T���v���Đ��̎n�܂莞�_
	float fBPM;									// �e���|
	int nLevel[MAX_LEVEL];						// ���x��
	char c_Title[MAX_WORD];						// �^�C�g��
	char c_Subtitle[MAX_WORD];					// �T�u�^�C�g��
	char c_NoteDesigner[MAX_WORD];				// ���ʍ쐬��
	bool bDemo;									// �f�����[�h�ŏo�����ǂ���
}SSSONGSTATUS;									// �Ȃ̃f�[�^

typedef struct {
	D3DXVECTOR2 vPos;							// ���W
	int nA;										// ���l
	int nV;										// V�̒l
	int	nFadeInCount;							// �t�F�[�h�C���J�E���^�[
}SONG_LEVEL_WORD;								// ��Փx(����)

//---------------------------�O���[�o���ϐ�-----------------------------------

//---------------------------�v���g�^�C�v�錾---------------------------------

void InitSelectSongPanel();
void UninitSelectSongPanel();
void UpdateSelectSongPanel();
void DrawSelectSongPanel();

void InitAllSongBuffer();

//------------------------------------------------------------------------------
//	�֐���:	SetSongStatus
//	����:	�t�@�C���̔ԍ��@�t�@�C����URL
//	�߂�l:	�Ȃ�
//	����:	�ȃf�[�^��ݒ肷��
//------------------------------------------------------------------------------
void SetSongStatus(int nSongNum, char *cNoteAddress);

void DrawSongStatus();

//------------------------------------------------------------------------------
//	�֐���:	GetLevel
//	����:	�Ȃ�
//	�߂�l:	���x��
//	����:	���x�����擾����
//------------------------------------------------------------------------------
int GetLevel();

//------------------------------------------------------------------------------
//	�֐���:	SetLevel
//	����:	���x��
//	�߂�l:	�Ȃ�
//	����:	���x����ݒ肷��
//------------------------------------------------------------------------------
void SetLevel(int nLevel);

//------------------------------------------------------------------------------
//	�֐���:	GetSelectNum
//	����:	�Ȃ�
//	�߂�l:	�Ȃ̔ԍ�
//	����:	�Ȃ̔ԍ����擾����
//------------------------------------------------------------------------------
int GetSelectNum();

//------------------------------------------------------------------------------
//	�֐���:	SetSelectNum
//	����:	�Ȃ̔ԍ�
//	�߂�l:	�Ȃ�
//	����:	�Ȃ̔ԍ���ݒ肷��
//------------------------------------------------------------------------------
void SetSelectNum(int nSelectNum);

//------------------------------------------------------------------------------
//	�֐���:	GetLevelNum
//	����:	�Ȃ�
//	�߂�l:	���x������
//	����:	���x���������擾����
//------------------------------------------------------------------------------
int GetLevelNum();

void InitSongTitle();
void UninitSongTitle();
void UpdateSongTitle();
void DrawSongTitle();

void InitSongSubTitle();
void UninitSongSubTitle();
void UpdateSongSubTitle();
void DrawSongSubTitle();

void InitNoteDesigner();
void UninitNoteDesigner();
void UpdateNoteDesigner();
void DrawNoteDesigner();

void InitLevelWord();
void UninitLevelWord();
void UpdateLevelWord();
void DrawLevelWord();

void DrawLevelNum();
void DrawSongAlbum();

void InitLevelIcon();
void UninitLevelIcon();
void UpdateLevelIcon();
void DrawLevelIcon();

void InitAlbumFrame();
void UninitAlbumFrame();
void UpdateAlbumFrame();
void DrawAlbumFrame();

//------------------------------------------------------------------------------
//	�֐���:	SetSongPanelA
//	����:	���l�̔ԍ�
//	�߂�l:	�Ȃ�
//	����:	�ȃp�l���̃��l��ϊ�������
//------------------------------------------------------------------------------
void SetSongPanelA(int nParts);

//------------------------------------------------------------------------------
//	�֐���:	GetSongPanelChangeAStatus
//	����:	�Ȃ�
//	�߂�l:	�ȃp�l���̃��l�̕ω��t���O
//	����:	�ȃp�l���̃��l�̕ω��t���O���擾����
//------------------------------------------------------------------------------
bool GetSongPanelChangeAStatus();

//------------------------------------------------------------------------------
//	�֐���:	ChangeSelectNum
//	����:	�Ȃ̔ԍ�
//	�߂�l:	�Ȃ�
//	����:	�I�����Ă���Ȃ�ݒ肳����
//------------------------------------------------------------------------------
void ChangeSelectNum(int nNum);

SSSONGSTATUS GetSSSongStatus(int nSongNum);

bool GetIsDemo();

#endif // !SELECT_SONG_PANEL_H
