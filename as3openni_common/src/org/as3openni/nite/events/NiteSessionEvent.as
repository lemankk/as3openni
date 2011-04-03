package org.as3openni.nite.events
{
	import flash.events.Event;
	
	public class NiteSessionEvent extends Event
	{
		public static const SESSION_STARTED:String = "event:session_started";
		public static const SESSION_PROGRESS:String = "event:session_progress";
		public static const SESSION_ENDED:String = "event:session_ended";
		
		public function NiteSessionEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
	}
}