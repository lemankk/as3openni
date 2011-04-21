#include "capture.h"

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	#include "socket.h"
	extern SOCKET DEPTH_MAP_SOCKET, RGB_SOCKET;
#else
	#include <netdb.h>
	extern int DEPTH_MAP_SOCKET, RGB_SOCKET;
	extern struct sockaddr_in _depth_map_serv_addr;
#endif

#define MAX_BUFFER 4*640*480
#define MAX_DEPTH 10000

// Define vars.
unsigned char image_buffer[MAX_BUFFER];
float g_pDepthHist[MAX_DEPTH];
int _depthMapDelay = 0;
int _rgbDelay = 0;

// Setup the threading.
void* streamServer(void* arg);

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

FIMEMORY *depthMapMemory;
FIMEMORY *rgbMemory;

unsigned int getClosestPowerOfTwo(unsigned int n)
{
	unsigned int m = 2;
	while(m < n) m<<=1;
	return m;
}

unsigned int initTexture(void** buf, int& width, int& height)
{
	unsigned int texID = 0;
	width = getClosestPowerOfTwo(width);
	height = getClosestPowerOfTwo(height); 
	*buf = new unsigned char[width*height*4];
	return texID;
}

void sendDepthMapToSocket(BYTE *data, int size)
{
	pthread_mutex_lock(&dmapMutex);
	dmap_data_ready = 1;
	dmap_data_size = size;
	dmap_data = (char*)data;
	pthread_mutex_unlock(&dmapMutex);
}

void sendRGBToSocket(BYTE *data, int size)
{
	pthread_mutex_lock(&rgbMutex);
	rgb_data_ready = 1;
	rgb_data_size = size;
	rgb_data = (char*)data;
	pthread_mutex_unlock(&rgbMutex);
}

void captureDepthMapToMemory(unsigned char* buffer, int texWidth, int texHeight)
{	
	BYTE *mem_buffer = NULL;
	DWORD size_in_bytes = 0;
	
	// Define the image.
	FIBITMAP *img = FreeImage_ConvertFromRawBits(buffer, texWidth/1.6, ((texHeight/1.06666667)+1), 4*texWidth, 32, 0, 0, 0, true);
	FIBITMAP *img2 = FreeImage_ConvertTo24Bits(img);
	FreeImage_Unload(img);
	
	// Save to memory by quality type.
	if(depthmap_quality == 0)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img2, depthMapMemory, JPEG_QUALITYBAD);
	}
	
	if(depthmap_quality == 1)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img2, depthMapMemory, JPEG_QUALITYAVERAGE);
	}
	
	if(depthmap_quality == 2)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img2, depthMapMemory, JPEG_QUALITYNORMAL);
	}
	
	if(depthmap_quality >= 3)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img2, depthMapMemory, JPEG_DEFAULT);
	}
	
	// Unload the file.
	FreeImage_Unload(img2);
	
	// Wait for delay.
	while(_depthMapDelay > 1) 
	{
		// Get the buffer from the memory stream.
		FreeImage_SeekMemory(depthMapMemory, 0L, SEEK_SET); 
		FreeImage_AcquireMemory(depthMapMemory, &mem_buffer, &size_in_bytes);
		
		// Send to the socket.
		sendDepthMapToSocket(mem_buffer, size_in_bytes);
		
		// For debugging.
		if(_printDepthMapCapture) 
		{
			printf("DepthMap Size:%d\n", size_in_bytes);
			FILE *stream = fopen("depthmap_buffer.jpg", "wb"); 
			if(stream) 
			{
				fwrite(mem_buffer, sizeof(BYTE), size_in_bytes, stream); 
				fclose(stream);
			}
		}
		
		// Reset delay.
		_depthMapDelay = 0;
	}
	
	// Inc.
	_depthMapDelay++;
}

