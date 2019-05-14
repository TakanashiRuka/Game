//----------------------------------------------------------------------------
//
//	カメラ基礎
//
//														 制作者：劉喆
//                                                       作成日：2017/11/01
//----------------------------------------------------------------------------

#ifndef CAMERA_H
#define CAMERA_H

void InitCamera();
void ChangeCameraByKey();
void UpdateCamera();

D3DXMATRIX GetCameraView();
void SetCamera(int nGameMode);
int GetCameraMode();

//------------------------------------------------------------------
// スライド回転用
//------------------------------------------------------------------
void ChangeCameraAngle();
void ResetCameraAngleTarget();
void SetCameraAngleTarget(float fAngle);
void SetChangeAngleCountMax(int nNum);

//------------------------------------------------------------------
// スライド上下用
//------------------------------------------------------------------
void ResetCameraY();
void SetCameraYTarget(float fY);
void ChangeCameraY();

#endif // !CAMERA_H
