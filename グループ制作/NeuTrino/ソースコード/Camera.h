//----------------------------------------------------------------------------
//
//	�J������b
//
//														 ����ҁF����
//                                                       �쐬���F2017/11/01
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
// �X���C�h��]�p
//------------------------------------------------------------------
void ChangeCameraAngle();
void ResetCameraAngleTarget();
void SetCameraAngleTarget(float fAngle);
void SetChangeAngleCountMax(int nNum);

//------------------------------------------------------------------
// �X���C�h�㉺�p
//------------------------------------------------------------------
void ResetCameraY();
void SetCameraYTarget(float fY);
void ChangeCameraY();

#endif // !CAMERA_H
