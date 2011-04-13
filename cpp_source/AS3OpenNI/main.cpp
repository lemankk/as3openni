/****************************************************************************
*                                                                           *
*   Project:	AS3OpenNI	                                        		*
*   Version:	Alpha 1.0.9                                                 *
*   Author:     Tony Birleffi												*
*   URL: 		http://code.google.com/p/as3openni/							*
*                                                                           *
****************************************************************************/

/****************************************************************************
*                                                                           *
*   AS3OpenNI Alpha 1.0.9	                                                *
*   Copyright (C) 2011 Tony Birleffi. All Rights Reserved.                  *
*                                                                           *
*   This file has been provided pursuant to a License Agreement containing  *
*   restrictions on its use. This data contains valuable trade secrets      *
*   and proprietary information of PrimeSense Ltd. and is protected by law. *
*                                                                           *
****************************************************************************/

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------

#include <XnOpenNI.h>
#include <XnVHandPointContext.h>
#include <XnVSessionManager.h>
#include <XnVWaveDetector.h>
#include <XnVPushDetector.h>
#include <XnVBroadcaster.h>
#include <XnVCircleDetector.h>
#include <XnVSwipeDetector.h>
#include <XnVSteadyDetector.h>
#include <XnCppWrapper.h>
#include <XnModuleCppInterface.h>
#include <XnVSelectableSlider1D.h>
#include <XnVSelectableSlider2D.h>
#include <XnFPSCalculator.h>
#include <XnTypes.h>
#include <XnCodecIDs.h>
#include <XnOS.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "capture.h"
#include "kbhit.h"
#include "signal_catch.h"
#include "skeleton.h"
#include "params.h"
#include "setup_sockets.h"

//-----------------------------------------------------------------------------
// Setup Platform Mac/Win Sockets
//-----------------------------------------------------------------------------
#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	#include "socket.h"
	SOCKET POINT_SOCKET, SESSION_SOCKET, SLIDER_SOCKET, USER_TRACKING_SOCKET, DEPTH_MAP_SOCKET, RGB_SOCKET;
#else
    int POINT_SOCKET, SESSION_SOCKET, SLIDER_SOCKET, USER_TRACKING_SOCKET, DEPTH_MAP_SOCKET, RGB_SOCKET;
#endif

//-----------------------------------------------------------------------------
// Namespaces
//-----------------------------------------------------------------------------
using namespace xn;
using namespace std;

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
Context _context;
DepthGenerator _depth;
ImageGenerator _image;
HandsGenerator _hands;
GestureGenerator _gesture;
DepthMetaData _depthData;
ImageMetaData _imageData;
XnMapOutputMode _depthMode;
XnLicense _license;
UserGenerator _userGenerator;
SceneMetaData _sceneData;

XnVSessionManager* _sessionManager;
XnVBroadcaster* _broadcaster;
XnVWaveDetector* _waveDetector;
XnVPushDetector* _pushDetector;
XnVCircleDetector* _circleDetector;
XnVSwipeDetector* _swipeDetector;
XnVSteadyDetector* _steadyDetector;
XnVSelectableSlider1D* _leftRightSlider;
XnVSelectableSlider1D* _upDownSlider;
XnVSelectableSlider1D* _inOutSlider;
XnVSelectableSlider2D* _trackPad;

XnBool _inSession = false;
XnBool _quit = false;
XnBool _useSockets = true;

// For debugging purposes
XnBool _printGesture = false;
XnBool _printCircle = false;
XnBool _printSlider = false;
XnBool _printTrackPad = false;
XnBool _printSinglePoint = false;
XnBool _printUserTracking = false;
XnBool _printRGBCapture = false;
XnBool _printDepthMapCapture = false;

// Toggle on/off features
XnBool _featureGesture = false;
XnBool _featureCircle = false;
XnBool _featureSlider = false;
XnBool _featureTrackPad = false;
XnBool _featureSinglePoint = false;
XnBool _featureUserTracking = false;
XnBool _featureRGBCapture = false;
XnBool _featureDepthMapCapture = false;
XnBool _rgbGoGrey = false;
XnBool _snapPixels = false;
XnBool _mirror = true;

// User tracking vars
XnBool _needPose = false;
XnChar _strPose[20] = "";

// Define trackpad vars
int trackpad_columns = NULL, trackpad_rows = NULL;

// Set the frames per second.
XnFPSData xnFPS;

// Define the JPEG quality on the DepthMap and RGB capturing.
int depthmap_quality = 0, rgb_quality = 0;

