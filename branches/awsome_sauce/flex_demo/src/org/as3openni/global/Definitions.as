package org.as3openni.global
{
	public class Definitions
	{
		public static const AS3OPENNI_LABEL:String = 'AS3OpenNI :: ';
		public static const AS3OPENNI_BRIDGE_LABEL:String = 'AS3OpenNI-Bridge :: ';
		
		public static const HEADER:String = 'Info (' + AS3OPENNI_LABEL + 'Beta 1.0.0)';
		public static const BRIDGE_CONNECTED:String = AS3OPENNI_BRIDGE_LABEL + 'Server Connected';
		
		public static const CLIENT_SOCKET_CONNECTED:String = AS3OPENNI_LABEL + 'Client Socket Connected';
		public static const CLIENT_SOCKET_ERROR:String = AS3OPENNI_LABEL + 'Client Socket Error';
		
		public static const SERVER_IP:String = '127.0.0.1';
		public static const SOCKET_PORT:int = 9400;
		public static const SUCCESS:int = 0;
		public static const ERROR:int = -1;
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