#ifndef __SKELETON_H__
#define __SKELETON_H__

#include <XnOpenNI.h>
#include <XnTypes.h>
#include <XnCppWrapper.h>
#include <string>
#include <iostream>

using namespace xn;
using namespace std;

extern UserGenerator _userGenerator;
extern SceneMetaData _sceneData;
extern DepthGenerator _depth;

extern XnBool _printUserTracking;
extern XnBool _useSockets;

#define SKEL_FORMAT "user_tracking:player,%d,left_big_hand,%f,%f,%f,right_big_hand,%f,%f,%f,head,%f,%f,%f,neck,%f,%f,%f,torso,%f,%f,%f,left_shoulder,%f,%f,%f,left_elbow,%f,%f,%f,left_hand,%f,%f,%f,right_shoulder,%f,%f,%f,right_elbow,%f,%f,%f,right_hand,%f,%f,%f,left_hip,%f,%f,%f,left_knee,%f,%f,%f,left_foot,%f,%f,%f,right_hip,%f,%f,%f,right_knee,%f,%f,%f,right_foot,%f,%f,%f||"
#define COM_FORMAT "user_found:player,%d,com,%f,%f,%f||"
#define MAX_USERS 15

void renderSkeleton();
extern void sendToSocket(int socket, const char *data);

#endif