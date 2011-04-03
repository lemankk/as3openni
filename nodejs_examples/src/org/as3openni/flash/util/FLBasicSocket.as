package org.as3openni.flash.util
{
	import flash.display.Sprite;
	import flash.errors.IOError;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.IOErrorEvent;
	import flash.events.ProgressEvent;
	import flash.events.SecurityErrorEvent;
	import flash.net.Socket;
	
	public class FLBasicSocket extends EventDispatcher
	{
		protected var _socketMessage:String;
		protected var _traceLog:Boolean = false;
		protected var _name:String = "basic_socket";
		protected var _port:int = 9500;
		protected var _address:String = "localhost";
		protected var _clientSocket:Socket;
		
		public function FLBasicSocket()
		{
			this._clientSocket = new Socket(this.address, this.port);
			this.configureListeners();
		}
		
		protected function configureListeners():void 
		{
			this._clientSocket.addEventListener(Event.CLOSE, onClose);
			this._clientSocket.addEventListener(Event.CONNECT, onConnect);
			this._clientSocket.addEventListener(IOErrorEvent.IO_ERROR, onIOError);
			this._clientSocket.addEventListener(SecurityErrorEvent.SECURITY_ERROR, onSecurityError);
			this._clientSocket.addEventListener(ProgressEvent.SOCKET_DATA, onClientSocketData);
		}
		
		protected function onClientSocketData(event:ProgressEvent):void 
		{
			var str:String = this._clientSocket.readUTFBytes(this._clientSocket.bytesAvailable);
			if(str.length > 0)
			{
				this._socketMessage = str;
				this.log("Received: " + str);
			}
		}
		
		protected function onClose(event:Event):void 
		{
			this.log('Socket Closed');
			this._clientSocket.close();
		}
		
		protected function onConnect(event:Event):void 
		{
			this.log('Socket Connected');
		}
		
		protected function onIOError(event:IOErrorEvent):void 
		{
			this.log('Socket Error');
		}
		
		protected function onSecurityError(event:SecurityErrorEvent):void 
		{
			this.log('Socket Security Error');
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
		
		public function get client():Socket
		{
			return this._clientSocket;
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