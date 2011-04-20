package org.as3openni.util.math
{
	import org.as3openni.AS3OpenNI;
	import org.as3openni.objects.NiPoint2D;
	import org.as3openni.objects.NiPoint3D;

	public class NiPoint3DUtil
	{
		public function NiPoint3DUtil() {}
		
		public static function convertRealWorldToScreen(point3D:NiPoint3D, width:Number, height:Number):NiPoint2D
		{
			var offsetX:Number = width/AS3OpenNI.VGA_MAX_WIDTH;
			var offsetY:Number = height/AS3OpenNI.VGA_MAX_HEIGHT;
			
			var newX:Number = (width/2) + point3D.pointX * offsetY;
			var newY:Number = (height/2) - point3D.pointY * offsetY;
			
			var point2D:NiPoint2D = new NiPoint2D();
			point2D.pointX = newX;
			point2D.pointY = newY;
			point2D.user = point3D.user;
			point2D.pointTime = point3D.pointTime;
			
			return point2D;
		}
		
		public static function convertMMToInches(value:Number):Number
		{
			return value * 0.0393700787;
		}
	}
}