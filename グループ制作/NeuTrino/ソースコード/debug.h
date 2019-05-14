#ifndef DEBUG_H
#define DEBUG_H

typedef enum {
	F_EDIT,
	F_SELECT_SONG_TITLE,
	F_SELECT_SONG_SUBTITLE,
	F_SELECT_NOTE_DESIGNER,
	F_GAME_SONG_TITLE,
	F_SETTING,
	F_TEST_MODE,
	F_VERSION,
	F_USER_NAME,
	F_GAME_PLAYER_NAME,
	F_GAME_RIVAL_NAME,
	F_ERROR_LIST,
	F_RANKING_NAME,		//�ǉ�
	F_PLAYER_ENTRY_CENTER,
	F_PLAYER_ENTRY_SIDE,
	F_PLAYER_ENTRY_NAME,
	F_MAX
}FONTLIST;

void InitDebugText();
void UninitDebugText();

//------------------------------------------------------------------------------
//	�֐���:	SetDebugTextColor
//	����:	�����̐F
//	�߂�l:	�Ȃ�
//	����:	�f�o�b�O�e�L�X�g�F�̕ύX
//------------------------------------------------------------------------------
void SetDebugTextColor(D3DCOLOR color);
void DebugText(int x, int y, const char* pFormat, ...);

//------------------------------------------------------------------------------
//	�֐���:	InitText
//	����:	�t�H���g�i���o�[ �t�H���g�T�C�Y	�t�H���g�X�^�C��
//	�߂�l:	�Ȃ�
//	����:	�t�H���g�̏�����
//------------------------------------------------------------------------------
void InitText(int nNumFont, int nFontSize, const char* pFormat);
void UninitText(int nNumFont);
void OutputText(int nNumFont, int x, int y, const char* pFormat, ...);

//------------------------------------------------------------------------------
//	�֐���:	SetText
//	����:	�t�H���g�i���o�[ �t�H���g�T�C�Y	�t�H���g�X�^�C��
//	�߂�l:	�Ȃ�
//	����:	�t�H���g�T�C�Y�̕ύX(�Ȃ�ׂ��g��Ȃ���)
//------------------------------------------------------------------------------
void SetText(int nNumFont, int nSize, const char* pFormat);

//------------------------------------------------------------------------------
//	�֐���:	SetTextStatus
//	����:	�����̐F�@�t�`�̐F�@�t�`�̑���(0�̎��̓t�`�Ȃ�)
//	�߂�l:	�Ȃ�
//	����:	�t�H���g�F�̕ύX
//------------------------------------------------------------------------------
void SetTextStatus(D3DCOLOR FontColor, D3DCOLOR FontOutColor, int nSize);

//------------------------------------------------------------------------------
//	�֐���:	SetTextStyle
//	����:	DT_LEFT	�Ȃ�
//	�߂�l:	�Ȃ�
//	����:	�t�H���g�̕`��X�^�C���̕ύX
//------------------------------------------------------------------------------
void SetTextStyle(int nStyle);

#endif