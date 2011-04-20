package org.as3openni.util
{
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.events.IOErrorEvent;
	import flash.events.ProgressEvent;
	import flash.events.ServerSocketConnectEvent;
	import flash.net.ServerSocket;
	import flash.net.Socket;
	
	public class BasicSocket extends EventDispatcher
	{
		protected var _traceLog:Boolean = false;
		protected var _name:String = "basic_socket";
		protected var _port:int = 9500;
		protected var _address:String = "127.0.0.1";
		protected var _serverSocket:ServerSocket = new ServerSocket();
		protected var _clientSocket:Socket;
		protected var _socketMessage:String;
		
		public function BasicSocket()
		{
			this._serverSocket.addEventListener(ServerSocketConnectEvent.CONNECT, this.onConnect);
			this._serverSocket.addEventListener(IOErrorEvent.IO_ERROR, this.onError);
		}
		
		public function bind():void
		{
			if(this._serverSocket)
			{
				if(this._serverSocket.bound) 
				{
					this._serverSocket.close();
					this._serverSocket = new ServerSocket();
				}
				else
				{	
					try 
					{
						this._serverSocket.bind(this.port, this.address);
						this._serverSocket.listen();
						
						trace(this.name + " - Bound to: " + this._serverSocket.localAddress + ":" + this._serverSocket.localPort);
					} 
					catch(e:Error) 
					{ 
						this.log(e); 
					} 
				}
			}
		}
		
		public function send(value:String):void
		{
			try
			{
				if(this._clientSocket != null && this._clientSocket.connected)
				{
					this._clientSocket.writeUTFBytes(value);
					this._clientSocket.flush(); 
					this.log("Sent message to " + this._clientSocket.remoteAddress + ":" + this._clientSocket.remotePort);
				}
				else this.log("No socket connection.");
			}
			catch (error:Error)
			{
				this.log(error.message);
			}
		}
		
		protected function onError(event:IOErrorEvent):void
		{
			this.log("Error: The port was already in use.");
		}
		
		protected function onConnect(event:ServerSocketConnectEvent):void
		{
			this._clientSocket = event.socket;
			this._clientSocket.addEventListener(ProgressEvent.SOCKET_DATA, this.onClientSocketData);
			trace(this.name + " - Connection from: " + this._clientSocket.remoteAddress + ":" + this._clientSocket.remotePort);
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
		
		public function get server():ServerSocket
		{
			return this._serverSocket;
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