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
#include <XnVSessionManager.h>
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
#include "skeleton.h"

//---------------------------------------------------------------------------
// Namespaces
//---------------------------------------------------------------------------
using namespace xn;
using namespace std;

//---------------------------------------------------------------------------
// Definitions
//---------------------------------------------------------------------------
#define MAX_DEPTH 10000
#define SKEL_FORMAT "user_tracking:%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f"
#define COM_FORMAT "user_found:%d,%f,%f,%f"
#define MAX_USERS 15

//---------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------
Context g_Context;
HandsGenerator g_Hands;
GestureGenerator g_Gesture;
DepthGenerator g_DepthGenerator;
ImageGenerator g_ImageGenerator;
UserGenerator g_UserGenerator;
SceneMetaData g_SceneData;
XnLicense g_License;
XnMapOutputMode g_DepthMode;
XnVSessionManager* g_SessionManager;

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
skeleton g_ucUsersBuffer[MAX_USERS];

float g_pDepthHist[MAX_DEPTH];
pthread_t g_ServerThread;
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
	g_ucUsersBuffer[nId-1] = skeleton();

	//if(g_bNeedPose) g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_sPose, nId);
	//else g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, true);
}

void XN_CALLBACK_TYPE User_LostUser(UserGenerator& generator, XnUserID nId, void* pCookie)
{
	printf("AS3OpenNI-Bridge :: Lost user: %d\n", nId);
	//g_ucUsersBuffer[nId-1].~skeleton(); // Causes it to crash, not sure why yet.
}

//---------------------------------------------------------------------------
// Private Methods
//---------------------------------------------------------------------------
void CleanupExit()
{
	if (NULL != g_SessionManager)
	{
		delete g_SessionManager;
		g_SessionManager = NULL;
	}

	g_Context.Shutdown();
	g_AS3Network.closeConnection();
	g_Connected = 0;
	exit(1);
}

void renderSkeleton()
{	
	XnUserID aUsers[MAX_USERS];
	XnUInt16 nUsers = MAX_USERS;
	g_UserGenerator.GetUsers(aUsers, nUsers);
	
	for (int i = 0; i < nUsers; ++i)
	{
		XnPoint3D com;
		int player = aUsers[i];
		int index = player-1;
		g_UserGenerator.GetCoM(player, com);
		memcpy(g_ucUsersBuffer[index].user_id, &player, 4);
		
		// If a user is being tracked then do this.
		if(g_UserGenerator.GetSkeletonCap().IsTracking(player))
		{

		}
		// Else, just track the user's center point.
		else
		{
			int _flagId = 33;
			memcpy(g_ucUsersBuffer[index].flag, &_flagId, 4);

			float _x, _y, _z;
			unsigned char* dest = g_ucUsersBuffer[index].cpoint;
			_x = com.X;
			_y = com.Y;
			_z = com.Z;

			memcpy(dest, &_x, 4);
			memcpy(dest+4, &_y, 4);
			memcpy(dest+8, &_z, 4);
		}
	}
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
									for (int i = 0; i < MAX_USERS; ++i)
									{
										g_AS3Network.sendMessage(1,2,g_ucUsersBuffer[i].data,g_ucUsersBuffer[i].size);
									}
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

	// Create the hands generator.
	g_Status = g_Hands.Create(g_Context);
	CHECK_RC(g_Status, "Create hands generator");
	g_Hands.SetSmoothing(0.1);

	// Create the gesture generator.
	g_Status = g_Gesture.Create(g_Context);
	CHECK_RC(g_Status, "Create gesture generator");

	// Create and initialize point tracker
	g_SessionManager = new XnVSessionManager();
	g_Status = g_SessionManager->Initialize(&g_Context, "Wave", "RaiseHand");
	if (g_Status != XN_STATUS_OK)
	{
		printf("Couldn't initialize the Session Manager: %s\n", xnGetStatusString(g_Status));
		CleanupExit();
	}

	// Start generating all.
	g_Context.StartGeneratingAll();
	
	// Define the image and depth data.
	DepthMetaData dmd;
	ImageMetaData imd;
	g_DepthGenerator.GetMetaData(dmd);
	g_ImageGenerator.GetMetaData(imd);

	// Set the frame rate.
	g_Status = xnFPSInit(&xnFPS, 180);
	CHECK_RC(g_Status, "FPS Init");
	
	// Let Flash know the server is ready.
	g_AS3Network.sendMessage(0,1,0);
	
	while(!g_Exit)
	{
		xnFPSMarkFrame(&xnFPS);
		g_Context.WaitAndUpdateAll();
		g_SessionManager->Update(&g_Context);
		if(g_bFeatureDepthMapCapture) getDepthMap(g_ucDepthBuffer);
		if(g_bFeatureRGBCapture) getRGB(g_ucImageBuffer);
		if(g_bFeatureUserTracking) renderSkeleton();
	}
	
	CleanupExit();
}