//-----------------------------------------------------------------------------
// Define Events - Broadcast Out
//-----------------------------------------------------------------------------

// Session Events
#define SESSION_PROGRESS "event:session_progress"
#define SESSION_STARTED "event:session_started"
#define SESSION_ENDED "event:session_ended"

// Gesture Events
#define WAVE "event:wave"

// Circle Events
#define NO_CIRCLE "event:no_circle"
#define CIRCLE_CREATED "event:circle_created"
#define CIRCLE_DESTROYED "event:circle_destroyed"

// Left/Right Events
#define SLIDER_LEFTRIGHT_CREATED "event:slider_leftright_created"
#define SLIDER_LEFTRIGHT_DESTROYED "event:slider_leftright_destroyed"
#define SLIDER_LEFTRIGHT_ACTIVATED "event:slider_leftright_activated"
#define SLIDER_LEFTRIGHT_DEACTIVATED "event:slider_leftright_deactivated"

// Up/Down Events
#define SLIDER_UPDOWN_CREATED "event:slider_updown_created"
#define SLIDER_UPDOWN_DESTROYED "event:slider_updown_destroyed"
#define SLIDER_UPDOWN_ACTIVATED "event:slider_updown_activated"
#define SLIDER_UPDOWN_DEACTIVATED "event:slider_updown_deactivated"

// In/Out Events
#define SLIDER_INOUT_CREATED "event:slider_inout_created"
#define SLIDER_INOUT_DESTROYED "event:slider_inout_destroyed"
#define SLIDER_INOUT_ACTIVATED "event:slider_inout_activated"
#define SLIDER_INOUT_DEACTIVATED "event:slider_inout_deactivated"

// TrackPad Events
#define TRACKPAD_CREATED "event:trackpad_created"
#define TRACKPAD_DESTROYED "event:trackpad_destroyed"

//-----------------------------------------------------------------------------
// Error Handling
//-----------------------------------------------------------------------------

#define CHECK_RC(_status, what)											\
if (_status != XN_STATUS_OK)											\
{																\
	printf("%s failed: %s\n", what, xnGetStatusString(_status));		\
	return _status;													\
}

#define CHECK_ERRORS(_status, _errors, what)		\
if (_status == XN_STATUS_NO_NODE_PRESENT)	\
{										\
	XnChar strError[1024];				\
	_errors.ToString(strError, 1024);	\
	printf("%s\n", strError);			\
	return (_status);						\
}

//-----------------------------------------------------------------------------
// Private Methods
//-----------------------------------------------------------------------------

void addListeners()
{
	_broadcaster->AddListener(_waveDetector);
	if(_featureGesture)
	{
		_broadcaster->AddListener(_pushDetector);
		_broadcaster->AddListener(_swipeDetector);
		_broadcaster->AddListener(_steadyDetector);
	}
	
	if(_featureCircle) _broadcaster->AddListener(_circleDetector);
	
	if(_featureSlider)
	{
		_broadcaster->AddListener(_leftRightSlider);
		_broadcaster->AddListener(_upDownSlider);
		_broadcaster->AddListener(_inOutSlider);
	}
	
	if(_featureTrackPad) _broadcaster->AddListener(_trackPad);
	_sessionManager->AddListener(_broadcaster);
}

void removeListeners()
{
	_broadcaster->RemoveListener(_waveDetector);
	if(_featureGesture)
	{	
		_broadcaster->RemoveListener(_pushDetector);
		_broadcaster->RemoveListener(_swipeDetector);
		_broadcaster->RemoveListener(_steadyDetector);
	}
	
	if(_featureCircle) _broadcaster->RemoveListener(_circleDetector);
	
	if(_featureSlider)
	{
		_broadcaster->RemoveListener(_leftRightSlider);
		_broadcaster->RemoveListener(_upDownSlider);
		_broadcaster->RemoveListener(_inOutSlider);
	}
	
	if(_featureSlider) _broadcaster->RemoveListener(_trackPad);
	_sessionManager->RemoveListener(_broadcaster);
}

