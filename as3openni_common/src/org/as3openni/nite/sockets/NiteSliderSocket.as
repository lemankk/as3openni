package org.as3openni.nite.sockets
{
	import flash.events.Event;
	import flash.events.ProgressEvent;
	
	import org.as3openni.nite.events.NiteSliderEvent;
	import org.as3openni.util.BasicSocket;

	public class NiteSliderSocket extends BasicSocket
	{
		public static const NITE_SLIDER_SOCKET:String = "nite_slider_socket";
		
		public function NiteSliderSocket()
		{
			super();
			this.name = NITE_SLIDER_SOCKET;
			this.port = 9502;
		}
		
		override protected function onClientSocketData(event:ProgressEvent):void
		{
			super.onClientSocketData(event);
			
			if(this.socketMessage.length > 0)
			{
				switch(this.socketMessage)
				{
					case NiteSliderEvent.SLIDER_IN_OUT_CREATED:
						this.dispatchEvent(new NiteSliderEvent(NiteSliderEvent.SLIDER_IN_OUT_CREATED));
						break;
					
					case NiteSliderEvent.SLIDER_IN_OUT_DESTROYED:
						this.dispatchEvent(new NiteSliderEvent(NiteSliderEvent.SLIDER_IN_OUT_DESTROYED));
						break;
					
					case NiteSliderEvent.SLIDER_LEFT_RIGHT_CREATED:
						this.dispatchEvent(new NiteSliderEvent(NiteSliderEvent.SLIDER_LEFT_RIGHT_CREATED));
						break;
					
					case NiteSliderEvent.SLIDER_LEFT_RIGHT_DESTROYED:
						this.dispatchEvent(new NiteSliderEvent(NiteSliderEvent.SLIDER_LEFT_RIGHT_DESTROYED));
						break;
					
					case NiteSliderEvent.SLIDER_UP_DOWN_CREATED:
						this.dispatchEvent(new NiteSliderEvent(NiteSliderEvent.SLIDER_UP_DOWN_CREATED));
						break;
					
					case NiteSliderEvent.SLIDER_UP_DOWN_DESTROYED:
						this.dispatchEvent(new NiteSliderEvent(NiteSliderEvent.SLIDER_UP_DOWN_DESTROYED));
						break;
				}
				
				// For the left/right value event.
				if(this.socketMessage.substr(0, NiteSliderEvent.SLIDER_LEFT_RIGHT_VALUE.length) == NiteSliderEvent.SLIDER_LEFT_RIGHT_VALUE)
				{
					var arr:Array = this.socketMessage.split(':');
					var val:Number = Number(arr[1]);
					var dir:String = (val < .5) ? NiteSliderEvent.SLIDER_LEFT : NiteSliderEvent.SLIDER_RIGHT;
					this.dispatchEvent(new NiteSliderEvent(NiteSliderEvent.SLIDER_LEFT_RIGHT_VALUE, val, dir));
				}
				
				// For the up/down value event.
				else if(this.socketMessage.substr(0, NiteSliderEvent.SLIDER_UP_DOWN_VALUE.length) == NiteSliderEvent.SLIDER_UP_DOWN_VALUE)
				{
					var arr2:Array = this.socketMessage.split(':');
					var val2:Number = Number(arr2[1]);
					var dir2:String = (val2 < .5) ? NiteSliderEvent.SLIDER_DOWN : NiteSliderEvent.SLIDER_UP;
					this.dispatchEvent(new NiteSliderEvent(NiteSliderEvent.SLIDER_UP_DOWN_VALUE, val2, dir2));
				}
				
				// For the in/own value event.
				else if(this.socketMessage.substr(0, NiteSliderEvent.SLIDER_IN_OUT_VALUE.length) == NiteSliderEvent.SLIDER_IN_OUT_VALUE)
				{
					var arr3:Array = this.socketMessage.split(':');
					var val3:Number = Number(arr3[1]);
					var dir3:String = (val3 < .5) ? NiteSliderEvent.SLIDER_IN : NiteSliderEvent.SLIDER_OUT;
					this.dispatchEvent(new NiteSliderEvent(NiteSliderEvent.SLIDER_IN_OUT_VALUE, val3, dir3));
				}
			}
		}
	}
}