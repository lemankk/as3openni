/*
* This file is part of the AS3OpenNI Project. http://code.google.com/p/as3openni/
*
* Copyright (c) 2010 individual as3kinect contributors. See the CONTRIB file
* for details.
*
* This code is licensed to you under the terms of the Apache License, version
* 2.0, or, at your option, the terms of the GNU General Public License,
* version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
* or the following URLs:
* http://www.apache.org/licenses/LICENSE-2.0
* http://www.gnu.org/licenses/gpl-2.0.txt
*
* If you redistribute this file in source form, modified or unmodified, you
* may:
*   1) Leave this header intact and distribute it under the same terms,
*      accompanying it with the APACHE20 and GPL20 files, or
*   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
*   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
* In all cases you must keep the copyright notice intact and include a copy
* of the CONTRIB file.
*
* Binary distributions must follow the binary distribution requirements of
* either License.
*/

package org.as3openni
{
	import flash.events.EventDispatcher;
	
	import org.as3openni.nite.sockets.NitePointSocket;
	import org.as3openni.nite.sockets.NiteSessionSocket;
	import org.as3openni.nite.sockets.NiteSliderSocket;
	import org.as3openni.openni.sockets.ONIDepthMapSocket;
	import org.as3openni.openni.sockets.ONIUserTrackingSocket;

	public class AS3OpenNI extends EventDispatcher
	{
		public static const VGA_MAX_WIDTH:Number = 640;
		public static const VGA_MAX_HEIGHT:Number = 480;
		
		public var singlePoint:NitePointSocket;
		public var session:NiteSessionSocket;
		public var slider:NiteSliderSocket;
		public var userTracking:ONIUserTrackingSocket;
		//public var depthMap:ONIDepthMapSocket;
		
		public function AS3OpenNI()
		{
			this.session = new NiteSessionSocket();
			this.session.bind();
			
			this.singlePoint = new NitePointSocket();
			this.singlePoint.bind();
			
			this.slider = new NiteSliderSocket();
			this.slider.bind();
			
			this.userTracking = new ONIUserTrackingSocket();
			this.userTracking.bind();
			
			//this.depthMap = new ONIDepthMapSocket();
			//this.depthMap.bind();
		}
		
		public function closeSockets():void
		{
			if(this.session.server) this.session.server.close();
			if(this.session.client) this.session.client.close();
			
			if(this.singlePoint.server) this.singlePoint.server.close();
			if(this.singlePoint.client) this.singlePoint.client.close();
			
			if(this.slider.server) this.slider.server.close();
			if(this.slider.client) this.slider.client.close();
			
			if(this.userTracking.server) this.userTracking.server.close();
			if(this.userTracking.client) this.userTracking.client.close();
			
			//if(this.depthMap.server) this.depthMap.server.close();
			//if(this.depthMap.client) this.depthMap.client.close();
		}
	}
}