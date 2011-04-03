package org.as3openni.flash.nite.events
{
	import flash.events.Event;
	
	public class NiteCircleEvent extends Event
	{
		public static const NO_CIRCLE:String = "event:no_circle";
		public static const ANGLE:String = "circle_angle:";
		public static const CIRCLE_CREATED:String = "event:circle_created";
		public static const CIRCLE_DESTROYED:String = "event:circle_destroyed";
		
		public var angle:Number;
		public var clockwise:Boolean = true;
		
		public function NiteCircleEvent(type:String, angle:Number = 0, clockwise:Boolean = true, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			this.angle = angle;
			this.clockwise = clockwise;
			super(type, bubbles, cancelable);
		}
	}
}