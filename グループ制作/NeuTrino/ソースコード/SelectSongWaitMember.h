#ifndef _SELECT_SONG_WAIT_MEMBER_H_
#define _SELECT_SONG_WAIT_MEMBER_H_

void InitSelectSongWaitMember();
void UninitSelectSongWaitMember();
void UpdateSelectSongWaitMember();
void DrawSelectSongWaitMember();
bool GetRoomCreate();
bool GetRoomEntry();
bool GetRoomReady();
void SetRoomCreate(bool create);
void SetRoomEntry(bool entry);
bool GetRoomEntryReady();
bool GetReadyFinalCheck();

#endif // !_SELECT_SONG_WAIT_MEMBER_H_