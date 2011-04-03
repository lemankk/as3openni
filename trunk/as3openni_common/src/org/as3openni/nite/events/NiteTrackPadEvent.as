package org.as3openni.nite.events
{
	import flash.events.Event;
	
	public class NiteTrackPadEvent extends Event
	{
		public static const TRACKPAD_CREATED:String = "event:trackpad_created";
		public static const TRACKPAD_DESTROYED:String = "event:trackpad_destroyed";
		public static const TRACKPAD_HOVER:String = "trackpad_hover:";
		public static const TRACKPAD_UP:String = "Backward";
		public static const TRACKPAD_DOWN:String = "Forward";
		
		public var column:Number;
		public var row:Number;
		public var direction:String = "";
		
		public function NiteTrackPadEvent(type:String, column:Number = 0, row:Number = 0, 
										  direction:String = "", bubbles:Boolean=false, cancelable:Boolean=false)
		{
			this.column = column;
			this.row = row;
			this.direction = direction;
			super(type, bubbles, cancelable);
		}
	}
}