void CleanupExit()
{
	if (NULL != _sessionManager)
	{
		delete _sessionManager;
		_sessionManager = NULL;
	}
	
	removeListeners();
	delete _broadcaster;
	delete _waveDetector;
	delete _pushDetector;
	delete _swipeDetector;
	delete _steadyDetector;
	delete _circleDetector;
	delete _leftRightSlider;
	delete _upDownSlider;
	delete _inOutSlider;
	delete _trackPad;
	
	_context.Shutdown();
	
	if(_featureSinglePoint) close(POINT_SOCKET);
	if(_featureSlider) close(SLIDER_SOCKET);
	if(_featureUserTracking) close(USER_TRACKING_SOCKET);
	if(_featureDepthMapCapture) close(DEPTH_MAP_SOCKET);
	if(_featureRGBCapture) close(RGB_SOCKET);
	
	close(SESSION_SOCKET);
	exit(1);
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

//-----------------------------------------------------------------------------
// Session Event Handlers
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE SessionProgress(const XnChar* strFocus, const XnPoint3D& ptFocusPoint, XnFloat fProgress, void* UserCxt)
{
	printf("Session Progress\n");
	if(_useSockets) sendToSocket(SESSION_SOCKET, SESSION_PROGRESS);
}

void XN_CALLBACK_TYPE SessionStart(const XnPoint3D& pFocus, void* UserCxt)
{
	printf("Session Started\n");
	_inSession = true;
	addListeners();
	if(_useSockets) sendToSocket(SESSION_SOCKET, SESSION_STARTED);
}

void XN_CALLBACK_TYPE SessionEnd(void* UserCxt)
{
	printf("Session Ended\n");
	_inSession = false;
	if(_useSockets) sendToSocket(SESSION_SOCKET, SESSION_ENDED);
}

//-----------------------------------------------------------------------------
// Circle Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE CircleCB(XnFloat fTimes, XnBool bConfident, const XnVCircle* pCircle, void* pUserCxt)
{
	float fAngle = fmod((double)fTimes, 1.0) * 2 * XnVMathCommon::PI;
	if(_printCircle) printf("Circle Angle:%6.2f\n", fAngle);
	
	char cAngle[10];
	sprintf(cAngle,"circle_angle:%f",fAngle);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cAngle);
}

void XN_CALLBACK_TYPE NoCircleCB(XnFloat fLastValue, XnVCircleDetector::XnVNoCircleReason reason, void * pUserCxt)
{
	if(_printCircle) printf("No Circle\n");
	if(_useSockets) sendToSocket(SESSION_SOCKET, NO_CIRCLE);
}

void XN_CALLBACK_TYPE Circle_PrimaryCreate(const XnVHandPointContext *cxt, const XnPoint3D& ptFocus, void * pUserCxt)
{
	if(_printCircle) printf("Circle Primary Create\n");
	if(_useSockets) sendToSocket(SESSION_SOCKET, CIRCLE_CREATED);
}

void XN_CALLBACK_TYPE Circle_PrimaryDestroy(XnUInt32 nID, void * pUserCxt)
{
	if(_printCircle) printf("Circle Primary Destroy\n");
	if(_useSockets) sendToSocket(SESSION_SOCKET, CIRCLE_DESTROYED);
}

//-----------------------------------------------------------------------------
// Gesture Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE Swipe_SwipeUp(XnFloat fVelocity, XnFloat fAngle, void* cxt)
{
	if(_printGesture) printf("Swipe Up - Velocity:%f, Angle:%f\n", fVelocity, fAngle);
	char cSwipeUp[10];
	sprintf(cSwipeUp,"swipe_up:%f",fVelocity,fAngle);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cSwipeUp);
}

void XN_CALLBACK_TYPE Swipe_SwipeDown(XnFloat fVelocity, XnFloat fAngle, void* cxt)
{
	if(_printGesture) printf("Swipe Down - Velocity:%f, Angle:%f\n", fVelocity, fAngle);
	char cSwipeDown[10];
	sprintf(cSwipeDown,"swipe_down:%f",fVelocity,fAngle);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cSwipeDown);
}

void XN_CALLBACK_TYPE Swipe_SwipeLeft(XnFloat fVelocity, XnFloat fAngle, void* cxt)
{
	if(_printGesture) printf("Swipe Left - Velocity:%f, Angle:%f\n", fVelocity, fAngle);
	char cSwipeLeft[10];
	sprintf(cSwipeLeft,"swipe_left:%f",fVelocity,fAngle);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cSwipeLeft);
}

void XN_CALLBACK_TYPE Swipe_SwipeRight(XnFloat fVelocity, XnFloat fAngle, void* cxt)
{
	if(_printGesture) printf("Swipe Right - Velocity:%f, Angle:%f\n", fVelocity, fAngle);
	char cSwipeRight[10];
	sprintf(cSwipeRight,"swipe_right:%f",fVelocity,fAngle);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cSwipeRight);
}

