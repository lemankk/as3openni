package org.as3openni.events.openni
{
	import flash.events.Event;
	
	public class OpenNIEvent extends Event
	{
		public static const ON_VIDEO:String = "onVideo";
		public static const ON_DEPTH:String = "onDepth";
		public static const USER_FOUND:String = "userFound";
		public static const USER_LOST:String = "userLost";
		public static const POSE_DETECTED:String = "poseDetected";
		public static const CALIBRATION_STARTED:String = "calibrationStarted";
		public static const CALIBRATION_COMPLETE:String = "calibrationComplete";
		public static const CALIBRATION_FAILED:String = "calibrationFailed";
		
		public var data:*;
		public function OpenNIEvent(type:String, data:*)
		{
			this.data = data;
			super(type);
		}
	}
}