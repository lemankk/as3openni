package org.as3openni.faast.gestures
{
	import flash.events.EventDispatcher;
	
	import org.as3openni.objects.NiSkeleton;

	public class FAASTBasicGesture extends EventDispatcher
	{
		public var distance:Number = 0;
		public var senseRange:Number = 75;
		public var useInches:Boolean = false;
		
		public function FAASTBasicGesture(senseRange:Number = 75, useInches:Boolean = false)
		{
			this.senseRange = senseRange;
			this.useInches = useInches;
		}
		
		public function configure(skeleton:NiSkeleton):void
		{
			// Abstract.
		}
	}
}