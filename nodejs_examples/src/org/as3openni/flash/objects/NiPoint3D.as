package org.as3openni.flash.objects
{
	public class NiPoint3D
	{
		public var user:Number;
		public var pointX:Number;
		public var pointY:Number;
		public var pointZ:Number;
		public var pointTime:Number;
		
		public function NiPoint3D(){}
		
		public function toString():String
		{
			var str:String = "id: " + user + 
							 ", x: " + this.pointX + 
							 ", y: " + this.pointY + 
							 ", z: " + this.pointZ + 
							 ", t: " + this.pointTime;
			return str;
		}
	}
}