package org.as3openni.openni.sockets
{
	import flash.events.ProgressEvent;
	
	import org.as3openni.global.Definitions;
	import org.as3openni.objects.NiPoint3D;
	import org.as3openni.objects.NiSkeleton;
	import org.as3openni.openni.events.ONISkeletonEvent;
	import org.as3openni.openni.events.ONIUserTrackingEvent;
	import org.as3openni.util.BasicSocket;
	
	public class ONIUserTrackingSocket extends BasicSocket
	{
		public static const OPEN_NI_USER_TRACKING_SOCKET:String = "open_ni_user_tracking_socket";
		
		public function ONIUserTrackingSocket()
		{
			super();
			this.name = Definitions.OPEN_NI_USER_TRACKING_SOCKET;
			this.port = 9503;
		}
		
		override protected function onClientSocketData(event:ProgressEvent):void
		{
			super.onClientSocketData(event);
			
			if(this.socketMessage.length > 0)
			{		
				// Detect for new user.
				if(this.socketMessage.substr(0, ONIUserTrackingEvent.USER_TRACKING_NEW_USER.length) == ONIUserTrackingEvent.USER_TRACKING_NEW_USER)
				{
					var arr1:Array = this.socketMessage.split(':');
					var newUser:Number = Number(arr1[1]);
					this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_NEW_USER, newUser));
				}
				
				// Detect for lost user.
				if(this.socketMessage.substr(0, ONIUserTrackingEvent.USER_TRACKING_LOST_USER.length) == ONIUserTrackingEvent.USER_TRACKING_LOST_USER)
				{
					var arr2:Array = this.socketMessage.split(':');
					var lostUser:Number = Number(arr2[1]);
					this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_LOST_USER, lostUser));
				}
				
				// Detect for user pose detected.
				if(this.socketMessage.substr(0, ONIUserTrackingEvent.USER_TRACKING_POSE_DETECTED.length) == ONIUserTrackingEvent.USER_TRACKING_POSE_DETECTED)
				{
					var arr3:Array = this.socketMessage.split(':');
					var poseDetectedUser:Number = Number(arr3[1]);
					this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_POSE_DETECTED, poseDetectedUser));
				}
				
				// Detect for user calibration started.
				if(this.socketMessage.substr(0, ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_START.length) == ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_START)
				{
					var arr4:Array = this.socketMessage.split(':');
					var userCalStart:Number = Number(arr4[1]);
					this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_START, userCalStart));
				}
				
				// Detect for user calibration completed.
				if(this.socketMessage.substr(0, ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_COMPLETE.length) == ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_COMPLETE)
				{
					var arr5:Array = this.socketMessage.split(':');
					var userCalComplete:Number = Number(arr5[1]);
					this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_COMPLETE, userCalComplete));
				}
				
				// Detect for user calibration failed.
				if(this.socketMessage.substr(0, ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_FAILED.length) == ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_FAILED)
				{
					var arr6:Array = this.socketMessage.split(':');
					var userCalFailed:Number = Number(arr6[1]);
					this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_FAILED, userCalFailed));
				}
				
				// Define the all players array.
				var allPlayers:Array = this.socketMessage.split('|');
				var len:Number = allPlayers.length;
				
				for(var p:Number = 0; p < len; p++)
				{
					var eachPlayer:String = String(allPlayers[p]);
					if(eachPlayer.length > 0)
					{
						var eachPlayerArr:Array = eachPlayer.split(':');
						var dataLbl:String = String(String(eachPlayerArr[0]) + ':');
						var dataStr:String = String(eachPlayerArr[1]);
						var points:Array = dataStr.split(',');
						var user:Number = Number(points[0]);
						
						// If a user has been found.
						if(dataLbl == ONIUserTrackingEvent.USER_TRACKING_USER_FOUND)
						{
							// Define.
							var userSinglePoint:NiPoint3D = new NiPoint3D();
							userSinglePoint.user = user;
							userSinglePoint.pointX = Number(points[1]);
							userSinglePoint.pointY = Number(points[2]);
							userSinglePoint.pointZ = Number(points[3]);
							
							// Then dispatch the user tracking event out.
							this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_USER_FOUND, user, userSinglePoint));
						}
						
						// If a user is being tracked.
						if(dataLbl == ONISkeletonEvent.USER_TRACKING)
						{
							// Define the body parts.
							var head:NiPoint3D = new NiPoint3D();
							head.user = user;
							head.pointX = Number(points[1]);
							head.pointY = Number(points[2]);
							head.pointZ = Number(points[3]);
							
							var neck:NiPoint3D = new NiPoint3D();
							neck.user = user;
							neck.pointX = Number(points[4]);
							neck.pointY = Number(points[5]);
							neck.pointZ = Number(points[6]);
							
							var torso:NiPoint3D = new NiPoint3D();
							torso.user = user;
							torso.pointX = Number(points[7]);
							torso.pointY = Number(points[8]);
							torso.pointZ = Number(points[9]);
							
							var leftShoulder:NiPoint3D = new NiPoint3D();
							leftShoulder.user = user;
							leftShoulder.pointX = Number(points[10]);
							leftShoulder.pointY = Number(points[11]);
							leftShoulder.pointZ = Number(points[12]);
							
							var leftElbow:NiPoint3D = new NiPoint3D();
							leftElbow.user = user;
							leftElbow.pointX = Number(points[13]);
							leftElbow.pointY = Number(points[14]);
							leftElbow.pointZ = Number(points[15]);
							
							var leftHand:NiPoint3D = new NiPoint3D();
							leftHand.user = user;
							leftHand.pointX = Number(points[16]);
							leftHand.pointY = Number(points[17]);
							leftHand.pointZ = Number(points[18]);
							
							var leftBigHand:NiPoint3D = new NiPoint3D();
							leftBigHand.user = user;
							leftBigHand.pointX = Number(points[16]);
							leftBigHand.pointY = Number(points[17]);
							leftBigHand.pointZ = Number(points[18]);
							
							var rightShoulder:NiPoint3D = new NiPoint3D();
							rightShoulder.user = user;
							rightShoulder.pointX = Number(points[19]);
							rightShoulder.pointY = Number(points[20]);
							rightShoulder.pointZ = Number(points[21]);
							
							var rightElbow:NiPoint3D = new NiPoint3D();
							rightElbow.user = user;
							rightElbow.pointX = Number(points[22]);
							rightElbow.pointY = Number(points[23]);
							rightElbow.pointZ = Number(points[24]);
							
							var rightHand:NiPoint3D = new NiPoint3D();
							rightHand.user = user;
							rightHand.pointX = Number(points[25]);
							rightHand.pointY = Number(points[26]);
							rightHand.pointZ = Number(points[27]);
							
							var rightBigHand:NiPoint3D = new NiPoint3D();
							rightBigHand.user = user;
							rightBigHand.pointX = Number(points[25]);
							rightBigHand.pointY = Number(points[26]);
							rightBigHand.pointZ = Number(points[27]);
							
							var leftHip:NiPoint3D = new NiPoint3D();
							leftHip.user = user;
							leftHip.pointX = Number(points[28]);
							leftHip.pointY = Number(points[29]);
							leftHip.pointZ = Number(points[30]);
							
							var leftKnee:NiPoint3D = new NiPoint3D();
							leftKnee.user = user;
							leftKnee.pointX = Number(points[31]);
							leftKnee.pointY = Number(points[32]);
							leftKnee.pointZ = Number(points[33]);
							
							var leftFoot:NiPoint3D = new NiPoint3D();
							leftFoot.user = user;
							leftFoot.pointX = Number(points[34]);
							leftFoot.pointY = Number(points[35]);
							leftFoot.pointZ = Number(points[36]);
							
							var rightHip:NiPoint3D = new NiPoint3D();
							rightHip.user = user;
							rightHip.pointX = Number(points[37]);
							rightHip.pointY = Number(points[38]);
							rightHip.pointZ = Number(points[39]);
							
							var rightKnee:NiPoint3D = new NiPoint3D();
							rightKnee.user = user;
							rightKnee.pointX = Number(points[40]);
							rightKnee.pointY = Number(points[41]);
							rightKnee.pointZ = Number(points[42]);
							
							var rightFoot:NiPoint3D = new NiPoint3D();
							rightFoot.user = user;
							rightFoot.pointX = Number(points[43]);
							rightFoot.pointY = Number(points[44]);
							rightFoot.pointZ = Number(points[45]);
							
							// Populate the skeleton object.
							var skeleton:NiSkeleton = new NiSkeleton();
							skeleton.user = user;
							skeleton.head = head;
							skeleton.neck = neck;
							skeleton.torso = torso;
							
							skeleton.leftShoulder = leftShoulder;
							skeleton.leftElbow = leftElbow;
							skeleton.leftHand = leftHand;
							
							skeleton.rightShoulder = rightShoulder;
							skeleton.rightElbow = rightElbow;
							skeleton.rightHand = rightHand;
							
							skeleton.leftHip = leftHip;
							skeleton.leftKnee = leftKnee;
							skeleton.leftFoot = leftFoot;
							
							skeleton.rightHip = rightHip;
							skeleton.rightKnee = rightKnee;
							skeleton.rightFoot = rightFoot;
							
							// Then dispatch the user tracking event out.
							this.dispatchEvent
							(
								new ONISkeletonEvent
								(
									ONISkeletonEvent.USER_TRACKING, 
									user,
									leftBigHand,
									rightBigHand,
									skeleton
								)
							);
						}
					}
				}
			}
		}
	}
}