#ifndef __PARAMS_H__
#define __PARAMS_H__

#include <XnTypes.h>
#include <iostream>
#include <string>

using namespace std;

// For debugging purposes
extern XnBool _printGesture;
extern XnBool _printCircle;
extern XnBool _printSlider;
extern XnBool _printTrackPad;
extern XnBool _printSinglePoint;
extern XnBool _printUserTracking;
extern XnBool _printRGBCapture;
extern XnBool _printDepthMapCapture;

// Toggle on/off features
extern XnBool _featureGesture;
extern XnBool _featureCircle;
extern XnBool _featureSlider;
extern XnBool _featureTrackPad;
extern XnBool _featureSinglePoint;
extern XnBool _featureUserTracking;
extern XnBool _featureRGBCapture;
extern XnBool _featureDepthMapCapture;
extern XnBool _rgbGoGrey;
extern XnBool _snapPixels;
extern XnBool _mirror;

// Use sockets or not
extern XnBool _useSockets;

// Trackpad vars.
extern int trackpad_columns, trackpad_rows;

// Define the JPEG quality on the DepthMap and RGB capturing.
extern int depthmap_quality, rgb_quality;

void setupParams(int argc, char *argv[]);

#endif