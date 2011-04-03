package org.as3openni.nite.sockets
{
	import flash.events.Event;
	import flash.events.ProgressEvent;
	
	import org.as3openni.nite.events.NiteCircleEvent;
	import org.as3openni.nite.events.NiteGestureEvent;
	import org.as3openni.nite.events.NiteSessionEvent;
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
					case NiteGestureEvent.STEADY:
						this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.STEADY));
						break;
					
					case NiteGestureEvent.PUSH:
						this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.PUSH));
						break;
					
					case NiteGestureEvent.WAVE:
						this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.WAVE));
						break;
					
					case NiteGestureEvent.SWIPE_DOWN:
						this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.SWIPE_DOWN));
						break;
					
					case NiteGestureEvent.SWIPE_UP:
						this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.SWIPE_UP));
						break;
					
					case NiteGestureEvent.SWIPE_LEFT:
						this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.SWIPE_LEFT));
						break;
					
					case NiteGestureEvent.SWIPE_RIGHT:
						this.dispatchEvent(new NiteGestureEvent(NiteGestureEvent.SWIPE_RIGHT));
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
				
				// For the circle radius event.
				if(this.socketMessage.substr(0, NiteCircleEvent.ANGLE.length) == NiteCircleEvent.ANGLE)
				{
					var arr:Array = this.socketMessage.split(':');
					var angle:Number = Number(arr[1]);
					var clockwise:Boolean = (angle < 0) ? false : true;
					this.dispatchEvent(new NiteCircleEvent(NiteCircleEvent.ANGLE, angle, clockwise));
				}
				
				// For the trackpad event hover states.
				if(this.socketMessage.substr(0, NiteTrackPadEvent.TRACKPAD_HOVER.length) == NiteTrackPadEvent.TRACKPAD_HOVER)
				{
					var arr2:Array = this.socketMessage.split(':');
					var column:Number = Number(arr2[1]);
					var row:Number = Number(arr2[2]);
					var direction:String = String(arr2[3]);
					if(direction == "undefined") direction = "";
					this.dispatchEvent(new NiteTrackPadEvent(NiteTrackPadEvent.TRACKPAD_HOVER, column, row, direction));
				}
			}
		}
	}
}