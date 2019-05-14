//--------------------------------------------------------------------------------------------------
//
// �e�N�X�`������
//																				    2017.11.27 RYU
//--------------------------------------------------------------------------------------------------

#include "main.h"
#include "Texture.h"
#include <d3dx9.h>

static LPDIRECT3DTEXTURE9 g_pTexture[TEX_MAX] = { NULL };

static int g_nTexNum = 0;

static TEXTURE TextureMessage[TEX_MAX] =
{
	{ "Data/Texture/GameLane.png", 456, 1, 456, 1 },					// ���[��
	{ "Data/Texture/LaneEffect.png", 25, 400, 25, 400 },				// ���[���̃G�t�F�N�g
	{ "Data/Texture/GameBtnLight.png", 100, 1000, 100, 1000 },			// �{�^�����C�g
	{ "Data/Texture/GameJudgeLine.png", 406, 10, 406, 10 },				// ����o�[
	{ "Data/Texture/GameJudgeLineSp.png", 204, 10, 204, 10 },			// ����o�[(����)
	{ "Data/Texture/GameNoteTapNormal.png", 100, 20, 100, 20 },			// �^�b�v
	{ "Data/Texture/GameNoteTapHighScore.png", 100, 20, 100, 20 },		// �u���C�N
	{ "Data/Texture/GameNoteTapNormal.png", 100, 20, 100, 20 },			// �z�[���h�̎n�܂�
	{ "Data/Texture/GameHoldMiddle.png", 100, 1, 100, 1 },				// �z�[���h�̒���
	{ "Data/Texture/GameNoteTapNormal.png", 100, 20, 100, 20 },			// �z�[���h�̏I���
	{ "Data/Texture/GameFlick.png", 200, 20, 200, 20 },					// �t���b�N
	{ "Data/Texture/GameFlickArrowUp.png", 140, 160, 140, 160 },		// �t���b�N�̃A���[(��)
	{ "Data/Texture/GameFlickArrowDown.png", 140, 160, 140, 160 },		// �t���b�N�̃A���[(��)
	{ "Data/Texture/GameFlickArrowLeft.png", 180, 80, 180, 80 },		// �t���b�N�̃A���[(��)
	{ "Data/Texture/GameFlickArrowRight.png", 180, 80, 180, 80 },		// �t���b�N�̃A���[(�E)
	{ "Data/Texture/GameSlideStart.png", 100, 20, 100, 20 },			// �X���C�h�̎n�܂�
	{ "Data/Texture/GameSlideMiddle.png", 100, 1, 100, 1 },				// �X���C�h�̒���
	{ "Data/Texture/GameSlideStart.png", 100, 20, 100, 20 },			// �X���C�h�̏I���
	{ "Data/Texture/GameSlideShadow.png", 1, 1, 1, 1 },					// �X���C�h�̉e
	{ "Data/Texture/GameSlideStartMiddle.png", 100, 20, 100, 20 },		// �X���C�h�̎n�܂�/�I���(�J�n�I���t���O�Ȃ�)
	{ "Data/Texture/GameLine.png", 1, 1, 1, 1 },						// ���߃��C��
	{ "Data/Texture/JudgeWord.png", 75, 60, 75, 15 },					// ���蕶��
	{ "Data/Texture/SelectSongBG.png", 1280, 720, 1280, 720 },			// �ȑI����ʂ̔w�i
	{ "Data/Texture/SelectSongFrame.png", 1280, 183, 1280, 183 },		// �ȑI����ʂ̃t���[��
	{ "Data/Texture/SelectSongBlue.png", 1280, 720, 1280, 720 },		// �ȑI����ʂ̘Z�p�`(��)
	{ "Data/Texture/SelectSongRed.png", 1280, 720, 1280, 720 },			// �ȑI����ʂ̘Z�p�`(��)
	{ "Data/Texture/SelectSongLine.png", 1280, 720, 1280, 720 },		// �ȑI����ʂ̔w�i(�c��)
	{ "Data/Texture/SelectSongBoard.png", 1280, 516, 1280, 516 },		// �Ȃ̔w�i
	{ "Data/Texture/SelectSongWord.png", 195, 24, 195, 24 },			// �ȑI�����(�����p)
	{ "Data/Texture/SelectSongPanel.png", 813, 271, 271, 271 },			// �ȃp�l��
	{ "Data/Texture/LevelWord.png", 146, 72, 146, 24 },					// ��Փx(����)
	{ "Data/Texture/LevelNum.png", 170, 23, 17, 23 },					// ��Փx(����)
	{ "Data/Texture/GameSlideShadow.png", 100, 100, 100, 100 },			// �Ȃ̃A���o���摜(�Q�[���p)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// �Ȃ̃A���o���摜(01)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// �Ȃ̃A���o���摜(02)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// �Ȃ̃A���o���摜(03)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// �Ȃ̃A���o���摜(04)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// �Ȃ̃A���o���摜(05)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// �Ȃ̃A���o���摜(06)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// �Ȃ̃A���o���摜(07)
	{ "Data/Texture/LevelIcon.png", 48, 69, 24, 23 },					// ���x���A�C�R��
	{ "Data/Texture/AlbumFrame.png", 291, 291, 291, 291 },				// �A���o���摜�̃t�`
	{ "Data/Texture/JudgeTapEffectUnder.png", 100, 300, 100, 100 },		// �^�b�v����G�t�F�N�g�̉�����
	{ "Data/Texture/JudgeTapEffectUp.png", 242, 360, 242, 120 },		// �^�b�v����G�t�F�N�g�̏㕔��
	{ "Data/Texture/JudgeFlickEffectUp.png", 200, 160, 200, 80 },		// �^�b�v����G�t�F�N�g�̏㕔��
	{ "Data/Texture/JudgeSlideEffectUnder.png", 240, 120, 240, 120 },	// �X���C�h����G�t�F�N�g�̉�����
	{ "Data/Texture/ScoreBoard.png", 414, 117, 414, 117 },				// �X�R�A�{�[�h
	{ "Data/Texture/ScoreWord.png", 49, 14, 49, 14 },					// �X�R�A����
	{ "Data/Texture/ScoreNum.png", 250, 36, 25, 36 },					// �X�R�A����
	{ "Data/Texture/GaugeBoard.png", 363, 42, 363, 42 },				// �Q�[�W�{�[�h
	{ "Data/Texture/GaugeBG.png", 326, 26, 326, 26 },					// �Q�[�W�w�i
	{ "Data/Texture/GaugeClearLine.png", 44, 24, 44, 24 },				// �Q�[�W���i���C��
	{ "Data/Texture/GaugeInside.png", 322, 44, 322, 22 },				// �Q�[�W�̓��e
	{ "Data/Texture/ComboWord.png", 202, 42, 202, 42 },					// �R���{����
	{ "Data/Texture/ComboNum.png", 1000, 100, 100, 100 },				// �R���{����
	{ "Data/Texture/MaxComboWord.png", 88, 14, 88, 14 },				// �ő�R���{����
	{ "Data/Texture/MaxComboNum.png", 100, 14, 10, 14 },				// �ő�R���{����
	{ "Data/Texture/SongBoard.png", 414, 165, 414, 165 },				// �ȃX�e�[�^�X�p�w�i
	{ "Data/Texture/SongStatusWord.png", 101, 13, 101, 13 },			// �ȃX�e�[�^�X����(�����p)
	{ "Data/Texture/GameAlbumBoard.png", 104, 104, 104, 104 },			// �A���o���摜�̔w�i
	{ "Data/Texture/LevelWordGame.png", 100, 48, 100, 16 },				// �Q�[���p��Փx�\��
	{ "Data/Texture/LevelNumGame.png", 110, 15, 11, 15 },				// �Q�[���p��Փx�\��
	{ "Data/Texture/ResultBoard.png", 694, 414, 694, 414 },				// ���U���g(�ڍ׌���)�̔w�i
	{ "Data/Texture/ResultWord.png", 90, 20, 90, 20 },					// ���U���g����
	{ "Data/Texture/ResultScoreWord.png", 90, 22, 90, 22 },				// ���U���g�̃X�R�A����
	{ "Data/Texture/ResultRankWord.png", 71, 21, 71, 21 },				// ���U���g�̃����N����
	{ "Data/Texture/ResultDetailBoard.png", 290, 31, 290, 31 },			// ���U���g�ڍ׌��ʂ̔w�i
	{ "Data/Texture/ResultScoreNum.png", 500, 70, 50, 70 },				// ���U���g�̃X�R�A
	{ "Data/Texture/ResultDetailJudgeLevel.png", 133, 90, 133, 18 },	// ���U���g�̔��背�x��
	{ "Data/Texture/ResultJudgeDetailNum.png", 140, 18, 14, 18 },		// ���U���g�̏ڍ׌��ʂ̐���
	{ "Data/Texture/ResultRankBoard.png", 183, 208, 183, 208 },			// ���U���g�̃����N�̔w�i
	{ "Data/Texture/ResultRank.png", 250, 1210, 250, 110 },				// ���U���g�̃����N
	{ "Data/Texture/GameClearFailedBoard.png", 325, 740, 325, 370 },	// �Q�[���]��(�N���A/���s)�̔w�i
	{ "Data/Texture/GameClearFailedWord.png", 506, 266, 506, 133 },		// �Q�[���]��(�N���A/���s)�̕���
	{ "Data/Texture/GameFCAPBoard.png", 164, 372, 164, 186 },			// �Q�[���]��(FC_AP)�̔w�i
	{ "Data/Texture/GameFCAPWord.png", 978, 278, 978, 139 },			// �Q�[���]��(FC_AP)�̕���
	{ "Data/Texture/SettingBoard.png", 421, 352, 421, 352 },			// �ݒ�̔w�i
	{ "Data/Texture/SettingTitleWord.png", 106, 25, 106, 25 },			// �ݒ��ʂ̃^�C�g��
	{ "Data/Texture/SettingWord.png", 73, 51, 71, 17 },					// �ݒ��ʂ̍���
	{ "Data/Texture/SettingArrow.png", 34, 32, 17, 32 },				// �ݒ��ʂ̖��
	{ "Data/Texture/SettingManualBoard.png", 676, 129, 676, 129 },		// �ݒ荀�ڂ̐���(�w�i)
	{ "Data/Texture/SettingManualList.png", 360, 207, 360, 69 },		// �ݒ荀�ڂ̐���(���e)
	{ "Data/Texture/ResultAchievementIcon.png", 90, 52, 45, 52 },		// ���ʉ�ʂ̒B������
	{ "Data/Texture/TitleBG.png", 1280, 720, 1280, 720 },				// �^�C�g���̔w�i
	{ "Data/Texture/TitleBGSquareSmall.png", 1280, 720, 1280, 720 },	// �^�C�g���̎l�p�`(��)
	{ "Data/Texture/TitleBGSquareBig.png", 1280, 720, 1280, 720 },		// �^�C�g���̎l�p�`(��)
	{ "Data/Texture/TitleBGFloor.png", 1280, 252, 1280, 252 },			// �^�C�g���̏�
	{ "Data/Texture/TitleBGHexagonal.png", 1280, 720, 1280, 720 },		// �^�C�g���̘Z�p�`
	{ "Data/Texture/TitleBGLogo.png",829, 191, 829, 191 },				// �^�C�g���̃��S
	{ "Data/Texture/PrefaceController.png",520, 149, 520, 149 },		// �O����(�R���g���[��)
	{ "Data/Texture/PrefaceTeamLogo.png",1280, 720, 1280, 720 },		// �O����(�`�[�����S)
	{ "Data/Texture/TitleBtnManual.png",325, 90, 325, 45 },				// �^�C�g���̑������
	{ "Data/Texture/TitleAllRightsReserved.png",408, 36, 408, 36 },		// �^�C�g����AllRightReserved
	{ "Data/Texture/GameBG.png",1280, 720, 1280, 720 },					// �Q�[���̔w�i(���C��)
	{ "Data/Texture/GameBGMask.png",1280, 720, 1280, 720 },				// �Q�[���̔w�i(�}�X�N)
	{ "Data/Texture/GameBGWall.png",153, 596, 153, 298 },				// �Q�[���̔w�i(�ǂ̘Z�p�`)
	{ "Data/Texture/GameBGLine.png",200, 1000, 200, 1000 },				// �Q�[���̔w�i(����)
	{ "Data/Texture/GameBgFloor.png",1280, 480, 1280, 480 },			// �Q�[���̔w�i(��)
	{ "Data/Texture/FadeInOutCenter.png",387, 446, 387, 446 },			// �t�F�[�h�C���A�E�g(���S)
	{ "Data/Texture/FadeInOutLeft.png",449, 722, 449, 722 },			// �t�F�[�h�C���A�E�g(��)
	{ "Data/Texture/FadeInOutRight.png",449, 722, 449, 722 },			// �t�F�[�h�C���A�E�g(�E)
	{ "Data/Texture/FadeInOutLeftUp.png",628, 252, 628, 252 },			// �t�F�[�h�C���A�E�g(����)
	{ "Data/Texture/FadeInOutRightUp.png",628, 252, 628, 252 },			// �t�F�[�h�C���A�E�g(�E��)
	{ "Data/Texture/FadeInOutLeftDown.png",628, 252, 628, 252 },		// �t�F�[�h�C���A�E�g(����)
	{ "Data/Texture/FadeInOutRightDown.png",628, 252, 628, 252 },		// �t�F�[�h�C���A�E�g(�E��)
	{ "Data/Texture/ResultFailedClear.png",111, 62, 111, 31 },			// ���U���g��ʂ̃N���A/���s�A�C�R��
	{ "Data/Texture/FinalCheckBG.png", 443, 206, 443, 206 },				//
	{ "Data/Texture/FinalCheckReadyBG.png", 309, 79, 309, 79 },				//
	{ "Data/Texture/FinalCheckReadyStatus.png", 143, 40, 143, 20 },			//
	{ "Data/Texture/FinalCheckPlayerIcon.png", 106, 212, 106, 106 },		//
	{ "Data/Texture/FinalCheckHeader.png", 451, 120, 451, 60 },				//
	{ "Data/Texture/FinalCheckLevelWord.png", 56, 66, 56, 22 },				//
	{ "Data/Texture/FinalCheckMemberJoinStatus.png", 443, 412, 443, 206 },	//
	{ "Data/Texture/SelectSongWaitMemberBG.png", 353, 101, 353, 101 },		//
	{ "Data/Texture/SelectSongWaitMemberBtn.png", 38, 76, 38, 38 },			//
	{ "Data/Texture/NetworkStatus.png", 40, 80, 40, 40 },					//
	{ "Data/Texture/BtnManual.png", 2560, 294, 1280, 42 },					// �{�^���̑������
	{ "Data/Texture/TitleBGMask.png", 1280, 720, 1280, 720 },				// �^�C�g���}�X�N
	{ "Data/Texture/GamePlayerBoard.png", 188, 157, 188, 157 },				//
	{ "Data/Texture/GamePlayerMatchingRank.png", 105, 120, 105, 60 },		//
	{ "Data/Texture/GameRivalBoard.png", 250, 82, 250, 82 },				//
	{ "Data/Texture/JudgeEffectSlideUp.png", 200, 230, 200, 230 },			// �X���C�h�̔���G�t�F�N�g(��)
	{ "Data/Texture/ResultBtn.png", 150, 94, 150, 47 },						// ���U���g(�{�^��)
	{ "Data/Texture/ErrorListBG.png", 540, 217, 540, 217 },					// �G���[���X�g�̔w�i
	{ "Data/Texture/GamePrefaceBGUnder.png", 1280, 276, 1280, 276 },		// �Q�[���O����(�w�i�E��)
	{ "Data/Texture/GamePrefaceBGUp.png", 1280, 199, 1280, 199 },			// �Q�[���O����(�w�i�E��)
	{ "Data/Texture/GamePrefaceReady.png", 313, 84, 313, 84 },				// �Q�[���O����(READY)
	{ "Data/Texture/GamePrefaceStart.png", 312, 85, 312, 85 },				// �Q�[���O����(START)
	{ "Data/Texture/RankingBG.png", 238, 118, 238, 118 },					//
	{ "Data/Texture/RankingHeader.png", 241, 99, 241, 33 },					//
	{ "Data/Texture/RankingRank.png", 44, 84, 44, 28 },						//
	{ "Data/Texture/RankingScoreNum.png", 160, 23, 16, 23 },				//
	{ "Data/Texture/TutorialBtn.png", 157, 106, 157, 53 },					//
	{ "Data/Texture/ResultRankingFilter.png", 238, 112, 238, 112 },			//
	{ "Data/Texture/JudgeEarlyLate.png", 46, 26, 46, 13 },					// �p�[�t�F�N�g����̌덷
	{ "Data/Texture/ThankYouForPlayingBG.png", 1280, 135, 1280, 135 },		// �㏑���̔w�i
	{ "Data/Texture/ThankYouForPlayingWord.png", 759, 101, 759, 101 },		// �㏑���̕���
	{ "Data/Texture/ThanksMask.png", 1, 1, 1, 1 },							// �㏑���̕���
	{ "Data/Texture/PlayerEntryHeader.png", 210, 24, 210, 24 },				//
	{ "Data/Texture/PlayerEntryWordBoard.png", 294, 340, 294, 340 },		//
	{ "Data/Texture/PlayerEntryNameBG.png", 232, 43, 232, 43 },				//
	{ "Data/Texture/PlayerEntryArrow.png", 48, 79, 24, 79 },				//
	{ "Data/Texture/ContinueWord.png", 311, 63, 311, 63 },				//
	{ "Data/Texture/ContinueBtn.png", 534, 108, 534, 54 },				//
};

