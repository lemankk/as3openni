package org.as3openni.flash.nite.events
{
	import flash.events.Event;
	
	import org.as3openni.flash.objects.NiPoint3D;
	
	public class NitePointEvent extends Event
	{
		public static const POINT_DETECTED:String = "event:point";
		public var point3D:NiPoint3D;
		
		public function NitePointEvent(type:String, point3D:NiPoint3D, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			this.point3D = point3D;
			super(type, bubbles, cancelable);
		}
	}
}