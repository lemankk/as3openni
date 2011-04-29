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

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	#include <pthread/pthread.h>
#else
	#include <pthread.h>
#endif

#include "network.h"
#include "params.h"
#include "capture.h"

//---------------------------------------------------------------------------
// Namespaces
//---------------------------------------------------------------------------
using namespace xn;

//---------------------------------------------------------------------------
// Definitions
//---------------------------------------------------------------------------
#define MAX_DEPTH 10000

//---------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------
Context g_Context;
DepthGenerator g_DepthGenerator;
ImageGenerator g_ImageGenerator;
UserGenerator g_UserGenerator;
XnLicense g_License;
XnMapOutputMode g_DepthMode;
DepthMetaData g_DepthData;
ImageMetaData g_ImageData;

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

pthread_t g_ServerThread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

network g_AS3Network;
capture g_Capture;

int g_Exit = 0;
int g_Connected = 0;

XnFPSData xnFPS;
int g_intTrackpadColumns = NULL, g_intTrackpadRows = NULL;

void playServerThread();
void pauseServerThread();

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
	g_AS3Network.sendMessage(1,3,nId);
}

void XN_CALLBACK_TYPE User_LostUser(UserGenerator& generator, XnUserID nId, void* pCookie)
{
	printf("AS3OpenNI-Bridge :: Lost user: %d\n", nId);
	g_AS3Network.sendMessage(1,4,nId);
}

//---------------------------------------------------------------------------
// Private Methods
//---------------------------------------------------------------------------
void CleanupExit()
{
	pthread_mutex_destroy(&mutex);
	g_Context.Shutdown();
	g_AS3Network.closeConnection();
	g_Connected = 0;
	exit(1);
}

void pauseServerThread()
{
	pthread_mutex_lock(&mutex);
	g_Connected = 0;
	pthread_mutex_unlock(&mutex);
}

void playServerThread()
{
	pthread_mutex_lock(&mutex);
	g_Connected = 1;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
}

void *serverData(void *arg) 
{
	pthread_mutex_lock(&mutex);
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
								g_AS3Network.sendMessage(1,0,g_ucDepthBuffer,sizeof(g_ucDepthBuffer));
							break;
								
							case 1: // GET RGB
								g_AS3Network.sendMessage(1,1,g_ucImageBuffer,sizeof(g_ucImageBuffer));
							break;
								
							case 2: // GET SKEL
								// Abstract for now.
							break;
						}
					break;
				}
			}
		}
	}
	pthread_mutex_unlock(&mutex);
	return 0;
}

void setupServer() 
{
	printf("AS3OpenNI-Bridge :: Server Connected\n");
	if (pthread_create(&g_ServerThread, NULL, &serverData, NULL)) 
	{
		fprintf(stderr, "AS3OpenNI-Bridge :: Error on pthread_create() for the server\n");
	}
	g_AS3Network.sendMessage(0,0,0);

	// Start thread.
	playServerThread();
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
	if(g_bUseSockets) g_AS3Network.sendMessage(0,1,0);
	
	while(!g_Exit)
	{
		g_Context.WaitAndUpdateAll();
		xnFPSMarkFrame(&xnFPS);
		if(g_bFeatureDepthMapCapture) g_Capture.getDepthMap(g_ucDepthBuffer);
		if(g_bFeatureRGBCapture) g_Capture.getRGB(g_ucImageBuffer);
	}
	
	CleanupExit();
}