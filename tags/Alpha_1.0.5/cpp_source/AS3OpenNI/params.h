#ifndef __PARAMS_H__
#define __PARAMS_H__

#include <XnTypes.h>
#include <iostream>
#include <string>

using namespace std;

// For debugging purposes
extern XnBool _printGestureOnly;
extern XnBool _printCircleOnly;
extern XnBool _printSliderOnly;
extern XnBool _printTrackPadOnly;
extern XnBool _printSinglePointOnly;
extern XnBool _printUserTrackingOnly;

// Toggle on/off features
extern XnBool _featureGesture;
extern XnBool _featureCircle;
extern XnBool _featureSlider;
extern XnBool _featureTrackPad;
extern XnBool _featureSinglePoint;
extern XnBool _featureUserTracking;

// Use sockets or not
extern XnBool _useSockets;

// Trackpad vars.
extern int trackpad_columns, trackpad_rows;

void setupParams(int argc, char *argv[]);

#endif