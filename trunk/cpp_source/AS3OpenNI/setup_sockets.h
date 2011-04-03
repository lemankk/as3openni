#ifndef __SETUP_SOCKETS_H__
#define __SETUP_SOCKETS_H__

#include <XnTypes.h>
#include <fstream>
#include <iostream>

// Use namespace.
using namespace std;

//Toggle on/off features
extern XnBool _featureGesture;
extern XnBool _featureCircle;
extern XnBool _featureSlider;
extern XnBool _featureTrackPad;
extern XnBool _featureSinglePoint;
extern XnBool _featureUserTracking;
extern XnBool _featureRGBCapture;
extern XnBool _featureDepthMapCapture;
extern XnBool _useSockets;

void getDepthMap();
void setupSockets();
void sendToSocket(int socket, const char *data);

extern void error(const char *msg);
#endif