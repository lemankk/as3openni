package org.as3openni.openni.events
{
	import flash.events.Event;
	
	import org.as3openni.objects.NiPoint3D;
	import org.as3openni.objects.NiSkeleton;
	
	public class ONISkeletonEvent extends Event
	{
		public static const USER_TRACKING:String = "user_tracking:";
		
		public var user:Number = 0;
		public var leftHand:NiPoint3D;
		public var rightHand:NiPoint3D;
		public var skeleton:NiSkeleton;
		
		public function ONISkeletonEvent(type:String, user:Number, leftHand:NiPoint3D, rightHand:NiPoint3D,
										 	skeleton:NiSkeleton, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			this.user = user;
			this.leftHand = leftHand;
			this.rightHand = rightHand;
			this.skeleton = skeleton;
			
			super(type, bubbles, cancelable);
		}
	}
}