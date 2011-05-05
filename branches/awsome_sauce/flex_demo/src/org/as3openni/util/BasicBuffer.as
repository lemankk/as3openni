package org.as3openni.util
{
	import flash.net.Socket;
	import flash.utils.ByteArray;
	
	import org.as3openni.global.Definitions;

	public class BasicBuffer
	{
		private var _socket:ClientSocket;
		private var _data:ByteArray;
		private var _busy:Boolean = false;
		
		public function BasicBuffer(socket:ClientSocket)
		{
			this._socket = socket as ClientSocket;
			this._data = new ByteArray();
		}
		
		public function getBuffer():void
		{
			if(!this.busy)
			{
				this._busy = true;
				this._data.clear();
				this.writeBytes();
				this._data.writeInt(0);
				if(this._socket.sendCommand(this._data) != Definitions.SUCCESS)
				{
					throw new Error(Definitions.AS3OPENNI_LABEL + 'Data was not complete');
				}
			}
		}
		
		protected function writeBytes():void
		{
			// Abstract, this class will be subclassed.
		}
		
		public function set busy(flag:Boolean):void 
		{
			this._busy = flag;
		}
		
		public function get busy():Boolean 
		{
			return this._busy;
		}
		
		public function get data():ByteArray 
		{
			return this._data;
		}
	}
}