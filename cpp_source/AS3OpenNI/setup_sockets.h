#ifndef __SETUP_SOCKETS_H__
#define __SETUP_SOCKETS_H__

#include <XnTypes.h>
//#include <FreeImage.h>
#include <fstream>

//Toggle on/off features
extern XnBool _featureGesture;
extern XnBool _featureCircle;
extern XnBool _featureSlider;
extern XnBool _featureTrackPad;
extern XnBool _featureSinglePoint;
extern XnBool _featureUserTracking;
extern XnBool _useSockets;

void setupSockets();
void sendToSocket(int socket, const char *data);
//void sendByteToSocket(int socket, FIBITMAP* dib);
extern void error(const char *msg);

#endif