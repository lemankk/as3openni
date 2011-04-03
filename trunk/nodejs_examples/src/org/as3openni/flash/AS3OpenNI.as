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

package org.as3openni.flash
{
	import flash.events.EventDispatcher;
	
	import org.as3openni.flash.nite.sockets.FLNitePointSocket;
	import org.as3openni.flash.nite.sockets.FLNiteSessionSocket;
	import org.as3openni.flash.nite.sockets.FLNiteSliderSocket;
	import org.as3openni.flash.openni.sockets.FLONIDepthMapSocket;
	import org.as3openni.flash.openni.sockets.FLONIRGBSocket;
	import org.as3openni.flash.openni.sockets.FLONIUserTrackingSocket;

	public class AS3OpenNI extends EventDispatcher
	{
		public static const VGA_MAX_WIDTH:Number = 640;
		public static const VGA_MAX_HEIGHT:Number = 480;
		
		public var singlePoint:FLNitePointSocket;
		public var session:FLNiteSessionSocket;
		public var slider:FLNiteSliderSocket;
		public var userTracker:FLONIUserTrackingSocket;
		public var depthMap:FLONIDepthMapSocket;
		public var video:FLONIRGBSocket;
		
		public function AS3OpenNI()
		{
			this.singlePoint = new FLNitePointSocket();
			this.session = new FLNiteSessionSocket();
			this.slider = new FLNiteSliderSocket();
			this.userTracker = new FLONIUserTrackingSocket();
			this.depthMap = new FLONIDepthMapSocket();
			this.video = new FLONIRGBSocket();
		}
	}
}