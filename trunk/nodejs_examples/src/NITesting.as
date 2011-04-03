package
{
	import com.senocular.ui.IVirtualMouseEvent;
	import com.senocular.ui.VirtualMouse;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.as3openni.flash.AS3OpenNI;
	import org.as3openni.flash.nite.events.NitePointEvent;
	import org.as3openni.flash.nite.sockets.FLNitePointSocket;
	import org.as3openni.flash.objects.NiPoint2D;
	import org.as3openni.flash.objects.NiPoint3D;
	import org.as3openni.flash.util.math.NiPoint3DUtil;
	
	public class NITesting extends Sprite
	{
		[Embed(source="../proj-assets/swf/hand.swf")]
		private var Hand:Class;
		
		private var _as3OpenNI:AS3OpenNI;
		private var _virtualMouse:VirtualMouse;
		private var _currentX:Number = 0;
		private var _currentY:Number = 0;
		private var _hand:Sprite;
		private var _stageWidth:Number = 0;
		private var _stageHeight:Number = 0;
		
		public function NITesting()
		{
			this.stage.scaleMode = StageScaleMode.NO_SCALE;
			this.stage.align = StageAlign.TOP_LEFT;
			this.addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		protected function init(event:Event):void
		{
			this.removeEventListener(Event.ADDED_TO_STAGE, init);
			this.stage.addEventListener(MouseEvent.MOUSE_MOVE, mouseMove);
			this.stage.addEventListener(Event.RESIZE, resize);
			
			this._hand = new Hand();
			this.addChild(this._hand);
			this._hand.width = 75;
			this._hand.height = 75;
			
			this._virtualMouse = new VirtualMouse(this.stage, this._hand.x, this._hand.y);
			this._virtualMouse.ignore(this._hand);
			
			this._as3OpenNI = new AS3OpenNI();
			this._as3OpenNI.singlePoint.addEventListener(NitePointEvent.POINT_DETECTED, pointDetected);
			this._as3OpenNI.singlePoint.traceLog = false;
			
			this.resize();
		}
		
		protected function resize(event:Event = null):void
		{
			this._stageWidth = this.stage.stageWidth;
			this._stageHeight = this.stage.stageHeight;
		}
		
		protected function pointDetected(event:NitePointEvent):void
		{
			var point3D:NiPoint3D = event.point3D as NiPoint3D;
			var obj:NiPoint2D = NiPoint3DUtil.convertRealWorldToScreen(point3D, this._stageWidth, this._stageHeight);
			this._currentX = obj.pointX;
			this._currentY = obj.pointY;
			
			if(point3D)
			{
				this._virtualMouse.lock();
				this._virtualMouse.x = this._currentX;
				this._virtualMouse.y = this._currentY;
				this._virtualMouse.unlock();
			}
		}
		
		protected function mouseMove(event:MouseEvent):void
		{
			if(event is IVirtualMouseEvent)
			{
				this._hand.x = this._virtualMouse.x;
				this._hand.y = this._virtualMouse.y;
			}
		}
	}
}