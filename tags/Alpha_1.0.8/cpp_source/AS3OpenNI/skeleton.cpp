#include "skeleton.h"

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	#include "socket.h"
	extern SOCKET USER_TRACKING_SOCKET;
	extern SOCKET SECOND_USER_TRACKING_SOCKET;
#else
	extern int USER_TRACKING_SOCKET;
	extern int SECOND_USER_TRACKING_SOCKET;
#endif

char * _primaryPlayer;
char * _secondaryPlayer;

int _primaryLength;
int _secondaryLength;

void renderSkeleton()
{
	_userGenerator.GetUserPixels(0, _sceneData);
	XnUserID aUsers[MAX_USERS];
	XnUInt16 nUsers = MAX_USERS;
	_userGenerator.GetUsers(aUsers, nUsers);
	
	for (int i = 0; i < nUsers; ++i)
	{
		if(_userGenerator.GetSkeletonCap().IsTracking(aUsers[i]))
		{
			XnUserID player;
			XnSkeletonJointPosition head, neck, left_shoulder, left_elbow, left_hand, right_shoulder, right_elbow, right_hand;
			XnSkeletonJointPosition torso, left_hip, left_knee, left_foot, right_hip, right_knee, right_foot;
			XnSkeletonJointPosition left_big_hand, right_big_hand;
			
			player = aUsers[i];
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_HEAD, head);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_NECK, neck);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_TORSO, torso);
			
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_SHOULDER, left_shoulder);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_ELBOW, left_elbow);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_HAND, left_hand);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_HAND, left_big_hand);
			
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_SHOULDER, right_shoulder);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_ELBOW, right_elbow);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_HAND, right_hand);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_HAND, right_big_hand);
		
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_HIP, left_hip);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_KNEE, left_knee);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_FOOT, left_foot);
			
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_HIP, right_hip);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_KNEE, right_knee);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_FOOT, right_foot);
			
			//------------------------------------------------------------------------------------------//
			//------------------------- SEND DATA TO USER_TRACKING_SOCKET SERVER -----------------------//
			//------------------------------------------------------------------------------------------//
			
			#if (XN_PLATFORM == XN_PLATFORM_WIN32)
				if(player > 1)
				{
					_secondaryLength = _snprintf
					(
							NULL, 0, FORMAT2, player,
							left_big_hand.position.X, left_big_hand.position.Y, left_big_hand.position.Z,
							right_big_hand.position.X, right_big_hand.position.Y, right_big_hand.position.Z,
							
							head.position.X, head.position.Y, head.position.Z, 
							neck.position.X, neck.position.Y, neck.position.Z,
							torso.position.X, torso.position.Y, torso.position.Z,
							
							left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
							left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
							left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
							
							right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
							right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
							right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
							
							left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
							left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
							left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
							
							right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
							right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
							right_foot.position.X, right_foot.position.Y, right_foot.position.Z
					);
				}
				else
				{
					_primaryLength = _snprintf
					(
							NULL, 0, FORMAT, player,
							left_big_hand.position.X, left_big_hand.position.Y, left_big_hand.position.Z,
							right_big_hand.position.X, right_big_hand.position.Y, right_big_hand.position.Z,
							
							head.position.X, head.position.Y, head.position.Z, 
							neck.position.X, neck.position.Y, neck.position.Z,
							torso.position.X, torso.position.Y, torso.position.Z,
							
							left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
							left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
							left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
							
							right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
							right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
							right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
							
							left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
							left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
							left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
							
							right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
							right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
							right_foot.position.X, right_foot.position.Y, right_foot.position.Z
					);
				}
			#else
				if(player > 1)
				{
					_secondaryLength = snprintf
					(
							NULL, 0, FORMAT2, player,
							left_big_hand.position.X, left_big_hand.position.Y, left_big_hand.position.Z,
							right_big_hand.position.X, right_big_hand.position.Y, right_big_hand.position.Z,
							
							head.position.X, head.position.Y, head.position.Z, 
							neck.position.X, neck.position.Y, neck.position.Z,
							torso.position.X, torso.position.Y, torso.position.Z,
							
							left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
							left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
							left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
							
							right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
							right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
							right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
							
							left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
							left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
							left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
							
							right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
							right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
							right_foot.position.X, right_foot.position.Y, right_foot.position.Z
					);
				}
				else
				{
					_primaryLength = snprintf
					(
							NULL, 0, FORMAT, player,
							left_big_hand.position.X, left_big_hand.position.Y, left_big_hand.position.Z,
							right_big_hand.position.X, right_big_hand.position.Y, right_big_hand.position.Z,
							
							head.position.X, head.position.Y, head.position.Z, 
							neck.position.X, neck.position.Y, neck.position.Z,
							torso.position.X, torso.position.Y, torso.position.Z,
							
							left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
							left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
							left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
							
							right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
							right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
							right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
							
							left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
							left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
							left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
							
							right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
							right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
							right_foot.position.X, right_foot.position.Y, right_foot.position.Z
					);
				}
			#endif
			
			// Character object that will store the string.
			_secondaryPlayer = (char*) malloc((_secondaryLength + 1) * sizeof(char));
			_primaryPlayer = (char*) malloc((_primaryLength + 1) * sizeof(char));
			
			// Print string in to format.
			#if (XN_PLATFORM == XN_PLATFORM_WIN32)
				if(player > 1)
				{
					_snprintf
					(
							_secondaryPlayer, _secondaryLength, FORMAT2, player,
							left_big_hand.position.X, left_big_hand.position.Y, left_big_hand.position.Z,
							right_big_hand.position.X, right_big_hand.position.Y, right_big_hand.position.Z,
							
							head.position.X, head.position.Y, head.position.Z, 
							neck.position.X, neck.position.Y, neck.position.Z,
							torso.position.X, torso.position.Y, torso.position.Z,
							
							left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
							left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
							left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
							
							right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
							right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
							right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
							
							left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
							left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
							left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
							
							right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
							right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
							right_foot.position.X, right_foot.position.Y, right_foot.position.Z
					);
				}
				else
				{
					_snprintf
					(
							_primaryPlayer, _primaryLength, FORMAT, player,
							left_big_hand.position.X, left_big_hand.position.Y, left_big_hand.position.Z,
							right_big_hand.position.X, right_big_hand.position.Y, right_big_hand.position.Z,
							
							head.position.X, head.position.Y, head.position.Z, 
							neck.position.X, neck.position.Y, neck.position.Z,
							torso.position.X, torso.position.Y, torso.position.Z,
							
							left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
							left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
							left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
							
							right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
							right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
							right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
							
							left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
							left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
							left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
							
							right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
							right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
							right_foot.position.X, right_foot.position.Y, right_foot.position.Z
					);
				}
			#else
				if(player > 1)
				{
					snprintf
					(
							_secondaryPlayer, _secondaryLength, FORMAT2, player,
							left_big_hand.position.X, left_big_hand.position.Y, left_big_hand.position.Z,
							right_big_hand.position.X, right_big_hand.position.Y, right_big_hand.position.Z,
							
							head.position.X, head.position.Y, head.position.Z, 
							neck.position.X, neck.position.Y, neck.position.Z,
							torso.position.X, torso.position.Y, torso.position.Z,
							
							left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
							left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
							left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
							
							right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
							right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
							right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
							
							left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
							left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
							left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
							
							right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
							right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
							right_foot.position.X, right_foot.position.Y, right_foot.position.Z
					);
				}
				else
				{
					snprintf
					(
							_primaryPlayer, _primaryLength, FORMAT, player,
							left_big_hand.position.X, left_big_hand.position.Y, left_big_hand.position.Z,
							right_big_hand.position.X, right_big_hand.position.Y, right_big_hand.position.Z,
							
							head.position.X, head.position.Y, head.position.Z, 
							neck.position.X, neck.position.Y, neck.position.Z,
							torso.position.X, torso.position.Y, torso.position.Z,
							
							left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
							left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
							left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
							
							right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
							right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
							right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
							
							left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
							left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
							left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
							
							right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
							right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
							right_foot.position.X, right_foot.position.Y, right_foot.position.Z
					);
				}
			#endif
			
			if(_printUserTracking) 
			{
				printf("Primary Skeleton - %s\n", _primaryPlayer);
				if(player > 1) printf("Secondary Skeleton - %s\n", _secondaryPlayer);
			}
			
			if(_useSockets) 
			{
				sendToSocket(USER_TRACKING_SOCKET, _primaryPlayer);
				if(player > 1) sendToSocket(SECOND_USER_TRACKING_SOCKET, _secondaryPlayer);
			}
			
			free(_secondaryPlayer);
			free(_primaryPlayer);
			
			//------------------------------------------------------------------------------------------//
			//------------------------- SEND DATA TO USER_TRACKING_SOCKET SERVER -----------------------//
			//------------------------------------------------------------------------------------------//
		}
	}
}