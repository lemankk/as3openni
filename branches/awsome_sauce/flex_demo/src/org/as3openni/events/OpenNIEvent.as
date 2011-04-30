package org.as3openni.events
{
	import flash.events.Event;
	
	public class OpenNIEvent extends Event
	{
		public static const ON_VIDEO:String = "onVideo";
		public static const ON_DEPTH:String = "onDepth";
		public static const ON_USERS:String = "onUsers";
		public static const ON_SKELETON:String = "onSkeleton";
		
		public var data:*;
		public function OpenNIEvent(type:String, data:*)
		{
			this.data = data;
			super(type);
		}
	}
}