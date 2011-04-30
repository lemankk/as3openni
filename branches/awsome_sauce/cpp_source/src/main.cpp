/****************************************************************************
*                                                                           *
*   Project:	AS3OpenNI-Bridge	                                        *
*   Version:	Beta 1.0.0                                                  *
*   Author:     Tony Birleffi												*
*   URL: 		http://code.google.com/p/as3openni-bridge/					*
*   License: 	GNU GPL v2.0												*
*                                                                           *
****************************************************************************/

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------
#include <XnTypes.h>
#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
#include <XnFPSCalculator.h>
#include <XnOS.h>

#include <stdio.h>
#include <string>
#include <iostream>

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	#include <pthread/pthread.h>
#else
	#include <pthread.h>
#endif

#include "network.h"
#include "params.h"

//---------------------------------------------------------------------------
// Namespaces
//---------------------------------------------------------------------------
using namespace xn;
using namespace std;

//---------------------------------------------------------------------------
// Definitions
//---------------------------------------------------------------------------
#define MAX_DEPTH 10000
#define SKEL_FORMAT "user_tracking:%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f||"
#define COM_FORMAT "user_found:%d,%f,%f,%f||"
#define MAX_USERS 15

//---------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------
Context g_Context;
DepthGenerator g_DepthGenerator;
ImageGenerator g_ImageGenerator;
UserGenerator g_UserGenerator;
SceneMetaData g_SceneData;
XnLicense g_License;
XnMapOutputMode g_DepthMode;

XnBool g_bDrawBackground = FALSE;
XnBool g_bDrawPixels = TRUE;
XnBool g_bSnapPixels = TRUE;
XnBool g_bMirror = TRUE;
XnBool g_bUseSockets = TRUE;

XnBool g_bFeatureUserTracking = FALSE;
XnBool g_bFeatureRGBCapture = FALSE;
XnBool g_bFeatureDepthMapCapture = FALSE;

XnBool g_bNeedPose = false;
XnChar g_sPose[20] = "";

unsigned char g_ucDepthBuffer[4*640*480];
unsigned char g_ucImageBuffer[4*640*480];
unsigned char g_ucUsersBuffer[8*MAX_USERS*375];

float g_pDepthHist[MAX_DEPTH];
pthread_t g_ServerThread;
pthread_t g_FeaturesThread;
network g_AS3Network;

int g_Exit = 0;
int g_Connected = 0;

XnFPSData xnFPS;
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
int g_intTrackpadColumns = NULL, g_intTrackpadRows = NULL;

struct NIPlayers
{
	string *data;
};
NIPlayers *g_niPlayers;

//-----------------------------------------------------------------------------
// Error Handling
//-----------------------------------------------------------------------------

