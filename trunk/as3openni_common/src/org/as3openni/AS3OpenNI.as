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
	import flash.events.IEventDispatcher;
	import flash.events.NativeProcessExitEvent;
	import flash.events.ProgressEvent;
	import flash.filesystem.File;
	import flash.system.Capabilities;
	import flash.utils.ByteArray;
	import flash.utils.setTimeout;
	
	import org.as3openni.buffers.DepthBuffer;
	import org.as3openni.buffers.SkeletonsBuffer;
	import org.as3openni.buffers.UserTrackingBuffer;
	import org.as3openni.buffers.VideoBuffer;
	import org.as3openni.events.AS3OpenNIEvent;
	import org.as3openni.events.ClientSocketEvent;
	import org.as3openni.events.openni.OpenNIEvent;
	import org.as3openni.global.Definitions;
	import org.as3openni.nite.events.NiteCircleEvent;
	import org.as3openni.nite.events.NiteGestureEvent;
	import org.as3openni.nite.events.NitePointEvent;
	import org.as3openni.nite.events.NiteSessionEvent;
	import org.as3openni.nite.events.NiteSliderEvent;
	import org.as3openni.nite.events.NiteTrackPadEvent;
	import org.as3openni.nite.sockets.NitePointSocket;
	import org.as3openni.nite.sockets.NiteSessionSocket;
	import org.as3openni.nite.sockets.NiteSliderSocket;
	import org.as3openni.objects.NiPoint3D;
	import org.as3openni.objects.NiSkeleton;
	import org.as3openni.openni.events.ONISkeletonEvent;
	import org.as3openni.openni.events.ONIUserTrackingEvent;
	import org.as3openni.openni.sockets.ONIUserTrackingSocket;
	import org.as3openni.util.ClientSocket;

	public class AS3OpenNI extends EventDispatcher
	{
		public var singlePointSocket:NitePointSocket;
		public var sessionSocket:NiteSessionSocket;
		public var sliderSocket:NiteSliderSocket;
		public var userTrackingSocket:ONIUserTrackingSocket;
		public var bridge:NativeProcess;
		
		public var binaryPath:String = "";
		public var trackPadColumns:Number = 4;
		public var trackPadRows:Number = 9;
		public var waitTime:Number = 8;
		
		public var debug:Boolean = false;
		public var traceLog:Boolean = false;
		public var singlePoint:Boolean = false;
		public var gestures:Boolean = false;
		public var circle:Boolean = false;
		public var slider:Boolean = false;
		public var trackPad:Boolean = false;
		public var userTracking:Boolean = false;
		public var depthMap:Boolean = false;
		public var depthMapBackground:Boolean = false;
		public var depthMapSnapOff:Boolean = false;
		public var video:Boolean = false;
		public var mirrorModeOff:Boolean = false;
		public var convertRealWorldToProjective:Boolean = false;
		public var isWindows:Boolean = true;
		
		private var _bridgeReady:Boolean = false;
		private var _clientReady:Boolean = false;
		private var _bridge:NativeProcess;
		private var _captureClientSocket:ClientSocket;
		private var _videoBuffer:VideoBuffer;
		private var _depthBuffer:DepthBuffer;
		private var _userTrackingBuffer:UserTrackingBuffer;
		private var _skeletonsBuffer:SkeletonsBuffer;
		
		/**
		 * AS3OpenNI - Constructor.
		 * @target	IEventDispatcher
		 */
		public function AS3OpenNI(target:IEventDispatcher=null)
		{
			super(target);
			trace(Definitions.HEADER);
		}
		
		public function init():void
		{
			if(NativeProcess.isSupported)
			{
				if(this.binaryPath.length > 0)
				{
					// Determine which OS.
					this.findOS();
					
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
					
					if(this.userTracking && !this.isWindows)
					{
						this.userTrackingSocket = new ONIUserTrackingSocket();
						this.userTrackingSocket.traceLog = this.traceLog;
						this.userTrackingSocket.bind();
						this.addUserTrackingListeners();
					}
					
					// Setup the client socket.
					if(this.video || this.depthMap || this.userTracking)
					{
						setTimeout(this.setupCaptureClientSocket, (this.waitTime*1000));
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
		
		public function getVideoBuffer():void
		{
			if(this.isReady() && this.video) this._videoBuffer.getBuffer();
		}
		
		public function getDepthBuffer():void
		{
			if(this.isReady() && this.depthMap) this._depthBuffer.getBuffer();
		}
		
		public function getUserTrackingBuffer():void
		{
			if(this.isReady() && this.userTracking && this.isWindows) this._userTrackingBuffer.getBuffer();
		}
		
		public function getSkeletonsBuffer():void
		{
			if(this.isReady() && this.userTracking && this.isWindows) this._skeletonsBuffer.getBuffer();
		}
		
		public function isReady():Boolean
		{
			return (this._clientReady && this._bridgeReady) ? true : false;
		}
		
		public function closeSockets():void
		{
			if(this.sessionSocket.server && this.sessionSocket.server.bound) this.sessionSocket.server.close();
			if(this.sessionSocket.client && this.sessionSocket.client.connected) this.sessionSocket.client.close();
			
			if(this.singlePoint && this.singlePointSocket.server && this.singlePointSocket.server.bound) this.singlePointSocket.server.close();
			if(this.singlePoint && this.singlePointSocket.client && this.singlePointSocket.client.connected) this.singlePointSocket.client.close();
			
			if(this.slider && this.sliderSocket.server && this.sliderSocket.server.bound) this.sliderSocket.server.close();
			if(this.slider && this.sliderSocket.client && this.sliderSocket.client.connected) this.sliderSocket.client.close();
			
			if(!this.isWindows && this.userTracking && this.userTrackingSocket.server && this.userTrackingSocket.server.bound) this.userTrackingSocket.server.close();
			if(!this.isWindows && this.userTracking && this.userTrackingSocket.client && this.userTrackingSocket.client.connected) this.userTrackingSocket.client.close();
		}
		
		/**
		 * Protected methods will go here.
		 */
		protected function setupCaptureClientSocket():void
		{
			this._captureClientSocket = new ClientSocket();
			this._captureClientSocket.addEventListener(ClientSocketEvent.ON_DATA, onDataReceived);
			this._captureClientSocket.addEventListener(ClientSocketEvent.ON_ERROR, onClientSocketError);
			this._captureClientSocket.addEventListener(ClientSocketEvent.ON_CONNECT, onClientSocketConnected);
			this._captureClientSocket.connect();
		}
		
		protected function onDataReceived(event:ClientSocketEvent):void
		{
			if(this._clientReady)
			{
				var first:Number = event.data.first as Number;
				var second:Number = event.data.second as Number;
				var buffer:ByteArray = event.data.buffer as ByteArray;
				
				switch(first)
				{
					case Definitions.AS3OPENNI_ID:
						switch(second)
						{
							case Definitions.AS3OPENNI_SERVER_INIT:
								this._bridgeReady = true;
								this.setupCaptureFeatures();
								this.log(Definitions.AS3OPENNI_CAPTURE_CONNECTED);
								this.dispatchEvent(new AS3OpenNIEvent(AS3OpenNIEvent.READY));
								break;
						}
						break;
					
					case Definitions.OPENNI_ID:
						switch(second)
						{
							case Definitions.OPENNI_GET_DEPTH:
								this.dispatchEvent(new OpenNIEvent(OpenNIEvent.ON_DEPTH, buffer));
								this._depthBuffer.busy = false;
								break;
							
							case Definitions.OPENNI_GET_VIDEO:
								this.dispatchEvent(new OpenNIEvent(OpenNIEvent.ON_VIDEO, buffer));
								this._videoBuffer.busy = false;
								break;
							
							case Definitions.OPENNI_USER_FOUND:
								this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_NEW_USER, Number(buffer.readInt())));
								break;
							
							case Definitions.OPENNI_USER_LOST:
								this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_LOST_USER, Number(buffer.readInt())));
								break;
							
							case Definitions.OPENNI_GET_USERS:
								var userId:uint = buffer.readInt();
								if(userId.toString().length < 2 && userId > 0)
								{
									var userData:NiPoint3D = new NiPoint3D();
									userData.user = userId;
									userData.pointX = buffer.readFloat();
									userData.pointY = buffer.readFloat();
									userData.pointZ = buffer.readFloat();
									this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_USER_FOUND, userId, userData));
								}
								this._userTrackingBuffer.busy = false;
								break;
							
							case Definitions.OPENNI_GET_SKELETONS:
								var skelId:uint = buffer.readInt();
								if(skelId.toString().length < 2 && skelId > 0)
								{
									var skel:NiSkeleton = new NiSkeleton();
									skel.user = skelId;
									skel.update(buffer);
									this.dispatchEvent(new ONISkeletonEvent(ONISkeletonEvent.USER_TRACKING, skelId, skel.leftHand, skel.rightHand, skel));
								}
								this._skeletonsBuffer.busy = false;
								break;
							
							case Definitions.OPENNI_POSE_DETECTED:
								this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_POSE_DETECTED, Number(buffer.readInt())));
								break;
							
							case Definitions.OPENNI_CALIBRATION_STARTED:
								this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_START, Number(buffer.readInt())));
								break;
							
							case Definitions.OPENNI_CALIBRATION_COMPLETE:
								this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_COMPLETE, Number(buffer.readInt())));
								break;
							
							case Definitions.OPENNI_CALIBRATION_FAILED:
								this.dispatchEvent(new ONIUserTrackingEvent(ONIUserTrackingEvent.USER_TRACKING_USER_CALIBRATION_FAILED, Number(buffer.readInt())));
								break;
						}
						break;
					
					case Definitions.NITE_ID:
						break;
				}
			}
		}
		
		protected function onClientSocketConnected(event:ClientSocketEvent):void
		{
			this._clientReady = true;
			this.log(Definitions.CLIENT_SOCKET_CONNECTED);
			this.dispatchEvent(new ClientSocketEvent(event.type, event.data));
		}
		
		protected function onClientSocketError(event:ClientSocketEvent):void
		{
			this.log(Definitions.CLIENT_SOCKET_ERROR);
			this.dispatchEvent(new ClientSocketEvent(event.type, event.data));
		}
		
		protected function setupCaptureFeatures():void
		{
			if(this._clientReady && this._bridgeReady)
			{
				// Setup the video buffer.
				if(this.video) this._videoBuffer = new VideoBuffer(this._captureClientSocket);
				
				// Setup the depth map buffer.
				if(this.depthMap) this._depthBuffer = new DepthBuffer(this._captureClientSocket);
				
				// Setup the user tracking buffer.
				if(this.userTracking && this.isWindows)
				{
					this._userTrackingBuffer = new UserTrackingBuffer(this._captureClientSocket);
					this._skeletonsBuffer = new SkeletonsBuffer(this._captureClientSocket);
				}
			}
		}
		
		protected function findOS():void
		{
			if(Capabilities.os.toLowerCase().indexOf("win") > -1)
			{
				this.isWindows = true;
			}
			else if (Capabilities.os.toLowerCase().indexOf("mac") > -1) 
			{
				this.isWindows = false;
			}
		}
		
		protected function startupBridge():void
		{     
			var file:File = new File();
			var startupInfo:NativeProcessStartupInfo = new NativeProcessStartupInfo();
			
			if(this.isWindows)
			{
				var ext:String = this.binaryPath.substr((this.binaryPath.length-4), this.binaryPath.length);
				var path:String = (ext == '.exe') ? this.binaryPath : (this.binaryPath + '.exe');
				file = File.applicationDirectory.resolvePath(path);
			} 
			else if (!this.isWindows) 
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
			
			// Turn on the DepthMapCapture feature only testing one or the other in this file.
			if(this.depthMap) 
			{
				// Pass to the binary.
				processArgs.push("-odmc");
				
				// Turn the DepthMaptCapture background on.
				if(this.depthMapBackground) processArgs.push("-dmbg");
				
				// Snap the pixels of the DepthMap and RGB together or not, default is false.
				if(this.depthMapSnapOff) processArgs.push("-snapoff");
			}
			
			// Turn on the RGBCapture feature only testing one or the other in this file.
			if(this.video)
			{
				// Pass to the binary.
				processArgs.push("-orgbc");
			}
			
			// Turn on the UserTracking feature.
			if(this.userTracking) 
			{
				// Pass to the binary.
				processArgs.push("-outf");
				
				// Turn on the convert real world to projective for the skeleton data.
				if(this.convertRealWorldToProjective) processArgs.push("-crwp");
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
		
		protected function log(msg:String):void
		{
			if(this.debug) trace(msg);
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
		
		protected function onClose(event:Event):void
		{
			this.dispatchEvent(new Event(event.type, event.bubbles, event.cancelable));
			if(this.bridge) this.bridge.exit(true);
		}
		
		protected function onOutputData(event:ProgressEvent):void
		{
			this.dispatchEvent(new ProgressEvent(event.type, event.bubbles, event.cancelable, event.bytesLoaded, event.bytesTotal));
			var msg:String = this.bridge.standardOutput.readMultiByte(this.bridge.standardOutput.bytesAvailable, File.systemCharset);
			this.log(msg);
		}
		
		protected function onErrorData(event:ProgressEvent):void
		{
			this.dispatchEvent(new ProgressEvent(event.type, event.bubbles, event.cancelable, event.bytesLoaded, event.bytesTotal));
			if(this.bridge)
			{
				var ba:Number = this.bridge.standardOutput.bytesAvailable;
				if(ba > 0) 
				{
					var msg:String = this.bridge.standardError.readMultiByte(ba, File.systemCharset);
					this.log(msg);
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