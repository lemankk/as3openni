package org.as3openni.util
{
	import flash.events.DatagramSocketDataEvent;
	import flash.events.EventDispatcher;
	import flash.net.DatagramSocket;
	import flash.net.Socket;
	import flash.utils.ByteArray;

	public class UDPBasicSocket extends EventDispatcher
	{
		protected var _traceLog:Boolean = false;
		protected var _name:String = "basic_socket";
		protected var _port:int = 9500;
		protected var _address:String = "127.0.0.1";
		protected var _socketMessage:String;
		protected var _datagramSocket:DatagramSocket = new DatagramSocket();

		public function UDPBasicSocket()
		{
			// Abstract.
		}
		
		public function bind():void
		{
			if(datagramSocket.bound) 
			{
				this._datagramSocket.close();
				this._datagramSocket = new DatagramSocket();	
			}
			
			this._datagramSocket.bind(this.port, this.address);
			this._datagramSocket.addEventListener(DatagramSocketDataEvent.DATA, onDataReceived);
			this._datagramSocket.receive();
			trace(this.name + " - Bound to: " + this._datagramSocket.localAddress + ":" + this._datagramSocket.localPort);
		}
		
		public function send(value:String):void
		{
			var data:ByteArray = new ByteArray();
			data.writeUTFBytes(value);
			
			try
			{
				this._datagramSocket.send(data, 0, 0, this.address, this.port); 
				this.log("Sent message to " + this.address + ":" + this.port);
			}
			catch (error:Error)
			{
				this.log(error.message);
			}
		}
		
		protected function onDataReceived(event:DatagramSocketDataEvent):void
		{
			var str:String = event.data.readUTFBytes(event.data.bytesAvailable);
			if(str.length > 0)
			{
				this._socketMessage = str;
				this.log("Received: " + str);
			}
		}
		
		protected function log(text:String):void
		{
			if(this.traceLog)
			{
				if(this.name.length > 0)
				{
					trace(this.name + ' -', text);
				}
				else
				{
					trace(this + ' -', text);
				}
			}
		}
		
		public function get datagramSocket():DatagramSocket
		{
			return this._datagramSocket;
		}
		
		public function get traceLog():Boolean
		{
			return this._traceLog;
		}
		
		public function set traceLog(value:Boolean):void
		{
			this._traceLog = value;
		}
		
		public function get name():String
		{
			return this._name;
		}
		
		public function set name(value:String):void
		{
			this._name = value;
		}
		
		public function get address():String
		{
			return this._address;
		}
		
		public function set address(value:String):void
		{
			this._address = value;
		}
		
		public function get port():int
		{
			return this._port;
		}
		
		public function set port(value:int):void
		{
			this._port = value;
		}
		
		public function get socketMessage():String
		{
			return this._socketMessage;
		}
	}
}