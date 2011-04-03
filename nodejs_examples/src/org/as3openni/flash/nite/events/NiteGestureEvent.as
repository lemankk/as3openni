package org.as3openni.flash.nite.events
{
	import flash.events.Event;
	
	public class NiteGestureEvent extends Event
	{
		public static const WAVE:String = "event:wave";
		public static const STEADY:String = "steady:";
		public static const PUSH:String = "push:";
		public static const SWIPE_DOWN:String = "swipe_down:";
		public static const SWIPE_UP:String = "swipe_up:";
		public static const SWIPE_LEFT:String = "swipe_left:";
		public static const SWIPE_RIGHT:String = "swipe_right:";
		
		public var velocity:Number = 0;
		public var angle:Number = 0;
		
		public function NiteGestureEvent(type:String, velocity:Number = 0, angle:Number = 0, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			this.velocity = velocity;
			this.angle = angle;
			super(type, bubbles, cancelable);
		}
	}
}