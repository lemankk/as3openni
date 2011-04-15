#ifndef __CAPTURE_H__
#define __CAPTURE_H__

#include <XnFPSCalculator.h>
#include <XnCppWrapper.h>
#include <XnTypes.h>
#include <XnOS.h>
#include <stdio.h>
#include "FreeImage.h"

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
extern XnBool _useSockets;
extern XnBool _featureRGBCapture;
extern XnBool _featureDepthMapCapture;
extern XnBool _printRGBCapture;
extern XnBool _printDepthMapCapture;
extern XnBool _rgbGoGrey;
extern XnBool _depthMapBackground;
extern XnFPSData xnFPS;

//Define the JPEG quality on the DepthMap and RGB capturing.
extern int rgb_quality, depthmap_quality;

#define GL_WIN_SIZE_X 640
#define GL_WIN_SIZE_Y 480

void captureDepthMap();
void captureRGB();

#endif