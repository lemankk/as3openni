package org.as3openni.faast
{
	import flash.display.Sprite;
	import flash.events.Event;
	
	import org.as3openni.faast.events.FAASTEvent;
	import org.as3openni.faast.gestures.FAASTLeftArmGestures;
	import org.as3openni.faast.gestures.FAASTLeftFootGestures;
	import org.as3openni.faast.gestures.FAASTRightArmGestures;
	import org.as3openni.faast.gestures.FAASTRightFootGestures;
	import org.as3openni.objects.NiPoint3D;
	import org.as3openni.objects.NiSkeleton;
	import org.as3openni.util.math.NiPoint3DUtil;
	
	public class FAAST extends Sprite
	{
		public var skeleton:NiSkeleton;
		public var leftArmGestures:FAASTLeftArmGestures;
		public var rightArmGestures:FAASTRightArmGestures;
		public var leftFootGestures:FAASTLeftFootGestures;
		public var rightFootGestures:FAASTRightFootGestures;
		
		public function FAAST(armSenseRange:Number = 75, footSenseRange:Number = 450, useInches:Boolean = false)
		{
			super();
			
			this.leftArmGestures = new FAASTLeftArmGestures(armSenseRange, useInches);
			this.rightArmGestures = new FAASTRightArmGestures(armSenseRange, useInches);
			this.leftFootGestures = new FAASTLeftFootGestures(footSenseRange, useInches);
			this.rightFootGestures = new FAASTRightFootGestures(footSenseRange, useInches);
		}
		
		public function startRendering():void
		{
			this.addEventListener(Event.ENTER_FRAME, onRender);
		}
		
		public function stopRendering():void
		{
			this.removeEventListener(Event.ENTER_FRAME, onRender);
		}
		
		protected function onRender(event:Event):void
		{
			if(this.skeleton)
			{
				this.leftArmGestures.configure(this.skeleton);
				this.rightArmGestures.configure(this.skeleton);
				this.leftFootGestures.configure(this.skeleton);
				this.rightFootGestures.configure(this.skeleton);
			}
		}
	}
}