package org.as3openni.objects
{
	import flash.utils.ByteArray;

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
		
		public function NiSkeleton()
		{
			this.head = new NiPoint3D();
			this.neck = new NiPoint3D();
			this.torso = new NiPoint3D();
			
			this.leftShoulder = new NiPoint3D();
			this.leftElbow = new NiPoint3D();
			this.leftHand = new NiPoint3D();
			
			this.rightShoulder = new NiPoint3D();
			this.rightElbow = new NiPoint3D();
			this.rightHand = new NiPoint3D();
			
			this.leftHip = new NiPoint3D();
			this.leftKnee = new NiPoint3D();
			this.leftFoot = new NiPoint3D();
			
			this.rightHip = new NiPoint3D();
			this.rightKnee = new NiPoint3D();
			this.rightFoot = new NiPoint3D();
		}
		
		public function update(bytes:ByteArray):void
		{
			this.head.pointX = bytes.readFloat();
			this.head.pointY = bytes.readFloat();
			this.head.pointZ = bytes.readFloat();
			
			this.neck.pointX = bytes.readFloat();
			this.neck.pointY = bytes.readFloat();
			this.neck.pointZ = bytes.readFloat();
			
			this.torso.pointX = bytes.readFloat();
			this.torso.pointY = bytes.readFloat();
			this.torso.pointZ = bytes.readFloat();
			
			this.leftShoulder.pointX = bytes.readFloat();
			this.leftShoulder.pointY = bytes.readFloat();
			this.leftShoulder.pointZ = bytes.readFloat();
			
			this.leftElbow.pointX = bytes.readFloat();
			this.leftElbow.pointY = bytes.readFloat();
			this.leftElbow.pointZ = bytes.readFloat();
			
			this.leftHand.pointX = bytes.readFloat();
			this.leftHand.pointY = bytes.readFloat();
			this.leftHand.pointZ = bytes.readFloat();
			
			this.rightShoulder.pointX = bytes.readFloat();
			this.rightShoulder.pointY = bytes.readFloat();
			this.rightShoulder.pointZ = bytes.readFloat();
			
			this.rightElbow.pointX = bytes.readFloat();
			this.rightElbow.pointY = bytes.readFloat();
			this.rightElbow.pointZ = bytes.readFloat();
			
			this.rightHand.pointX = bytes.readFloat();
			this.rightHand.pointY = bytes.readFloat();
			this.rightHand.pointZ = bytes.readFloat();
			
			this.leftHip.pointX = bytes.readFloat();
			this.leftHip.pointY = bytes.readFloat();
			this.leftHip.pointZ = bytes.readFloat();
			
			this.leftKnee.pointX = bytes.readFloat();
			this.leftKnee.pointY = bytes.readFloat();
			this.leftKnee.pointZ = bytes.readFloat();
			
			this.leftFoot.pointX = bytes.readFloat();
			this.leftFoot.pointY = bytes.readFloat();
			this.leftFoot.pointZ = bytes.readFloat();
			
			this.rightHip.pointX = bytes.readFloat();
			this.rightHip.pointY = bytes.readFloat();
			this.rightHip.pointZ = bytes.readFloat();
			
			this.rightKnee.pointX = bytes.readFloat();
			this.rightKnee.pointY = bytes.readFloat();
			this.rightKnee.pointZ = bytes.readFloat();
			
			this.rightFoot.pointX = bytes.readFloat();
			this.rightFoot.pointY = bytes.readFloat();
			this.rightFoot.pointZ = bytes.readFloat();
		}
	}
}