package org.as3openni.util
{
	import flash.display.BitmapData;
	import flash.geom.Rectangle;
	import flash.utils.ByteArray;
	
	import org.as3openni.AS3OpenNI;

	public class NiBitmapUtil
	{
		public function NiBitmapUtil(){}
		
		public static function byteArrayToBitmapData(bytes:ByteArray, bmd:BitmapData):void
		{
			bmd.lock();
			bmd.setPixels(new Rectangle(0,0, AS3OpenNI.VGA_MAX_WIDTH, AS3OpenNI.VGA_MAX_HEIGHT), bytes);
			bmd.unlock();
		}
	}
}