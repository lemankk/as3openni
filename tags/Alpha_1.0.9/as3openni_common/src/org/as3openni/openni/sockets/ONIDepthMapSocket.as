package org.as3openni.openni.sockets
{
	import flash.events.ProgressEvent;
	import flash.utils.ByteArray;
	
	import org.as3openni.openni.events.ONICaptureEvent;
	import org.as3openni.util.BasicSocket;
	
	public class ONIDepthMapSocket extends BasicSocket
	{
		public static const OPEN_NI_DEPTH_MAP_SOCKET:String = "open_ni_depth_map_socket";
		
		public function ONIDepthMapSocket()
		{
			super();
			this.name = OPEN_NI_DEPTH_MAP_SOCKET;
			this.port = 9504;
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