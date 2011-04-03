package org.as3openni.openni.sockets
{
	import flash.events.ProgressEvent;
	
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
			this.name = OPEN_NI_USER_TRACKING_SOCKET;
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
				
				// Detect for user tracking skeleton.
				if(this.socketMessage.substr(0, ONISkeletonEvent.USER_TRACKING.length) == ONISkeletonEvent.USER_TRACKING)
				{
					var arr7:Array = this.socketMessage.split(':');
					var dataStr:String = String(arr7[1]);
					var points:Array = dataStr.split(',');
					var user:Number = Number(points[1]);
					
					var skeleton:NiSkeleton = new NiSkeleton();
					var head:NiPoint3D = new NiPoint3D();
					var neck:NiPoint3D = new NiPoint3D();
					var torso:NiPoint3D = new NiPoint3D();
					
					var leftBigHand:NiPoint3D = new NiPoint3D();
					var rightBigHand:NiPoint3D = new NiPoint3D();
					
					var leftShoulder:NiPoint3D = new NiPoint3D();
					var leftElbow:NiPoint3D = new NiPoint3D();
					var leftHand:NiPoint3D = new NiPoint3D();
					
					var rightShoulder:NiPoint3D = new NiPoint3D();
					var rightElbow:NiPoint3D = new NiPoint3D();
					var rightHand:NiPoint3D = new NiPoint3D();
					
					var leftHip:NiPoint3D = new NiPoint3D();
					var leftKnee:NiPoint3D = new NiPoint3D();
					var leftFoot:NiPoint3D = new NiPoint3D();
					
					var rightHip:NiPoint3D = new NiPoint3D();
					var rightKnee:NiPoint3D = new NiPoint3D();
					var rightFoot:NiPoint3D = new NiPoint3D();
					
					for(var i:Number = 0; i < points.length; i++)
					{
						var val:String = points[i] as String;
						switch(val)
						{
							case 'head':
								head.user = user;
								head.pointX = Number(points[i+1]);
								head.pointY = Number(points[i+2]);
								head.pointZ = Number(points[i+3]);
								break;
							
							case 'neck':
								neck.user = user;
								neck.pointX = Number(points[i+1]);
								neck.pointY = Number(points[i+2]);
								neck.pointZ = Number(points[i+3]);
								break;
							
							case 'torso':
								torso.user = user;
								torso.pointX = Number(points[i+1]);
								torso.pointY = Number(points[i+2]);
								torso.pointZ = Number(points[i+3]);
								break;
							
							case 'left_big_hand':
								leftBigHand.user = user;
								leftBigHand.pointX = Number(points[i+1]);
								leftBigHand.pointY = Number(points[i+2]);
								leftBigHand.pointZ = Number(points[i+3]);
								break;
							
							case 'right_big_hand':
								rightBigHand.user = user;
								rightBigHand.pointX = Number(points[i+1]);
								rightBigHand.pointY = Number(points[i+2]);
								rightBigHand.pointZ = Number(points[i+3]);
								break;
							
							case 'left_shoulder':
								leftShoulder.user = user;
								leftShoulder.pointX = Number(points[i+1]);
								leftShoulder.pointY = Number(points[i+2]);
								leftShoulder.pointZ = Number(points[i+3]);
								break;
							
							case 'left_elbow':
								leftElbow.user = user;
								leftElbow.pointX = Number(points[i+1]);
								leftElbow.pointY = Number(points[i+2]);
								leftElbow.pointZ = Number(points[i+3]);
								break;
							
							case 'left_hand':
								leftHand.user = user;
								leftHand.pointX = Number(points[i+1]);
								leftHand.pointY = Number(points[i+2]);
								leftHand.pointZ = Number(points[i+3]);
								break;
							
							case 'right_shoulder':
								rightShoulder.user = user;
								rightShoulder.pointX = Number(points[i+1]);
								rightShoulder.pointY = Number(points[i+2]);
								rightShoulder.pointZ = Number(points[i+3]);
								break;
							
							case 'right_elbow':
								rightElbow.user = user;
								rightElbow.pointX = Number(points[i+1]);
								rightElbow.pointY = Number(points[i+2]);
								rightElbow.pointZ = Number(points[i+3]);
								break;
							
							case 'right_hand':
								rightHand.user = user;
								rightHand.pointX = Number(points[i+1]);
								rightHand.pointY = Number(points[i+2]);
								rightHand.pointZ = Number(points[i+3]);
								break;
							
							case 'left_hip':
								leftHip.user = user;
								leftHip.pointX = Number(points[i+1]);
								leftHip.pointY = Number(points[i+2]);
								leftHip.pointZ = Number(points[i+3]);
								break;
							
							case 'left_knee':
								leftKnee.user = user;
								leftKnee.pointX = Number(points[i+1]);
								leftKnee.pointY = Number(points[i+2]);
								leftKnee.pointZ = Number(points[i+3]);
								break;
							
							case 'left_foot':
								leftFoot.user = user;
								leftFoot.pointX = Number(points[i+1]);
								leftFoot.pointY = Number(points[i+2]);
								leftFoot.pointZ = Number(points[i+3]);
								break;
							
							case 'right_hip':
								rightHip.user = user;
								rightHip.pointX = Number(points[i+1]);
								rightHip.pointY = Number(points[i+2]);
								rightHip.pointZ = Number(points[i+3]);
								break;
							
							case 'right_knee':
								rightKnee.user = user;
								rightKnee.pointX = Number(points[i+1]);
								rightKnee.pointY = Number(points[i+2]);
								rightKnee.pointZ = Number(points[i+3]);
								break;
							
							case 'right_foot':
								rightFoot.user = user;
								rightFoot.pointX = Number(points[i+1]);
								rightFoot.pointY = Number(points[i+2]);
								rightFoot.pointZ = Number(points[i+3]);
								break;
						}
					}
					
					// Populate the skeleton object.
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