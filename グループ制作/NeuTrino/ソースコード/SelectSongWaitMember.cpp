#include "SelectSongWaitMember.h"
#include"main.h"
#include"SelectSongPanel.h"
#include"SelectSongFinalCheck.h"
#include"Texture.h"
#include"Polygon.h"
#include"input.h"
#include"JoyStick.h"
#include"Light.h"
#include"data.h"
#include "Setting.h"
#include "BtnManual.h"
#include "FadeInOut.h"
#include "sound.h"

#define WAIT_MEMBER_INIT_X		(1453.0f)		//初期値
#define WAIT_MEMBER_X			(1107.0f)		//ここまで移動
#define WAIT_MEMBER_INIT_Y		(84.0f)			//初期値
#define WAIT_MEMBER_SPEED		(30.0f)			//移動速度
#define WAIT_CNT				(60)			//
#define ANIMATION_CNT			(20)			//

static TEXTURE g_tex_bg;
static TEXTURE g_tex_btn;
static D3DXVECTOR2 g_wait_member;
static float g_fAlfa;
static bool g_bCreate;
static bool g_bEntry;
static bool g_bReady;
static int g_nAnimationCnt;
static bool g_bRoomEntryReady;
static int g_nMessageBtnV;

static const int gc_nReadyCancelCountMax = 90;
static const int gc_nReadyCancelCountMaxAble = gc_nReadyCancelCountMax - 30;
static int g_nReadyCancelCount;
static bool g_bReadyFinalCheck;
static bool g_bMessageSE;
static bool g_bCreateSE;

void InitSelectSongWaitMember()
{
	g_bMessageSE = false;
	g_bCreateSE = false;
	g_tex_bg = GetTextureStatus(TEXTURE_NUM::TEX_SELECT_SONG_WAIT_MEMBER_BG);
	g_tex_btn = GetTextureStatus(TEXTURE_NUM::TEX_SELECT_SONG_WAIT_MEMBER_BTN);
	g_wait_member = D3DXVECTOR2(WAIT_MEMBER_INIT_X, WAIT_MEMBER_INIT_Y);
	g_bCreate = false;
	g_bEntry = false;
	g_bReady = false;
	g_bRoomEntryReady = false;
	g_bReadyFinalCheck = false;
	g_fAlfa = 0.0f;
	g_nReadyCancelCount = 0;
	if (!GetJSDev())
	{
		g_nMessageBtnV = 0;
	}
	else
	{
		g_nMessageBtnV = 1;
	}
}

void UninitSelectSongWaitMember()
{

}

