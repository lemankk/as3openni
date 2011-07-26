package org.as3openni.events
{
	import flash.events.Event;
	
	public class ClientSocketEvent extends Event
	{
		public static const ON_CONNECT:String = "onConnect";
		public static const ON_DATA:String = "onData";
		public static const ON_ERROR:String = "onError";
		
		public var data:*;
		public function ClientSocketEvent(type:String, data:*)
		{
			this.data = data;
			super(type);
		}
	}
}