void captureRGBToMemory()
{	
	BYTE *mem_buffer = NULL;
	DWORD size_in_bytes = 0;
	
	// Define the image.
	FIBITMAP *img = FreeImage_ConvertFromRawBits((BYTE*)image_buffer, GL_WIN_SIZE_X, GL_WIN_SIZE_Y, 4*GL_WIN_SIZE_X, 32, 0, 0, 0, true);
	FIBITMAP *img2 = FreeImage_ConvertTo24Bits(img);
	FreeImage_Unload(img);
	
	// Change to greyscale if requested.
	if(_rgbGoGrey)
	{
		FIBITMAP *tmp = FreeImage_ConvertToGreyscale(img2);
		FreeImage_Unload(img2);
		img2 = tmp;
	}
	
	// Save to memory by quality type.
	if(rgb_quality == 0)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img2, rgbMemory, JPEG_QUALITYBAD);
	}
	
	if(rgb_quality == 1)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img2, rgbMemory, JPEG_QUALITYAVERAGE);
	}
	
	if(rgb_quality == 2)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img2, rgbMemory, JPEG_QUALITYNORMAL);
	}
	
	if(rgb_quality >= 3)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img2, rgbMemory, JPEG_DEFAULT);
	}
	
	// Unload the file.
	FreeImage_Unload(img2);
	
	// Wait for delay.
	while(_rgbDelay > 1) 
	{
		// Get the buffer from the memory stream.
		FreeImage_SeekMemory(rgbMemory, 0L, SEEK_SET);
		FreeImage_AcquireMemory(rgbMemory, &mem_buffer, &size_in_bytes);
		
		// Send to the socket.
		sendRGBToSocket(mem_buffer, size_in_bytes);
		
		// For debugging.
		if(_printRGBCapture) 
		{
			printf("RGB Size:%d\n", size_in_bytes);
			FILE *stream = fopen("img_buffer.jpg", "wb"); 
			if(stream) 
			{
				fwrite(mem_buffer, sizeof(BYTE), size_in_bytes, stream); 
				fclose(stream);
			}
		}
		
		// Reset delay.
		_rgbDelay = 0;
	}
	
	// Inc.
	_rgbDelay++;
}

void captureRGB()
{
	unsigned int nX = 0;
	unsigned int nY = 0;
	XnUInt16 g_nXRes = _imageData.XRes();
	XnUInt16 g_nYRes = _imageData.YRes();
	
	const XnRGB24Pixel * pImageMap = _image.GetRGB24ImageMap();
	for (nY=0; nY<g_nYRes; nY++) 
	{
		for (nX=0; nX < g_nXRes; nX++) 
		{
			((unsigned char*)image_buffer)[(nY*g_nXRes+nX)*4+0] = pImageMap[nY*g_nXRes+nX].nBlue;
			((unsigned char*)image_buffer)[(nY*g_nXRes+nX)*4+1] = pImageMap[nY*g_nXRes+nX].nGreen;
	        ((unsigned char*)image_buffer)[(nY*g_nXRes+nX)*4+2] = pImageMap[nY*g_nXRes+nX].nRed;
			((unsigned char*)image_buffer)[(nY*g_nXRes+nX)*4+3] = 0x00;
		}
	}
	
	rgbMemory = FreeImage_OpenMemory();
	captureRGBToMemory();
	FreeImage_CloseMemory(rgbMemory);
}

void captureDepthMap()
{
	_depth.GetMetaData(_depthData);
	_userGenerator.GetUserPixels(0, _sceneData);
	
	static unsigned int depthTexID;
	static bool bInitialized = false;
	static unsigned char* pDepthTexBuf;
	static int texWidth, texHeight;
	
	if(!bInitialized)
	{
		texWidth = getClosestPowerOfTwo(_depthData.XRes());
		texHeight = getClosestPowerOfTwo(_depthData.YRes());
		//printf("Initializing depth texture: width = %d, height = %d\n", texWidth, texHeight);
		depthTexID = initTexture((void**)&pDepthTexBuf,texWidth, texHeight);
		bInitialized = true;
	}
	
	unsigned int nValue = 0;
	unsigned int nHistValue = 0;
	unsigned int nIndex = 0;
	unsigned int nX = 0;
	unsigned int nY = 0;
	unsigned int nNumberOfPoints = 0;
	XnUInt16 g_nXRes = _depthData.XRes();
	XnUInt16 g_nYRes = _depthData.YRes();

	unsigned char* pDestImage = pDepthTexBuf;
	const XnDepthPixel* pDepth = _depthData.Data();
	const XnLabel* pLabels = _sceneData.Data();
	
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

	pDepth = _depthData.Data();
	{
		XnUInt32 nIndex = 0;
		
		// Prepare the texture map.
		for (nY=0; nY<g_nYRes; nY++)
		{
			for (nX=0; nX < g_nXRes; nX++, nIndex++)
			{
				pDestImage[0] = 0;
				pDestImage[1] = 0;
				pDestImage[2] = 0;
				pDestImage[3] = 0x00;
				
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
						pDestImage[0] = nHistValue * Colors[nColorID][0]; 
						pDestImage[1] = nHistValue * Colors[nColorID][1];
						pDestImage[2] = nHistValue * Colors[nColorID][2];
						pDestImage[3] = 0xFF;
					}
				}

				pDepth++;
				pLabels++;
				pDestImage+=4;
			}

			pDestImage += (texWidth - g_nXRes) *4;
		}
	}
	
	depthMapMemory = FreeImage_OpenMemory();
	captureDepthMapToMemory(pDepthTexBuf, texWidth, texHeight);
	FreeImage_CloseMemory(depthMapMemory);
}
