#ifndef TEXTURE_H
#define TEXTURE_H

typedef enum TEXTURE_NUM
{
	TEX_LANE,							// ���[��
	TEX_LANE_EFFECT,					// ���[���̃G�t�F�N�g
	TEX_BTN_LIGHT,						// �{�^�����C�g
	TEX_JUDGE_LINE,						// ����o�[
	TEX_JUDGE_LINE_SP,					// ����o�[
	TEX_TAP,							// �^�b�v
	TEX_BREAK,							// �u���C�N
	TEX_HOLD_START,						// �z�[���h�̎n�܂�
	TEX_HOLD_MIDDLE,					// �z�[���h�̒���
	TEX_HOLD_END,						// �z�[���h�̏I���
	TEX_FLICK,							// �t���b�N
	TEX_FLICK_ARROW_UP,					// �t���b�N�̃A���[(��)
	TEX_FLICK_ARROW_DOWN,				// �t���b�N�̃A���[(��)
	TEX_FLICK_ARROW_LEFT,				// �t���b�N�̃A���[(��)
	TEX_FLICK_ARROW_RIGHT,				// �t���b�N�̃A���[(�E)
	TEX_SLIDE_START,					// �X���C�h�̎n�܂�
	TEX_SLIDE_MIDDLE,					// �X���C�h�̒���
	TEX_SLIDE_END,						// �X���C�h�̏I���
	TEX_SLIDE_SHADOW,					// �X���C�h�̉e
	TEX_SLIDE_S_E_MIDDLE,				// �X���C�h�̎n�܂�/�I���(�J�n�I���t���O�Ȃ�)
	TEX_LINE,							// ���߃��C��
	TEX_JUDGE_WORD,						// ���蕶��
	TEX_SELECT_SONG_BG,					// �ȑI����ʂ̔w�i
	TEX_SELECT_SONG_FRAME,				// �ȑI����ʂ̃t���[��
	TEX_SELECT_SONG_BLUE,				// �ȑI����ʂ̘Z�p�`(��)
	TEX_SELECT_SONG_RED,				// �ȑI����ʂ̘Z�p�`(��)
	TEX_SELECT_SONG_LINE,				// �ȑI����ʂ̔w�i(�c��)
	TEX_SELECT_SONG_BOARD,				// �Ȃ̔w�i
	TEX_SELECT_SONG_WORD,				// �ȑI�����(�����p)
	TEX_SELECT_SONG_PANEL,				// �ȃp�l��
	TEX_SELECT_LEVEL_WORD,				// ��Փx(����)
	TEX_SELECT_LEVEL_NUM,				// ��Փx(����)
	TEX_SELECT_ALBUM_00,				// �Ȃ̃A���o���摜(�Q�[���p)
	TEX_SELECT_ALBUM_01,				// �Ȃ̃A���o���摜(01)
	TEX_SELECT_ALBUM_02,				// �Ȃ̃A���o���摜(02)
	TEX_SELECT_ALBUM_03,				// �Ȃ̃A���o���摜(03)
	TEX_SELECT_ALBUM_04,				// �Ȃ̃A���o���摜(04)
	TEX_SELECT_ALBUM_05,				// �Ȃ̃A���o���摜(05)
	TEX_SELECT_ALBUM_06,				// �Ȃ̃A���o���摜(06)
	TEX_SELECT_ALBUM_07,				// �Ȃ̃A���o���摜(07)
	TEX_SELECT_LEVEL_ICON,				// ���x���A�C�R��
	TEX_SELECT_ALBUM_FRAME,				// �A���o���摜�̃t�`
	TEX_GAME_JUDGE_TAP_EFFECT_UNDER,	// �^�b�v����G�t�F�N�g�̉�����
	TEX_GAME_JUDGE_TAP_EFFECT_UP,		// �^�b�v����G�t�F�N�g�̏㕔��
	TEX_GAME_JUDGE_FLICK_EFFECT_UP,		// �t���b�N����G�t�F�N�g�̏㕔��
	TEX_GAME_JUDGE_SLIDE_EFFECT_UNDER,	// �X���C�h����G�t�F�N�g�̉�����
	TEX_GAME_SCORE_BOARD,				// �X�R�A�{�[�h
	TEX_GAME_SCORE_WORD,				// �X�R�A����
	TEX_GAME_SCORE_NUM,					// �X�R�A����
	TEX_GAME_GAUGE_BOARD,				// �Q�[�W�{�[�h
	TEX_GAME_GAUGE_BG,					// �Q�[�W�̔w�i
	TEX_GAME_GAUGE_CLEAR_LINE,			// �Q�[�W�̍��i���C��
	TEX_GAME_GAUGE_INSIDE,				// �Q�[�W�̓��e
	TEX_GAME_COMBO_WORD,				// �R���{����
	TEX_GAME_COMBO_NUM,					// �R���{����
	TEX_GAME_MAX_COMBO_WORD,			// �ő�R���{����
	TEX_GAME_MAX_COMBO_NUM,				// �ő�R���{����
	TEX_GAME_SONG_BOARD,				// �ȃX�e�[�^�X�p�w�i
	TEX_GAME_SONG_STATUS_WORD,			// �ȃX�e�[�^�X����(�����p)
	TEX_GAME_ALBUM_BOARD,				// �A���o���摜�̔w�i
	TEX_GAME_LEVEL_WORD,				// �Q�[���p��Փx�\��
	TEX_GAME_LEVEL_NUM,					// �Q�[���p��Փx�\��
	TEX_RESULT_BOARD,					// ���U���g(�ڍ׌���)�̔w�i
	TEX_RESULT_WORD,					// ���U���g����
	TEX_RESULT_SCORE_WORD,				// ���U���g�̃X�R�A����
	TEX_RESULT_RANK_WORD,				// ���U���g�̃����N����
	TEX_RESULT_JUDGE_DETAIL_BOARD,		// ���U���g�ڍ׌��ʂ̔w�i
	TEX_RESULT_SCORE_NUM,				// ���U���g�̃X�R�A
	TEX_RESULT_JUDGE_LEVEL,				// ���U���g�̔��背�x��
	TEX_RESULT_DETAIL_NUM,				// ���U���g�̏ڍ׌��ʂ̐���
	TEX_RESULT_RANK_BOARD,				// ���U���g�̃����N�̔w�i
	TEX_RESULT_RANK,					// ���U���g�̃����N
	TEX_GAME_CLEAR_FAILED_BOARD,		// �Q�[���]��(�N���A/���s)�̔w�i
	TEX_GAME_CLEAR_FAILED_WORD,			// �Q�[���]��(�N���A/���s)�̕���
	TEX_GAME_FC_AP_BOARD,				// �Q�[���]��(FC/AP)�̔w�i
	TEX_GAME_FC_AP_WORD,				// �Q�[���]��(FC/AP)�̕���
	TEX_SETTING_BOARD,					// �ݒ�̔w�i
	TEX_SETTING_TITLE_WORD,				// �ݒ��ʂ̃^�C�g��
	TEX_SETTING_WORD,					// �ݒ��ʂ̍���
	TEX_SETTING_ARROW,					// �ݒ��ʂ̖��
	TEX_SETTING_MANUAL_BOARD,			// �ݒ荀�ڂ̐���(�w�i)
	TEX_SETTING_MANUAL_LIST,			// �ݒ荀�ڂ̐���(���e)
	TEX_RESULT_ACHIEVEMENT_ICON,		// ���ʉ�ʂ̒B������
	TEX_TITLE_BG,						// �^�C�g���̔w�i
	TEX_TITLE_BG_SQUARE_SMALL,			// �^�C�g���̎l�p�`(��)
	TEX_TITLE_BG_SQUARE_BIG,			// �^�C�g���̎l�p�`(��)
	TEX_TITLE_BG_FLOOR,					// �^�C�g���̏�
	TEX_TITLE_BG_HEXANGONAL,			// �^�C�g���̘Z�p�`
	TEX_TITLE_BG_LOGO,					// �^�C�g���̃��S
	TEX_PREFACE_CONTROLLER,				// �O����(�R���g���[��)
	TEX_PREFACE_TEAM_LOGO,				// �O����(�`�[�����S)
	TEX_TITLE_BTN_MANUAL,				// �^�C�g���̑������
	TEX_TITLE_ALL_RIGHTS_RESERVED,		// �^�C�g����AllRightReserved
	TEX_GAME_MAIN_BG,					// �Q�[���̔w�i(���C��)
	TEX_GAME_MAIN_BG_MASK,				// �Q�[���̔w�i(�}�X�N)
	TEX_GAME_BG_WALL,					// �Q�[���̔w�i(�ǂ̘Z�p�`)
	TEX_GAME_BG_LINE,					// �Q�[���̔w�i(����)
	TEX_GAME_BG_FLOOR,					// �Q�[���̔w�i(��)
	TEX_FADEINOUT_CENTER,				// �t�F�[�h�C���A�E�g(���S)
	TEX_FADEINOUT_LEFT,					// �t�F�[�h�C���A�E�g(��)
	TEX_FADEINOUT_RIGHT,				// �t�F�[�h�C���A�E�g(�E)
	TEX_FADEINOUT_LEFT_UP,				// �t�F�[�h�C���A�E�g(����)
	TEX_FADEINOUT_RIGHT_UP,				// �t�F�[�h�C���A�E�g(�E��)
	TEX_FADEINOUT_LEFT_DOWN,			// �t�F�[�h�C���A�E�g(����)
	TEX_FADEINOUT_RIGHT_DOWN,			// �t�F�[�h�C���A�E�g(�E��)
	TEX_RESULT_CLEAR_FAILED,			// ���U���g��ʂ̃N���A/���s�A�C�R��
	TEX_FINAL_CHECK_BG,
	TEX_FINAL_CHECK_READY_BG,
	TEX_FINAL_CHECK_READY_STATUS,
	TEX_FINAL_CHECK_PLAYER_ICON,
	TEX_FINAL_CHECK_HEADER,
	TEX_FINAL_CHECK_LEVEL_WORD,
	TEX_FINAL_CHECK_MEMBER_JOIN_STATUS,
	TEX_SELECT_SONG_WAIT_MEMBER_BG,
	TEX_SELECT_SONG_WAIT_MEMBER_BTN,
	TEX_NETWORK_STATUS,
	TEX_BTN_MANUAL,						// �{�^���̑������
	TEX_TITLE_MASK,						// �^�C�g���}�X�N
	TEX_GAME_PLAYER_BOARD,
	TEX_GAME_PLAYER_MATCHING_RANK,
	TEX_GAME_RIVAL_BOARD,
	TEX_GAME_JUDGE_SLIDE_EFFECT_UP,		// �X���C�h�̔���G�t�F�N�g(��)
	TEX_RESULT_BTN,						// ���U���g(�{�^��)
	TEX_ERROR_LIST_BG,					// �G���[���X�g�̔w�i
	TEX_GAME_PREFACE_BG_UNDER,			// �Q�[���O����(�w�i�E��)
	TEX_GAME_PREFACE_BG_UP,				// �Q�[���O����(�w�i�E��)
	TEX_GAME_PREFACE_READY,				// �Q�[���O����(READY)
	TEX_GAME_PREFACE_START,				// �Q�[���O����(START)
	TEX_RANKING_BG,
	TEX_RANKING_HEADER,
	TEX_RANKING_RANK,
	TEX_RANKING_SCORE_NUM,
	TEX_TUTORIAL_BTN,					// �`���[�g���A���{�^��
	TEX_RESULT_RANKING_FILTER,
	TEX_GAME_JUDGE_EARLY_LATE,			// �p�[�t�F�N�g����̌덷
	TEX_THANKS_BG,						// �㏑���̔w�i
	TEX_THANKS_WORD,					// �㏑���̕���
	TEX_THANKS_MASK,					// �㏑���̕���
	TEX_PLAYER_ENTRY_HEADER,
	TEX_PLAYER_ENTRY_WORD_BORAD,
	TEX_PLAYER_ENTRY_NAME_BG,
	TEX_PLAYER_ENTRY_ARROW,
	TEX_CONTINUE_WORD,
	TEX_CONTINUE_BTN,
	TEX_MAX
};

