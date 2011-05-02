package org.as3openni.events.openni
{
	import flash.events.Event;
	
	import org.as3openni.objects.NiPoint3D;
	import org.as3openni.objects.NiSkeleton;
	
	public class SkeletonEvent extends Event
	{
		public static const SKELETONS:String = "skeletons";
		
		public var userId:uint;
		public var skeleton:NiSkeleton;
		
		public function SkeletonEvent(type:String, userId:uint, skeleton:NiSkeleton)
		{
			this.userId = userId;
			this.skeleton = skeleton;
			super(type);
		}
	}
}