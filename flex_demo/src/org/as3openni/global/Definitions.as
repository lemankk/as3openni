package org.as3openni.global
{
	public class Definitions
	{
		public static const AS3OPENNI_LABEL:String = 'AS3OpenNI :: ';
		public static const AS3OPENNI_BRIDGE_LABEL:String = 'AS3OpenNI-Bridge :: ';
		
		public static const HEADER:String = 'Info (' + AS3OPENNI_LABEL + 'Beta 1.0.0)';
		public static const BRIDGE_CONNECTED:String = AS3OPENNI_BRIDGE_LABEL + 'Server Connected';
		public static const BRIDGE_READY:String = AS3OPENNI_BRIDGE_LABEL + 'Server Ready';
		
		public static const CLIENT_SOCKET_CONNECTED:String = AS3OPENNI_LABEL + 'Client Socket Connected';
		public static const CLIENT_SOCKET_ERROR:String = AS3OPENNI_LABEL + 'Client Socket Error';
		
		public static const SERVER_IP:String = '127.0.0.1';
		public static const SOCKET_PORT:int = 9400;
		public static const SUCCESS:int = 0;
		public static const ERROR:int = -1;
		public static const COMMAND_SIZE:int = 6;
		
		public static const AS3OPENNI_ID:int = 0;
		public static const OPENNI_ID:int = 1;
		public static const NITE_ID:int = 2;
		
		public static const OPENNI_GET_DEPTH:int = 0;
		public static const OPENNI_GET_VIDEO:int = 1;
		public static const OPENNI_GET_USERS:int = 2;
		
		public static const AS3OPENNI_SERVER_INIT:int = 0;
		public static const AS3OPENNI_SERVER_READY:int = 1;
		
		public static const VGA_MAX_WIDTH:int = 640;
		public static const VGA_MAX_HEIGHT:int = 480;
		
		public function Definitions(){}
	}
}