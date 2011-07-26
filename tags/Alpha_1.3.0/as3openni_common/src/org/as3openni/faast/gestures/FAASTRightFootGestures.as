package org.as3openni.faast.gestures
{
	import flash.events.EventDispatcher;
	
	import org.as3openni.faast.events.FAASTEvent;
	import org.as3openni.objects.NiPoint3D;
	import org.as3openni.objects.NiSkeleton;
	import org.as3openni.util.math.NiPoint3DUtil;

	public class FAASTRightFootGestures extends FAASTBasicGestures
	{
		public function FAASTRightFootGestures(useInches:Boolean = false)
		{
			super(useInches);
		}
		
		override public function configure(skeleton:NiSkeleton):void
		{
			// Call the super.
			super.configure(skeleton);
			
			// Define Left Foot.
			var leftFoot:NiPoint3D = skeleton.leftFoot;
			
			// Define Right Foot.
			var rightHip:NiPoint3D = skeleton.rightHip;
			var rightKnee:NiPoint3D = skeleton.rightKnee;
			var rightFoot:NiPoint3D = skeleton.rightFoot;
			
			// Define Ranges.
			var rightFootRangeX:Number = (Math.max(rightHip.pointX, rightKnee.pointX, rightFoot.pointX) - Math.min(rightHip.pointX, rightKnee.pointX, rightFoot.pointX));
			var rightFootRangeY:Number = (Math.max(rightHip.pointY, rightKnee.pointY, rightFoot.pointY) - Math.min(rightHip.pointY, rightKnee.pointY, rightFoot.pointY));
			var rightFootRangeZ:Number = (Math.max(rightHip.pointZ, rightKnee.pointZ, rightFoot.pointZ) - Math.min(rightHip.pointZ, rightKnee.pointZ, rightFoot.pointZ));
			var rightFootRange:Number = (rightFootRangeX + rightFootRangeY + rightFootRangeZ)/3;
			
			// Right Foot Sideways.
			if(rightFootRangeY <= rightFootRange && rightFootRangeZ <= rightFootRange
				&& rightFoot.pointX > rightHip.pointX)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(rightFootRangeX) : rightFootRangeX;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.RIGHT_FOOT_SIDEWAYS, this.distance));
			}
			
			// Right Foot Forward.
			if(rightFootRangeX <= rightFootRange && rightFootRangeZ >= rightFootRange
				&& rightFoot.pointZ < leftFoot.pointZ)
			{
				var footHeight:Number = Math.abs(rightFoot.pointY-leftFoot.pointY);
				footHeight = (this.useInches) ? NiPoint3DUtil.convertMMToInches(footHeight) : footHeight;
				
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(rightFootRangeY) : rightFootRangeY;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.RIGHT_FOOT_FORWARD, this.distance, footHeight));
			}
			
			// Right Foot Backward.
			if(rightFootRangeX <= rightFootRange && rightFootRangeZ >= rightFootRange
				&& rightFoot.pointZ > leftFoot.pointZ)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(rightFootRangeY) : rightFootRangeY;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.RIGHT_FOOT_BACKWARD, this.distance));
			}
			
			// Right Foot Up.
			if(rightFootRangeX <= rightFootRange && rightFootRangeY <= rightFootRange
				&& rightFoot.pointY > leftFoot.pointY)
			{
				var val:Number = Math.abs(rightFoot.pointZ-leftFoot.pointZ);
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(val) : val;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.RIGHT_FOOT_UP, this.distance));
			}
		}
	}
}