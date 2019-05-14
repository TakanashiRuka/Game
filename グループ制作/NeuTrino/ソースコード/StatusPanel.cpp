#include"StatusPanel.h"
#include"main.h"
#include"number.h"
#include"SelectSongPanel.h"
#include"SelectSongFinalCheck.h"
#include"SelectSongWaitMember.h"
#include"Texture.h"
#include"Polygon.h"
#include"Light.h"
#include"debug.h"
#include"data.h"
#include "name.h"		//�ǉ�
#include "sound.h"
#include "icon.h"		//�ǉ�

#define YOUR_INIT_X		(-222.0f)			//�����̏����lX���W
#define YOUR_X			(259.0f)			//�����܂ňړ�
#define RIVAL_INIT_X	(1501.0f)			//����̏����lX���W
#define RIVAL_X			(1020.0f)			//�����܂ňړ�
#define PANEL_Y			(318.0f)			//�p�l���̍���
#define PANEL_SPEED		(60.0f)				//�p�l���̈ړ����x
#define PANEL_ALFA		(0.01f)				//�p�l���̃��ύX�l
#define NAME_FONT_SIZE	(28)				//���O�̕`��T�C�Y

static TEXTURE g_tex_bg;					//�e�N�X�`���̃f�[�^
static TEXTURE g_tex_ready_bg;				//�e�N�X�`���̃f�[�^
static TEXTURE g_tex_ready_status;			//�e�N�X�`���̃f�[�^
static TEXTURE g_tex_icon;					//�e�N�X�`���̃f�[�^
static TEXTURE g_tex_header;				//�e�N�X�`���̃f�[�^
static TEXTURE g_tex_level_word;			//�e�N�X�`���̃f�[�^
static TEXTURE g_tex_join_status;			//�e�N�X�`���̃f�[�^
static TEXTURE g_tex_select_level_num;		//�e�N�X�`���̃f�[�^
static D3DXVECTOR2 g_your;					//�����̍��W
static D3DXVECTOR2 g_rival;					//����̍��W
static bool g_bName;		//�ǉ�
static bool g_bJoinSE;
static bool g_bReadySE;

void InitStatusPanel()
{
	g_tex_bg = GetTextureStatus(TEXTURE_NUM::TEX_FINAL_CHECK_BG);
	g_tex_ready_bg = GetTextureStatus(TEXTURE_NUM::TEX_FINAL_CHECK_READY_BG);
	g_tex_ready_status = GetTextureStatus(TEXTURE_NUM::TEX_FINAL_CHECK_READY_STATUS);
	g_tex_icon = GetTextureStatus(TEXTURE_NUM::TEX_FINAL_CHECK_PLAYER_ICON);
	g_tex_header = GetTextureStatus(TEXTURE_NUM::TEX_FINAL_CHECK_HEADER);
	g_tex_level_word = GetTextureStatus(TEXTURE_NUM::TEX_FINAL_CHECK_LEVEL_WORD);
	g_tex_join_status = GetTextureStatus(TEXTURE_NUM::TEX_FINAL_CHECK_MEMBER_JOIN_STATUS);
	g_tex_select_level_num = GetTextureStatus(TEXTURE_NUM::TEX_SELECT_LEVEL_NUM);
	g_your = D3DXVECTOR2(YOUR_INIT_X, PANEL_Y);
	g_rival = D3DXVECTOR2(RIVAL_INIT_X, PANEL_Y);
	g_bName = false;
	g_bJoinSE = false;
	g_bReadySE = false;
	InitText(FONTLIST::F_USER_NAME, NAME_FONT_SIZE, "MS �R�W�b�N");
}

void UninitStatusPanel()
{
	UninitText(FONTLIST::F_USER_NAME);
}

void UpdateStatusPanel()
{
	if (GetSelectSongFinalCheckStatus())
	{//�I���ȈȊO���t�F�[�h�A�E�g�����ꍇ
		if (!g_bName)		//�ǉ�
		{
			g_bName = true;
		}
		if (g_your.x < YOUR_X && g_rival.x > RIVAL_X)
		{
			g_your.x += PANEL_SPEED;
			g_rival.x -= PANEL_SPEED;
			if (g_your.x > YOUR_X || g_rival.x < RIVAL_X)
			{
				g_your.x = YOUR_X;
				g_rival.x = RIVAL_X;
			}
		}
	}
	else
	{
		if (g_bName)		//�ǉ�
		{
			g_bName = false;
		}
		if (g_your.x > YOUR_INIT_X && g_rival.x < RIVAL_INIT_X)
		{
			g_your.x -= PANEL_SPEED;
			g_rival.x += PANEL_SPEED;
			if (g_your.x < YOUR_INIT_X || g_rival.x > RIVAL_INIT_X)
			{
				g_your.x = YOUR_INIT_X;
				g_rival.x = RIVAL_INIT_X;
			}
		}
	}
}

