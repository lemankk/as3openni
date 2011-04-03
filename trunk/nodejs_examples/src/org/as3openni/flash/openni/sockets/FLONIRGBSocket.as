package org.as3openni.flash.openni.sockets
{
	import flash.events.ProgressEvent;
	import flash.utils.ByteArray;
	
	import org.as3openni.flash.openni.events.ONICaptureEvent;
	import org.as3openni.flash.util.FLBasicSocket;
	
	public class FLONIRGBSocket extends FLBasicSocket
	{
		public static const OPEN_NI_DEPTH_MAP_SOCKET:String = "open_ni_rgb_socket";
		
		public function FLONIRGBSocket()
		{
			super();
			this.name = OPEN_NI_DEPTH_MAP_SOCKET;
			this.port = 9505;
		}
		
		override protected function onClientSocketData(event:ProgressEvent):void
		{	
			if(this._clientSocket.bytesAvailable > 0)
			{
				var buffer:ByteArray = new ByteArray();
				this._clientSocket.readBytes(buffer);
				this.dispatchEvent(new ONICaptureEvent(ONICaptureEvent.ONI_DEPTH_MAP, buffer));
			}
		}
	}
}