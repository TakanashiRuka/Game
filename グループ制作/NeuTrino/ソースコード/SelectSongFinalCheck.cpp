//----------------------------------------------------------------------------
//
//	�ȑI���̍ŏI�m�F
//
//														 ����ҁF����
//                                                       �쐬���F2018/01/24
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "SelectSongFinalCheck.h"
#include "input.h"
#include "JoyStick.h"
#include "SelectSongPanel.h"
#include "SelectSong.h"
#include "game.h"
#include "Setting.h"
#include "title.h"
#include "sound.h"
#include"SelectSongWaitMember.h"		//�ǉ�
#include"data.h"						//�ǉ�
#include "BtnManual.h"
#include "icon.h"				//�ǉ�

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�O���[�o���ϐ�-----------------------------------
static bool g_bFinalCheck;
static bool g_bFadeIn;

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitSelectSongFinalCheck()
{
	g_bFinalCheck = false;
	g_bFadeIn = false;
}

void UninitSelectSongFinalCheck()
{

}

void UpdateSelectSongFinalCheck()
{
	if ((GetKeyboardTrigger(DIK_T) || GetJoyStickTrigger(JOY_OPTION)) &&
		GetSelectSongFadeInFlag() && !GetSettingStatus() && !GetSongPanelChangeAStatus()
		&& !GetFadeFlag() && !GetErrorListStatus() && !GetBtnManualStatus())
	{
		if (!g_bFinalCheck)
		{
			SetGameModeTarget(MODE_TUTORIAL);
			SetFadeWaitFrame(120);
			SetFadeInOut(FIOF_FADEIN);
		}
	}
	if ((GetKeyboardTrigger(DIK_RETURN) || GetJoyStickTrigger(JOY_SPHERE))&&
		GetSelectSongFadeInFlag() && !GetSettingStatus() && !GetSongPanelChangeAStatus()
		&& !GetFadeFlag() && !GetErrorListStatus())
	{
		if (!g_bFinalCheck)
		{
			if (GetPlayStatus(SE_FINAL_CHECK))
			{
				StopSound(SE_FINAL_CHECK);
			}
			PlaySound(SE_FINAL_CHECK);
			g_bFinalCheck = true;
			SetSongPanelA(RDP_END);
			SetIcon();							//�ǉ�
			if (!GetTestModeMemberStatus(TMML_NETWORK))
			{
				SetBtnManualTarget(BMP_FINAL_CHECK_OFFLINE);
			}
			else
				if (GetTestModeMemberStatus(TMML_NETWORK))
				{
					SetBtnManualTarget(BMP_FINAL_CHECK_ONLINE);
				}
			SetBtnManualPos(RDP_INIT);
		}
		else if (!GetRoomCreate() && !GetRoomEntry())		//������ǉ�
		{
			if (GetPlayStatus(SE_ENTER))
			{
				StopSound(SE_ENTER);
			}
			PlaySound(SE_ENTER);
			SetGameModeTarget(MODE_GAME);
			SetFadeWaitFrame(120);
			SetFadeInOut(FIOF_FADEIN);
			//ChangeGameMode();
		}
	}
	//�ǉ�
	if ((GetRoomCreate() && Data::g_player.room_entry) || (GetRoomEntry() && Data::g_player.room_create))
	{
		if (!g_bFinalCheck)
		{
			if (GetPlayStatus(SE_FINAL_CHECK))
			{
				StopSound(SE_FINAL_CHECK);
			}
			PlaySound(SE_FINAL_CHECK);
			g_bFinalCheck = true;
			SetSongPanelA(RDP_END);
		}
		else if (GetRoomReady() && Data::g_player.ready && GetReadyFinalCheck() && Data::g_player.game_mode == MODE_SELECT_SONG)
		{
			UpdateSendData();		//�ǉ�
			if (!g_bFadeIn)
			{
				SetGameModeTarget(MODE_GAME);
				SetFadeWaitFrame(120);
				SetFadeInOut(FIOF_FADEIN);
				g_bFadeIn = true;
			}
		}
	}
	if (GetRoomEntry() && Data::g_player.room_create == false)
	{
		SetRoomCreate(true);
		SetRoomEntry(false);
		g_bFinalCheck = true;
	}
	if ((GetKeyboardTrigger(DIK_ESCAPE) || GetJoyStickTrigger(JOY_CROSS)) &&
		GetSelectSongFadeInFlag() && !GetSettingStatus() && !GetSongPanelChangeAStatus()
		&& !GetFadeFlag() && !GetRoomCreate() && !GetRoomEntry() && !GetErrorListStatus() && !GetBtnManualStatus())
	{
		if (!g_bFinalCheck)
		{
			SetGameModeTarget(MODE_TITLE);
			ChangeGameMode();
		}
		else
		{
			g_bFinalCheck = false;
			SetSongPanelA(RDP_INIT);
			SetBtnManualTarget(BMP_SELECT_SONG);
			SetBtnManualPos(RDP_INIT);
		}
	}
}

void DrawSelectSongFinalCheck()
{

}

bool GetSelectSongFinalCheckStatus()
{
	return g_bFinalCheck;
}