package org.as3openni.openni.sockets
{
	import flash.display.Loader;
	import flash.events.ProgressEvent;
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	import mx.graphics.codec.JPEGEncoder;
	
	import org.as3openni.AS3OpenNI;
	import org.as3openni.openni.events.ONICaptureEvent;
	import org.as3openni.util.BasicSocket;
	
	public class ONIDepthMapSocket extends BasicSocket
	{
		public static const OPEN_NI_DEPTH_MAP_SOCKET:String = "open_ni_depth_map_socket";
		
		private var _buffer:ByteArray;
		private var _bytesAvailable:Number;
		private var _packetSize:Number;
		
		public function ONIDepthMapSocket()
		{
			super();
			
			this._packetSize = 0;
			this._buffer = new ByteArray();
			this._buffer.endian = Endian.LITTLE_ENDIAN;
			this.name = OPEN_NI_DEPTH_MAP_SOCKET;
			this.port = 9504;
		}
		
		/**
		 * TODO: Still trying to figure out how to pass the correct byte array in.
		 */
		override protected function onClientSocketData(event:ProgressEvent):void
		{
			if(this._clientSocket.bytesAvailable > 0)
			{
				trace('Bytes available: ' + this._clientSocket.bytesAvailable);
				/*this._bytesAvailable = this._clientSocket.bytesAvailable;
				
				if(this._packetSize == 0)
				{
					this._clientSocket.endian = Endian.LITTLE_ENDIAN;
					this._packetSize = this._clientSocket.readInt();
				}*/
				
				trace('Packet Size: ' + this._packetSize);
				/*if(this._bytesAvailable >= this._packetSize && this._packetSize != 0)
				{
					trace('ba: ' + this._bytesAvailable);
					this._clientSocket.readBytes(this._buffer, 0, this._bytesAvailable);
					this._buffer.endian = Endian.LITTLE_ENDIAN;
					this._buffer.position = 0;
					this._packetSize = 0;
					this.dispatchEvent(new ONICaptureEvent(ONICaptureEvent.ONI_DEPTH_MAP, this._buffer));
				}*/
			}
		}
	}
}