package org.as3openni.flash.nite.events
{
	import flash.events.Event;
	
	public class NiteSliderEvent extends Event
	{
		public static const SLIDER_LEFT:String = "left";
		public static const SLIDER_RIGHT:String = "right";
		
		public static const SLIDER_UP:String = "up";
		public static const SLIDER_DOWN:String = "down";
		
		public static const SLIDER_IN:String = "in";
		public static const SLIDER_OUT:String = "out";
		
		public static const SLIDER_LEFT_RIGHT_VALUE:String = "slider_leftright_value:";
		public static const SLIDER_UP_DOWN_VALUE:String = "slider_updown_value:";
		public static const SLIDER_IN_OUT_VALUE:String = "slider_inout_value:";
		
		public static const SLIDER_LEFT_RIGHT_CREATED:String = "event:slider_leftright_created";
		public static const SLIDER_LEFT_RIGHT_DESTROYED:String = "event:slider_leftright_destroyed";
		
		public static const SLIDER_UP_DOWN_CREATED:String = "event:slider_updown_created";
		public static const SLIDER_UP_DOWN_DESTROYED:String = "event:slider_updown_destroyed";
		
		public static const SLIDER_IN_OUT_CREATED:String = "event:slider_inout_created";
		public static const SLIDER_IN_OUT_DESTROYED:String = "event:slider_inout_destroyed";
		
		public var value:Number = 0;
		public var direction:String = "";
		
		public function NiteSliderEvent(type:String, value:Number = 0, direction:String = '', bubbles:Boolean=false, cancelable:Boolean=false)
		{
			this.value = value;
			this.direction = direction;
			super(type, bubbles, cancelable);
		}
	}
}