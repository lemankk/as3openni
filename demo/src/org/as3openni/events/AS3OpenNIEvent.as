package org.as3openni.events
{
	import flash.events.Event;

	public class AS3OpenNIEvent extends Event
	{
		public static const READY:String = "ready";
		
		public function AS3OpenNIEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
	}
}