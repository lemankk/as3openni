package org.as3openni.faast.gestures
{
	import flash.events.EventDispatcher;
	
	import org.as3openni.objects.NiSkeleton;

	public class FAASTBasicGestures extends EventDispatcher
	{
		public var distance:Number = 0;
		public var useInches:Boolean = false;
		public var skeleton:NiSkeleton;
		
		public function FAASTBasicGestures(useInches:Boolean = false)
		{
			this.useInches = useInches;
		}
		
		public function configure(skeleton:NiSkeleton):void
		{
			this.skeleton = skeleton;
		}
	}
}