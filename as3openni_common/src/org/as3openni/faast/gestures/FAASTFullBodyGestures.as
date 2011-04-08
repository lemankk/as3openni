package org.as3openni.faast.gestures
{
	import flash.events.EventDispatcher;
	
	import org.as3openni.faast.events.FAASTEvent;
	import org.as3openni.objects.NiPoint3D;
	import org.as3openni.objects.NiSkeleton;
	import org.as3openni.util.math.NiPoint3DUtil;

	public class FAASTFullBodyGestures extends FAASTBasicGestures
	{
		private var _configHeight:Number;
		private var _configLeftFoot:NiPoint3D;
		private var _configRightFoot:NiPoint3D;
		private var _configHead:NiPoint3D;
		private var _jumpTolerance:Number = 50;
		
		public function FAASTFullBodyGestures(useInches:Boolean = false)
		{
			super(useInches);
		}
		
		override public function configure(skeleton:NiSkeleton):void
		{
			// Call the super.
			super.configure(skeleton);
			
			// Define Feet and Head.
			var leftFoot:NiPoint3D = skeleton.leftFoot;
			var leftKnee:NiPoint3D = skeleton.leftKnee;
			var rightFoot:NiPoint3D = skeleton.rightFoot;
			var rightKnee:NiPoint3D = skeleton.rightKnee;
			var head:NiPoint3D = skeleton.head;
			var torso:NiPoint3D = skeleton.torso;
			
			// Define the heights.
			var fullBodyHeight:Number = (Math.max(leftFoot.pointY, rightFoot.pointY, head.pointY) - Math.min(leftFoot.pointY, rightFoot.pointY, head.pointY));
			var torsoHeight:Number = (Math.max(torso.pointY, head.pointY) - Math.min(torso.pointY, head.pointY));
			
			// Grab the first recorded full body height.
			if(!_configHeight)
			{
				this._configHeight = fullBodyHeight;
				this._configLeftFoot = leftFoot;
				this._configRightFoot = rightFoot;
				this._configHead = head;
			}
			
			// Detect if the user is crouching.
			if(fullBodyHeight <= ((this._configHeight/2) + torsoHeight))
			{
				var diffHeight:Number = Math.abs(this._configHeight-fullBodyHeight);
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(diffHeight) : diffHeight;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.CROUCHED, this.distance));
			}
			
			// Detect if the user is in the standing position first.
			if(fullBodyHeight >= (this._configHeight - torsoHeight))
			{
				// Detect if the user is jumping.
				if(head.pointY >= this._configHead.pointY)
				{
					var leftFootDis:Number = Math.abs(leftFoot.pointY - this._configLeftFoot.pointY);
					var rightFootDis:Number = Math.abs(rightFoot.pointY - this._configRightFoot.pointY);
					
					if(leftFootDis >= this._jumpTolerance && rightFootDis >= this._jumpTolerance)
					{
						var sum:Number = (leftFootDis + rightFootDis)/2;
						this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(sum) : sum;
						this.dispatchEvent(new FAASTEvent(FAASTEvent.JUMPING, this.distance));
					}
				}
			}
		}
	}
}