void InitTexture()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// �e�N�X�`���̓ǂݍ���

	for (int count = 0; count < TEX_MAX; count++)
	{
		HRESULT hr;

		hr = D3DXCreateTextureFromFile
		(
			pDevice,
			TextureMessage[count].filename,
			&g_pTexture[count]
		);

		if (FAILED(hr))
		{
			MessageBox(NULL, "�e�N�X�`�����ǂݍ��߂Ȃ�����", "�G���[", MB_OK);
		}
	}
}

void UninitTexture()
{
	for (int count = 0; count < TEX_MAX; count++)
	{
		if (g_pTexture[count] != NULL)
		{
			g_pTexture[count]->Release();
			g_pTexture[count] = NULL;
		}
	}
}

bool InitTextureOnlyOne(int nNum)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	if (pDevice == NULL)
	{
		return false;
	}

	// �e�N�X�`���̓ǂݍ���

	HRESULT hr;

	hr = D3DXCreateTextureFromFile
	(
		pDevice,
		TextureMessage[nNum].filename,
		&g_pTexture[nNum]
	);

	if (FAILED(hr))
	{
		MessageBox(NULL, "�e�N�X�`�����ǂݍ��߂Ȃ�����", "�G���[", MB_OK);
		return false;
	}
	return true;
}

void UninitTextureOnlyOne(int nNum)
{
	if (g_pTexture[nNum] != NULL)
	{
		g_pTexture[nNum]->Release();
		g_pTexture[nNum] = NULL;
	}
}

void SetTextureAddress(int nNumTex, char *pFileAddress)
{
	UninitTextureOnlyOne(nNumTex);

	TextureMessage[nNumTex].filename[0] = '\0';

	strcat(TextureMessage[nNumTex].filename, pFileAddress);

	InitTextureOnlyOne(nNumTex);
}

TEXTURE GetTextureStatus(int nNum)
{
	return TextureMessage[nNum];
}

LPDIRECT3DTEXTURE9 GetTexture(int nNum)
{
	return g_pTexture[nNum];
}

void SetTexture(int nTexNum)
{
	g_nTexNum = nTexNum;

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	pDevice->SetTexture(0, g_pTexture[nTexNum]);
}

int GetTextureNum()
{
	return g_nTexNum;
}