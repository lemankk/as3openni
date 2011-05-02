package org.as3openni.buffers
{
	import org.as3openni.global.Definitions;
	import org.as3openni.util.BasicBuffer;
	import org.as3openni.util.ClientSocket;
	
	public class SkeletonsBuffer extends BasicBuffer
	{
		public function SkeletonsBuffer(socket:ClientSocket)
		{
			super(socket);
		}
		
		override protected function writeBytes():void
		{
			this.data.writeByte(Definitions.OPENNI_ID);
			this.data.writeByte(Definitions.OPENNI_GET_SKELETONS);
		}
	}
}