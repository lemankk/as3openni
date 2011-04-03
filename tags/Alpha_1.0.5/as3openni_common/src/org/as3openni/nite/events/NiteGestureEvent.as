package org.as3openni.nite.events
{
	import flash.events.Event;
	
	public class NiteGestureEvent extends Event
	{
		public static const WAVE:String = "event:wave";
		public static const STEADY:String = "event:steady";
		public static const PUSH:String = "event:push";
		public static const SWIPE_DOWN:String = "event:swipe_down";
		public static const SWIPE_UP:String = "event:swipe_up";
		public static const SWIPE_LEFT:String = "event:swipe_left";
		public static const SWIPE_RIGHT:String = "event:swipe_right";
		
		public function NiteGestureEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
	}
}