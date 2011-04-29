#include "capture.h"

#define MAX_DEPTH 10000
float g_pDepthHist[MAX_DEPTH];

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

void capture::getRGB(unsigned char* g_ucImageBuffer)
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

void capture::getDepthMap(unsigned char* g_ucDepthBuffer)
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