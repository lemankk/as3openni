# New Project: AS3NUI #
Similar project to check out too, that I've been working on and using with my projects is AS3NUI, it allows you to build an Adobe Air application that runs with a Native Extension, so the performance is awesome: Check it out here: http://www.as3nui.com/

# Current Version: Alpha 1.3.0 #

This project has taken the OpenNI and Prime Sense NITE frameworks and exposed them to Adobe Flash and the Adobe Flex framework through the use of socket servers. Now allowing Flash developers the ability to quickly build out Adobe Air applications that are controlled all by Natural Interaction, or in other words YOU!

### How it works? ###
Your Adobe Air application will run as the socket server and the AS3OpenNI binary file will execute through your Air application connecting to the Microsoft Kinect camera or other Prime Sense devices and act as a socket client broadcasting out the data from the camera to your Air application. The AS3OpenNI binary acts as a bridge between the NI device and Flash through sockets running locally on your computer. Keep in mind the AS3OpenNI binary is not limited to just Flash as long as you setup a socket server somewhere listening to the correct ports then the AS3OpenNI client will broadcast out the camera's data to those dedicated ports.

### License: ###
  * All Action Script 3.0 or higher code is released under the MIT license agreement.
  * All C++ code is released under the GNU GPL v2 license agreement.

### Installation help: ###

**Hardware/Middleware Support:**
http://www.openni.org/downloadfiles

**For PC:**
http://www.codeproject.com/Articles/148251/How-to-Successfully-Install-Kinect-on-Windows-Open.aspx

**For Mac OS X:**
http://c4c.posterous.com/installing-openni-kinect-drivers-and-nite-on

### OS Support: ###
  * Mac OS X
  * Windows

### Flash Builder - Examples Project Setup: ###
  1. Open up Adobe Flash Builder
  1. File>Import>Flash Builder Project...
  1. Browse by File, and find this file '..Examples.fxp'
  1. Make sure you've created a 'bin' folder in your 'bin-debug' folder with the 'AS3OpenNI.exe' binary included in it.
  1. For Mac, it's the 'AS3OpenNI' binary file.
  1. Then make a wave gesture and watch the magic!

### Version: Alpha 1.0.0: Features: ###
  * Multiple Socket Support
  * Primary Point
  * Push Detector
  * Steady Detector
  * Swipe Detector
    1. Swipe Left
    1. Swipe Right
    1. Swipe Up
    1. Swipe Down
  * Wave Detector
  * Circle Detector

## Version: Alpha 1.0.5: Features: ##
  * Selectable Slider 1D/Slider
  * Selectable Slider 2D/(Track Pad Example)
  * Control which features/process you wish to use on startup of the application
  * Command Line Parameters added mainly for debugging and memory control
    1. -tpc 4 || # of Track Pad Columns, default is 4
    1. -tpr 9 || # of Track Pad Rows, default is 9
    1. -aso || Turn on all sockets
    1. -ospf || Turn on the Single Point feature
    1. -ogf || Turn on the Gestures feature
    1. -ocf || Turn on the Circle feature
    1. -osldf || Turn on the Slider feature
    1. -otpf || Turn on the Track Pad feature
    1. -outf || Turn on the User Tracking feature
    1. -pspf || Print out the Single Point events
    1. -pgf || Print out the Gesture events
    1. -pcf || Print out the Circle events
    1. -psldf || Print out the Slider events
    1. -ptpf || Print out the Track Pad events
    1. -putf || Print out the User Tracking events
  * User Tracking
    1. Single Player Detection
    1. Skeleton Detection per user

### Version: Alpha 1.0.8: Features: ###
  * Depth Map Capture socket added, plus image quality control
  * RGB Capture socket added, plus image quality control
  * More Command Line Parameters added
    1. -dmq 1 || Depth Map Capture quality, 0 = Low, 1 = Average, 2 = High, 3 = Super High, default is 1
    1. -rgbq 1 || RGB Capture quality, 0 = Low, 1 = Average, 2 = High, 3 = Super High, default is 1
    1. -grey || RGB Capture render in gray scale, default is false
    1. -snap || Snap the RGB Capture pixels with the Depth Map Capture pixels, default is false
  * Velocity and Angle data available for the Gestures, Push Detector, Swipe Left, Swipe Right, Swipe Up, and Swipe Down
  * For the Steady Hand Detector the Velocity data will be available to
  * Second player detection and additional socket added

### Version: Alpha 1.0.9: Features: ###
  * Cleaned up the socket connections support for Mac
  * Adding the FAAST action list to the AS3OpenNI code base
  * Get single point data when a user is found, but not tracked
  * Can now track up to 15 players, however max tested was up to three on both Mac and PC
  * Turn off the mirror mode, so everything looks opposite
  * Fixing the RGB and Depth Map capture to work better
  * Plus, it will have the Depth Map from black feature
  * Updated the build to unstable version OpenNI v1.1.0.41 framework to work on Mac and PC
  * Updated the build to unstable version Prime Sense NITE v1.3.1.5 framework to work on Mac and PC
  * Add multiple skeleton support with the Skeletons examples

### Version: Alpha 1.3.0: Features: ###
  * Need to improve RGB capture performance on PC and Mac, still pretty slow
  * Add multiple skeleton support with the Skeletons examples.
  * Removed the Free Image support.
  * Added an option to convert the skeleton data from real world to projective.

### TODO LIST: ###
  * Getting Started video and tutorial document
  * Add Hand/Finger Tracking
  * Node.js/Pure Action Script 3.0 project examples
  * Flash Builder Pro examples
  * Multiple camera support
  * Face detection

**Wish List:** http://groups.google.com/group/as3openni/browse_thread/thread/2547da99564173f7

### Framework Support on PC: ###
  * OpenNI Framework 1.1.0.38
  * Prime Sense NITE Framework 1.3.1.13
  * Avin2 - Sensor Kinect v5.0.1.34

### Framework Support on Mac: ###
  * OpenNI Framework 1.1.041
  * Prime Sense NITE Framework 1.3.1.5
  * Avin2 - Sensor Kinect v5.0.1.34
  * KNOWN USB BUG: Mac will freeze from time to time, no idea why. Sorry!
  * If you hit (Command+Tab) and toggle to another application and then back it fixes the bug.

### Forum Support: ###

**OpenNI/Nite Support:**
http://groups.google.com/group/openni-dev

**AS3OpenNI Support:**
http://groups.google.com/group/as3openni

## Pay Pal Donations Link: ##
**Email:** paypal@mosaiceye.com

**Form Link:** https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=PCC32BPZVLXX2

All donations will go back into the research and support for this project you can go here to make a donation.