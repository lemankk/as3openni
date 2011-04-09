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
		private var _centerRadians:Number;
		private var _centerDegrees:Number;
		
		private var _configLeftFoot:NiPoint3D;
		private var _configRightFoot:NiPoint3D;
		private var _configHead:NiPoint3D;
		private var _configTorso:NiPoint3D;
		
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
			var leftShoulder:NiPoint3D = skeleton.leftShoulder;
			var leftFoot:NiPoint3D = skeleton.leftFoot;
			var leftKnee:NiPoint3D = skeleton.leftKnee;
			var rightFoot:NiPoint3D = skeleton.rightFoot;
			var rightKnee:NiPoint3D = skeleton.rightKnee;
			var rightShoulder:NiPoint3D = skeleton.rightShoulder;
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
				this._configTorso = torso;
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
				
				// Detect center angle.
				if(!this._centerDegrees)
				{
					this._centerRadians = Math.atan2((torso.pointY - this._configTorso.pointY), (torso.pointX - this._configTorso.pointX));
					this._centerDegrees = Math.round(this._centerRadians*180/Math.PI);
				}
			}
			
			// Detect the full body angles.
			var fullBodyRadians:Number = Math.atan2((torso.pointY - this._configTorso.pointY), (torso.pointX - this._configTorso.pointX));
			var fullBodyDegrees:Number = Math.abs(Math.round(fullBodyRadians*180/Math.PI));
			
			// Detect if the user is leaning left or right.
			if(fullBodyDegrees < 180)
			{
				if(fullBodyDegrees < 173 && fullBodyDegrees > 167)
				{
					this.dispatchEvent(new FAASTEvent(FAASTEvent.LEAN_LEFT, 0, 0, fullBodyDegrees));
				}
				
				if(fullBodyDegrees > 5 && fullBodyDegrees < 35)
				{
					this.dispatchEvent(new FAASTEvent(FAASTEvent.LEAN_RIGHT, 0, 0, fullBodyDegrees));
				}
			}
		}
	}
}