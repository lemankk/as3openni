package org.as3openni.nite.sockets
{
	import flash.events.ProgressEvent;
	
	import org.as3openni.nite.events.NiteCircleEvent;
	import org.as3openni.nite.events.NiteGestureEvent;
	import org.as3openni.nite.events.NiteSessionEvent;
	import org.as3openni.nite.events.NiteSliderEvent;
	import org.as3openni.nite.events.NiteTrackPadEvent;
	import org.as3openni.util.BasicSocket;

	public class NiteSessionSocket extends BasicSocket
	{
		public static const NITE_SESSION_SOCKET:String = "nite_session_socket";
		
		public function NiteSessionSocket()
		{
			super();
			this.name = NITE_SESSION_SOCKET;
			this.port = 9501;
		}
		
		override protected function onClientSocketData(event:ProgressEvent):void
		{
			super.onClientSocketData(event);
			
			if(this.socketMessage.length > 0)
			{
				switch(this.socketMessage)
				{					
					// Gesture Events.
					case NiteGestureEvent.WAVE:
						this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.WAVE));
						break;
					
					// Session Events.
					case NiteSessionEvent.SESSION_STARTED:
						this.dispatchEvent(new NiteSessionEvent(NiteSessionEvent.SESSION_STARTED));
						break;
					
					case NiteSessionEvent.SESSION_PROGRESS:
						this.dispatchEvent(new NiteSessionEvent(NiteSessionEvent.SESSION_PROGRESS));
						break;
					
					case NiteSessionEvent.SESSION_ENDED:
						this.dispatchEvent(new NiteSessionEvent(NiteSessionEvent.SESSION_ENDED));
						break;
					
					// Circle Events
					case NiteCircleEvent.CIRCLE_CREATED:
						this.dispatchEvent(new NiteCircleEvent(NiteCircleEvent.CIRCLE_CREATED));
						break;
					
					case NiteCircleEvent.CIRCLE_DESTROYED:
						this.dispatchEvent(new NiteCircleEvent(NiteCircleEvent.CIRCLE_DESTROYED));
						break;
					
					case NiteCircleEvent.NO_CIRCLE:
						this.dispatchEvent(new NiteCircleEvent(NiteCircleEvent.NO_CIRCLE));
						break;
					
					// TrackPad Events
					case NiteTrackPadEvent.TRACKPAD_CREATED:
						this.dispatchEvent(new NiteTrackPadEvent(NiteTrackPadEvent.TRACKPAD_CREATED));
						break;
					
					case NiteTrackPadEvent.TRACKPAD_DESTROYED:
						this.dispatchEvent(new NiteTrackPadEvent(NiteTrackPadEvent.TRACKPAD_DESTROYED));
						break;
				}
				
				// For the steady hand event.
				if(this.socketMessage.substr(0, NiteGestureEvent.STEADY.length) == NiteGestureEvent.STEADY)
				{
					var arr:Array = this.socketMessage.split(':');
					var v:Number = Number(arr[1]);
					if(isNaN(v)) v = 0;
					this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.STEADY, v));
				}
				
				// For the push event.
				if(this.socketMessage.substr(0, NiteGestureEvent.PUSH.length) == NiteGestureEvent.PUSH)
				{
					var arr1:Array = this.socketMessage.split(':');
					var v2:Number = Number(arr1[1]);
					if(isNaN(v2)) v2 = 0;
					var a1:Number = Number(arr1[2]);
					if(isNaN(a1)) a1 = 0;
					this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.PUSH, v2, a1));
				}
				
				// For the swipe down event.
				if(this.socketMessage.substr(0, NiteGestureEvent.SWIPE_DOWN.length) == NiteGestureEvent.SWIPE_DOWN)
				{
					var arr2:Array = this.socketMessage.split(':');
					var v3:Number = Number(arr2[1]);
					if(isNaN(v3)) v3 = 0;
					var a3:Number = Number(arr2[2]);
					if(isNaN(a3)) a3 = 0;
					this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.SWIPE_DOWN, v3, a3));
				}
				
				// For the swipe up event.
				if(this.socketMessage.substr(0, NiteGestureEvent.SWIPE_UP.length) == NiteGestureEvent.SWIPE_UP)
				{
					var arr3:Array = this.socketMessage.split(':');
					var v4:Number = Number(arr3[1]);
					if(isNaN(v4)) v4 = 0;
					var a4:Number = Number(arr3[2]);
					if(isNaN(a4)) a4 = 0;
					this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.SWIPE_UP, v4, a4));
				}
				
				// For the swipe left event.
				if(this.socketMessage.substr(0, NiteGestureEvent.SWIPE_LEFT.length) == NiteGestureEvent.SWIPE_LEFT)
				{
					var arr4:Array = this.socketMessage.split(':');
					var v5:Number = Number(arr4[1]);
					if(isNaN(v5)) v5 = 0;
					var a5:Number = Number(arr4[2]);
					if(isNaN(a5)) a5 = 0;
					this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.SWIPE_LEFT, v5, a5));
				}
				
				// For the swipe right event.
				if(this.socketMessage.substr(0, NiteGestureEvent.SWIPE_RIGHT.length) == NiteGestureEvent.SWIPE_RIGHT)
				{
					var arr5:Array = this.socketMessage.split(':');
					var v6:Number = Number(arr5[1]);
					if(isNaN(v6)) v6 = 0;
					var a6:Number = Number(arr5[2]);
					if(isNaN(a6)) a6 = 0;
					this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.SWIPE_RIGHT, v6, a6));
				}
				
				// For the circle radius event.
				if(this.socketMessage.substr(0, NiteCircleEvent.ANGLE.length) == NiteCircleEvent.ANGLE)
				{
					var arr6:Array = this.socketMessage.split(':');
					var angle:Number = Number(arr6[1]);
					var clockwise:Boolean = (angle < 0) ? false : true;
					this.dispatchEvent(new NiteCircleEvent(NiteCircleEvent.ANGLE, angle, clockwise));
				}
				
				// For the trackpad event hover states.
				if(this.socketMessage.substr(0, NiteTrackPadEvent.TRACKPAD_HOVER.length) == NiteTrackPadEvent.TRACKPAD_HOVER)
				{
					var arr7:Array = this.socketMessage.split(':');
					var column:Number = Number(arr7[1]);
					var row:Number = Number(arr7[2]);
					var direction:String = String(arr7[3]);
					if(direction == "undefined") direction = "";
					this.dispatchEvent(new NiteTrackPadEvent(NiteTrackPadEvent.TRACKPAD_HOVER, column, row, direction));
				}
			}
		}
	}
}