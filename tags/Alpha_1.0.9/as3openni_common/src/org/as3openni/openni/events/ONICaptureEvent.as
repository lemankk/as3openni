package org.as3openni.openni.events
{
	import flash.events.Event;
	import flash.utils.ByteArray;
	
	public class ONICaptureEvent extends Event
	{
		public static const ONI_DEPTH_MAP:String = "oni_depth_map";
		public static const ONI_RGB:String = "oni_rgb";
		
		public var bytes:ByteArray;
		
		public function ONICaptureEvent(type:String, bytes:ByteArray, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			this.bytes = bytes;
			super(type, bubbles, cancelable);
		}
	}
}