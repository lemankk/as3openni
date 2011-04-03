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
XnRGB24Pixel* _depthMap = NULL;
XnRGB24Pixel* _videoMap = NULL;
unsigned int g_nTexMapX = 0;
unsigned int g_nTexMapY = 0;
float g_pDepthHist[MAX_DEPTH];
int _depthMapDelay = 0;
int _rgbDelay = 0;

FIMEMORY *depthMapMemory;
FIMEMORY *rgbMemory;

//INPLACESWAP adopted from codeguru.com 
template <class T> void INPLACESWAP(T& a, T& b) 
{
	a ^= b; b ^= a; a ^= b;
}

BOOL SwapRedBlue(FIBITMAP* dib) 
{
	if(FreeImage_GetImageType(dib) != FIT_BITMAP) 
	{
		return FALSE;
	}
		
	const unsigned bytesperpixel = FreeImage_GetBPP(dib) / 8;
	if(bytesperpixel > 4 || bytesperpixel < 3) 
	{
		return FALSE;
	}
		
	const unsigned height = FreeImage_GetHeight(dib);
	const unsigned pitch = FreeImage_GetPitch(dib);
	const unsigned lineSize = FreeImage_GetLine(dib);
	
	BYTE* line = FreeImage_GetBits(dib);
	for(unsigned y = 0; y < height; ++y, line += pitch) 
	{
		for(BYTE* pixel = line; pixel < line + lineSize ; pixel += bytesperpixel) 
		{
			INPLACESWAP(pixel[0], pixel[2]);
		}
	}
	
	return TRUE;
}

void setupCapture()
{	
	// Maintain the same pointers.
	if(_snapPixels) _depth.GetAlternativeViewPointCap().SetViewPoint(_image);
	
	// Texture map init.
	g_nTexMapX = (((unsigned short)(_depthData.FullXRes()-1) / 92) + 1) * 92;
	g_nTexMapY = (((unsigned short)(_depthData.FullYRes()-1) / 240) + 1) * 240;
	_depthMap = (XnRGB24Pixel*)malloc(g_nTexMapX * g_nTexMapY * sizeof(XnRGB24Pixel));
	_videoMap = (XnRGB24Pixel*)malloc(g_nTexMapX * g_nTexMapY * sizeof(XnRGB24Pixel));
}

void sendDepthMapToSocket(BYTE *data, int size)
{
	if(_useSockets)
	{
		if(_featureDepthMapCapture)
		{
			#if (XN_PLATFORM == XN_PLATFORM_WIN32)
				send(DEPTH_MAP_SOCKET, (char*)data, size, 0);
			#else
				write(DEPTH_MAP_SOCKET, (char*)data, size);
			#endif
		}
	}
}

void sendRGBToSocket(BYTE *data, int size)
{
	if(_useSockets)
	{
		if(_featureRGBCapture)
		{
			#if (XN_PLATFORM == XN_PLATFORM_WIN32)
				send(RGB_SOCKET, (char*)data, size, 0);
			#else
				write(RGB_SOCKET, (char*)data, size);
			#endif
		}
	}
}

void captureDepthMapToMemory()
{	
	BYTE *mem_buffer = NULL;
	DWORD size_in_bytes = 0;
	
	// Define the image.
	FIBITMAP *img = FreeImage_ConvertFromRawBits((BYTE*)_depthMap, g_nTexMapX, g_nTexMapY, 3*g_nTexMapX, 24, 0, 0, 0, true);
	
	// Save to memory by quality type.
	if(depthmap_quality == 0)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img, depthMapMemory, JPEG_QUALITYBAD);
	}
	
	if(depthmap_quality == 1)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img, depthMapMemory, JPEG_QUALITYAVERAGE);
	}
	
	if(depthmap_quality == 2)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img, depthMapMemory, JPEG_QUALITYNORMAL);
	}
	
	if(depthmap_quality == 3)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img, depthMapMemory, JPEG_DEFAULT);
	}
	
	// Unload the file.
	FreeImage_Unload(img);
	
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
	FIBITMAP *img = FreeImage_ConvertFromRawBits((BYTE*)_videoMap, g_nTexMapX, g_nTexMapY, 3*g_nTexMapX, 24, 0, 0, 0, true);
	
	// Swap the colors.
	SwapRedBlue(img);
	
	// Change to greyscale if requested.
	if(_rgbGoGrey)
	{
		FIBITMAP *tmp = FreeImage_ConvertToGreyscale(img);
		FreeImage_Unload(img);
		img = tmp;
	}
	
	// Save to memory by quality type.
	if(rgb_quality == 0)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img, rgbMemory, JPEG_QUALITYBAD);
	}
	
	if(rgb_quality == 1)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img, rgbMemory, JPEG_QUALITYAVERAGE);
	}
	
	if(rgb_quality == 2)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img, rgbMemory, JPEG_QUALITYNORMAL);
	}
	
	if(rgb_quality == 3)
	{
		FreeImage_SaveToMemory(FIF_JPEG, img, rgbMemory, JPEG_DEFAULT);
	}
	
	// Unload the file.
	FreeImage_Unload(img);
	
	// Wait for delay.
	while(_rgbDelay > 1) 
	{
		// Get the buffer from the memory stream.
		FreeImage_SeekMemory(rgbMemory, 0L, SEEK_SET);
		FreeImage_AcquireMemory(rgbMemory, &mem_buffer, &size_in_bytes);
		
		// Send to the socket.
		sendRGBToSocket(mem_buffer, size_in_bytes);
		
		// For debugging.
		if(_printRGBCapture) printf("RGB Size:%d\n", size_in_bytes);
		
		// Reset delay.
		_rgbDelay = 0;
	}
	
	// Inc.
	_rgbDelay++;
}

