package org.as3openni.faast.events
{
	import flash.events.Event;
	
	public class FAASTEvent extends Event
	{
		public static const LEFT_ARM_FORWARD:String = "event:left_arm_forward";
		public static const LEFT_ARM_DOWN:String = "event:left_arm_down";
		public static const LEFT_ARM_UP:String = "event:left_arm_up";
		public static const LEFT_ARM_OUT:String = "event:left_arm_out";
		public static const LEFT_ARM_ACCROSS:String = "event:left_arm_across";
		
		public static const RIGHT_ARM_FORWARD:String = "event:right_arm_forward";
		public static const RIGHT_ARM_DOWN:String = "event:right_arm_down";
		public static const RIGHT_ARM_UP:String = "event:right_arm_up";
		public static const RIGHT_ARM_OUT:String = "event:right_arm_out";
		public static const RIGHT_ARM_ACCROSS:String = "event:right_arm_across";
		
		public var value:Number;
		
		public function FAASTEvent(type:String, value:Number = 0, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			this.value = value;
			super(type, bubbles, cancelable);
		}
	}
}