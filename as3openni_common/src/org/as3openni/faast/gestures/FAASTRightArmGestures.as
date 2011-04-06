package org.as3openni.faast.gestures
{
	import flash.events.EventDispatcher;
	
	import org.as3openni.faast.events.FAASTEvent;
	import org.as3openni.objects.NiPoint3D;
	import org.as3openni.objects.NiSkeleton;
	import org.as3openni.util.math.NiPoint3DUtil;

	public class FAASTRightArmGestures extends FAASTBasicGestures
	{
		public function FAASTRightArmGestures(senseRange:Number = 75, useInches:Boolean = false)
		{
			super(senseRange, useInches);
		}
		
		override public function configure(skeleton:NiSkeleton):void
		{
			// Call the super.
			super.configure(skeleton);
			
			// Define Right Arm.
			var rightShoulder:NiPoint3D = skeleton.rightShoulder;
			var rightElbow:NiPoint3D = skeleton.rightElbow;
			var rightHand:NiPoint3D = skeleton.rightHand;
			
			var rightArmRangeX:Number = (Math.max(rightShoulder.pointX, rightElbow.pointX, rightHand.pointX) - Math.min(rightShoulder.pointX, rightElbow.pointX, rightHand.pointX));
			var rightArmRangeY:Number = (Math.max(rightShoulder.pointY, rightElbow.pointY, rightHand.pointY) - Math.min(rightShoulder.pointY, rightElbow.pointY, rightHand.pointY));
			var rightArmRangeZ:Number = (Math.max(rightShoulder.pointZ, rightElbow.pointZ, rightHand.pointZ) - Math.min(rightShoulder.pointZ, rightElbow.pointZ, rightHand.pointZ));
			
			// Right Arm Out.
			if(rightArmRangeY <= this.senseRange && rightHand.pointX > rightShoulder.pointX 
				&& rightArmRangeZ <= this.senseRange)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(rightArmRangeX) : rightArmRangeX;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.RIGHT_ARM_OUT, this.distance));
			}
			
			// Right Arm Across.
			if(rightArmRangeY <= this.senseRange && rightHand.pointX < rightShoulder.pointY 
				&& rightArmRangeZ <= this.senseRange)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(rightArmRangeX) : rightArmRangeX;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.RIGHT_ARM_ACCROSS, this.distance));
			}
			
			// Right Arm Forward.
			if(rightArmRangeX <= this.senseRange && rightHand.pointZ < rightShoulder.pointZ
				&& rightArmRangeY <= this.senseRange)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(rightArmRangeZ) : rightArmRangeZ;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.RIGHT_ARM_FORWARD, this.distance));
			}
			
			// Right Arm Up.
			if(rightArmRangeX <= this.senseRange && rightHand.pointY > rightShoulder.pointY 
				&& rightArmRangeZ <= (this.senseRange*3))
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(rightArmRangeY) : rightArmRangeY;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.RIGHT_ARM_UP, this.distance));
			}
			
			// Right Arm Down.
			if(rightArmRangeX <= this.senseRange && rightHand.pointY < rightShoulder.pointY 
				&& rightArmRangeZ <= (this.senseRange*3))
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(rightArmRangeY) : rightArmRangeY;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.RIGHT_ARM_DOWN, this.distance));
			}
		}
	}
}