typedef struct {
	char filename[256];
	int width;
	int height;
	int u, v;
}TEXTURE;

void InitTexture();
void UninitTexture();

//------------------------------------------------------------------------------
//	�֐���:	SetTextureAddress
//	����:	TEXTURE_NUM	 �t�@�C����URL
//	�߂�l:	�Ȃ�
//	����:	�w�肳�ꂽ�e�N�X�`���̓���ւ�
//------------------------------------------------------------------------------
void SetTextureAddress(int nNumTex, char *pFileAddress);

//------------------------------------------------------------------------------
//	�֐���:	GetTextureStatus
//	����:	TEXTURE_NUM	
//	�߂�l:	�Ȃ�
//	����:	�e�N�X�`���̊e�f�[�^�̎擾
//------------------------------------------------------------------------------
TEXTURE GetTextureStatus(int nNum);

//------------------------------------------------------------------------------
//	�֐���:	GetTexture
//	����:	TEXTURE_NUM	
//	�߂�l:	�e�N�X�`���^�f�[�^
//	����:	�e�N�X�`���̎擾
//------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 GetTexture(int nNum);

//------------------------------------------------------------------------------
//	�֐���:	SetTexture
//	����:	TEXTURE_NUM	
//	�߂�l:	�Ȃ�
//	����:	�e�N�X�`���̐ݒ�
//------------------------------------------------------------------------------
void SetTexture(int nTexNum);

//------------------------------------------------------------------------------
//	�֐���:	GetTextureNum
//	����:	�Ȃ�	
//	�߂�l:	�e�N�X�`���̔ԍ�
//	����:	�e�N�X�`���̔ԍ����擾
//------------------------------------------------------------------------------
int GetTextureNum();
#endif // !TEXTURE_H
