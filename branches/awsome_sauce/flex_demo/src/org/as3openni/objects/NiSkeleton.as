package org.as3openni.objects
{
	public class NiSkeleton
	{
		import flash.utils.ByteArray;
		
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
			this.head.x = bytes.readFloat();
			this.head.y = bytes.readFloat();
			this.head.z = bytes.readFloat();
			
			this.neck.x = bytes.readFloat();
			this.neck.y = bytes.readFloat();
			this.head.z = bytes.readFloat();
			
			this.torso.x = bytes.readFloat();
			this.torso.y = bytes.readFloat();
			this.torso.z = bytes.readFloat();
			
			this.leftShoulder.x = bytes.readFloat();
			this.leftShoulder.y = bytes.readFloat();
			this.leftShoulder.z = bytes.readFloat();
			
			this.leftElbow.x = bytes.readFloat();
			this.leftElbow.y = bytes.readFloat();
			this.leftElbow.z = bytes.readFloat();
			
			this.leftHand.x = bytes.readFloat();
			this.leftHand.y = bytes.readFloat();
			this.leftHand.z = bytes.readFloat();
			
			this.rightShoulder.x = bytes.readFloat();
			this.rightShoulder.y = bytes.readFloat();
			this.rightShoulder.z = bytes.readFloat();
			
			this.rightElbow.x = bytes.readFloat();
			this.rightElbow.y = bytes.readFloat();
			this.rightElbow.z = bytes.readFloat();
			
			this.rightHand.x = bytes.readFloat();
			this.rightHand.y = bytes.readFloat();
			this.rightHand.z = bytes.readFloat();
			
			this.leftHip.x = bytes.readFloat();
			this.leftHip.y = bytes.readFloat();
			this.leftHip.z = bytes.readFloat();
			
			this.leftKnee.x = bytes.readFloat();
			this.leftKnee.y = bytes.readFloat();
			this.leftKnee.z = bytes.readFloat();
			
			this.leftFoot.x = bytes.readFloat();
			this.leftFoot.y = bytes.readFloat();
			this.leftFoot.z = bytes.readFloat();
			
			this.rightHip.x = bytes.readFloat();
			this.rightHip.y = bytes.readFloat();
			this.rightHip.z = bytes.readFloat();
			
			this.rightKnee.x = bytes.readFloat();
			this.rightKnee.y = bytes.readFloat();
			this.rightKnee.z = bytes.readFloat();
			
			this.rightFoot.x = bytes.readFloat();
			this.rightFoot.y = bytes.readFloat();
			this.rightFoot.z = bytes.readFloat();
		}
	}
}