void definePixels()
{
	_depth.GetMetaData(_depthData);
	_image.GetMetaData(_imageData);
	
	//printf("Frame %d Middle point is: %u. FPS: %f\n", _depthData.FrameID(), _depthData(_depthData.XRes() / 2, _depthData.YRes() / 2), xnFPSCalc(&xnFPS));
	
	const XnDepthPixel* pDepth = _depthData.Data();
	const XnUInt8* pImage = _imageData.Data();
	unsigned int nImageScale = GL_WIN_SIZE_X / _depthData.FullXRes();
	xnOSMemSet(g_pDepthHist, 0, MAX_DEPTH*sizeof(float));
	
	unsigned int nNumberOfPoints = 0;
	for (XnUInt y = 0; y < _depthData.YRes(); ++y)
	{
		for (XnUInt x = 0; x < _depthData.XRes(); ++x, ++pDepth)
		{
			if (*pDepth != 0)
			{
				g_pDepthHist[*pDepth]++;
				nNumberOfPoints++;
			}
		}
	}
	
	for (int nIndex=1; nIndex<MAX_DEPTH; nIndex++)
	{
		g_pDepthHist[nIndex] += g_pDepthHist[nIndex-1];
	}
	
	if (nNumberOfPoints)
	{
		for (int nIndex=1; nIndex<MAX_DEPTH; nIndex++)
		{
			g_pDepthHist[nIndex] = (unsigned int)(256 * (1.0f - (g_pDepthHist[nIndex] / nNumberOfPoints)));
		}
	}
}

void captureRGB()
{
	rgbMemory = FreeImage_OpenMemory();
	definePixels();
	xnOSMemSet(_videoMap, 0, g_nTexMapX*g_nTexMapY*sizeof(XnRGB24Pixel));
	
	const XnRGB24Pixel* pImageRow = _imageData.RGB24Data();
	XnRGB24Pixel* pTexRow = _videoMap + _imageData.YOffset() * g_nTexMapX;

	for (XnUInt y = 0; y < _imageData.YRes(); ++y)
	{
		const XnRGB24Pixel* pImage = pImageRow;
		XnRGB24Pixel* pTex = pTexRow + _imageData.XOffset();

		for (XnUInt x = 0; x < _imageData.XRes(); ++x, ++pImage, ++pTex)
		{
			*pTex = *pImage;
		}

		pImageRow += _imageData.XRes();
		pTexRow += g_nTexMapX;
	}
	
	captureRGBToMemory();
	FreeImage_CloseMemory(depthMapMemory);
}

void captureDepthMap()
{
	depthMapMemory = FreeImage_OpenMemory();
	definePixels();
	xnOSMemSet(_depthMap, 0, g_nTexMapX*g_nTexMapY*sizeof(XnRGB24Pixel));
	
	const XnDepthPixel* pDepthRow = _depthData.Data();
	XnRGB24Pixel* pTexRow = _depthMap + _depthData.YOffset() * g_nTexMapX;

	for (XnUInt y = 0; y < _depthData.YRes(); ++y)
	{
		const XnDepthPixel* pDepth = pDepthRow;
		XnRGB24Pixel* pTex = pTexRow + _depthData.XOffset();

		for (XnUInt x = 0; x < _depthData.XRes(); ++x, ++pDepth, ++pTex)
		{
			if (*pDepth != 0)
			{
				int nHistValue = g_pDepthHist[*pDepth];
				pTex->nRed = 0; //nHistValue; - This makes it grey.
				pTex->nGreen = nHistValue;
				pTex->nBlue = nHistValue;
			}
		}

		pDepthRow += _depthData.XRes();
		pTexRow += g_nTexMapX;
	}
	
	captureDepthMapToMemory();
	FreeImage_CloseMemory(rgbMemory);
}
