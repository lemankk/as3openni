/**
 * AS3-OpenNI (MIT License)
 *
 * AS3-OpenNI - Copyright (c) 2011 Tony Birleffi
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
**/

package org.as3openni
{
	import flash.desktop.NativeApplication;
	import flash.desktop.NativeProcess;
	import flash.desktop.NativeProcessStartupInfo;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.NativeProcessExitEvent;
	import flash.events.ProgressEvent;
	import flash.filesystem.File;
	import flash.system.Capabilities;
	
	import org.as3openni.nite.events.NiteCircleEvent;
	import org.as3openni.nite.events.NiteGestureEvent;
	import org.as3openni.nite.events.NitePointEvent;
	import org.as3openni.nite.events.NiteSessionEvent;
	import org.as3openni.nite.events.NiteSliderEvent;
	import org.as3openni.nite.events.NiteTrackPadEvent;
	import org.as3openni.nite.sockets.NitePointSocket;
	import org.as3openni.nite.sockets.NiteSessionSocket;
	import org.as3openni.nite.sockets.NiteSliderSocket;
	import org.as3openni.openni.events.ONICaptureEvent;
	import org.as3openni.openni.events.ONISkeletonEvent;
	import org.as3openni.openni.events.ONIUserTrackingEvent;
	import org.as3openni.openni.sockets.ONIDepthMapSocket;
	import org.as3openni.openni.sockets.ONIRGBSocket;
	import org.as3openni.openni.sockets.ONIUserTrackingSocket;

	public class AS3OpenNI extends EventDispatcher
	{
		public static const VGA_MAX_WIDTH:Number = 640;
		public static const VGA_MAX_HEIGHT:Number = 480;
		
		public var singlePointSocket:NitePointSocket;
		public var sessionSocket:NiteSessionSocket;
		public var sliderSocket:NiteSliderSocket;
		public var userTrackingSocket:ONIUserTrackingSocket;
		public var depthMapSocket:ONIDepthMapSocket;
		public var videoSocket:ONIRGBSocket;
		public var bridge:NativeProcess;
		
		public var binaryPath:String = "";
		public var outputMessage:String = "";
		public var trackPadColumns:Number = 4;
		public var trackPadRows:Number = 9;
		public var depthMapQuality:Number = 0;
		public var videoQuality:Number = 0;
		
		public var debug:Boolean = false;
		public var traceLog:Boolean = false;
		public var singlePoint:Boolean = false;
		public var gestures:Boolean = false;
		public var circle:Boolean = false;
		public var slider:Boolean = false;
		public var trackPad:Boolean = false;
		public var userTracking:Boolean = false;
		public var depthMap:Boolean = false;
		public var depthMapSnap:Boolean = false;
		public var video:Boolean = false;
		public var videoGrayscale:Boolean = false;
		public var mirrorModeOff:Boolean = false;
		
		public function AS3OpenNI()
		{
			trace('INFO: AS3OpenNI - Alpha 1.0.9');
		}
		
		public function init():void
		{
			if(NativeProcess.isSupported)
			{
				if(this.binaryPath.length > 0)
				{
					// Startup the session server it's required.
					this.sessionSocket = new NiteSessionSocket();
					this.sessionSocket.traceLog = this.traceLog;
					this.sessionSocket.bind();
					
					// Startup optional socket servers if requested.
					if(this.singlePoint)
					{
						this.singlePointSocket = new NitePointSocket();
						this.singlePointSocket.traceLog = this.traceLog;
						this.singlePointSocket.bind();
						this.addSinglePointListeners();
					}
					
					if(this.slider)
					{
						this.sliderSocket = new NiteSliderSocket();
						this.sliderSocket.traceLog = this.traceLog;
						this.sliderSocket.bind();
						this.addSliderListeners();
					}
					
					if(this.userTracking)
					{
						this.userTrackingSocket = new ONIUserTrackingSocket();
						this.userTrackingSocket.traceLog = this.traceLog;
						this.userTrackingSocket.bind();
						this.addUserTrackingListeners();
					}
					
					if(this.depthMap)
					{
						this.depthMapSocket = new ONIDepthMapSocket();
						this.depthMapSocket.traceLog = this.traceLog;
						this.depthMapSocket.bind();
						this.addDepthMapCaptureListeners();
					}
					
					if(this.video)
					{
						this.videoSocket = new ONIRGBSocket();
						this.videoSocket.traceLog = this.traceLog;
						this.videoSocket.bind();
						this.addRGBCaptureListeners();
					}
					
					// Add optional listeners.
					if(this.gestures) this.addGestureListeners();
					if(this.circle) this.addCircleListeners();
					if(this.trackPad) this.addTrackPadListeners();
					
					// Startup the AS3OpenNI bridge.
					this.startupBridge();
					
					// Add required listeners.
					this.addListeners();
					this.addSessionListeners();
				}
			}
		}
		
		public function closeSockets():void
		{
			if(this.sessionSocket.server && this.sessionSocket.server.bound) this.sessionSocket.server.close();
			if(this.sessionSocket.client && this.sessionSocket.client.connected) this.sessionSocket.client.close();
			
			if(this.singlePoint && this.singlePointSocket.server && this.singlePointSocket.server.bound) this.singlePointSocket.server.close();
			if(this.singlePoint && this.singlePointSocket.client && this.singlePointSocket.client.connected) this.singlePointSocket.client.close();
			
			if(this.slider && this.sliderSocket.server && this.sliderSocket.server.bound) this.sliderSocket.server.close();
			if(this.slider && this.sliderSocket.client && this.sliderSocket.client.connected) this.sliderSocket.client.close();
			
			if(this.userTracking && this.userTrackingSocket.server && this.userTrackingSocket.server.bound) this.userTrackingSocket.server.close();
			if(this.userTracking && this.userTrackingSocket.client && this.userTrackingSocket.client.connected) this.userTrackingSocket.client.close();
			
			if(this.depthMap && this.depthMapSocket.server && this.depthMapSocket.server.bound) this.depthMapSocket.server.close();
			if(this.depthMap && this.depthMapSocket.client && this.depthMapSocket.client.connected) this.depthMapSocket.client.close();
			
			if(this.video && this.videoSocket.server && this.videoSocket.server) this.videoSocket.server.close();
			if(this.video && this.videoSocket.client && this.videoSocket.client) this.videoSocket.client.close();
		}
		
		/**
		 * Protected methods will go here.
		 */
		protected function startupBridge():void
		{     
			var file:File = new File();
			var startupInfo:NativeProcessStartupInfo = new NativeProcessStartupInfo();
			
			if (Capabilities.os.toLowerCase().indexOf("win") > -1)
			{
				var ext:String = this.binaryPath.substr((this.binaryPath.length-4), this.binaryPath.length);
				var path:String = (ext == '.exe') ? this.binaryPath : (this.binaryPath + '.exe');
				file = File.applicationDirectory.resolvePath(path);
			} 
			else if (Capabilities.os.toLowerCase().indexOf("mac") > -1) 
			{
				file = File.applicationDirectory.resolvePath(this.binaryPath);
			}
			
			/**
			 * By default on startup all the features of the AS3OpenNI client are turned OFF, 
			 * however if you'd like to turn certain features on for use in your application
			 * you can pass certain process arguments to the client to turn on those features. 
			 */
			var processArgs:Vector.<String> = new Vector.<String>();
			
			// Turn off the mirror mode.
			if(this.mirrorModeOff) processArgs.push("-mrev");
			
			// Turn on the SinglePoint feature.
			if(this.singlePoint) processArgs.push("-ospf");
			
			// Turn on the Gesture feature.
			if(this.gestures) processArgs.push("-ogf");
			
			// Turn on the Circle feature.
			if(this.circle) processArgs.push("-ocf");
			
			// Turn on the Slider feature.
			if(this.slider) processArgs.push("-osldf");
			
			// Turn on the TrackPad feature.
			if(this.trackPad)
			{
				// Pass to the binary.
				processArgs.push("-otpf");
				
				// Change the number of TrackPad columns.
				processArgs.push("-tpc");
				processArgs.push(String(this.trackPadColumns + '')); // default is 4.
				
				// Change the number of TrackPad rows.
				processArgs.push("-tpr");
				processArgs.push(String(this.trackPadRows + '')); // default is 9.
			}
			
			// Turn on the UserTracking feature.
			if(this.userTracking) processArgs.push("-outf");
			
			// Turn on the DepthMapCapture feature only testing one or the other in this file.
			if(this.depthMap) 
			{
				// Pass to the binary.
				processArgs.push("-odmc");
				
				// Set the DepthMapCapture quality, default is 0 = Low, 1 = Average, 2 = High, 3 = Super High.
				processArgs.push("-dmq");
				processArgs.push(String(this.depthMapQuality + ''));
				
				// Snap the pixels of the DepthMap and RGB together or not, default is false.
				if(this.depthMapSnap) processArgs.push("-snap");
			}
			
			// Turn on the RGBCapture feature only testing one or the other in this file.
			if(this.video)
			{
				// Pass to the binary.
				processArgs.push("-orgbc");
				
				// Set the RGBCapture quality, default is 0 = Low, 1 = Average, 2 = High, 3 = Super High.
				processArgs.push("-rgbq");
				processArgs.push(String(this.videoQuality + ''));
				
				// Sets the RGBCapture to render in greyscale.
				if(this.videoGrayscale) processArgs.push("-grey");
			}
			
			// Continue...
			startupInfo.arguments = processArgs;
			startupInfo.executable = file;
			
			// Startup the bridge.
			this.bridge = new NativeProcess();
			this.bridge.start(startupInfo);
		}
		
		/**
		 * Adding the listener methods here.
		 */
		protected function addListeners():void
		{
			NativeApplication.nativeApplication.addEventListener(Event.EXITING, this.onClose);
			this.bridge.addEventListener(ProgressEvent.STANDARD_OUTPUT_DATA, this.onOutputData);
			this.bridge.addEventListener(ProgressEvent.STANDARD_ERROR_DATA, this.onErrorData);
			this.bridge.addEventListener(NativeProcessExitEvent.EXIT, this.onExit);
		}
		
		protected function addSessionListeners():void
		{
			this.sessionSocket.addEventListener(NiteSessionEvent.SESSION_STARTED, this.onSession);
			this.sessionSocket.addEventListener(NiteSessionEvent.SESSION_PROGRESS, this.onSession);
			this.sessionSocket.addEventListener(NiteSessionEvent.SESSION_ENDED, this.onSession);
		}
		
		protected function addSinglePointListeners():void
		{
			this.singlePointSocket.addEventListener(NitePointEvent.POINT_DETECTED, this.onPoint);
		}
		
		protected function addGestureListeners():void
		{
			this.sessionSocket.addEventListener(NiteGestureEvent.PUSH, this.onGesture);
			this.sessionSocket.addEventListener(NiteGestureEvent.WAVE, this.onGesture);
			this.sessionSocket.addEventListener(NiteGestureEvent.SWIPE_UP, this.onGesture);
			this.sessionSocket.addEventListener(NiteGestureEvent.SWIPE_DOWN, this.onGesture);
			this.sessionSocket.addEventListener(NiteGestureEvent.SWIPE_LEFT, this.onGesture);
			this.sessionSocket.addEventListener(NiteGestureEvent.SWIPE_RIGHT, this.onGesture);
			this.sessionSocket.addEventListener(NiteGestureEvent.STEADY, this.onGesture);
		}
		
		protected function addCircleListeners():void
		{
			this.sessionSocket.addEventListener(NiteCircleEvent.ANGLE, this.onCircle);
			this.sessionSocket.addEventListener(NiteCircleEvent.NO_CIRCLE, this.onCircle);
		}
		
		protected function addSliderListeners():void
		{
			this.sliderSocket.addEventListener(NiteSliderEvent.SLIDER_LEFT_RIGHT_VALUE, this.onSlider);
			this.sliderSocket.addEventListener(NiteSliderEvent.SLIDER_UP_DOWN_VALUE, this.onSlider);
			this.sliderSocket.addEventListener(NiteSliderEvent.SLIDER_IN_OUT_VALUE, this.onSlider);
		}
		
		protected function addTrackPadListeners():void
		{
			this.sessionSocket.addEventListener(NiteTrackPadEvent.TRACKPAD_HOVER, this.onTrackPad);
		}
		
		protected function addUserTrackingListeners():void
		{
			this.userTrackingSocket.addEventListener(ONIUserTrackingEvent.USER_TRACKING_NEW_USER, this.onUserTracking);
			this.userTrackingSocket.addEventListener(ONIUserTrackingEvent.USER_TRACKING_LOST_USER, this.onUserTracking);
			this.userTrackingSocket.addEventListener(ONIUserTrackingEvent.USER_TRACKING_POSE_DETECTED, this.onUserTracking);
			this.userTrackingSocket.addEventListener(ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_START, this.onUserTracking);
			this.userTrackingSocket.addEventListener(ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_COMPLETE, this.onUserTracking);
			this.userTrackingSocket.addEventListener(ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_FAILED, this.onUserTracking);
			this.userTrackingSocket.addEventListener(ONIUserTrackingEvent.USER_TRACKING_USER_FOUND, this.onUserTracking);
			this.userTrackingSocket.addEventListener(ONISkeletonEvent.USER_TRACKING, this.onSkeleton);
		}
		
		protected function addDepthMapCaptureListeners():void
		{
			this.depthMapSocket.addEventListener(ONICaptureEvent.ONI_DEPTH_MAP, this.onCapture);
		}
		
		protected function addRGBCaptureListeners():void
		{
			this.videoSocket.addEventListener(ONICaptureEvent.ONI_RGB, this.onCapture);
		}
		
		/**
		 * Protected event handlers for redispatching go here.
		 */
		protected function onSession(event:NiteSessionEvent):void
		{
			this.dispatchEvent(new NiteSessionEvent(event.type, event.bubbles, event.cancelable));
		}
		
		protected function onPoint(event:NitePointEvent):void
		{
			this.dispatchEvent(new NitePointEvent(event.type, event.point3D, event.bubbles, event.cancelable));
		}
		
		protected function onGesture(event:NiteGestureEvent):void
		{
			this.dispatchEvent(new NiteGestureEvent(event.type, event.velocity, event.angle, event.bubbles, event.cancelable));
		}
		
		protected function onCircle(event:NiteCircleEvent):void
		{
			this.dispatchEvent(new NiteCircleEvent(event.type, event.angle, event.clockwise, event.bubbles, event.cancelable));
		}
		
		protected function onSlider(event:NiteSliderEvent):void
		{
			this.dispatchEvent(new NiteSliderEvent(event.type, event.value, event.direction, event.bubbles, event.cancelable));
		}
		
		protected function onTrackPad(event:NiteTrackPadEvent):void
		{
			this.dispatchEvent(new NiteTrackPadEvent(event.type, event.column, event.row, event.direction, event.bubbles, event.cancelable));
		}
		
		protected function onUserTracking(event:ONIUserTrackingEvent):void
		{
			this.dispatchEvent(new ONIUserTrackingEvent(event.type, event.user, event.userSinglePoint, event.bubbles, event.cancelable));
		}
		
		protected function onSkeleton(event:ONISkeletonEvent):void
		{
			this.dispatchEvent(new ONISkeletonEvent(event.type, event.user, event.leftHand, event.rightHand, event.skeleton, event.bubbles, event.cancelable));
		}
		
		protected function onCapture(event:ONICaptureEvent):void
		{
			this.dispatchEvent(new ONICaptureEvent(event.type, event.bytes, event.bubbles, event.cancelable));
		}
		
		protected function onClose(event:Event):void
		{
			this.dispatchEvent(new Event(event.type, event.bubbles, event.cancelable));
			if(this.bridge) this.bridge.exit(true);
		}
		
		protected function onOutputData(event:ProgressEvent):void
		{
			this.dispatchEvent(new ProgressEvent(event.type, event.bubbles, event.cancelable, event.bytesLoaded, event.bytesTotal));
			this.outputMessage = this.bridge.standardOutput.readMultiByte(this.bridge.standardOutput.bytesAvailable, File.systemCharset);
			if(this.debug) trace("Console:", this.outputMessage);
		}
		
		protected function onErrorData(event:ProgressEvent):void
		{
			this.dispatchEvent(new ProgressEvent(event.type, event.bubbles, event.cancelable, event.bytesLoaded, event.bytesTotal));
			if(this.bridge)
			{
				var ba:Number = this.bridge.standardOutput.bytesAvailable;
				if(ba > 0) 
				{
					this.outputMessage = this.bridge.standardError.readMultiByte(ba, File.systemCharset);
					if(this.debug) trace(this.outputMessage);
				}
				this.bridge.closeInput();
			}
		}
		
		protected function onExit(event:NativeProcessExitEvent):void
		{
			this.dispatchEvent(new NativeProcessExitEvent(event.type, event.bubbles, event.cancelable, event.exitCode));
			this.closeSockets();
		}
	}
}