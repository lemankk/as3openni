package org.as3openni.flash.openni.events
{
	import flash.events.Event;
	
	public class ONIUserTrackingEvent extends Event
	{
		public static const USER_TRACKING_USER_CALIBRATION_FAILED:String = "user_tracking_user_calibration_failed:";
		public static const USER_TRACKING_USER_CALIBRATION_COMPLETE:String = "user_tracking_user_calibration_complete:";
		public static const USER_TRACKING_USER_CALIBRATION_START:String = "user_tracking_user_calibration_start:";
		public static const USER_TRACKING_POSE_DETECTED:String = "user_tracking_pose_detected:";
		public static const USER_TRACKING_LOST_USER:String = "user_tracking_lost_user:";
		public static const USER_TRACKING_NEW_USER:String = "user_tracking_new_user:";
		
		public var user:Number = 0;
		
		public function ONIUserTrackingEvent(type:String, user:Number, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			this.user = user;
			super(type, bubbles, cancelable);
		}
	}
}