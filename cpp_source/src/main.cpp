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
#include <termios.h>
#include <unistd.h>

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	#include <pthread/pthread.h>
#else
	#include <pthread.h>
#endif

#include "network.h"

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

XnBool g_bDrawBackground = FALSE;
XnBool g_bDrawPixels = TRUE;
XnBool g_bSnapPixels = TRUE;
XnBool g_bMirror = TRUE;

unsigned char g_ucDepthBuffer[4*640*480];
unsigned char g_ucImageBuffer[4*640*480];

float g_pDepthHist[MAX_DEPTH];
pthread_t g_ServerThread;
network g_AS3Network;

int g_Exit = 0;
int g_Connected = 0;
int g_Char = 0;

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

//-----------------------------------------------------------------------------
// Utility Methods
//-----------------------------------------------------------------------------
int mygetch() 
{
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

//-----------------------------------------------------------------------------
// Error Handling
//-----------------------------------------------------------------------------

#define CHECK_RC(g_Status, what)									\
if (g_Status != XN_STATUS_OK)										\
{																	\
	printf("AS3OpenNI-Bridge :: %s failed: %s\n", what, xnGetStatusString(g_Status));	\
	return g_Status;												\
}

//---------------------------------------------------------------------------
// Private Methods
//---------------------------------------------------------------------------
void CleanupExit()
{
	//g_Context.Shutdown();
	g_AS3Network.closeConnection();
	g_Char = 0;
	g_Exit = 1;
	g_Connected = 0;
	exit(1);
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
	while(g_Connected && !g_Exit && g_Char != 'q')
	{
		printf("AS3OpenNI-Bridge :: Thread Running\n");
	}
}

void setupServer() 
{
	printf("AS3OpenNI-Bridge :: Server Connected\n");
	g_Connected = 1;
	if (pthread_create(&g_ServerThread, NULL, &serverData, NULL)) 
	{
		fprintf(stderr, "AS3OpenNI-Bridge :: Error on pthread_create() for the server\n");
	}
	g_AS3Network.sendMessage("AS3OpenNI-Bridge :: Server Connected\n");
}

int main(int argc, char **argv)
{
	// Setup the status.
    XnStatus g_Status = XN_STATUS_OK;
    
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
	
	// Start generating all.
	g_Context.StartGeneratingAll();
	
	// Set the frame rate.
	g_Status = xnFPSInit(&xnFPS, 180);
	CHECK_RC(g_Status, "FPS Init");
	
	// Setup the view points to match between the depth and image maps.
	g_DepthGenerator.GetAlternativeViewPointCap().SetViewPoint(g_ImageGenerator);
	
	// Setup the socket server.
	g_AS3Network = network();
	g_AS3Network.init(setupServer);
	
	// Print out any messages for the user.
	printf("AS3OpenNI-Bridge :: Press 'q' to quit this application\n");
	
	while(!g_Exit && g_Char != 'q')
	{
		g_Char = mygetch();
		g_Context.WaitAndUpdateAll();
		xnFPSMarkFrame(&xnFPS);
		getDepthMap(g_ucDepthBuffer);
		getRGB(g_ucImageBuffer);
	}
	
	CleanupExit();
}