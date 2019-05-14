//----------------------------------------------------------------------------
//
//	譜面ファイルの読み取り
//
//														 制作者：劉��
//                                                       作成日：2017/12/10
//----------------------------------------------------------------------------
#ifndef READ_NOTE_FILE_H
#define READ_NOTE_FILE_H

//---------------------------インクルード-------------------------------------

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	float nStartTime;			// 小節の開始時間
	int nMaxNumOfNote;			// 小節の音符の最大数
	float fBPM;					// 小節のBPM
	float fMessare;				// 小節の長さ
}LINE;							// 小節

//---------------------------プロトタイプ宣言---------------------------------
void InitReadNote();
void SetReadNote();
void UninitReadNote();
void DrawReadNoteDebug();

//------------------------------------------------------------------------------
//	関数名:	GetBPMTime
//	引数:	小節の長さ　BPM
//	戻り値:	小節時間の長さ
//	説明:	小節時間の長さを計算する
//------------------------------------------------------------------------------
float GetBPMTime(float fMeasure, float fBPM);

//------------------------------------------------------------------------------
//	関数名:	GetMaxLineNum
//	引数:	なし
//	戻り値:	小節の本数
//	説明:	小節の本数を取得する
//------------------------------------------------------------------------------
int GetMaxLineNum();

//------------------------------------------------------------------------------
//	関数名:	GetLine
//	引数:	小節の番号
//	戻り値:	小節の属性
//	説明:	小節の属性を取得する
//------------------------------------------------------------------------------
LINE GetLine(int nNum);

//------------------------------------------------------------------------------
//	関数名:	GetRecordLine
//	引数:	時間
//	戻り値:	小節の番号
//	説明:	ある時間小節の番号を取得する
//------------------------------------------------------------------------------
int GetRecordLine(int nTime);

//------------------------------------------------------------------------------
//	関数名: GetNoteOffsetFromFile
//	引数:	なし
//	戻り値:	オフセット
//	説明:	譜面ファイルからオフセットを取得する
//------------------------------------------------------------------------------
int GetNoteOffsetFromFile();

//------------------------------------------------------------------------------
//	関数名: SetLineOffset
//	引数:	オフセット
//	戻り値:	なし 
//	説明:	小節ラインのオフセットを設定する(ファイル)
//------------------------------------------------------------------------------
void SetLineOffset(int nOffset);

//------------------------------------------------------------------------------
//	関数名: SetNoteFileURL
//	引数:	文字列の先頭アドレス
//	戻り値:	なし 
//	説明:	譜面ファイルのURLを設定する
//------------------------------------------------------------------------------
void SetNoteFileURL(char *cURL);

#endif // !READ_NOTE_FILE_H