void DrawStatusPanel()
{
	SetLight(FALSE);
	SetZBuffer(FALSE);
	SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_BG);
	DrawPolygon(g_your.x, g_your.y, 0, 0, g_tex_bg.u, g_tex_bg.v, false);
	SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_READY_BG);
	DrawPolygon(g_your.x + 60.0f, g_your.y + 48.0f, 0, 0, g_tex_ready_bg.u, g_tex_ready_bg.v, false);
	SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_READY_STATUS);
	if (GetRoomReady())
	{
		DrawPolygon(g_your.x + 60.0f, g_your.y + 48.0f, 0, g_tex_ready_status.v, g_tex_ready_status.u, g_tex_ready_status.v, false);
	}
	else
	{
		DrawPolygon(g_your.x + 60.0f, g_your.y + 48.0f, 0, 0, g_tex_ready_status.u, g_tex_ready_status.v, false);
	}
	SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_PLAYER_ICON);
	DrawPolygon(g_your.x - 156.0f, g_your.y + 43.0f, 0, g_tex_icon.v * GetIcon(), g_tex_icon.u, g_tex_icon.v, false);	//�ύX
	SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_HEADER);
	DrawPolygon(g_your.x, g_your.y - 74.0f, 0, 0, g_tex_header.u, g_tex_header.v, false);
	SetTextStatus(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0);
	SetTextStyle(DT_LEFT);
	OutputText(FONTLIST::F_USER_NAME, g_your.x - 205.0f, g_your.y - 42.0f, GetName().c_str());
	SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_LEVEL_WORD);
	DrawPolygon(g_your.x + 150.0f, g_your.y - 37.0f, 0, g_tex_level_word.v * GetLevel(), g_tex_level_word.u, g_tex_level_word.v, false);
	SetTexture(TEXTURE_NUM::TEX_SELECT_LEVEL_NUM);
	DrawPolygon(g_your.x + 193.0f, g_your.y - 37.0f, g_tex_select_level_num.u * (GetLevelNum() / 10), 0, g_tex_select_level_num.u, g_tex_select_level_num.v, false);
	DrawPolygon(g_your.x + 193.0f + 15.0f, g_your.y - 37.0f, g_tex_select_level_num.u * (GetLevelNum() % 10), 0, g_tex_select_level_num.u, g_tex_select_level_num.v, false);

	if (((GetRoomCreate() && Data::g_player.room_entry) || (Data::g_player.room_create && GetRoomEntry())) && Data::g_player.game_mode == MODE_SELECT_SONG)
	{//���C�o�����Q�����Ă��鎞
		if (!g_bJoinSE)
		{
			g_bJoinSE = true;
			if (GetPlayStatus(SE_JOIN));
			{
				StopSound(SE_JOIN);
			}
			PlaySound(SE_JOIN);
		}
		SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_BG);
		DrawPolygon(g_rival.x, g_rival.y, 0, 0, g_tex_bg.u, g_tex_bg.v, false);
		SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_READY_BG);
		DrawPolygon(g_rival.x + 60.0f, g_rival.y + 48.0f, 0, 0, g_tex_ready_bg.u, g_tex_ready_bg.v, false);
		SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_READY_STATUS);
		if (Data::g_player.ready)
		{
			if (!g_bReadySE)
			{
				g_bReadySE = true;
				if (GetPlayStatus(SE_READY))
				{
					StopSound(SE_READY);
				}
				PlaySound(SE_READY);
			}
			DrawPolygon(g_rival.x + 60.0f, g_rival.y + 48.0f, 0, g_tex_ready_status.v, g_tex_ready_status.u, g_tex_ready_status.v, false);
		}
		else
		{
			if (g_bReadySE)
			{
				g_bReadySE = false;
			}
			DrawPolygon(g_rival.x + 60.0f, g_rival.y + 48.0f, 0, 0, g_tex_ready_status.u, g_tex_ready_status.v, false);
		}
		SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_PLAYER_ICON);
		DrawPolygon(g_rival.x - 156.0f, g_rival.y + 43.0f, 0, g_tex_icon.v * Data::g_player.character_icon, g_tex_icon.u, g_tex_icon.v, false);
		SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_HEADER);
		DrawPolygon(g_rival.x, g_rival.y - 74.0f, 0, g_tex_header.v, g_tex_header.u, g_tex_header.v, false);
		SetTextStatus(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0);
		SetTextStyle(DT_LEFT);
		OutputText(FONTLIST::F_USER_NAME, g_rival.x - 205.0f, g_rival.y - 42.0f, "%s", Data::g_player.name.c_str());
		SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_LEVEL_WORD);
		DrawPolygon(g_rival.x + 153.0f, g_rival.y - 37.0f, 0, g_tex_level_word.v * Data::g_player.music_level, g_tex_level_word.u, g_tex_level_word.v, false);
		SetTexture(TEXTURE_NUM::TEX_SELECT_LEVEL_NUM);
		DrawPolygon(g_rival.x + 193.0f, g_rival.y - 37.0f, g_tex_select_level_num.u * (Data::g_player.music_level_num / 10), 0, g_tex_select_level_num.u, g_tex_select_level_num.v, false);
		DrawPolygon(g_rival.x + 193.0f + 15.0f, g_rival.y - 37.0f, g_tex_select_level_num.u * (Data::g_player.music_level_num % 10), 0, g_tex_select_level_num.u, g_tex_select_level_num.v, false);
	}
	else if (GetRoomCreate() && Data::g_player.room_entry == false)
	{//���C�o�����W���̎�
		if (g_bJoinSE)
		{
			g_bJoinSE = false;
		}
		SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_MEMBER_JOIN_STATUS);
		DrawPolygon(g_rival.x, g_rival.y, 0, g_tex_join_status.v, g_tex_join_status.u, g_tex_join_status.v, false);
	}
	else
	{//���C�o�������Ȃ���
		if (g_bJoinSE)
		{
			g_bJoinSE = false;
		}
		SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_MEMBER_JOIN_STATUS);
		DrawPolygon(g_rival.x, g_rival.y, 0, 0, g_tex_join_status.u, g_tex_join_status.v, false);
	}
}