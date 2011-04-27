package org.as3openni.buffers
{
	import org.as3openni.global.Definitions;
	import org.as3openni.util.BasicBuffer;
	import org.as3openni.util.ClientSocket;
	
	public class VideoBuffer extends BasicBuffer
	{
		public function VideoBuffer(socket:ClientSocket)
		{
			super(socket);
		}
		
		override protected function writeBytes():void
		{
			this.data.writeByte(Definitions.OPENNI_ID);
			this.data.writeByte(Definitions.OPENNI_GET_VIDEO);
		}
	}
}