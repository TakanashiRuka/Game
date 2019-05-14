//--------------------------------------------------------------------------------------------------
//
// テクスチャ相関
//																				    2017.11.27 RYU
//--------------------------------------------------------------------------------------------------

#include "main.h"
#include "Texture.h"
#include <d3dx9.h>

static LPDIRECT3DTEXTURE9 g_pTexture[TEX_MAX] = { NULL };

static int g_nTexNum = 0;

static TEXTURE TextureMessage[TEX_MAX] =
{
	{ "Data/Texture/GameLane.png", 456, 1, 456, 1 },					// レーン
	{ "Data/Texture/LaneEffect.png", 25, 400, 25, 400 },				// レーンのエフェクト
	{ "Data/Texture/GameBtnLight.png", 100, 1000, 100, 1000 },			// ボタンライト
	{ "Data/Texture/GameJudgeLine.png", 406, 10, 406, 10 },				// 判定バー
	{ "Data/Texture/GameJudgeLineSp.png", 204, 10, 204, 10 },			// 判定バー(特殊)
	{ "Data/Texture/GameNoteTapNormal.png", 100, 20, 100, 20 },			// タップ
	{ "Data/Texture/GameNoteTapHighScore.png", 100, 20, 100, 20 },		// ブレイク
	{ "Data/Texture/GameNoteTapNormal.png", 100, 20, 100, 20 },			// ホールドの始まり
	{ "Data/Texture/GameHoldMiddle.png", 100, 1, 100, 1 },				// ホールドの中間
	{ "Data/Texture/GameNoteTapNormal.png", 100, 20, 100, 20 },			// ホールドの終わり
	{ "Data/Texture/GameFlick.png", 200, 20, 200, 20 },					// フリック
	{ "Data/Texture/GameFlickArrowUp.png", 140, 160, 140, 160 },		// フリックのアロー(上)
	{ "Data/Texture/GameFlickArrowDown.png", 140, 160, 140, 160 },		// フリックのアロー(下)
	{ "Data/Texture/GameFlickArrowLeft.png", 180, 80, 180, 80 },		// フリックのアロー(左)
	{ "Data/Texture/GameFlickArrowRight.png", 180, 80, 180, 80 },		// フリックのアロー(右)
	{ "Data/Texture/GameSlideStart.png", 100, 20, 100, 20 },			// スライドの始まり
	{ "Data/Texture/GameSlideMiddle.png", 100, 1, 100, 1 },				// スライドの中間
	{ "Data/Texture/GameSlideStart.png", 100, 20, 100, 20 },			// スライドの終わり
	{ "Data/Texture/GameSlideShadow.png", 1, 1, 1, 1 },					// スライドの影
	{ "Data/Texture/GameSlideStartMiddle.png", 100, 20, 100, 20 },		// スライドの始まり/終わり(開始終了フラグなし)
	{ "Data/Texture/GameLine.png", 1, 1, 1, 1 },						// 小節ライン
	{ "Data/Texture/JudgeWord.png", 75, 60, 75, 15 },					// 判定文字
	{ "Data/Texture/SelectSongBG.png", 1280, 720, 1280, 720 },			// 曲選択画面の背景
	{ "Data/Texture/SelectSongFrame.png", 1280, 183, 1280, 183 },		// 曲選択画面のフレーム
	{ "Data/Texture/SelectSongBlue.png", 1280, 720, 1280, 720 },		// 曲選択画面の六角形(青)
	{ "Data/Texture/SelectSongRed.png", 1280, 720, 1280, 720 },			// 曲選択画面の六角形(赤)
	{ "Data/Texture/SelectSongLine.png", 1280, 720, 1280, 720 },		// 曲選択画面の背景(縦線)
	{ "Data/Texture/SelectSongBoard.png", 1280, 516, 1280, 516 },		// 曲の背景
	{ "Data/Texture/SelectSongWord.png", 195, 24, 195, 24 },			// 曲選択画面(説明用)
	{ "Data/Texture/SelectSongPanel.png", 813, 271, 271, 271 },			// 曲パネル
	{ "Data/Texture/LevelWord.png", 146, 72, 146, 24 },					// 難易度(文字)
	{ "Data/Texture/LevelNum.png", 170, 23, 17, 23 },					// 難易度(数字)
	{ "Data/Texture/GameSlideShadow.png", 100, 100, 100, 100 },			// 曲のアルバム画像(ゲーム用)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// 曲のアルバム画像(01)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// 曲のアルバム画像(02)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// 曲のアルバム画像(03)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// 曲のアルバム画像(04)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// 曲のアルバム画像(05)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// 曲のアルバム画像(06)
	{ "Data/Texture/GameSlideShadow.png", 250, 250, 250, 250 },			// 曲のアルバム画像(07)
	{ "Data/Texture/LevelIcon.png", 48, 69, 24, 23 },					// レベルアイコン
	{ "Data/Texture/AlbumFrame.png", 291, 291, 291, 291 },				// アルバム画像のフチ
	{ "Data/Texture/JudgeTapEffectUnder.png", 100, 300, 100, 100 },		// タップ判定エフェクトの下部分
	{ "Data/Texture/JudgeTapEffectUp.png", 242, 360, 242, 120 },		// タップ判定エフェクトの上部分
	{ "Data/Texture/JudgeFlickEffectUp.png", 200, 160, 200, 80 },		// タップ判定エフェクトの上部分
	{ "Data/Texture/JudgeSlideEffectUnder.png", 240, 120, 240, 120 },	// スライド判定エフェクトの下部分
	{ "Data/Texture/ScoreBoard.png", 414, 117, 414, 117 },				// スコアボード
	{ "Data/Texture/ScoreWord.png", 49, 14, 49, 14 },					// スコア文字
	{ "Data/Texture/ScoreNum.png", 250, 36, 25, 36 },					// スコア数字
	{ "Data/Texture/GaugeBoard.png", 363, 42, 363, 42 },				// ゲージボード
	{ "Data/Texture/GaugeBG.png", 326, 26, 326, 26 },					// ゲージ背景
	{ "Data/Texture/GaugeClearLine.png", 44, 24, 44, 24 },				// ゲージ合格ライン
	{ "Data/Texture/GaugeInside.png", 322, 44, 322, 22 },				// ゲージの内容
	{ "Data/Texture/ComboWord.png", 202, 42, 202, 42 },					// コンボ文字
	{ "Data/Texture/ComboNum.png", 1000, 100, 100, 100 },				// コンボ数字
	{ "Data/Texture/MaxComboWord.png", 88, 14, 88, 14 },				// 最大コンボ文字
	{ "Data/Texture/MaxComboNum.png", 100, 14, 10, 14 },				// 最大コンボ数字
	{ "Data/Texture/SongBoard.png", 414, 165, 414, 165 },				// 曲ステータス用背景
	{ "Data/Texture/SongStatusWord.png", 101, 13, 101, 13 },			// 曲ステータス文字(説明用)
	{ "Data/Texture/GameAlbumBoard.png", 104, 104, 104, 104 },			// アルバム画像の背景
	{ "Data/Texture/LevelWordGame.png", 100, 48, 100, 16 },				// ゲーム用難易度表示
	{ "Data/Texture/LevelNumGame.png", 110, 15, 11, 15 },				// ゲーム用難易度表示
	{ "Data/Texture/ResultBoard.png", 694, 414, 694, 414 },				// リザルト(詳細結果)の背景
	{ "Data/Texture/ResultWord.png", 90, 20, 90, 20 },					// リザルト文字
	{ "Data/Texture/ResultScoreWord.png", 90, 22, 90, 22 },				// リザルトのスコア文字
	{ "Data/Texture/ResultRankWord.png", 71, 21, 71, 21 },				// リザルトのランク文字
	{ "Data/Texture/ResultDetailBoard.png", 290, 31, 290, 31 },			// リザルト詳細結果の背景
	{ "Data/Texture/ResultScoreNum.png", 500, 70, 50, 70 },				// リザルトのスコア
	{ "Data/Texture/ResultDetailJudgeLevel.png", 133, 90, 133, 18 },	// リザルトの判定レベル
	{ "Data/Texture/ResultJudgeDetailNum.png", 140, 18, 14, 18 },		// リザルトの詳細結果の数字
	{ "Data/Texture/ResultRankBoard.png", 183, 208, 183, 208 },			// リザルトのランクの背景
	{ "Data/Texture/ResultRank.png", 250, 1210, 250, 110 },				// リザルトのランク
	{ "Data/Texture/GameClearFailedBoard.png", 325, 740, 325, 370 },	// ゲーム評価(クリア/失敗)の背景
	{ "Data/Texture/GameClearFailedWord.png", 506, 266, 506, 133 },		// ゲーム評価(クリア/失敗)の文字
	{ "Data/Texture/GameFCAPBoard.png", 164, 372, 164, 186 },			// ゲーム評価(FC_AP)の背景
	{ "Data/Texture/GameFCAPWord.png", 978, 278, 978, 139 },			// ゲーム評価(FC_AP)の文字
	{ "Data/Texture/SettingBoard.png", 421, 352, 421, 352 },			// 設定の背景
	{ "Data/Texture/SettingTitleWord.png", 106, 25, 106, 25 },			// 設定画面のタイトル
	{ "Data/Texture/SettingWord.png", 73, 51, 71, 17 },					// 設定画面の項目
	{ "Data/Texture/SettingArrow.png", 34, 32, 17, 32 },				// 設定画面の矢印
	{ "Data/Texture/SettingManualBoard.png", 676, 129, 676, 129 },		// 設定項目の説明(背景)
	{ "Data/Texture/SettingManualList.png", 360, 207, 360, 69 },		// 設定項目の説明(内容)
	{ "Data/Texture/ResultAchievementIcon.png", 90, 52, 45, 52 },		// 結果画面の達成項目
	{ "Data/Texture/TitleBG.png", 1280, 720, 1280, 720 },				// タイトルの背景
	{ "Data/Texture/TitleBGSquareSmall.png", 1280, 720, 1280, 720 },	// タイトルの四角形(小)
	{ "Data/Texture/TitleBGSquareBig.png", 1280, 720, 1280, 720 },		// タイトルの四角形(大)
	{ "Data/Texture/TitleBGFloor.png", 1280, 252, 1280, 252 },			// タイトルの床
	{ "Data/Texture/TitleBGHexagonal.png", 1280, 720, 1280, 720 },		// タイトルの六角形
	{ "Data/Texture/TitleBGLogo.png",829, 191, 829, 191 },				// タイトルのロゴ
	{ "Data/Texture/PrefaceController.png",520, 149, 520, 149 },		// 前書き(コントローラ)
	{ "Data/Texture/PrefaceTeamLogo.png",1280, 720, 1280, 720 },		// 前書き(チームロゴ)
	{ "Data/Texture/TitleBtnManual.png",325, 90, 325, 45 },				// タイトルの操作説明
	{ "Data/Texture/TitleAllRightsReserved.png",408, 36, 408, 36 },		// タイトルのAllRightReserved
	{ "Data/Texture/GameBG.png",1280, 720, 1280, 720 },					// ゲームの背景(メイン)
	{ "Data/Texture/GameBGMask.png",1280, 720, 1280, 720 },				// ゲームの背景(マスク)
	{ "Data/Texture/GameBGWall.png",153, 596, 153, 298 },				// ゲームの背景(壁の六角形)
	{ "Data/Texture/GameBGLine.png",200, 1000, 200, 1000 },				// ゲームの背景(線分)
	{ "Data/Texture/GameBgFloor.png",1280, 480, 1280, 480 },			// ゲームの背景(床)
	{ "Data/Texture/FadeInOutCenter.png",387, 446, 387, 446 },			// フェードインアウト(中心)
	{ "Data/Texture/FadeInOutLeft.png",449, 722, 449, 722 },			// フェードインアウト(左)
	{ "Data/Texture/FadeInOutRight.png",449, 722, 449, 722 },			// フェードインアウト(右)
	{ "Data/Texture/FadeInOutLeftUp.png",628, 252, 628, 252 },			// フェードインアウト(左上)
	{ "Data/Texture/FadeInOutRightUp.png",628, 252, 628, 252 },			// フェードインアウト(右上)
	{ "Data/Texture/FadeInOutLeftDown.png",628, 252, 628, 252 },		// フェードインアウト(左下)
	{ "Data/Texture/FadeInOutRightDown.png",628, 252, 628, 252 },		// フェードインアウト(右下)
	{ "Data/Texture/ResultFailedClear.png",111, 62, 111, 31 },			// リザルト画面のクリア/失敗アイコン
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
	{ "Data/Texture/BtnManual.png", 2560, 294, 1280, 42 },					// ボタンの操作説明
	{ "Data/Texture/TitleBGMask.png", 1280, 720, 1280, 720 },				// タイトルマスク
	{ "Data/Texture/GamePlayerBoard.png", 188, 157, 188, 157 },				//
	{ "Data/Texture/GamePlayerMatchingRank.png", 105, 120, 105, 60 },		//
	{ "Data/Texture/GameRivalBoard.png", 250, 82, 250, 82 },				//
	{ "Data/Texture/JudgeEffectSlideUp.png", 200, 230, 200, 230 },			// スライドの判定エフェクト(上)
	{ "Data/Texture/ResultBtn.png", 150, 94, 150, 47 },						// リザルト(ボタン)
	{ "Data/Texture/ErrorListBG.png", 540, 217, 540, 217 },					// エラーリストの背景
	{ "Data/Texture/GamePrefaceBGUnder.png", 1280, 276, 1280, 276 },		// ゲーム前書き(背景・下)
	{ "Data/Texture/GamePrefaceBGUp.png", 1280, 199, 1280, 199 },			// ゲーム前書き(背景・上)
	{ "Data/Texture/GamePrefaceReady.png", 313, 84, 313, 84 },				// ゲーム前書き(READY)
	{ "Data/Texture/GamePrefaceStart.png", 312, 85, 312, 85 },				// ゲーム前書き(START)
	{ "Data/Texture/RankingBG.png", 238, 118, 238, 118 },					//
	{ "Data/Texture/RankingHeader.png", 241, 99, 241, 33 },					//
	{ "Data/Texture/RankingRank.png", 44, 84, 44, 28 },						//
	{ "Data/Texture/RankingScoreNum.png", 160, 23, 16, 23 },				//
	{ "Data/Texture/TutorialBtn.png", 157, 106, 157, 53 },					//
	{ "Data/Texture/ResultRankingFilter.png", 238, 112, 238, 112 },			//
	{ "Data/Texture/JudgeEarlyLate.png", 46, 26, 46, 13 },					// パーフェクト判定の誤差
	{ "Data/Texture/ThankYouForPlayingBG.png", 1280, 135, 1280, 135 },		// 後書きの背景
	{ "Data/Texture/ThankYouForPlayingWord.png", 759, 101, 759, 101 },		// 後書きの文字
	{ "Data/Texture/ThanksMask.png", 1, 1, 1, 1 },							// 後書きの文字
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

	// テクスチャの読み込み

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
			MessageBox(NULL, "テクスチャが読み込めなかった", "エラー", MB_OK);
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

	// テクスチャの読み込み

	HRESULT hr;

	hr = D3DXCreateTextureFromFile
	(
		pDevice,
		TextureMessage[nNum].filename,
		&g_pTexture[nNum]
	);

	if (FAILED(hr))
	{
		MessageBox(NULL, "テクスチャが読み込めなかった", "エラー", MB_OK);
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