#define CHECK_RC(g_Status, what)									\
if (g_Status != XN_STATUS_OK)										\
{																	\
	printf("AS3OpenNI-Bridge :: %s failed: %s\n", what, xnGetStatusString(g_Status));	\
	return g_Status;												\
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
// Event Handlers
//-----------------------------------------------------------------------------

void XN_CALLBACK_TYPE User_NewUser(UserGenerator& generator, XnUserID nId, void* pCookie)
{
	printf("AS3OpenNI-Bridge :: New User: %d\n", nId);
	//if(g_bNeedPose) g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_sPose, nId);
	//else g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, true);
}

void XN_CALLBACK_TYPE User_LostUser(UserGenerator& generator, XnUserID nId, void* pCookie)
{
	printf("AS3OpenNI-Bridge :: Lost user: %d\n", nId);
}

//---------------------------------------------------------------------------
// Private Methods
//---------------------------------------------------------------------------
void CleanupExit()
{
	g_Context.Shutdown();
	g_AS3Network.closeConnection();
	g_Connected = 0;
	exit(1);
}

void renderSkeleton()
{	
	g_niPlayers = (NIPlayers*)malloc(sizeof(NIPlayers)*MAX_USERS*375);
	g_niPlayers->data = new string();
	
	XnUserID aUsers[MAX_USERS];
	XnUInt16 nUsers = MAX_USERS;
	g_UserGenerator.GetUsers(aUsers, nUsers);
	
	for (int i = 0; i < nUsers; ++i)
	{
		char * playerData;
		int playerLength;
		XnUserID player;
		XnPoint3D com;
		player = aUsers[i];
		g_UserGenerator.GetCoM(player, com);
		
		// If a user is being tracked then do this.
		if(g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i]))
		{
			XnSkeletonJointPosition head, neck, left_shoulder, left_elbow, left_hand, right_shoulder, right_elbow, right_hand;
			XnSkeletonJointPosition torso, left_hip, left_knee, left_foot, right_hip, right_knee, right_foot;
			XnSkeletonJointPosition left_big_hand, right_big_hand;
			
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_HEAD, head);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_NECK, neck);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_TORSO, torso);
			
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_SHOULDER, left_shoulder);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_ELBOW, left_elbow);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_HAND, left_hand);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_HAND, left_big_hand);
			
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_SHOULDER, right_shoulder);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_ELBOW, right_elbow);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_HAND, right_hand);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_HAND, right_big_hand);
		
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_HIP, left_hip);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_KNEE, left_knee);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_FOOT, left_foot);
			
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_HIP, right_hip);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_KNEE, right_knee);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_FOOT, right_foot);
			
			#if (XN_PLATFORM == XN_PLATFORM_WIN32)
				playerLength = _snprintf
				(
					NULL, 0, SKEL_FORMAT, player,
					head.position.X, head.position.Y, head.position.Z, 
					neck.position.X, neck.position.Y, neck.position.Z,
					torso.position.X, torso.position.Y, torso.position.Z,
					
					left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
					left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
					left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
					
					right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
					right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
					right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
					
					left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
					left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
					left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
					
					right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
					right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
					right_foot.position.X, right_foot.position.Y, right_foot.position.Z
				);
			#else
				playerLength = snprintf
				(
					NULL, 0, SKEL_FORMAT, player,
					head.position.X, head.position.Y, head.position.Z, 
					neck.position.X, neck.position.Y, neck.position.Z,
					torso.position.X, torso.position.Y, torso.position.Z,
					
					left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
					left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
					left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
					
					right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
					right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
					right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
					
					left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
					left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
					left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
					
					right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
					right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
					right_foot.position.X, right_foot.position.Y, right_foot.position.Z
				);
			#endif
			
			// Character object that will store the string.
			playerData = (char*) malloc((playerLength + 1) * sizeof(char));
			
			// Print string in to format.
			#if (XN_PLATFORM == XN_PLATFORM_WIN32)
				_snprintf
				(
					playerData, playerLength, SKEL_FORMAT, player,
					head.position.X, head.position.Y, head.position.Z, 
					neck.position.X, neck.position.Y, neck.position.Z,
					torso.position.X, torso.position.Y, torso.position.Z,
					
					left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
					left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
					left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
					
					right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
					right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
					right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
					
					left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
					left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
					left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
					
					right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
					right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
					right_foot.position.X, right_foot.position.Y, right_foot.position.Z
				);
			#else
				snprintf
				(
					playerData, playerLength, SKEL_FORMAT, player,
					head.position.X, head.position.Y, head.position.Z, 
					neck.position.X, neck.position.Y, neck.position.Z,
					torso.position.X, torso.position.Y, torso.position.Z,
					
					left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
					left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
					left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
					
					right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
					right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
					right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
					
					left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
					left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
					left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
					
					right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
					right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
					right_foot.position.X, right_foot.position.Y, right_foot.position.Z
				);
			#endif
		}
		// Else, just track the user's center point.
		else
		{
			#if (XN_PLATFORM == XN_PLATFORM_WIN32)
				playerLength = _snprintf(NULL, 0, COM_FORMAT, player, com.X, com.Y, com.Z);
			#else
				playerLength = snprintf(NULL, 0, COM_FORMAT, player, com.X, com.Y, com.Z);
			#endif
			
			// Character object that will store the string.
			playerData = (char*) malloc((playerLength + 1) * sizeof(char));
			
			#if (XN_PLATFORM == XN_PLATFORM_WIN32)
				_snprintf(playerData, playerLength, COM_FORMAT, player, com.X, com.Y, com.Z);
			#else
				snprintf(playerData, playerLength, COM_FORMAT, player, com.X, com.Y, com.Z);
			#endif
		}
		
		// Copy each result over to the buffer.
		g_niPlayers->data->append(playerData);
		
		// Free each player's data memory block.
		free(playerData);
	}
	
	int len = sizeof(NIPlayers)*MAX_USERS*375;
	memcpy(g_ucUsersBuffer, (unsigned char*)g_niPlayers->data->c_str(), len);
	g_niPlayers->data->clear();
	free(g_niPlayers);
}

