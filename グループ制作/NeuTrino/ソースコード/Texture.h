#ifndef TEXTURE_H
#define TEXTURE_H

typedef enum TEXTURE_NUM
{
	TEX_LANE,							// レーン
	TEX_LANE_EFFECT,					// レーンのエフェクト
	TEX_BTN_LIGHT,						// ボタンライト
	TEX_JUDGE_LINE,						// 判定バー
	TEX_JUDGE_LINE_SP,					// 判定バー
	TEX_TAP,							// タップ
	TEX_BREAK,							// ブレイク
	TEX_HOLD_START,						// ホールドの始まり
	TEX_HOLD_MIDDLE,					// ホールドの中間
	TEX_HOLD_END,						// ホールドの終わり
	TEX_FLICK,							// フリック
	TEX_FLICK_ARROW_UP,					// フリックのアロー(上)
	TEX_FLICK_ARROW_DOWN,				// フリックのアロー(下)
	TEX_FLICK_ARROW_LEFT,				// フリックのアロー(左)
	TEX_FLICK_ARROW_RIGHT,				// フリックのアロー(右)
	TEX_SLIDE_START,					// スライドの始まり
	TEX_SLIDE_MIDDLE,					// スライドの中間
	TEX_SLIDE_END,						// スライドの終わり
	TEX_SLIDE_SHADOW,					// スライドの影
	TEX_SLIDE_S_E_MIDDLE,				// スライドの始まり/終わり(開始終了フラグなし)
	TEX_LINE,							// 小節ライン
	TEX_JUDGE_WORD,						// 判定文字
	TEX_SELECT_SONG_BG,					// 曲選択画面の背景
	TEX_SELECT_SONG_FRAME,				// 曲選択画面のフレーム
	TEX_SELECT_SONG_BLUE,				// 曲選択画面の六角形(青)
	TEX_SELECT_SONG_RED,				// 曲選択画面の六角形(赤)
	TEX_SELECT_SONG_LINE,				// 曲選択画面の背景(縦線)
	TEX_SELECT_SONG_BOARD,				// 曲の背景
	TEX_SELECT_SONG_WORD,				// 曲選択画面(説明用)
	TEX_SELECT_SONG_PANEL,				// 曲パネル
	TEX_SELECT_LEVEL_WORD,				// 難易度(文字)
	TEX_SELECT_LEVEL_NUM,				// 難易度(数字)
	TEX_SELECT_ALBUM_00,				// 曲のアルバム画像(ゲーム用)
	TEX_SELECT_ALBUM_01,				// 曲のアルバム画像(01)
	TEX_SELECT_ALBUM_02,				// 曲のアルバム画像(02)
	TEX_SELECT_ALBUM_03,				// 曲のアルバム画像(03)
	TEX_SELECT_ALBUM_04,				// 曲のアルバム画像(04)
	TEX_SELECT_ALBUM_05,				// 曲のアルバム画像(05)
	TEX_SELECT_ALBUM_06,				// 曲のアルバム画像(06)
	TEX_SELECT_ALBUM_07,				// 曲のアルバム画像(07)
	TEX_SELECT_LEVEL_ICON,				// レベルアイコン
	TEX_SELECT_ALBUM_FRAME,				// アルバム画像のフチ
	TEX_GAME_JUDGE_TAP_EFFECT_UNDER,	// タップ判定エフェクトの下部分
	TEX_GAME_JUDGE_TAP_EFFECT_UP,		// タップ判定エフェクトの上部分
	TEX_GAME_JUDGE_FLICK_EFFECT_UP,		// フリック判定エフェクトの上部分
	TEX_GAME_JUDGE_SLIDE_EFFECT_UNDER,	// スライド判定エフェクトの下部分
	TEX_GAME_SCORE_BOARD,				// スコアボード
	TEX_GAME_SCORE_WORD,				// スコア文字
	TEX_GAME_SCORE_NUM,					// スコア数字
	TEX_GAME_GAUGE_BOARD,				// ゲージボード
	TEX_GAME_GAUGE_BG,					// ゲージの背景
	TEX_GAME_GAUGE_CLEAR_LINE,			// ゲージの合格ライン
	TEX_GAME_GAUGE_INSIDE,				// ゲージの内容
	TEX_GAME_COMBO_WORD,				// コンボ文字
	TEX_GAME_COMBO_NUM,					// コンボ数字
	TEX_GAME_MAX_COMBO_WORD,			// 最大コンボ文字
	TEX_GAME_MAX_COMBO_NUM,				// 最大コンボ数字
	TEX_GAME_SONG_BOARD,				// 曲ステータス用背景
	TEX_GAME_SONG_STATUS_WORD,			// 曲ステータス文字(説明用)
	TEX_GAME_ALBUM_BOARD,				// アルバム画像の背景
	TEX_GAME_LEVEL_WORD,				// ゲーム用難易度表示
	TEX_GAME_LEVEL_NUM,					// ゲーム用難易度表示
	TEX_RESULT_BOARD,					// リザルト(詳細結果)の背景
	TEX_RESULT_WORD,					// リザルト文字
	TEX_RESULT_SCORE_WORD,				// リザルトのスコア文字
	TEX_RESULT_RANK_WORD,				// リザルトのランク文字
	TEX_RESULT_JUDGE_DETAIL_BOARD,		// リザルト詳細結果の背景
	TEX_RESULT_SCORE_NUM,				// リザルトのスコア
	TEX_RESULT_JUDGE_LEVEL,				// リザルトの判定レベル
	TEX_RESULT_DETAIL_NUM,				// リザルトの詳細結果の数字
	TEX_RESULT_RANK_BOARD,				// リザルトのランクの背景
	TEX_RESULT_RANK,					// リザルトのランク
	TEX_GAME_CLEAR_FAILED_BOARD,		// ゲーム評価(クリア/失敗)の背景
	TEX_GAME_CLEAR_FAILED_WORD,			// ゲーム評価(クリア/失敗)の文字
	TEX_GAME_FC_AP_BOARD,				// ゲーム評価(FC/AP)の背景
	TEX_GAME_FC_AP_WORD,				// ゲーム評価(FC/AP)の文字
	TEX_SETTING_BOARD,					// 設定の背景
	TEX_SETTING_TITLE_WORD,				// 設定画面のタイトル
	TEX_SETTING_WORD,					// 設定画面の項目
	TEX_SETTING_ARROW,					// 設定画面の矢印
	TEX_SETTING_MANUAL_BOARD,			// 設定項目の説明(背景)
	TEX_SETTING_MANUAL_LIST,			// 設定項目の説明(内容)
	TEX_RESULT_ACHIEVEMENT_ICON,		// 結果画面の達成項目
	TEX_TITLE_BG,						// タイトルの背景
	TEX_TITLE_BG_SQUARE_SMALL,			// タイトルの四角形(小)
	TEX_TITLE_BG_SQUARE_BIG,			// タイトルの四角形(大)
	TEX_TITLE_BG_FLOOR,					// タイトルの床
	TEX_TITLE_BG_HEXANGONAL,			// タイトルの六角形
	TEX_TITLE_BG_LOGO,					// タイトルのロゴ
	TEX_PREFACE_CONTROLLER,				// 前書き(コントローラ)
	TEX_PREFACE_TEAM_LOGO,				// 前書き(チームロゴ)
	TEX_TITLE_BTN_MANUAL,				// タイトルの操作説明
	TEX_TITLE_ALL_RIGHTS_RESERVED,		// タイトルのAllRightReserved
	TEX_GAME_MAIN_BG,					// ゲームの背景(メイン)
	TEX_GAME_MAIN_BG_MASK,				// ゲームの背景(マスク)
	TEX_GAME_BG_WALL,					// ゲームの背景(壁の六角形)
	TEX_GAME_BG_LINE,					// ゲームの背景(線分)
	TEX_GAME_BG_FLOOR,					// ゲームの背景(床)
	TEX_FADEINOUT_CENTER,				// フェードインアウト(中心)
	TEX_FADEINOUT_LEFT,					// フェードインアウト(左)
	TEX_FADEINOUT_RIGHT,				// フェードインアウト(右)
	TEX_FADEINOUT_LEFT_UP,				// フェードインアウト(左上)
	TEX_FADEINOUT_RIGHT_UP,				// フェードインアウト(右上)
	TEX_FADEINOUT_LEFT_DOWN,			// フェードインアウト(左下)
	TEX_FADEINOUT_RIGHT_DOWN,			// フェードインアウト(右下)
	TEX_RESULT_CLEAR_FAILED,			// リザルト画面のクリア/失敗アイコン
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
	TEX_BTN_MANUAL,						// ボタンの操作説明
	TEX_TITLE_MASK,						// タイトルマスク
	TEX_GAME_PLAYER_BOARD,
	TEX_GAME_PLAYER_MATCHING_RANK,
	TEX_GAME_RIVAL_BOARD,
	TEX_GAME_JUDGE_SLIDE_EFFECT_UP,		// スライドの判定エフェクト(上)
	TEX_RESULT_BTN,						// リザルト(ボタン)
	TEX_ERROR_LIST_BG,					// エラーリストの背景
	TEX_GAME_PREFACE_BG_UNDER,			// ゲーム前書き(背景・下)
	TEX_GAME_PREFACE_BG_UP,				// ゲーム前書き(背景・上)
	TEX_GAME_PREFACE_READY,				// ゲーム前書き(READY)
	TEX_GAME_PREFACE_START,				// ゲーム前書き(START)
	TEX_RANKING_BG,
	TEX_RANKING_HEADER,
	TEX_RANKING_RANK,
	TEX_RANKING_SCORE_NUM,
	TEX_TUTORIAL_BTN,					// チュートリアルボタン
	TEX_RESULT_RANKING_FILTER,
	TEX_GAME_JUDGE_EARLY_LATE,			// パーフェクト判定の誤差
	TEX_THANKS_BG,						// 後書きの背景
	TEX_THANKS_WORD,					// 後書きの文字
	TEX_THANKS_MASK,					// 後書きの文字
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
//	関数名:	SetTextureAddress
//	引数:	TEXTURE_NUM	 ファイルのURL
//	戻り値:	なし
//	説明:	指定されたテクスチャの入り替え
//------------------------------------------------------------------------------
void SetTextureAddress(int nNumTex, char *pFileAddress);

//------------------------------------------------------------------------------
//	関数名:	GetTextureStatus
//	引数:	TEXTURE_NUM	
//	戻り値:	なし
//	説明:	テクスチャの各データの取得
//------------------------------------------------------------------------------
TEXTURE GetTextureStatus(int nNum);

//------------------------------------------------------------------------------
//	関数名:	GetTexture
//	引数:	TEXTURE_NUM	
//	戻り値:	テクスチャ型データ
//	説明:	テクスチャの取得
//------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 GetTexture(int nNum);

//------------------------------------------------------------------------------
//	関数名:	SetTexture
//	引数:	TEXTURE_NUM	
//	戻り値:	なし
//	説明:	テクスチャの設定
//------------------------------------------------------------------------------
void SetTexture(int nTexNum);

//------------------------------------------------------------------------------
//	関数名:	GetTextureNum
//	引数:	なし	
//	戻り値:	テクスチャの番号
//	説明:	テクスチャの番号を取得
//------------------------------------------------------------------------------
int GetTextureNum();
#endif // !TEXTURE_H
