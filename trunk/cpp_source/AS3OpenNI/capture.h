#ifndef __CAPTURE_H__
#define __CAPTURE_H__

#include <XnFPSCalculator.h>
#include <XnCppWrapper.h>
#include <XnTypes.h>
#include <XnOS.h>
#include <stdio.h>
#include "FreeImage.h"

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	#include <pthread/pthread.h>
#else
	#include <pthread.h>
#endif

// Setup namespaces.
using namespace xn;
using namespace std;

// Get classes.
extern DepthMetaData _depthData;
extern ImageMetaData _imageData;
extern SceneMetaData _sceneData;

extern UserGenerator _userGenerator;
extern DepthGenerator _depth;
extern ImageGenerator _image;
extern Context _context;

// Use sockets or not
extern XnBool _printRGBCapture;
extern XnBool _printDepthMapCapture;
extern XnBool _rgbGoGrey;
extern XnBool _depthMapBackground;
extern XnFPSData xnFPS;

// Define the JPEG quality on the DepthMap and RGB capturing.
extern int rgb_quality, depthmap_quality;
extern int rgb_data_ready, dmap_data_ready;
extern int rgb_data_size, dmap_data_size;
extern char* rgb_data;
extern char* dmap_data;
extern pthread_mutex_t rgbMutex;
extern pthread_mutex_t dmapMutex;

#define GL_WIN_SIZE_X 640
#define GL_WIN_SIZE_Y 480

void* streamServer(void* arg);
void captureDepthMap();
void captureRGB();
extern void setupSockets();

#endif