void XN_CALLBACK_TYPE OnWave(void* cxt)
{
	if(_printGesture) printf("Wave Occured\n");
	if(_useSockets) sendToSocket(SESSION_SOCKET, WAVE);
}

void XN_CALLBACK_TYPE onPush(XnFloat fVelocity, XnFloat fAngle, void* cxt)
{
	if(_printGesture) printf("Push Occured - Velocity:%f, Angle:%f\n", fVelocity, fAngle);
	char cPush[10];
	sprintf(cPush,"push:%f:%f",fVelocity,fAngle);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cPush);
}

void XN_CALLBACK_TYPE Steady_OnSteady(XnUInt32 nId, XnFloat fVelocity, void* cxt)
{
	if(_printGesture) printf("Steady Occured - Velocity:%f\n", fVelocity);
	char cSteady[10];
	sprintf(cSteady,"steady:%f",fVelocity);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cSteady);
}

//-----------------------------------------------------------------------------
// Left/Right Slider Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE LeftRightSlider_OnValueChange(XnFloat fValue, void* cxt)
{
	if(_printSlider) printf("Slider - LeftRight - Value:%f\n", fValue);
	char cValue[50];
	sprintf(cValue,"slider_leftright_value:%f",fValue);
	if(_useSockets) sendToSocket(SLIDER_SOCKET, cValue);
}

