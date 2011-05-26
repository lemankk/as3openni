/****************************************************************************
*                                                                           *
*   Project:	AS3OpenNI	                                        		*
*   Version:	Alpha 1.3.0                                                 *
*   Author:     Tony Birleffi												*
*   URL: 		http://code.google.com/p/as3openni/							*
*                                                                           *
****************************************************************************/

/****************************************************************************
*                                                                           *
*   AS3OpenNI Alpha 1.3.0	                                                *
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

#include "kbhit.h"
#include "signal_catch.h"
#include "skeletons_string.h"
#include "params.h"
#include "setup_sockets.h"

#include "network.h"
#include "players.h"
#include "skeletons.h"

//-----------------------------------------------------------------------------
// Setup Platform Mac/Win Sockets
//-----------------------------------------------------------------------------
#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	#include <pthread/pthread.h>
	SOCKET POINT_SOCKET, SESSION_SOCKET, SLIDER_SOCKET, USER_TRACKING_SOCKET;
#else
    int POINT_SOCKET, SESSION_SOCKET, SLIDER_SOCKET, USER_TRACKING_SOCKET;
	#include <pthread.h>
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

// For debugging purposes
XnBool _printGesture = false;
XnBool _printCircle = false;
XnBool _printSlider = false;
XnBool _printTrackPad = false;
XnBool _printSinglePoint = false;
XnBool _printUserTracking = false;

// Toggle on/off features
XnBool _featureGesture = false;
XnBool _featureCircle = false;
XnBool _featureSlider = false;
XnBool _featureTrackPad = false;
XnBool _featureSinglePoint = false;
XnBool _featureUserTracking = false;
XnBool _featureRGBCapture = false;
XnBool _featureDepthMapCapture = false;

// Toggle extra features
XnBool _snapPixels = true;
XnBool _mirror = true;
XnBool _depthMapBackground = false;
XnBool _drawPixels = true;
XnBool _inSession = false;
XnBool _quit = false;
XnBool _useSockets = true;

// User tracking vars
XnBool _needPose = false;
XnChar _strPose[20] = "";

// Define trackpad vars
int trackpad_columns = NULL, trackpad_rows = NULL;

// Set the frames per second
XnFPSData xnFPS;

// Setup the depth map colors
XnFloat Colors[][3] =
{
	{0,1,1},
	{0,0,1},
	{0,1,0},
	{1,1,0},
	{1,0,0},
	{1,.5,0},
	{.5,1,0},
	{0,.5,1},
	{.5,0,1},
	{1,1,.5},
	{1,1,1}
};
XnUInt32 nColors = 10;

// Capture Definitions
#define MAX_USERS 5
#define MAX_DEPTH 10000

// Capture vars.
unsigned char g_ucDepthBuffer[4*640*480];
unsigned char g_ucImageBuffer[4*640*480];

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	players g_ucPlayersBuffer[MAX_USERS];
	skeletons g_ucSkeletonsBuffer[MAX_USERS];
#endif

float g_pDepthHist[MAX_DEPTH];
pthread_t g_ServerThread;
network g_AS3Network;
int g_Connected = 0;

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
	printf("AS3OpenNI :: %s failed: %s\n", what, xnGetStatusString(_status));		\
	return _status;													\
}

#define CHECK_ERRORS(_status, _errors, what)		\
if (_status == XN_STATUS_NO_NODE_PRESENT)	\
{										\
	XnChar strError[1024];				\
	_errors.ToString(strError, 1024);	\
	printf("AS3OpenNI :: %s\n", strError);			\
	return (_status);						\
}

//-----------------------------------------------------------------------------
// Private Methods
//-----------------------------------------------------------------------------

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	
	void getJointPosition(XnUserID player, XnSkeletonJoint eJoint1, unsigned char * dest)
	{
		if (!g_UserGenerator.GetSkeletonCap().IsTracking(player))
		{
			printf("not tracked!\n");
			return;
		}
		
		XnSkeletonJointPosition joint1;
		g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, eJoint1, joint1);
	
		if (joint1.fConfidence < 0.5)
		{
			return;
		}
		
		XnPoint3D pt[1];
		pt[0] = joint1.position;
		g_DepthGenerator.ConvertRealWorldToProjective(1, pt, pt);
		
		float _x, _y, _z;
		_x = pt[0].X;
		_y = pt[0].Y;
		_z = pt[0].Z;
		memcpy(dest, &_x, 4);
		memcpy(dest+4, &_y, 4);
		memcpy(dest+8, &_z, 4);
	}

	void copyNIData(unsigned char * dest, float x, float y, float z)
	{
		memcpy(dest, &x, 4);
		memcpy(dest+4, &y, 4);
		memcpy(dest+8, &z, 4);
	}
	
	void getPlayers()
	{	
		XnUserID aUsers[MAX_USERS];
		XnUInt16 nUsers = MAX_USERS;
		g_UserGenerator.GetUsers(aUsers, nUsers);
		
		for (int i = 0; i < nUsers; ++i)
		{
			XnUserID player;
			player = aUsers[i];
	
			// Track each player.
			XnPoint3D com;
			g_UserGenerator.GetCoM(player, com);
			memcpy(g_ucPlayersBuffer[i].player_id, &player, 4);
			copyNIData(g_ucPlayersBuffer[i].player_data, com.X, com.Y, com.Z);
	
			//printf("AS3OpenNI :: User Tracking: %f, %f, %f\n", com.X, com.Y, com.Z);
			
			// If a user is being tracked then do this.
			if(g_UserGenerator.GetSkeletonCap().IsTracking(player))
			{
				memcpy(g_ucSkeletonsBuffer[i].player_id, &player, 4);
				XnSkeletonJointPosition head, neck, left_shoulder, left_elbow, left_hand, right_shoulder, right_elbow, right_hand;
				XnSkeletonJointPosition torso, left_hip, left_knee, left_foot, right_hip, right_knee, right_foot;
				
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_HEAD, head);
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_NECK, neck);
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_TORSO, torso);
	
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_SHOULDER, left_shoulder);
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_ELBOW, left_elbow);
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_HAND, left_hand);
	
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_SHOULDER, right_shoulder);
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_ELBOW, right_elbow);
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_HAND, right_hand);
	
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_HIP, left_hip);
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_KNEE, left_knee);
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_FOOT, left_foot);
	
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_HIP, right_hip);
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_KNEE, right_knee);
				g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_FOOT, right_foot);
	
				copyNIData(g_ucSkeletonsBuffer[i].head, head.position.X, head.position.Y, head.position.Z);
				copyNIData(g_ucSkeletonsBuffer[i].neck, neck.position.X, neck.position.Y, neck.position.Z);
				copyNIData(g_ucSkeletonsBuffer[i].torso, torso.position.X, torso.position.Y, torso.position.Z);
	
				copyNIData(g_ucSkeletonsBuffer[i].lshoulder, left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z);
				copyNIData(g_ucSkeletonsBuffer[i].lelbow, left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z);
				copyNIData(g_ucSkeletonsBuffer[i].lhand, left_hand.position.X, left_hand.position.Y, left_hand.position.Z);
	
				copyNIData(g_ucSkeletonsBuffer[i].rshoulder, right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z);
				copyNIData(g_ucSkeletonsBuffer[i].relbow, right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z);
				copyNIData(g_ucSkeletonsBuffer[i].rhand, right_hand.position.X, right_hand.position.Y, right_hand.position.Z);
	
				copyNIData(g_ucSkeletonsBuffer[i].lhip, left_hip.position.X, left_hip.position.Y, left_hip.position.Z);
				copyNIData(g_ucSkeletonsBuffer[i].lknee, left_knee.position.X, left_knee.position.Y, left_knee.position.Z);
				copyNIData(g_ucSkeletonsBuffer[i].lfoot, left_foot.position.X, left_foot.position.Y, left_foot.position.Z);
	
				copyNIData(g_ucSkeletonsBuffer[i].rhip, right_hip.position.X, right_hip.position.Y, right_hip.position.Z);
				copyNIData(g_ucSkeletonsBuffer[i].rknee, right_knee.position.X, right_knee.position.Y, right_knee.position.Z);
				copyNIData(g_ucSkeletonsBuffer[i].rfoot, right_foot.position.X, right_foot.position.Y, right_foot.position.Z);
			}
		}
	}
#endif

void captureDepthMap(unsigned char* g_ucDepthBuffer)
{
	SceneMetaData smd;
	DepthMetaData dmd;
	_depth.GetMetaData(dmd);

	//printf("AS3OpenNI :: Frame %d Middle point is: %u. FPS: %f\n", dmd.FrameID(), dmd(dmd.XRes() / 2, dmd.YRes() / 2), xnFPSCalc(&xnFPS));

	_depth.GetMetaData(dmd);
	_userGenerator.GetUserPixels(0, smd);
	unsigned int nValue = 0;
	unsigned int nHistValue = 0;
	unsigned int nIndex = 0;
	unsigned int nX = 0;
	unsigned int nY = 0;
	unsigned int nNumberOfPoints = 0;
	XnUInt16 g_nXRes = dmd.XRes();
	XnUInt16 g_nYRes = dmd.YRes();

	const XnDepthPixel* pDepth = dmd.Data();
	const XnLabel* pLabels = smd.Data();

	// Calculate the accumulative histogram
	memset(g_pDepthHist, 0, MAX_DEPTH*sizeof(float));
	for (nY=0; nY<g_nYRes; nY++)
	{
		for (nX=0; nX<g_nXRes; nX++)
		{
			nValue = *pDepth;
			if (nValue != 0)
			{
				g_pDepthHist[nValue]++;
				nNumberOfPoints++;
			}
			pDepth++;
		}
	}

	for (nIndex=1; nIndex<MAX_DEPTH; nIndex++)
	{
		g_pDepthHist[nIndex] += g_pDepthHist[nIndex-1];
	}

	if (nNumberOfPoints)
	{
		for (nIndex=1; nIndex<MAX_DEPTH; nIndex++)
		{
			g_pDepthHist[nIndex] = (unsigned int)(256 * (1.0f - (g_pDepthHist[nIndex] / nNumberOfPoints)));
		}
	}
	
	pDepth = dmd.Data();
	if (_drawPixels)
	{
		XnUInt32 nIndex = 0;
		for (nY=0; nY<g_nYRes; nY++)
		{
			for (nX=0; nX < g_nXRes; nX++, nIndex++)
			{
				g_ucDepthBuffer[0] = 0;
				g_ucDepthBuffer[1] = 0;
				g_ucDepthBuffer[2] = 0;
				g_ucDepthBuffer[3] = 0x00;
				if (_depthMapBackground || *pLabels != 0)
				{
					nValue = *pDepth;
					XnLabel label = *pLabels;
					XnUInt32 nColorID = label % nColors;
					if (label == 0)
					{
						nColorID = nColors;
					}

					if (nValue != 0)
					{
						nHistValue = g_pDepthHist[nValue];
						g_ucDepthBuffer[0] = nHistValue * Colors[nColorID][0]; 
						g_ucDepthBuffer[1] = nHistValue * Colors[nColorID][1];
						g_ucDepthBuffer[2] = nHistValue * Colors[nColorID][2];
						g_ucDepthBuffer[3] = 0xFF;
					}
				}
				pDepth++;
				pLabels++;
				g_ucDepthBuffer+=4;
			}
		}
	}
}

void captureRGB(unsigned char* g_ucImageBuffer)
{
	ImageMetaData imd;
	_image.GetMetaData(imd);

	unsigned int nValue = 0;
	unsigned int nX = 0;
	unsigned int nY = 0;
	XnUInt16 g_nXRes = imd.XRes();
	XnUInt16 g_nYRes = imd.YRes();

	const XnRGB24Pixel * pImageMap = _image.GetRGB24ImageMap();
	for (nY=0; nY<g_nYRes; nY++) 
	{
		for (nX=0; nX < g_nXRes; nX++) 
		{
			((unsigned char*)g_ucImageBuffer)[(nY*g_nXRes+nX)*4+0] = pImageMap[nY*g_nXRes+nX].nBlue;
			((unsigned char*)g_ucImageBuffer)[(nY*g_nXRes+nX)*4+1] = pImageMap[nY*g_nXRes+nX].nGreen;
	        ((unsigned char*)g_ucImageBuffer)[(nY*g_nXRes+nX)*4+2] = pImageMap[nY*g_nXRes+nX].nRed;
			((unsigned char*)g_ucImageBuffer)[(nY*g_nXRes+nX)*4+3] = 0x00;
		}
	}
}

void *serverData(void *arg) 
{
	int len = 8*10;
	unsigned char *buff = (unsigned char*)malloc(len); // Command buffer
	while(g_Connected)
	{
		len = g_AS3Network.getData(buff, 1024);
		if(len > 0 && len % 6 == 0)
		{
			// Get the number of commands received.
			int max = len / 6;
			int i;
			
			// For each command received.
			for(i = 0; i < max; i++)
			{
				int code = buff[0 + (i*6)];
				switch(code)
				{
					case 1: // OPENNI
						switch(buff[1 + (i*6)])
						{
							case 0: // GET DEPTH
								if(_featureDepthMapCapture) g_AS3Network.sendMessage(1,0,g_ucDepthBuffer,sizeof(g_ucDepthBuffer));
							break;
							
							case 1: // GET RGB
								if(_featureRGBCapture) g_AS3Network.sendMessage(1,1,g_ucImageBuffer,sizeof(g_ucImageBuffer));
							break;
							
							#if (XN_PLATFORM == XN_PLATFORM_WIN32)
								
								case 4: // GET USERS
									if(_featureUserTracking) 
									{
										for (int i = 0; i < MAX_USERS; ++i)
										{
											g_AS3Network.sendMessage(1,4,g_ucPlayersBuffer[i].data,g_ucPlayersBuffer[i].size);
										}
									}
								break;
	
								case 5: // GET SKELETONS
									if(_featureUserTracking) 
									{
										for (int i = 0; i < MAX_USERS; ++i)
										{
											g_AS3Network.sendMessage(1,5,g_ucSkeletonsBuffer[i].data,g_ucSkeletonsBuffer[i].size);
										}
									}
								break;
							
							#endif
						}
					break;
				}
			}
		}
	}
	return 0;
}

void setupServer() 
{
	g_Connected = 1;
	if (pthread_create(&g_ServerThread, NULL, &serverData, NULL)) 
	{
		fprintf(stderr, "AS3OpenNI :: Error on pthread_create() for the server\n");
	}
	g_AS3Network.sendMessage(0,0,0);
}

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
	
	g_AS3Network.closeConnection();
	g_Connected = 0;
	
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
	printf("AS3OpenNI :: Session Progress\n");
	if(_useSockets) sendToSocket(SESSION_SOCKET, SESSION_PROGRESS);
}

void XN_CALLBACK_TYPE SessionStart(const XnPoint3D& pFocus, void* UserCxt)
{
	printf("AS3OpenNI :: Session Started\n");
	_inSession = true;
	addListeners();
	if(_useSockets) sendToSocket(SESSION_SOCKET, SESSION_STARTED);
}

void XN_CALLBACK_TYPE SessionEnd(void* UserCxt)
{
	printf("AS3OpenNI :: Session Ended\n");
	_inSession = false;
	if(_useSockets) sendToSocket(SESSION_SOCKET, SESSION_ENDED);
}

//-----------------------------------------------------------------------------
// Circle Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE CircleCB(XnFloat fTimes, XnBool bConfident, const XnVCircle* pCircle, void* pUserCxt)
{
	float fAngle = fmod((double)fTimes, 1.0) * 2 * XnVMathCommon::PI;
	if(_printCircle) printf("AS3OpenNI :: Circle Angle:%6.2f\n", fAngle);
	
	char cAngle[10];
	sprintf(cAngle,"circle_angle:%f",fAngle);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cAngle);
}

void XN_CALLBACK_TYPE NoCircleCB(XnFloat fLastValue, XnVCircleDetector::XnVNoCircleReason reason, void * pUserCxt)
{
	if(_printCircle) printf("AS3OpenNI :: No Circle\n");
	if(_useSockets) sendToSocket(SESSION_SOCKET, NO_CIRCLE);
}

void XN_CALLBACK_TYPE Circle_PrimaryCreate(const XnVHandPointContext *cxt, const XnPoint3D& ptFocus, void * pUserCxt)
{
	if(_printCircle) printf("AS3OpenNI :: Circle Primary Create\n");
	if(_useSockets) sendToSocket(SESSION_SOCKET, CIRCLE_CREATED);
}

void XN_CALLBACK_TYPE Circle_PrimaryDestroy(XnUInt32 nID, void * pUserCxt)
{
	if(_printCircle) printf("AS3OpenNI :: Circle Primary Destroy\n");
	if(_useSockets) sendToSocket(SESSION_SOCKET, CIRCLE_DESTROYED);
}

//-----------------------------------------------------------------------------
// Gesture Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE Swipe_SwipeUp(XnFloat fVelocity, XnFloat fAngle, void* cxt)
{
	if(_printGesture) printf("AS3OpenNI :: Swipe Up - Velocity:%f, Angle:%f\n", fVelocity, fAngle);
	char cSwipeUp[10];
	sprintf(cSwipeUp,"swipe_up:%f",fVelocity,fAngle);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cSwipeUp);
}

void XN_CALLBACK_TYPE Swipe_SwipeDown(XnFloat fVelocity, XnFloat fAngle, void* cxt)
{
	if(_printGesture) printf("AS3OpenNI :: Swipe Down - Velocity:%f, Angle:%f\n", fVelocity, fAngle);
	char cSwipeDown[10];
	sprintf(cSwipeDown,"swipe_down:%f",fVelocity,fAngle);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cSwipeDown);
}

void XN_CALLBACK_TYPE Swipe_SwipeLeft(XnFloat fVelocity, XnFloat fAngle, void* cxt)
{
	if(_printGesture) printf("AS3OpenNI :: Swipe Left - Velocity:%f, Angle:%f\n", fVelocity, fAngle);
	char cSwipeLeft[10];
	sprintf(cSwipeLeft,"swipe_left:%f",fVelocity,fAngle);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cSwipeLeft);
}

void XN_CALLBACK_TYPE Swipe_SwipeRight(XnFloat fVelocity, XnFloat fAngle, void* cxt)
{
	if(_printGesture) printf("AS3OpenNI :: Swipe Right - Velocity:%f, Angle:%f\n", fVelocity, fAngle);
	char cSwipeRight[10];
	sprintf(cSwipeRight,"swipe_right:%f",fVelocity,fAngle);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cSwipeRight);
}

void XN_CALLBACK_TYPE OnWave(void* cxt)
{
	if(_printGesture) printf("AS3OpenNI :: Wave Occured\n");
	if(_useSockets) sendToSocket(SESSION_SOCKET, WAVE);
}

void XN_CALLBACK_TYPE onPush(XnFloat fVelocity, XnFloat fAngle, void* cxt)
{
	if(_printGesture) printf("AS3OpenNI :: Push Occured - Velocity:%f, Angle:%f\n", fVelocity, fAngle);
	char cPush[10];
	sprintf(cPush,"push:%f:%f",fVelocity,fAngle);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cPush);
}

void XN_CALLBACK_TYPE Steady_OnSteady(XnFloat fVelocity, void* cxt)
{
	if(_printGesture) printf("AS3OpenNI :: Steady Occured - Velocity:%f\n", fVelocity);
	char cSteady[10];
	sprintf(cSteady,"steady:%f",fVelocity);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cSteady);
}

//-----------------------------------------------------------------------------
// Left/Right Slider Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE LeftRightSlider_OnValueChange(XnFloat fValue, void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - LeftRight - Value:%f\n", fValue);
	char cValue[50];
	sprintf(cValue,"slider_leftright_value:%f",fValue);
	if(_useSockets) sendToSocket(SLIDER_SOCKET, cValue);
}

void XN_CALLBACK_TYPE LeftRightSlider_OnActivate(void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - LeftRight - Activated\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_LEFTRIGHT_ACTIVATED);
}

void XN_CALLBACK_TYPE LeftRightSlider_OnDeactivate(void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - LeftRight - Deactivated\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_LEFTRIGHT_DEACTIVATED);
}

void XN_CALLBACK_TYPE LeftRightSlider_OnPrimaryCreate(const XnVHandPointContext* hand, const XnPoint3D& ptFocus, void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - LeftRight - Created\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_LEFTRIGHT_CREATED);
}

void XN_CALLBACK_TYPE LeftRightSlider_OnPrimaryDestroy(XnUInt32 nID, void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - LeftRight - Destroyed\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_LEFTRIGHT_DESTROYED);
}

//-----------------------------------------------------------------------------
// Up/Down Slider Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE UpDownSlider_OnValueChange(XnFloat fValue, void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - UpDown - Value:%f\n", fValue);
	char cValue[50];
	sprintf(cValue,"slider_updown_value:%f",fValue);
	if(_useSockets) sendToSocket(SLIDER_SOCKET, cValue);
}

void XN_CALLBACK_TYPE UpDownSlider_OnActivate(void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - UpDown - Activated\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_UPDOWN_ACTIVATED);
}

void XN_CALLBACK_TYPE UpDownSlider_OnDeactivate(void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - UpDown - Deactivated\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_UPDOWN_DEACTIVATED);
}

void XN_CALLBACK_TYPE UpDownSlider_OnPrimaryCreate(const XnVHandPointContext* hand, const XnPoint3D& ptFocus, void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - UpDown - Created\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_UPDOWN_CREATED);
}

void XN_CALLBACK_TYPE UpDownSlider_OnPrimaryDestroy(XnUInt32 nID, void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - UpDown - Destroyed\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_UPDOWN_DESTROYED);
}

//-----------------------------------------------------------------------------
// In/Out Slider Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE InOutSlider_OnValueChange(XnFloat fValue, void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - InOut - Value:%f\n", fValue);
	char cValue[50];
	sprintf(cValue,"slider_inout_value:%f",fValue);
	if(_useSockets) sendToSocket(SLIDER_SOCKET, cValue);
}

void XN_CALLBACK_TYPE InOutSlider_OnActivate(void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - InOut - Activated\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_INOUT_ACTIVATED);
}

void XN_CALLBACK_TYPE InOutSlider_OnDeactivate(void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - InOut - Deactivated\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_INOUT_DEACTIVATED);
}

void XN_CALLBACK_TYPE InOutSlider_OnPrimaryCreate(const XnVHandPointContext* hand, const XnPoint3D& ptFocus, void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - InOut - Created\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_INOUT_CREATED);
}

void XN_CALLBACK_TYPE InOutSlider_OnPrimaryDestroy(XnUInt32 nID, void* cxt)
{
	if(_printSlider) printf("AS3OpenNI :: Slider - InOut - Destroyed\n");
	if(_useSockets) sendToSocket(SLIDER_SOCKET, SLIDER_INOUT_DESTROYED);
}

//-----------------------------------------------------------------------------
// TrackPad Events
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE TrackPad_ItemHover(XnInt32 nXItem, XnInt32 nYItem, void* cxt)
{
  	if(_printTrackPad) printf("AS3OpenNI :: TrackPad Hover: %d,%d\n", nXItem, nYItem);
	char cValue[50];
	sprintf(cValue,"trackpad_hover:%d:%d", nXItem, nYItem);
	if(_useSockets) sendToSocket(SESSION_SOCKET, cValue);
}

void XN_CALLBACK_TYPE TrackPad_ItemSelect(XnInt32 nXItem, XnInt32 nYItem, XnVDirection eDir, void* cxt)
{
	if(_printTrackPad) printf("AS3OpenNI :: TrackPad Select: %d,%d (%s)\n", nXItem, nYItem, XnVDirectionAsString(eDir));
	char cValue[50];
	sprintf(cValue,"trackpad_hover:%d:%d:%s", nXItem, nYItem, XnVDirectionAsString(eDir));
	if(_useSockets) sendToSocket(SESSION_SOCKET, cValue);
}

void XN_CALLBACK_TYPE TrackPad_PrimaryCreate(const XnVHandPointContext* cxt, const XnPoint3D& ptFocus, void* UserCxt)
{
  	if(_printTrackPad)
  	{
  		printf("AS3OpenNI :: TrackPad Input Started, Point ID: [%d] ", cxt->nID);
  		printf("AS3OpenNI :: Starting Point Position: [%f],[%f],[%f]\n", cxt->ptPosition.X, cxt->ptPosition.Y, cxt->ptPosition.Z);
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
	if(_printSinglePoint) printf("AS3OpenNI :: id:%d,x:%f,y:%f,z:%f\n", pContext->nID, pContext->ptPosition.X, pContext->ptPosition.Y, pContext->ptPosition.Z);
	
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
	if(_printUserTracking) printf("AS3OpenNI :: New User: %d\n", nId);
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
	if(_useSockets) 
	{
		sendToSocket(USER_TRACKING_SOCKET, cValue);
		#if (XN_PLATFORM == XN_PLATFORM_WIN32)
			g_AS3Network.sendMessage(1,2,nId);
		#endif
	}
}

void XN_CALLBACK_TYPE User_LostUser(UserGenerator& generator, XnUserID nId, void* pCookie)
{
	if(_printUserTracking) printf("AS3OpenNI :: Lost user: %d\n", nId);
	
	char cValue[50];
	sprintf(cValue, "user_tracking_lost_user:%d", nId);
	if(_useSockets) 
	{
		sendToSocket(USER_TRACKING_SOCKET, cValue);
		#if (XN_PLATFORM == XN_PLATFORM_WIN32)
			g_AS3Network.sendMessage(1,3,nId);
		#endif
	}
}

void XN_CALLBACK_TYPE UserPose_PoseDetected(PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie)
{
	if(_printUserTracking) printf("AS3OpenNI :: Pose %s detected for user: %d\n", strPose, nId);
	_userGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
	_userGenerator.GetSkeletonCap().RequestCalibration(nId, true);
	
	char cValue[50];
	sprintf(cValue, "user_tracking_pose_detected:%d", nId);
	if(_useSockets) 
	{
		sendToSocket(USER_TRACKING_SOCKET, cValue);
		#if (XN_PLATFORM == XN_PLATFORM_WIN32)
			g_AS3Network.sendMessage(1,6,nId);
		#endif
	}
}

void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
	if(_printUserTracking) printf("AS3OpenNI :: Calibration started for user: %d\n", nId);
	
	char cValue[50];
	sprintf(cValue, "user_tracking_user_calibration_start:%d", nId);
	if(_useSockets) 
	{
		sendToSocket(USER_TRACKING_SOCKET, cValue);
		#if (XN_PLATFORM == XN_PLATFORM_WIN32)
			g_AS3Network.sendMessage(1,7,nId);
		#endif
	}
}

void XN_CALLBACK_TYPE UserCalibration_CalibrationEnd(SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie)
{
	if (bSuccess)
	{
		if(_printUserTracking) printf("AS3OpenNI :: Calibration complete, start tracking user: %d\n", nId);
		_userGenerator.GetSkeletonCap().StartTracking(nId);
		
		char cValue[50];
		sprintf(cValue, "user_tracking_user_calibration_complete:%d", nId);
		if(_useSockets) 
		{
			sendToSocket(USER_TRACKING_SOCKET, cValue);
			#if (XN_PLATFORM == XN_PLATFORM_WIN32)
				g_AS3Network.sendMessage(1,8,nId);
			#endif
		}
	}
	else
	{
		if(_printUserTracking) printf("AS3OpenNI :: Calibration failed for user: %d\n", nId);
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
		if(_useSockets) 
		{
			sendToSocket(USER_TRACKING_SOCKET, cValue);
			#if (XN_PLATFORM == XN_PLATFORM_WIN32)
				g_AS3Network.sendMessage(1,9,nId);
			#endif
		}
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
    
	// Setup the capture socket server for Mac.
	#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
		if(_featureDepthMapCapture || _featureRGBCapture)
		{
			if(_useSockets)
			{
				g_AS3Network = network();
				g_AS3Network.init(setupServer);
			}
		}
	#endif
	
	// Setup the status.
    XnStatus _status = XN_STATUS_OK;
    EnumerationErrors _errors;
    
    // Context Init and Add license.
	_status = _context.Init();
	CHECK_RC(_status, "AS3OpenNI :: Initialize context");
	_context.SetGlobalMirror(_mirror);
	
	XnChar vendor[XN_MAX_NAME_LENGTH];
	XnChar license[XN_MAX_LICENSE_LENGTH];

	_license.strVendor[XN_MAX_NAME_LENGTH] = strcmp(vendor, "PrimeSense");
	_license.strKey[XN_MAX_LICENSE_LENGTH] = strcmp(license, "0KOIk2JeIBYClPWVnMoRKn5cdY4=");
		
	_status = _context.AddLicense(_license);
   	CHECK_RC(_status, "AS3OpenNI :: Added license");
   	
   	// Set it to VGA maps at 30 FPS
	_depthMode.nXRes = 640;
	_depthMode.nYRes = 480;
	_depthMode.nFPS = 30;
	
	// Depth map create.
	_status = _depth.Create(_context);
	CHECK_RC(_status, "AS3OpenNI :: Create depth generator");
	_status = _depth.SetMapOutputMode(_depthMode);
	
	// Depth map create.
	_status = _image.Create(_context);
	CHECK_RC(_status, "AS3OpenNI :: Create image generator");
	_status = _image.SetMapOutputMode(_depthMode);
	_status = _image.SetPixelFormat(XN_PIXEL_FORMAT_RGB24);
	
	// Create the hands generator.
	_status = _hands.Create(_context);
	CHECK_RC(_status, "AS3OpenNI :: Create hands generator");
	_hands.SetSmoothing(0.1);

	// Create the gesture generator.
	_status = _gesture.Create(_context);
	CHECK_RC(_status, "AS3OpenNI :: Create gesture generator");
	
	// Create user generator.
	_status = _userGenerator.Create(_context);
	CHECK_RC(_status, "AS3OpenNI :: Find user generator");
	
	// Create and initialize point tracker
	_sessionManager = new XnVSessionManager();
	_status = _sessionManager->Initialize(&_context, "Wave", "RaiseHand");
	
	if (_status != XN_STATUS_OK)
	{
		printf("AS3OpenNI :: Couldn't initialize the Session Manager: %s\n", xnGetStatusString(_status));
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
			printf("AS3OpenNI :: Supplied user generator doesn't support skeleton\n");
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
				printf("AS3OpenNI :: Pose required, but not supported\n");
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
	CHECK_RC(_status, "AS3OpenNI :: FPS Init");
	
	//----------------------------------------------------------------------//
	//------------------------- SETUP DISPLAY SUPPORT ---------------------//
	//--------------------------------------------------------------------//
	
	// Setup depth and image data.
	_depth.GetMetaData(_depthData);
	_image.GetMetaData(_imageData);
	
	// Hybrid mode isn't supported in this sample
	if (_imageData.FullXRes() != _depthData.FullXRes() || _imageData.FullYRes() != _depthData.FullYRes())
	{
		printf ("AS3OpenNI :: The device depth and image resolution must be equal!\n");
		return 1;
	}

	// RGB is the only image format supported.
	if (_imageData.PixelFormat() != XN_PIXEL_FORMAT_RGB24)
	{
		printf("AS3OpenNI :: The device image format must be RGB24\n");
		return 1;
	}
	
	// Setup the view points to match between the depth and image maps.
	if(_snapPixels) _depth.GetAlternativeViewPointCap().SetViewPoint(_image);
	
	//-------------------------------------------------------------//
	//------------------------- MAIN LOOP ------------------------//
	//-----------------------------------------------------------//
	
	// Setup the capture socket server for PC.
	#if (XN_PLATFORM == XN_PLATFORM_WIN32)
		if(_featureDepthMapCapture || _featureRGBCapture)
		{
			if(_useSockets)
			{
				g_AS3Network = network();
				g_AS3Network.init(setupServer);
			}
		}
	#endif
	
	// Main loop
	while ((!_kbhit()) && (!_quit))
	{
		xnFPSMarkFrame(&xnFPS);
		_context.WaitAndUpdateAll();
		_sessionManager->Update(&_context);
		
		#if (XN_PLATFORM == XN_PLATFORM_WIN32)
			//if(_featureUserTracking) getPlayers();
			
			// Test this out first on PC, and see if you get the same results.
			// Known bug with this, is that it can crash quicker on the PC.
			// So using the getPlayers method doesn't crash.
			if(_featureUserTracking) renderSkeleton();
		#else
			if(_featureUserTracking) renderSkeleton();
		#endif
		
		if(_featureDepthMapCapture) captureDepthMap(g_ucDepthBuffer);
		if(_featureRGBCapture) captureRGB(g_ucImageBuffer);
	}
	
	CleanupExit();
}

