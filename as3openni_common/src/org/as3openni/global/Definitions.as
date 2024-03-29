package org.as3openni.global
{
	public class Definitions
	{
		public static const AS3OPENNI_LABEL:String = 'AS3OpenNI :: ';
		
		public static const NITE_SESSION_SOCKET:String = AS3OPENNI_LABEL + 'NITE Session Socket';
		public static const NITE_POINT_SOCKET:String = AS3OPENNI_LABEL + 'NITE Point Socket';
		public static const NITE_SLIDER_SOCKET:String = AS3OPENNI_LABEL + 'NITE Slider Socket';
		
		public static const OPEN_NI_USER_TRACKING_SOCKET:String = AS3OPENNI_LABEL + 'OPENNI User Tracking Socket';
		
		public static const HEADER:String = 'Info (' + AS3OPENNI_LABEL + 'Alpha 1.4.0)';
		public static const AS3OPENNI_CAPTURE_CONNECTED:String = AS3OPENNI_LABEL + 'Server Connected';
		
		public static const CLIENT_SOCKET_CONNECTED:String = AS3OPENNI_LABEL + 'Capture Socket Connected';
		public static const CLIENT_SOCKET_ERROR:String = AS3OPENNI_LABEL + 'Capture Socket Error';
		
		public static const SERVER_IP:String = '127.0.0.1';
		public static const SUCCESS:int = 0;
		public static const COMMAND_SIZE:int = 6;
		
		public static const AS3OPENNI_SERVER_INIT:int = 0;
		public static const AS3OPENNI_ID:int = 0;
		public static const OPENNI_ID:int = 1;
		public static const NITE_ID:int = 2;
		
		public static const OPENNI_GET_DEPTH:int = 0;
		public static const OPENNI_GET_VIDEO:int = 1;
		public static const OPENNI_USER_FOUND:int = 2;
		public static const OPENNI_USER_LOST:int = 3;
		public static const OPENNI_GET_USERS:int = 4;
		public static const OPENNI_GET_SKELETONS:int = 5;
		public static const OPENNI_POSE_DETECTED:int = 6;
		public static const OPENNI_CALIBRATION_STARTED:int = 7;
		public static const OPENNI_CALIBRATION_COMPLETE:int = 8;
		public static const OPENNI_CALIBRATION_FAILED:int = 9;
		
		public static const VGA_MAX_WIDTH:int = 640;
		public static const VGA_MAX_HEIGHT:int = 480;
		
		public static const PLAYER_ONE:int = 1;
		public static const PLAYER_TWO:int = 2;
		public static const PLAYER_THREE:int = 3;
		public static const PLAYER_FOUR:int = 4;
		public static const PLAYER_FIVE:int = 5;
		
		public function Definitions(){}
	}
}