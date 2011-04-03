package org.as3openni.openni.sockets
{
	import flash.events.ProgressEvent;
	import flash.utils.ByteArray;
	
	import org.as3openni.openni.events.ONICaptureEvent;
	import org.as3openni.util.BasicSocket;
	
	public class ONIRGBSocket extends BasicSocket
	{
		public static const OPEN_NI_RGB_SOCKET:String = "open_ni_rgb_socket";
		
		public function ONIRGBSocket()
		{
			super();
			this.name = OPEN_NI_RGB_SOCKET;
			this.port = 9505;
		}
		
		override protected function onClientSocketData(event:ProgressEvent):void
		{	
			if(this._clientSocket.bytesAvailable > 0)
			{
				var buffer:ByteArray = new ByteArray();
				this._clientSocket.readBytes(buffer);
				this.dispatchEvent(new ONICaptureEvent(ONICaptureEvent.ONI_RGB, buffer));
			}
		}
	}
}