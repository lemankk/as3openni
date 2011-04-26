#ifndef __PARAMS_H__
#define __PARAMS_H__

#include <XnTypes.h>
#include <iostream>
#include <string>

//---------------------------------------------------------------------------
// Namespaces
//---------------------------------------------------------------------------
using namespace std;

/*
// For debugging purposes.
extern XnBool _printGesture;
extern XnBool _printCircle;
extern XnBool _printSlider;
extern XnBool _printTrackPad;
extern XnBool _printSinglePoint;
extern XnBool _printUserTracking;*/

// Toggle on/off features.
extern XnBool g_bFeatureRGBCapture;
extern XnBool g_bFeatureDepthMapCapture;

// Extra functions.
extern XnBool g_bMirror;
extern XnBool g_bDrawBackground;
extern XnBool g_bUseSockets;
extern int g_intTrackpadColumns, g_intTrackpadRows;

void setupParams(int argc, char *argv[]);

#endif