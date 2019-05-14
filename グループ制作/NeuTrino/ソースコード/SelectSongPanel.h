//----------------------------------------------------------------------------
//
//	曲パネル
//
//														 制作者：劉喆
//                                                       作成日：2017/12/27
//----------------------------------------------------------------------------

#ifndef SELECT_SONG_PANEL_H
#define SELECT_SONG_PANEL_H

//---------------------------インクルード-------------------------------------

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	char c_NoteFileAddress[MAX_WORD];			// 譜面ファイルのURL
	char c_SongFileAddress[MAX_WORD];			// 曲ファイルのURL
	char c_AlbumFileAddress[MAX_WORD];			// 曲サムネファイルのURL
	unsigned int n_SampleBeginTime;				// サンプル再生の始まり時点
	float fBPM;									// テンポ
	int nLevel[MAX_LEVEL];						// レベル
	char c_Title[MAX_WORD];						// タイトル
	char c_Subtitle[MAX_WORD];					// サブタイトル
	char c_NoteDesigner[MAX_WORD];				// 譜面作成者
	bool bDemo;									// デモモードで出すかどうか
}SSSONGSTATUS;									// 曲のデータ

typedef struct {
	D3DXVECTOR2 vPos;							// 座標
	int nA;										// α値
	int nV;										// Vの値
	int	nFadeInCount;							// フェードインカウンター
}SONG_LEVEL_WORD;								// 難易度(文字)

//---------------------------グローバル変数-----------------------------------

//---------------------------プロトタイプ宣言---------------------------------

void InitSelectSongPanel();
void UninitSelectSongPanel();
void UpdateSelectSongPanel();
void DrawSelectSongPanel();

void InitAllSongBuffer();

//------------------------------------------------------------------------------
//	関数名:	SetSongStatus
//	引数:	ファイルの番号　ファイルのURL
//	戻り値:	なし
//	説明:	曲データを設定する
//------------------------------------------------------------------------------
void SetSongStatus(int nSongNum, char *cNoteAddress);

void DrawSongStatus();

//------------------------------------------------------------------------------
//	関数名:	GetLevel
//	引数:	なし
//	戻り値:	レベル
//	説明:	レベルを取得する
//------------------------------------------------------------------------------
int GetLevel();

//------------------------------------------------------------------------------
//	関数名:	SetLevel
//	引数:	レベル
//	戻り値:	なし
//	説明:	レベルを設定する
//------------------------------------------------------------------------------
void SetLevel(int nLevel);

//------------------------------------------------------------------------------
//	関数名:	GetSelectNum
//	引数:	なし
//	戻り値:	曲の番号
//	説明:	曲の番号を取得する
//------------------------------------------------------------------------------
int GetSelectNum();

//------------------------------------------------------------------------------
//	関数名:	SetSelectNum
//	引数:	曲の番号
//	戻り値:	なし
//	説明:	曲の番号を設定する
//------------------------------------------------------------------------------
void SetSelectNum(int nSelectNum);

//------------------------------------------------------------------------------
//	関数名:	GetLevelNum
//	引数:	なし
//	戻り値:	レベル数字
//	説明:	レベル数字を取得する
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
//	関数名:	SetSongPanelA
//	引数:	α値の番号
//	戻り値:	なし
//	説明:	曲パネルのα値を変換させる
//------------------------------------------------------------------------------
void SetSongPanelA(int nParts);

//------------------------------------------------------------------------------
//	関数名:	GetSongPanelChangeAStatus
//	引数:	なし
//	戻り値:	曲パネルのα値の変化フラグ
//	説明:	曲パネルのα値の変化フラグを取得する
//------------------------------------------------------------------------------
bool GetSongPanelChangeAStatus();

//------------------------------------------------------------------------------
//	関数名:	ChangeSelectNum
//	引数:	曲の番号
//	戻り値:	なし
//	説明:	選択している曲を設定させる
//------------------------------------------------------------------------------
void ChangeSelectNum(int nNum);

SSSONGSTATUS GetSSSongStatus(int nSongNum);

bool GetIsDemo();

#endif // !SELECT_SONG_PANEL_H