void XN_CALLBACK_TYPE LeftRightSlider_OnActivate(void* cxt)
{
	if(_printSlider) printf("Slider - LeftRight - Activated\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_LEFTRIGHT_ACTIVATED);
}

void XN_CALLBACK_TYPE LeftRightSlider_OnDeactivate(void* cxt)
{
	if(_printSlider) printf("Slider - LeftRight - Deactivated\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_LEFTRIGHT_DEACTIVATED);
}

void XN_CALLBACK_TYPE LeftRightSlider_OnPrimaryCreate(const XnVHandPointContext* hand, const XnPoint3D& ptFocus, void* cxt)
{
	if(_printSlider) printf("Slider - LeftRight - Created\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_LEFTRIGHT_CREATED);
}

void XN_CALLBACK_TYPE LeftRightSlider_OnPrimaryDestroy(XnUInt32 nID, void* cxt)
{
	if(_printSlider) printf("Slider - LeftRight - Destroyed\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_LEFTRIGHT_DESTROYED);
}

//-----------------------------------------------------------------------------
// Up/Down Slider Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE UpDownSlider_OnValueChange(XnFloat fValue, void* cxt)
{
	if(_printSlider) printf("Slider - UpDown - Value:%f\n", fValue);
	char cValue[50];
	sprintf(cValue,"slider_updown_value:%f",fValue);
	if(_useSockets) sendToSocket(SLIDER_SOCKET, cValue);
}

void XN_CALLBACK_TYPE UpDownSlider_OnActivate(void* cxt)
{
	if(_printSlider) printf("Slider - UpDown - Activated\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_UPDOWN_ACTIVATED);
}

void XN_CALLBACK_TYPE UpDownSlider_OnDeactivate(void* cxt)
{
	if(_printSlider) printf("Slider - UpDown - Deactivated\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_UPDOWN_DEACTIVATED);
}

void XN_CALLBACK_TYPE UpDownSlider_OnPrimaryCreate(const XnVHandPointContext* hand, const XnPoint3D& ptFocus, void* cxt)
{
	if(_printSlider) printf("Slider - UpDown - Created\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_UPDOWN_CREATED);
}

void XN_CALLBACK_TYPE UpDownSlider_OnPrimaryDestroy(XnUInt32 nID, void* cxt)
{
	if(_printSlider) printf("Slider - UpDown - Destroyed\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_UPDOWN_DESTROYED);
}

//-----------------------------------------------------------------------------
// In/Out Slider Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE InOutSlider_OnValueChange(XnFloat fValue, void* cxt)
{
	if(_printSlider) printf("Slider - InOut - Value:%f\n", fValue);
	char cValue[50];
	sprintf(cValue,"slider_inout_value:%f",fValue);
	if(_useSockets) sendToSocket(SLIDER_SOCKET, cValue);
}

void XN_CALLBACK_TYPE InOutSlider_OnActivate(void* cxt)
{
	if(_printSlider) printf("Slider - InOut - Activated\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_INOUT_ACTIVATED);
}

void XN_CALLBACK_TYPE InOutSlider_OnDeactivate(void* cxt)
{
	if(_printSlider) printf("Slider - InOut - Deactivated\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_INOUT_DEACTIVATED);
}

void XN_CALLBACK_TYPE InOutSlider_OnPrimaryCreate(const XnVHandPointContext* hand, const XnPoint3D& ptFocus, void* cxt)
{
	if(_printSlider) printf("Slider - InOut - Created\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_INOUT_CREATED);
}

void XN_CALLBACK_TYPE InOutSlider_OnPrimaryDestroy(XnUInt32 nID, void* cxt)
{
	if(_printSlider) printf("Slider - InOut - Destroyed\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_INOUT_DESTROYED);
}

//-----------------------------------------------------------------------------
// TrackPad Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE TrackPad_ItemHover(XnInt32 nXItem, XnInt32 nYItem, void* cxt)
{
  	if(_printTrackPad) printf("TrackPad Hover: %d,%d\n", nXItem, nYItem);
	char cValue[50];
	sprintf(cValue,"trackpad_hover:%d:%d", nXItem, nYItem);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cValue);
}

void XN_CALLBACK_TYPE TrackPad_ItemSelect(XnInt32 nXItem, XnInt32 nYItem, XnVDirection eDir, void* cxt)
{
	if(_printTrackPad) printf("TrackPad Select: %d,%d (%s)\n", nXItem, nYItem, XnVDirectionAsString(eDir));
	char cValue[50];
	sprintf(cValue,"trackpad_hover:%d:%d:%s", nXItem, nYItem, XnVDirectionAsString(eDir));
	if(_useSockets) sendToSocket(SESSION_SOCKET, cValue);
}

void XN_CALLBACK_TYPE TrackPad_PrimaryCreate(const XnVHandPointContext* cxt, const XnPoint3D& ptFocus, void* UserCxt)
{
  	if(_printTrackPad)
  	{
  		printf("TrackPad Input Started, Point ID: [%d] ", cxt->nID);
  		printf("Starting Point Position: [%f],[%f],[%f]\n", cxt->ptPosition.X, cxt->ptPosition.Y, cxt->ptPosition.Z);
  	}
  	
  	if(_useSockets) sendToSocket(SESSION_SOCKET, TRACKPAD_CREATED);
}

void XN_CALLBACK_TYPE TrackPad_PrimaryDestroy(XnUInt32 nID, void* UserCxt)
{
  	if(_printTrackPad) printf("TrackPad Destroyed\n");
  	if(_useSockets) sendToSocket(SESSION_SOCKET, TRACKPAD_DESTROYED);
}

//-----------------------------------------------------------------------------
// Point Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE OnPointUpdate(const XnVHandPointContext* pContext, void* cxt)
{
	if(_printSinglePoint) printf("id:%d,x:%f,y:%f,z:%f\n", pContext->nID, pContext->ptPosition.X, pContext->ptPosition.Y, pContext->ptPosition.Z);
	
	//-------------------------------------------------------------------------------//
	//------------------------- SEND DATA TO POINT_SOCKET SERVER --------------------//
	//-------------------------------------------------------------------------------//
	
	// Allocate memory for the string we will send to the socket server.
	// length will be the size of the mem allocation for the string
	#if (XN_PLATFORM == XN_PLATFORM_WIN32)
		int length = _snprintf(NULL, 0, "%d,%f,%f,%f,%f", pContext->nID, pContext->ptPosition.X, pContext->ptPosition.Y, pContext->ptPosition.Z, pContext->fTime);
	#else
		int length = snprintf(NULL, 0, "%d,%f,%f,%f,%f", pContext->nID, pContext->ptPosition.X, pContext->ptPosition.Y, pContext->ptPosition.Z, pContext->fTime);
	#endif
	
	// Character object that will store the string
	char * data = (char*) malloc((length + 1) * sizeof(char));
	
	// Print string in format of: [id,x,y,x,time]
	#if (XN_PLATFORM == XN_PLATFORM_WIN32)
		_snprintf(data, length, "%d,%f,%f,%f,%f", pContext->nID, pContext->ptPosition.X, pContext->ptPosition.Y, pContext->ptPosition.Z, pContext->fTime);
	#else
		snprintf(data, length, "%d,%f,%f,%f,%f", pContext->nID, pContext->ptPosition.X, pContext->ptPosition.Y, pContext->ptPosition.Z, pContext->fTime);
	#endif
	
	// Send to socket.
	if(_useSockets) sendToSocket(POINT_SOCKET, data);
	free(data);
	
	//-------------------------------------------------------------------------------//
	//------------------------- SEND DATA TO POINT_SOCKET SERVER --------------------//
	//-------------------------------------------------------------------------------//
}

//-----------------------------------------------------------------------------
// User Tracking/Skeleton Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE User_NewUser(UserGenerator& generator, XnUserID nId, void* pCookie)
{
	if(_printUserTracking) printf("New User: %d\n", nId);
	if(_needPose)
	{
		_userGenerator.GetPoseDetectionCap().StartPoseDetection(_strPose, nId);
	}
	else
	{
		_userGenerator.GetSkeletonCap().RequestCalibration(nId, true);
	}
	
	char cValue[50];
	sprintf(cValue, "user_tracking_new_user:%d", nId);
	if(_useSockets) sendToSocket(USER_TRACKING_SOCKET, cValue);
}

void XN_CALLBACK_TYPE User_LostUser(UserGenerator& generator, XnUserID nId, void* pCookie)
{
	if(_printUserTracking) printf("Lost user: %d\n", nId);
	
	char cValue[50];
	sprintf(cValue, "user_tracking_lost_user:%d", nId);
	if(_useSockets) sendToSocket(USER_TRACKING_SOCKET, cValue);
}

void XN_CALLBACK_TYPE UserPose_PoseDetected(PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie)
{
	if(_printUserTracking) printf("Pose %s detected for user: %d\n", strPose, nId);
	_userGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
	_userGenerator.GetSkeletonCap().RequestCalibration(nId, true);
	
	char cValue[50];
	sprintf(cValue, "user_tracking_pose_detected:%d", nId);
	if(_useSockets) sendToSocket(USER_TRACKING_SOCKET, cValue);
}

void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
	if(_printUserTracking) printf("Calibration started for user: %d\n", nId);
	
	char cValue[50];
	sprintf(cValue, "user_tracking_user_calibration_start:%d", nId);
	if(_useSockets) sendToSocket(USER_TRACKING_SOCKET, cValue);
}

void XN_CALLBACK_TYPE UserCalibration_CalibrationEnd(SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie)
{
	if (bSuccess)
	{
		if(_printUserTracking) printf("Calibration complete, start tracking user: %d\n", nId);
		_userGenerator.GetSkeletonCap().StartTracking(nId);
		
		char cValue[50];
		sprintf(cValue, "user_tracking_user_calibration_complete:%d", nId);
		if(_useSockets) sendToSocket(USER_TRACKING_SOCKET, cValue);
	}
	else
	{
		if(_printUserTracking) printf("Calibration failed for user: %d\n", nId);
		if (_needPose)
		{
			_userGenerator.GetPoseDetectionCap().StartPoseDetection(_strPose, nId);
		}
		else
		{
			_userGenerator.GetSkeletonCap().RequestCalibration(nId, true);
		}
		
		char cValue[50];
		sprintf(cValue, "user_tracking_user_calibration_failed:%d", nId);
		if(_useSockets) sendToSocket(USER_TRACKING_SOCKET, cValue);
	}
}

//-----------------------------------------------------------------------------
// Init Methods
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{	
	//--------------------------------------------------------------------//
	//------------------------- SETUP REQUIRED NODES ---------------------//
	//--------------------------------------------------------------------//
	
	// Setup the command line parameters.
	setupParams(argc, argv);
	
	// Setup all the sockets.
	setupSockets();
    
	// Setup the status.
    XnStatus _status = XN_STATUS_OK;
    EnumerationErrors _errors;
    
    // Context Init and Add license.
	_status = _context.Init();
	CHECK_RC(_status, "Initialize context");
	_context.SetGlobalMirror(_mirror);
	
	XnChar vendor[XN_MAX_NAME_LENGTH];
	XnChar license[XN_MAX_LICENSE_LENGTH];

	_license.strVendor[XN_MAX_NAME_LENGTH] = strcmp(vendor, "PrimeSense");
	_license.strKey[XN_MAX_LICENSE_LENGTH] = strcmp(license, "0KOIk2JeIBYClPWVnMoRKn5cdY4=");
		
	_status = _context.AddLicense(_license);
   	CHECK_RC(_status, "Added license");
   	
   	// Set it to VGA maps at 30 FPS
	_depthMode.nXRes = 640;
	_depthMode.nYRes = 480;
	_depthMode.nFPS = 30;
	
	// Depth map create.
	_status = _depth.Create(_context);
	CHECK_RC(_status, "Create depth generator");
	_status = _depth.SetMapOutputMode(_depthMode);
	
	// Depth map create.
	_status = _image.Create(_context);
	CHECK_RC(_status, "Create image generator");
	_status = _image.SetMapOutputMode(_depthMode);
	_status = _image.SetPixelFormat(XN_PIXEL_FORMAT_RGB24);
	
	// Create the hands generator.
	_status = _hands.Create(_context);
	CHECK_RC(_status, "Create hands generator");
	_hands.SetSmoothing(0.1);

	// Create the gesture generator.
	_status = _gesture.Create(_context);
	CHECK_RC(_status, "Create gesture generator");
	
	// Create user generator.
	_status = _userGenerator.Create(_context);
	CHECK_RC(_status, "Find user generator");
	
	// Create and initialize point tracker
	_sessionManager = new XnVSessionManager();
	_status = _sessionManager->Initialize(&_context, "Wave", "RaiseHand");
	
	if (_status != XN_STATUS_OK)
	{
		printf("Couldn't initialize the Session Manager: %s\n", xnGetStatusString(_status));
		CleanupExit();
	}
	_sessionManager->RegisterSession(NULL, &SessionStart, &SessionEnd, &SessionProgress);
	
	// Start catching signals for quit indications
	CatchSignals(&_quit);
	
	//---------------------------------------------------------------//
	//------------------------- SETUP FEATURES ---------------------//
	//--------------------------------------------------------------//
	
	// Define the Wave and SinglePoint detectors.
	_waveDetector = new XnVWaveDetector();
	
	// SinglePoint detector.
	if(_featureSinglePoint) _waveDetector->RegisterPointUpdate(NULL, &OnPointUpdate);
	
	// Feature Gesture.
	if(_featureGesture)
	{
		// Wave detector.
		_waveDetector->RegisterWave(NULL, &OnWave);
		
		// Push detector.
		_pushDetector = new XnVPushDetector();
		_pushDetector->RegisterPush(NULL, &onPush);
	
		// Swipe detector.
		_swipeDetector = new XnVSwipeDetector();
		_swipeDetector->RegisterSwipeUp(NULL, &Swipe_SwipeUp);
		_swipeDetector->RegisterSwipeDown(NULL, &Swipe_SwipeDown);
		_swipeDetector->RegisterSwipeLeft(NULL, &Swipe_SwipeLeft);
		_swipeDetector->RegisterSwipeRight(NULL, &Swipe_SwipeRight);
	
		// Steady detector.
		_steadyDetector = new XnVSteadyDetector();
		_steadyDetector->RegisterSteady(NULL, &Steady_OnSteady);
	}
	
	// Feature Circle.
	if(_featureCircle)
	{
		// Circle detector.
		_circleDetector = new XnVCircleDetector();
		_circleDetector->RegisterCircle(NULL, &CircleCB);
		_circleDetector->RegisterNoCircle(NULL, &NoCircleCB);
		_circleDetector->RegisterPrimaryPointCreate(NULL, &Circle_PrimaryCreate);
		_circleDetector->RegisterPrimaryPointDestroy(NULL, &Circle_PrimaryDestroy);
	}
	
	// Feature Slider.
	if(_featureSlider)
	{
		// Left/Right slider.
		_leftRightSlider = new XnVSelectableSlider1D(3, 0, AXIS_X);
		_leftRightSlider->RegisterActivate(NULL, &LeftRightSlider_OnActivate);
		_leftRightSlider->RegisterDeactivate(NULL, &LeftRightSlider_OnDeactivate);
		_leftRightSlider->RegisterPrimaryPointCreate(NULL, &LeftRightSlider_OnPrimaryCreate);
		_leftRightSlider->RegisterPrimaryPointDestroy(NULL, &LeftRightSlider_OnPrimaryDestroy);
		_leftRightSlider->RegisterValueChange(NULL, &LeftRightSlider_OnValueChange);
		_leftRightSlider->SetValueChangeOnOffAxis(false);
		
		// Up/Down slider.
		_upDownSlider = new XnVSelectableSlider1D(3, 0, AXIS_Y);
		_upDownSlider->RegisterActivate(NULL, &UpDownSlider_OnActivate);
		_upDownSlider->RegisterDeactivate(NULL, &UpDownSlider_OnDeactivate);
		_upDownSlider->RegisterPrimaryPointCreate(NULL, &UpDownSlider_OnPrimaryCreate);
		_upDownSlider->RegisterPrimaryPointDestroy(NULL, &UpDownSlider_OnPrimaryDestroy);
		_upDownSlider->RegisterValueChange(NULL, &UpDownSlider_OnValueChange);
		_upDownSlider->SetValueChangeOnOffAxis(false);
		
		// In/Out slider.
		_inOutSlider = new XnVSelectableSlider1D(3, 0, AXIS_Z);
		_inOutSlider->RegisterActivate(NULL, &InOutSlider_OnActivate);
		_inOutSlider->RegisterDeactivate(NULL, &InOutSlider_OnDeactivate);
		_inOutSlider->RegisterPrimaryPointCreate(NULL, &InOutSlider_OnPrimaryCreate);
		_inOutSlider->RegisterPrimaryPointDestroy(NULL, &InOutSlider_OnPrimaryDestroy);
		_inOutSlider->RegisterValueChange(NULL, &InOutSlider_OnValueChange);
		_inOutSlider->SetValueChangeOnOffAxis(false);
	}
	
	// Feature TrackPad.
	if(_featureTrackPad)
	{
		// Track Pad.
		if(trackpad_columns > 0 && trackpad_rows > 0)
		{
			_trackPad = new XnVSelectableSlider2D(trackpad_columns, trackpad_rows);
		}
		else
		{
			_trackPad = new XnVSelectableSlider2D(4, 9);
		}
		
		_trackPad->RegisterItemHover(NULL, &TrackPad_ItemHover);
		_trackPad->RegisterItemSelect(NULL, &TrackPad_ItemSelect);
	    _trackPad->RegisterPrimaryPointCreate(NULL, &TrackPad_PrimaryCreate);
	  	_trackPad->RegisterPrimaryPointDestroy(NULL, &TrackPad_PrimaryDestroy);
	}
	
	// Feature User Tracking.
	if(_featureUserTracking)
	{
		// Setup user generator callbacks.
		XnCallbackHandle hUserCallbacks, hCalibrationCallbacks, hPoseCallbacks;
		if (!_userGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
		{
			printf("Supplied user generator doesn't support skeleton\n");
			return 1;
		}
		_userGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
		
		// Setup Skeleton detection.
		_userGenerator.GetSkeletonCap().RegisterCalibrationCallbacks(UserCalibration_CalibrationStart, UserCalibration_CalibrationEnd, NULL, hCalibrationCallbacks);
		if (_userGenerator.GetSkeletonCap().NeedPoseForCalibration())
		{
			_needPose = true;
			if (!_userGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
			{
				printf("Pose required, but not supported\n");
				return 1;
			}
			_userGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks(UserPose_PoseDetected, NULL, NULL, hPoseCallbacks);
			_userGenerator.GetSkeletonCap().GetCalibrationPose(_strPose);
		}
		_userGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
	}
	
	// Create the broadcaster manager.
	_broadcaster = new XnVBroadcaster();
	
	// Start generating all.
	_context.StartGeneratingAll();
	
	// Set the frame rate.
	_status = xnFPSInit(&xnFPS, 180);
	CHECK_RC(_status, "FPS Init");
	
	//----------------------------------------------------------------------//
	//------------------------- SETUP DISPLAY SUPPORT ---------------------//
	//--------------------------------------------------------------------//
	
	// Setup depth and image data.
	_depth.GetMetaData(_depthData);
	_image.GetMetaData(_imageData);
	
	// Hybrid mode isn't supported in this sample
	if (_imageData.FullXRes() != _depthData.FullXRes() || _imageData.FullYRes() != _depthData.FullYRes())
	{
		printf ("The device depth and image resolution must be equal!\n");
		return 1;
	}

	// RGB is the only image format supported.
	if (_imageData.PixelFormat() != XN_PIXEL_FORMAT_RGB24)
	{
		printf("The device image format must be RGB24\n");
		return 1;
	}
	
	// Setup capture if turned on.
	if(_featureRGBCapture || _featureDepthMapCapture) setupCapture();
	
	//-------------------------------------------------------------//
	//------------------------- MAIN LOOP ------------------------//
	//-----------------------------------------------------------//
	
	// Main loop
	while ((!_kbhit()) && (!_quit))
	{
		xnFPSMarkFrame(&xnFPS);
		_context.WaitAndUpdateAll();
		_sessionManager->Update(&_context);
		if(_featureUserTracking) renderSkeleton();
		if(_featureDepthMapCapture) captureDepthMap();
		if(_featureRGBCapture) captureRGB();
	}
	
	CleanupExit();
}

