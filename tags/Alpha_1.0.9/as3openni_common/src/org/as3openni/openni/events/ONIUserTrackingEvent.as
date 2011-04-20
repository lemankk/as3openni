package org.as3openni.openni.events
{
	import flash.events.Event;
	
	import org.as3openni.objects.NiPoint3D;
	
	public class ONIUserTrackingEvent extends Event
	{
		public static const USER_TRACKING_USER_CALIBRATION_FAILED:String = "user_tracking_user_calibration_failed:";
		public static const USER_TRACKING_USER_CALIBRATION_COMPLETE:String = "user_tracking_user_calibration_complete:";
		public static const USER_TRACKING_USER_CALIBRATION_START:String = "user_tracking_user_calibration_start:";
		public static const USER_TRACKING_POSE_DETECTED:String = "user_tracking_pose_detected:";
		public static const USER_TRACKING_LOST_USER:String = "user_tracking_lost_user:";
		public static const USER_TRACKING_NEW_USER:String = "user_tracking_new_user:";
		public static const USER_TRACKING_USER_FOUND:String = "user_found:";
		
		public var user:Number = 0;
		public var userSinglePoint:NiPoint3D = null;
		
		public function ONIUserTrackingEvent(type:String, user:Number, userSinglePoint:NiPoint3D = null, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			this.user = user;
			this.userSinglePoint = userSinglePoint;
			super(type, bubbles, cancelable);
		}
	}
}