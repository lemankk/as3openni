package org.as3openni.faast.gestures
{
	import flash.events.EventDispatcher;
	
	import org.as3openni.faast.events.FAASTEvent;
	import org.as3openni.objects.NiPoint3D;
	import org.as3openni.objects.NiSkeleton;
	import org.as3openni.util.math.NiPoint3DUtil;

	public class FAASTLeftArmGestures extends FAASTBasicGestures
	{
		public function FAASTLeftArmGestures(senseRange:Number = 75, useInches:Boolean = false)
		{
			super(senseRange, useInches);
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
			
			// Left Arm Out.
			if(leftArmRangeY <= this.senseRange && leftHand.pointX < leftShoulder.pointX 
				&& leftArmRangeZ <= this.senseRange)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(leftArmRangeX) : leftArmRangeX;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.LEFT_ARM_OUT, this.distance));
			}
			
			// Left Arm Across.
			if(leftArmRangeY <= this.senseRange && leftHand.pointX > leftShoulder.pointX 
				&& leftArmRangeZ <= this.senseRange)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(leftArmRangeX) : leftArmRangeX;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.LEFT_ARM_ACCROSS, this.distance));
			}
			
			// Left Arm Forward.
			if(leftArmRangeX <= this.senseRange && leftHand.pointZ < leftShoulder.pointZ
				&& leftArmRangeY <= this.senseRange)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(leftArmRangeZ) : leftArmRangeZ;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.LEFT_ARM_FORWARD, this.distance));
			}
			
			// Left Arm Up.
			if(leftArmRangeX <= this.senseRange && leftHand.pointY > leftShoulder.pointY 
				&& leftArmRangeZ <= (this.senseRange*3))
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(leftArmRangeY) : leftArmRangeY;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.LEFT_ARM_UP, this.distance));
			}
			
			// Left Arm Down.
			if(leftArmRangeX <= this.senseRange && leftHand.pointY < leftShoulder.pointY 
				&& leftArmRangeZ <= (this.senseRange*3))
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(leftArmRangeY) : leftArmRangeY;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.LEFT_ARM_DOWN, this.distance));
			}
		}
	}
}