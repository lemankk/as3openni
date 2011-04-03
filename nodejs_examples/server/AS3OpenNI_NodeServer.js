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

var net = require('net');

// Point server.
var pointData;
var pointServer;
var point = net.createServer(function (socket) 
{
	pointServer = socket;
	pointServer.setEncoding("utf8");
	pointServer.addListener("connect", function (data) 
	{
		//console.log("AS3OpenNI - Point Server Connected\0");
 	});
 	
	pointServer.addListener("data", function (data) 
	{
		if(data)
		{
			pointData = data;
			pointServer.write(pointData + "\0");
			console.log(pointData + "\0");
		}
 	});
 	
	pointServer.addListener("end", function () 
	{
		pointServer.end();
 	});
});
point.listen(9500, "127.0.0.1");

// Session server.
var sessionData;
var sessionServer;
var session = net.createServer(function (socket) 
{
	sessionServer = socket;
	sessionServer.setEncoding("utf8");
	sessionServer.addListener("connect", function (data) 
	{
		console.log("AS3OpenNI - Session Server Connected\0");
 	});
 	
 	sessionServer.addListener("data", function (data) 
	{
		if(data)
		{
			sessionData = data;
			sessionServer.write(sessionData + "\0");
			//console.log(sessionData + "\0");
		}
 	});
 	
 	sessionServer.addListener("end", function () 
	{
		sessionServer.end();
 	});
});
session.listen(9501, "127.0.0.1");

// Slider server.
var sliderData;
var sliderServer;
var slider = net.createServer(function (socket) 
{
	sliderServer = socket;
	sliderServer.setEncoding("utf8");
	sliderServer.addListener("connect", function (data) 
	{
		console.log("AS3OpenNI - Slider Server Connected\0");
 	});
 	
	sliderServer.addListener("data", function (data) 
	{
		if(data)
		{
			sliderData = data;
			sliderServer.write(sliderData + "\0");
			//console.log(sessionData + "\0");
		}
 	});
 	
	sliderServer.addListener("end", function () 
	{
		sliderServer.end();
 	});
});
slider.listen(9502, "127.0.0.1");

// User tracking server.
var userTrackingData;
var userTrackingServer;
var userTracking = net.createServer(function (socket) 
{
	userTrackingServer = socket;
	userTrackingServer.setEncoding("utf8");
	userTrackingServer.addListener("connect", function (data) 
	{
		console.log("AS3OpenNI - User Tracking Server Connected\0");
 	});
 	
	userTrackingServer.addListener("data", function (data) 
	{
		if(data)
		{
			userTrackingData = data;
			userTrackingServer.write(userTrackingData + "\0");
			//console.log(sessionData + "\0");
		}
 	});
 	
	userTrackingServer.addListener("end", function () 
	{
		userTrackingServer.end();
 	});
});
userTracking.listen(9503, "127.0.0.1");

// DepthMap capture server.
var depthMapData;
var depthMapServer;
var depthMap = net.createServer(function (socket) 
{
	depthMapServer = socket;
	depthMapServer.setEncoding("utf8");
	depthMapServer.addListener("connect", function (data) 
	{
		console.log("AS3OpenNI - RGB Capture Server Connected\0");
 	});
 	
	depthMapServer.addListener("data", function (data) 
	{
		if(data)
		{
			depthMapData = data;
			depthMapServer.write(depthMapData + "\0");
			//console.log(sessionData + "\0");
		}
 	});
 	
	depthMapServer.addListener("end", function () 
	{
		depthMapServer.end();
 	});
});
depthMap.listen(9504, "127.0.0.1");

// RGB capture server.
var rgbData;
var rgbServer;
var rgb = net.createServer(function (socket) 
{
	rgbServer = socket;
	rgbServer.setEncoding("utf8");
	rgbServer.addListener("connect", function (data) 
	{
		console.log("AS3OpenNI - RGB Capture Server Connected\0");
 	});
 	
	rgbServer.addListener("data", function (data) 
	{
		if(data)
		{
			rgbData = data;
			rgbServer.write(rgbData + "\0");
			//console.log(sessionData + "\0");
		}
 	});
 	
	rgbServer.addListener("end", function () 
	{
		rgbServer.end();
 	});
});
rgb.listen(9505, "127.0.0.1");
