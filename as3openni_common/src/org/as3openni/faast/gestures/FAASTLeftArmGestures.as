package org.as3openni.faast.gestures
{
	import flash.events.EventDispatcher;
	
	import org.as3openni.faast.events.FAASTEvent;
	import org.as3openni.objects.NiPoint3D;
	import org.as3openni.objects.NiSkeleton;
	import org.as3openni.util.math.NiPoint3DUtil;

	public class FAASTLeftArmGestures extends FAASTBasicGestures
	{
		public function FAASTLeftArmGestures(useInches:Boolean = false)
		{
			super(useInches);
		}
		
		override public function configure(skeleton:NiSkeleton):void
		{
			// Call the super.
			super.configure(skeleton);
			
			// Define Left Arm.
			var leftShoulder:NiPoint3D = skeleton.leftShoulder;
			var leftElbow:NiPoint3D = skeleton.leftElbow;
			var leftHand:NiPoint3D = skeleton.leftHand;
			
			var leftArmRangeX:Number = (Math.max(leftShoulder.pointX, leftElbow.pointX, leftHand.pointX) - Math.min(leftShoulder.pointX, leftElbow.pointX, leftHand.pointX));
			var leftArmRangeY:Number = (Math.max(leftShoulder.pointY, leftElbow.pointY, leftHand.pointY) - Math.min(leftShoulder.pointY, leftElbow.pointY, leftHand.pointY));
			var leftArmRangeZ:Number = (Math.max(leftShoulder.pointZ, leftElbow.pointZ, leftHand.pointZ) - Math.min(leftShoulder.pointZ, leftElbow.pointZ, leftHand.pointZ));
			var leftArmRange:Number = (leftArmRangeX + leftArmRangeY + leftArmRangeZ)/3;
			
			// Left Arm Out.
			if(leftArmRangeY <= leftArmRange && leftHand.pointX < leftShoulder.pointX 
				&& leftArmRangeZ <= leftArmRange)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(leftArmRangeX) : leftArmRangeX;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.LEFT_ARM_OUT, this.distance));
			}
			
			// Left Arm Across.
			if(leftArmRangeY <= leftArmRange && leftHand.pointX > leftShoulder.pointX 
				&& leftArmRangeZ <= leftArmRange)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(leftArmRangeX) : leftArmRangeX;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.LEFT_ARM_ACCROSS, this.distance));
			}
			
			// Left Arm Forward.
			if(leftArmRangeX <= leftArmRange && leftHand.pointZ < leftShoulder.pointZ
				&& leftArmRangeY <= leftArmRange)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(leftArmRangeZ) : leftArmRangeZ;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.LEFT_ARM_FORWARD, this.distance));
			}
			
			// Left Arm Up.
			if(leftArmRangeX <= leftArmRange && leftHand.pointY > leftShoulder.pointY 
				&& leftArmRangeZ <= leftArmRange)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(leftArmRangeY) : leftArmRangeY;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.LEFT_ARM_UP, this.distance));
			}
			
			// Left Arm Down.
			if(leftArmRangeX <= leftArmRange && leftHand.pointY < leftShoulder.pointY 
				&& leftArmRangeZ <= leftArmRange)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(leftArmRangeY) : leftArmRangeY;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.LEFT_ARM_DOWN, this.distance));
			}
		}
	}
}