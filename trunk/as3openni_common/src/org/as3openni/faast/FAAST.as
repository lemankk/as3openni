package org.as3openni.faast
{
	import org.as3openni.faast.gestures.FAASTLeftArmGestures;
	import org.as3openni.faast.gestures.FAASTLeftFootGestures;
	import org.as3openni.faast.gestures.FAASTRightArmGestures;
	import org.as3openni.faast.gestures.FAASTRightFootGestures;
	import org.as3openni.objects.NiSkeleton;
	
	public class FAAST
	{
		public var leftArmGestures:FAASTLeftArmGestures;
		public var rightArmGestures:FAASTRightArmGestures;
		public var leftFootGestures:FAASTLeftFootGestures;
		public var rightFootGestures:FAASTRightFootGestures;
		
		public function FAAST(useInches:Boolean = false)
		{
			super();
			
			this.leftArmGestures = new FAASTLeftArmGestures(useInches);
			this.rightArmGestures = new FAASTRightArmGestures(useInches);
			this.leftFootGestures = new FAASTLeftFootGestures(useInches);
			this.rightFootGestures = new FAASTRightFootGestures(useInches);
		}
		
		public function configure(skeleton:NiSkeleton):void
		{
			if(skeleton)
			{
				this.leftArmGestures.configure(skeleton);
				this.rightArmGestures.configure(skeleton);
				this.leftFootGestures.configure(skeleton);
				this.rightFootGestures.configure(skeleton);
			}
		}
	}
}