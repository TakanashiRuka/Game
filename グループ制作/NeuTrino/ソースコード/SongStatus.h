//----------------------------------------------------------------------------
//
//	�Q�[���p�̋ȃX�e�[�^�X
//
//														 ����ҁF����
//                                                       �쐬���F2018/01/01
//----------------------------------------------------------------------------

#ifndef SONG_STATUS_H
#define SONG_STATUS_H

//---------------------------�C���N���[�h-------------------------------------

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------

//---------------------------�O���[�o���ϐ�-----------------------------------

//---------------------------�v���g�^�C�v�錾---------------------------------

void InitSongBoard();
void UninitSongBoard();
void UpdateSongBoard();
void DrawSongBoard();

void InitSongStatusWord();
void UninitSongStatusWord();
void UpdateSongStatusWord();
void DrawSongStatusWord();

void InitGameAlbumBoard();
void UninitGameAlbumBoard();
void UpdateGameAlbumBoard();
void DrawGameAlbumBoard();

void InitGameAlbum();
void UninitGameAlbum();
void UpdateGameAlbum();
void DrawGameAlbum();

void InitGameSongTitle();
void UninitGameSongTitle();
void UpdateGameSongTitle();
void DrawGameSongTitle();

//------------------------------------------------------------------------------
//	�֐���:	SetSongTitleGame
//	����:	�^�C�g��������̐擪�A�h���X
//	�߂�l:	�Ȃ�
//	����:	�Ȗ���ݒ肷��
//------------------------------------------------------------------------------
void SetSongTitleGame(char *cTitle);

void InitGameLevelWord();
void UninitGameLevelWord();
void UpdateGameLevelWord();
void DrawGameLevelWord();

void DrawGameLevelNum();

void InitPlayPosition();
void UninitPlayPosition();
void UpdatePlayPosition();
void DrawPlayPosition();

#endif // !SONG_STATUS_H
