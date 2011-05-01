package org.as3openni.objects
{
	public class NiPoint3D
	{
		public var x:Number;
		public var y:Number;
		public var z:Number;
		
		public function NiPoint3D(){}
		
		public function toString():String
		{
			var str:String = 'NiPoint3D:(x: ' + this.x + ', y: ' + this.y + ', z: ' + this.z + ')';
			return str;
		}
	}
}