void getDepthMap(unsigned char* g_ucDepthBuffer)
{
	SceneMetaData smd;
	DepthMetaData dmd;
	g_DepthGenerator.GetMetaData(dmd);

	//printf("AS3OpenNI-Bridge :: Frame %d Middle point is: %u. FPS: %f\n", dmd.FrameID(), dmd(dmd.XRes() / 2, dmd.YRes() / 2), xnFPSCalc(&xnFPS));

	g_DepthGenerator.GetMetaData(dmd);
	g_UserGenerator.GetUserPixels(0, smd);
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
	if (g_bDrawPixels)
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
				if (g_bDrawBackground || *pLabels != 0)
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

void getRGB(unsigned char* g_ucImageBuffer)
{
	ImageMetaData imd;
	g_ImageGenerator.GetMetaData(imd);

	unsigned int nValue = 0;
	unsigned int nX = 0;
	unsigned int nY = 0;
	XnUInt16 g_nXRes = imd.XRes();
	XnUInt16 g_nYRes = imd.YRes();

	const XnRGB24Pixel * pImageMap = g_ImageGenerator.GetRGB24ImageMap();
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
	printf("AS3OpenNI-Bridge :: Server Running\n");
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
								if(g_bFeatureDepthMapCapture) g_AS3Network.sendMessage(1,0,g_ucDepthBuffer,sizeof(g_ucDepthBuffer));
							break;
							
							case 1: // GET RGB
								if(g_bFeatureRGBCapture) g_AS3Network.sendMessage(1,1,g_ucImageBuffer,sizeof(g_ucImageBuffer));
							break;
							
							case 2: // GET USERS
								if(g_bFeatureUserTracking)
								{
									g_AS3Network.sendMessage(1,2,g_ucUsersBuffer,sizeof(g_ucUsersBuffer));
								}
							break;
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
	printf("AS3OpenNI-Bridge :: Server Connected\n");
	g_Connected = 1;
	if (pthread_create(&g_ServerThread, NULL, &serverData, NULL)) 
	{
		fprintf(stderr, "AS3OpenNI-Bridge :: Error on pthread_create() for the server\n");
	}
	g_AS3Network.sendMessage(0,0,0);
}

int main(int argc, char *argv[])
{
	printf("AS3OpenNI-Bridge :: Version: Beta v1.0.0\n");
	
	// Setup the command line parameters.
	setupParams(argc, argv);
	
	// Setup the socket server.
	if(g_bUseSockets)
	{
		g_AS3Network = network();
		g_AS3Network.init(setupServer);
	}
	
	// Setup the status.
    XnStatus g_Status = XN_STATUS_OK;
    EnumerationErrors _errors;
    
    // Context Init and Add license.
    g_Status = g_Context.Init();
	CHECK_RC(g_Status, "Initialize context");
	g_Context.SetGlobalMirror(g_bMirror);
	
	// Set the license up.
	XnChar vendor[XN_MAX_NAME_LENGTH];
	XnChar license[XN_MAX_LICENSE_LENGTH];
	g_License.strVendor[XN_MAX_NAME_LENGTH] = strcmp(vendor, "PrimeSense");
	g_License.strKey[XN_MAX_LICENSE_LENGTH] = strcmp(license, "0KOIk2JeIBYClPWVnMoRKn5cdY4=");
	
	g_Status = g_Context.AddLicense(g_License);
   	CHECK_RC(g_Status, "Added license");
   	
   	// Set it to VGA maps at 30 FPS
	g_DepthMode.nXRes = 640;
	g_DepthMode.nYRes = 480;
	g_DepthMode.nFPS = 30;
	
	// Depth map create.
	g_Status = g_DepthGenerator.Create(g_Context);
	CHECK_RC(g_Status, "Create depth generator");
	g_Status = g_DepthGenerator.SetMapOutputMode(g_DepthMode);
	
	// Depth map create.
	g_Status = g_ImageGenerator.Create(g_Context);
	CHECK_RC(g_Status, "Create image generator");
	g_Status = g_ImageGenerator.SetMapOutputMode(g_DepthMode);
	g_Status = g_ImageGenerator.SetPixelFormat(XN_PIXEL_FORMAT_RGB24);
	
	// Setup the view points to match between the depth and image maps.
	g_DepthGenerator.GetAlternativeViewPointCap().SetViewPoint(g_ImageGenerator);
	
	// Create user generator.
	g_Status = g_UserGenerator.Create(g_Context);
	CHECK_RC(g_Status, "Find user generator");
	
	// Feature User Tracking.
	if(g_bFeatureUserTracking)
	{
		// Setup user generator callbacks.
		XnCallbackHandle hUserCallbacks, hCalibrationCallbacks, hPoseCallbacks;
		g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
	}
	
	// Start generating all.
	g_Context.StartGeneratingAll();
	
	// Set the frame rate.
	g_Status = xnFPSInit(&xnFPS, 180);
	CHECK_RC(g_Status, "FPS Init");
	
	// Let Flash know the server is ready.
	g_AS3Network.sendMessage(0,1,0);
	
	while(!g_Exit)
	{
		g_Context.WaitAndUpdateAll();
		xnFPSMarkFrame(&xnFPS);
		if(g_bFeatureDepthMapCapture) getDepthMap(g_ucDepthBuffer);
		if(g_bFeatureRGBCapture) getRGB(g_ucImageBuffer);
		if(g_bFeatureUserTracking) renderSkeleton();
	}
	
	CleanupExit();
}