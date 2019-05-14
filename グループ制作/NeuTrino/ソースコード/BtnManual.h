//----------------------------------------------------------------------------
//
//	ボタンの操作説明
//
//														 制作者：劉��
//                                                       作成日：2017/02/06
//----------------------------------------------------------------------------

#ifndef BTN_MANUAL_H
#define BTN_MANUAL_H

//---------------------------インクルード-------------------------------------

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	BMP_PLAYER_ENTRY,
	BMP_SELECT_SONG,
	BMP_SETTING,
	BMP_FINAL_CHECK_OFFLINE,
	BMP_FINAL_CHECK_ONLINE,
	BMP_DESTROY_ROOM_GET_READY,
	BMP_READY,
	BMP_MAX
}BTN_MANUAL_PARTS;

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------

//---------------------------プロトタイプ宣言---------------------------------
void InitBtnManual(int nBtnManual);
void UninitBtnManual();
void UpdateBtnManual();
void DrawBtnManual();

//------------------------------------------------------------------------------
//	関数名:	SetBtnManualPos
//	引数:	目標座標の番号
//	戻り値:	なし
//	説明:	ボタンの操作説明の目標座標を設定する
//------------------------------------------------------------------------------
void SetBtnManualPos(int nPart);

//------------------------------------------------------------------------------
//	関数名:	SetBtnManualTarget
//	引数:	目標項目の番号
//	戻り値:	なし
//	説明:	ボタンの操作説明の目標項目を設定する
//------------------------------------------------------------------------------
void SetBtnManualTarget(int nNum);

bool GetBtnManualStatus();

#endif // !BTN_MANUAL_H


