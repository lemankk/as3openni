package org.as3openni.flash.objects
{
	public class NiSkeleton
	{
		public var user:Number;
		
		public var head:NiPoint3D;
		public var neck:NiPoint3D;
		public var torso:NiPoint3D;
		
		public var leftShoulder:NiPoint3D;
		public var leftElbow:NiPoint3D;
		public var leftHand:NiPoint3D;
		
		public var rightShoulder:NiPoint3D;
		public var rightElbow:NiPoint3D;
		public var rightHand:NiPoint3D;
		
		public var leftHip:NiPoint3D;
		public var leftKnee:NiPoint3D;
		public var leftFoot:NiPoint3D;
		
		public var rightHip:NiPoint3D;
		public var rightKnee:NiPoint3D;
		public var rightFoot:NiPoint3D;
		
		public function NiSkeleton() {}
	}
}