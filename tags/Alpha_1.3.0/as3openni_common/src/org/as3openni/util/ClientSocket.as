package org.as3openni.util
{
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.events.IOErrorEvent;
	import flash.events.ProgressEvent;
	import flash.net.Socket;
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	import org.as3openni.AS3OpenNI;
	import org.as3openni.events.ClientSocketEvent;
	import org.as3openni.global.Definitions;
	
	public class ClientSocket extends EventDispatcher
	{
		private var _firstByte:Number;
		private var _secondByte:Number;
		private var _packetSize:Number;
		private var _socket:Socket;
		private var _buffer:ByteArray;
		private var _dataObj:Object;
		private var _port:Number;
		
		public function ClientSocket()
		{
			this._socket = new Socket();
			this._buffer = new ByteArray();
			this._dataObj = new Object();
			
			this._socket.addEventListener(ProgressEvent.SOCKET_DATA, onSocketData);
			this._socket.addEventListener(IOErrorEvent.IO_ERROR, onSocketError);
			this._socket.addEventListener(Event.CONNECT, onSocketConnect);
		}
		
		public function connect(host:String = '127.0.0.1', port:uint = 9504):void
		{
			this._port = port;
			this._packetSize = 0;
			
			if (!this.connected) this._socket.connect(host, port);
			else dispatchEvent(new ClientSocketEvent(ClientSocketEvent.ON_CONNECT, null));
		}
		
		public function get connected():Boolean
		{
			return this._socket.connected;
		}
		
		public function close():void
		{
			if(this.connected) this._socket.close();
		}
		
		public function sendCommand(data:ByteArray):int
		{
			if(data.length == Definitions.COMMAND_SIZE)
			{
				this._socket.writeBytes(data, 0, Definitions.COMMAND_SIZE);
				this._socket.flush();
				return Definitions.SUCCESS;
			} 
			else 
			{
				throw new Error(Definitions.AS3OPENNI_LABEL + 'Incorrect data size (' + data.length + '). Expected: ' + Definitions.COMMAND_SIZE);
				return Definitions.ERROR;
			}
		}
		
		private function onSocketData(event:ProgressEvent):void
		{
			if(this._socket.bytesAvailable > 0) 
			{
				if(this._packetSize == 0) 
				{
					this._socket.endian = Endian.LITTLE_ENDIAN;
					this._firstByte = this._socket.readByte();
					this._secondByte = this._socket.readByte();
					this._packetSize = this._socket.readInt();
				}
				
				if(this._socket.bytesAvailable >= this._packetSize && this._packetSize != 0)
				{
					this._socket.readBytes(this._buffer, 0, this._packetSize);
					this._buffer.endian = Endian.LITTLE_ENDIAN;
					this._buffer.position = 0;
					this._dataObj.first = this._firstByte;
					this._dataObj.second = this._secondByte;
					this._dataObj.buffer = this._buffer;
					this._packetSize = 0;
					dispatchEvent(new ClientSocketEvent(ClientSocketEvent.ON_DATA, this._dataObj));
				}
			}
		}
		
		private function onSocketError(event:IOErrorEvent):void
		{
			dispatchEvent(new ClientSocketEvent(ClientSocketEvent.ON_ERROR, null));
		}
		
		private function onSocketConnect(event:Event):void
		{
			dispatchEvent(new ClientSocketEvent(ClientSocketEvent.ON_CONNECT, null));
		}
	}
}