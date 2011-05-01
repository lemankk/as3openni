package org.as3openni.events
{
	import flash.events.Event;
	
	import org.as3openni.objects.NiPoint3D;
	
	public class UserTrackingEvent extends Event
	{
		public static const USER_FOUND:String = "userFound";
		
		public var userId:uint;
		public var point3d:NiPoint3D;
		
		public function UserTrackingEvent(type:String, userId:uint, point3d:NiPoint3D)
		{
			this.userId = userId;
			this.point3d = point3d;
			super(type);
		}
	}
}