package org.as3openni.util
{
	import flash.display.BitmapData;
	import flash.display.Loader;
	import flash.events.Event;
	import flash.filters.ColorMatrixFilter;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.utils.ByteArray;
	
	import org.as3openni.global.Definitions;

	public class BitmapUtil
	{
		public function BitmapUtil(){}
		
		public static function byteArrayToBitmapData(bytes:ByteArray, canvas:BitmapData):void
		{
			canvas.lock();
			canvas.setPixels(new Rectangle(0, 0, Definitions.VGA_MAX_WIDTH, Definitions.VGA_MAX_HEIGHT), bytes);
			canvas.unlock();
		}
		
		public static function setBlackWhiteFilter(canvas:BitmapData, threshold:int = 128):void
		{
			var rLum:Number = 0.2225;
			var gLum:Number = 0.7169;
			var bLum:Number = 0.0606;
			var matrix:Array = [rLum * 256, gLum * 256, bLum * 256, 0, -256 * threshold,
				rLum * 256, gLum * 256, bLum * 256, 0, -256 * threshold,
				rLum * 256, gLum * 256, bLum * 256, 0, -256 * threshold,
				0,          0,          0,          1, 0                ]; 
			var filter:ColorMatrixFilter = new ColorMatrixFilter(matrix);
			canvas.applyFilter(canvas, new Rectangle(0,0,canvas.width,canvas.height), new Point(0,0), filter);
		}
	}
}