void UpdateSelectSongWaitMember()
{
	g_nAnimationCnt++;
	if (g_nAnimationCnt == WAIT_CNT) g_nAnimationCnt = 0;
	if (g_nAnimationCnt <= ANIMATION_CNT)
	{
		if (g_nAnimationCnt > 0 && g_nAnimationCnt <= 10) g_fAlfa += 0.1f;
		else if (g_nAnimationCnt >= 11 && g_nAnimationCnt <= 20) g_fAlfa += -0.1f;
	}

	if ((GetKeyboardTrigger(DIK_R) || GetJoyStickTrigger(JOY_RECT)) && GetSelectSongFinalCheckStatus() && !g_bReady && GetTestModeMemberStatus(TMML_NETWORK) && !GetSettingStatus() && !GetErrorListStatus() && !GetBtnManualStatus())
	{
		// 自分が相手のルームにいる場合
		if (g_bEntry)
		{
			if (g_bCreateSE)
			{
				g_bCreateSE = false;
			}
			g_bEntry = false;
			SetBtnManualTarget(BMP_FINAL_CHECK_ONLINE);
			SetBtnManualPos(RDP_INIT);
		}
		else
		{
			if (g_bCreate)
			{
				if (g_bCreateSE)
				{
					g_bCreateSE = false;
				}
				g_bCreate = false;
				SetBtnManualTarget(BMP_FINAL_CHECK_ONLINE);
				SetBtnManualPos(RDP_INIT);
			}
			else
			{
				if (!g_bCreateSE)
				{
					g_bCreateSE = true;
					if (GetPlayStatus(SE_CREATE))
					{
						StopSound(SE_CREATE);
					}
					PlaySound(SE_CREATE);
				}
				g_bCreate = true;
				SetBtnManualTarget(BMP_DESTROY_ROOM_GET_READY);
				SetBtnManualPos(RDP_INIT);
			}
		}
	}
	else if ((GetKeyboardTrigger(DIK_R) || GetJoyStickTrigger(JOY_RECT)) && !GetSelectSongFinalCheckStatus() && Data::g_player.room_create && !g_bReady && GetTestModeMemberStatus(TMML_NETWORK) && !GetSettingStatus() && Data::g_player.game_mode == MODE_SELECT_SONG && !GetErrorListStatus() && !GetBtnManualStatus())
	{
		if (g_bEntry) 
		{
			g_bEntry = false;
			SetBtnManualTarget(BMP_FINAL_CHECK_ONLINE);
			SetBtnManualPos(RDP_INIT);
		}
		else
		{
			int nSelectNum = GetSelectNum();
			if (GetSelectNum() != Data::g_player.music)
			{
				ChangeSelectNum(Data::g_player.music);
			}
			if (Data::g_player.room_create)
			{
				g_bEntry = true;
				SetBtnManualTarget(BMP_DESTROY_ROOM_GET_READY);
				SetBtnManualPos(RDP_INIT);
			}
			else
			{
				ChangeSelectNum(nSelectNum);
				//MessageBox(NULL, "相手はルームを解散したため、参加できませんでした。", "エラー", MB_OK);
				SetErrorList(ELW_ROOM_NOT_EXIST, RDP_END);
				SetBtnManualTarget(BMP_SELECT_SONG);
				SetBtnManualPos(RDP_INIT);
			}
		}
	}

	if ((GetKeyboardTrigger(DIK_RETURN) || GetJoyStickTrigger(JOY_SPHERE)) && GetSelectSongFinalCheckStatus() && !GetSettingStatus() && !GetSongPanelChangeAStatus() && !GetErrorListStatus() && !GetBtnManualStatus() && !GetFadeFlag())
	{
		if (!g_bReady)
		{
			g_bReady = true;
			SetBtnManualTarget(BMP_READY);
			SetBtnManualPos(RDP_INIT);
			if (GetPlayStatus(SE_ENTER))
			{
				StopSound(SE_ENTER);
			}
			PlaySound(SE_ENTER);
		}
	}
	if ((GetKeyboardTrigger(DIK_ESCAPE) || GetJoyStickTrigger(JOY_CROSS)) && GetSelectSongFinalCheckStatus() && !GetSettingStatus() && !GetSongPanelChangeAStatus() && !GetErrorListStatus() && !GetBtnManualStatus() && !GetFadeFlag())
	{
		if (g_bReady && g_nReadyCancelCount <= gc_nReadyCancelCountMaxAble)
		{
			g_bReady = false;
			SetBtnManualTarget(BMP_DESTROY_ROOM_GET_READY);
			SetBtnManualPos(RDP_INIT);
		}
	}
	if (g_bReady && Data::g_player.ready && Data::g_player.game_mode == MODE_SELECT_SONG)
	{
		if (g_nReadyCancelCount < gc_nReadyCancelCountMax)
		{
			g_nReadyCancelCount++;
		}
		else
		{
			g_bReadyFinalCheck = true;
		}
	}
	if (!g_bReady || !Data::g_player.ready)
	{
		g_nReadyCancelCount = 0;
		g_bReadyFinalCheck = false;
	}
	//if (GetSelectSongFinalCheckStatus() && !g_bReady && Data::g_player.game_mode == MODE_GAME)
	//{
	//	if (GetRoomEntry())
	//	{
	//		SetRoomCreate(true);
	//		SetRoomEntry(false);
	//	}
	//}

	if (Data::g_player.room_create && g_bEntry == false && !GetSelectSongFinalCheckStatus() && Data::g_player.game_mode == MODE_SELECT_SONG)
	{//相手がライバルを募集中の時
		if (!g_bMessageSE)
		{
			g_bMessageSE = true;
			if (GetPlayStatus(SE_MESSAGE))
			{
				StopSound(SE_MESSAGE);
			}
			PlaySound(SE_MESSAGE);
		}
		if (g_wait_member.x > WAIT_MEMBER_X)
		{
			g_wait_member.x -= WAIT_MEMBER_SPEED;
			if (g_wait_member.x < WAIT_MEMBER_X) g_wait_member.x = WAIT_MEMBER_X;
		}
	}
	else
	{
		if (g_bMessageSE)
		{
			g_bMessageSE = false;
		}
		if (g_wait_member.x < WAIT_MEMBER_INIT_X)
		{
			g_wait_member.x += WAIT_MEMBER_SPEED;
			if (g_wait_member.x > WAIT_MEMBER_INIT_X) g_wait_member.x = WAIT_MEMBER_INIT_X;
		}
	}
}

void DrawSelectSongWaitMember()
{
	SetLight(FALSE);
	SetZBuffer(FALSE);
	SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetTexture(TEXTURE_NUM::TEX_SELECT_SONG_WAIT_MEMBER_BG);
	DrawPolygon(g_wait_member.x, g_wait_member.y, 0, 0, g_tex_bg.u, g_tex_bg.v, false);
	SetTexture(TEXTURE_NUM::TEX_SELECT_SONG_WAIT_MEMBER_BTN);
	DrawPolygon(g_wait_member.x + 20.0f, g_wait_member.y + 15.0f, 0, g_tex_btn.v * g_nMessageBtnV, g_tex_btn.u, g_tex_btn.v, false);

	SetBlend(D3DBLEND_BLENDFACTOR);

	SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlfa));
	SetTexture(TEXTURE_NUM::TEX_SELECT_SONG_WAIT_MEMBER_BTN);
	DrawPolygon(g_wait_member.x + 20.0f, g_wait_member.y + 15.0f, 0, g_tex_btn.v * g_nMessageBtnV, g_tex_btn.u, g_tex_btn.v, false);

	SetBlend(D3DBLEND_INVSRCALPHA);
}

bool GetRoomCreate()
{
	return g_bCreate;
}

bool GetRoomEntry()
{
	return g_bEntry;
}

bool GetRoomReady()
{
	return g_bReady;
}

bool GetRoomEntryReady()
{
	return g_bRoomEntryReady;
}

void SetRoomCreate(bool create)
{
	g_bCreate = create;
}

void SetRoomEntry(bool entry)
{
	g_bEntry = entry;
}

bool GetReadyFinalCheck()
{
	return g_bReadyFinalCheck;
}