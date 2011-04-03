#include "skeleton.h"

#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
	extern int USER_TRACKING_SOCKET;
#else
	#include "socket.h"
	extern SOCKET USER_TRACKING_SOCKET;
#endif

void renderSkeleton()
{
	_userGenerator.GetUserPixels(0, _sceneData);
	XnUserID aUsers[MAX_USERS];
	XnUInt16 nUsers = MAX_USERS;
	_userGenerator.GetUsers(aUsers, nUsers);
	
	for (int i = 0; i < nUsers; ++i)
	{
		XnUserID player;
		XnSkeletonJointPosition head, neck, left_shoulder, left_elbow, left_hand, right_shoulder, right_elbow, right_hand;
		XnSkeletonJointPosition torso, left_hip, left_knee, left_foot, right_hip, right_knee, right_foot;
		XnSkeletonJointPosition left_big_hand, right_big_hand;
		
		if(_userGenerator.GetSkeletonCap().IsTracking(aUsers[i]))
		{
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
			
			// Check if body parts are available.
			/*if (head.fConfidence < 0.5 || neck.fConfidence < 0.5 || torso.fConfidence < 0.5 ||
			left_shoulder.fConfidence < 0.5 || left_elbow.fConfidence < 0.5 || left_hand.fConfidence < 0.5 ||
			right_shoulder.fConfidence < 0.5 || right_elbow.fConfidence < 0.5 || right_hand.fConfidence < 0.5 ||
			left_hip.fConfidence < 0.5 || left_knee.fConfidence < 0.5 || left_foot.fConfidence < 0.5 ||
			right_hip.fConfidence < 0.5 || right_knee.fConfidence < 0.5 || right_foot.fConfidence < 0.5)
			{
				return;
			}*/
			
			/*// Convert points.
			XnPoint3D ptHead(head.position), ptNeck(neck.position), ptTorso(torso.position);
			_depth.ConvertRealWorldToProjective(1, &ptHead, &ptHead);
			_depth.ConvertRealWorldToProjective(1, &ptNeck, &ptNeck);
			_depth.ConvertRealWorldToProjective(1, &ptTorso, &ptTorso);*/
			
			//------------------------------------------------------------------------------------------//
			//------------------------- SEND DATA TO USER_TRACKING_SOCKET SERVER -----------------------//
			//------------------------------------------------------------------------------------------//
			
			int length;
			
			// Allocate memory for the string we will send to the socket server.
			// length will be the size of the mem allocation for the string
			#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
				length = snprintf
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
			#else
				length = _snprintf
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
			#endif
			
			// Character object that will store the string
			char * data = (char*) malloc((length + 1) * sizeof(char));
			
			// Print string in to format.
			#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
				snprintf
				(
						data, length, FORMAT, player,
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
			#else
				_snprintf
				(
						data, length, FORMAT, player,
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
			#endif
			
			if(_printUserTrackingOnly) printf("Skeleton - %s\n", data);
			if(_useSockets) sendToSocket(USER_TRACKING_SOCKET, data);
			free(data);
			
			//------------------------------------------------------------------------------------------//
			//------------------------- SEND DATA TO USER_TRACKING_SOCKET SERVER -----------------------//
			//------------------------------------------------------------------------------------------//
		}
	}
}