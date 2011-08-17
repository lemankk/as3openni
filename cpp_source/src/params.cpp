#include "params.h"

void setupParams(int argc, char *argv[])
{
	if (argc >= 2)
	{
		for (int i=0; i<argc; i++)
		{
			string param = argv[i];
			
			// For the help menu.
			if(param == "-help")
			{
				cout<<" \n";
				cout<<"AS3OpenNI - Command Line Parameters\n";
				cout<<"Author: Tony Birleffi\n";
				cout<<"===============================================================\n";
				cout<<" \n";
				cout<<"Extra Functions:\n";
				cout<<"-tpc 4 || # of TrackPad Columns, default is 4\n";
				cout<<"-tpr 9 || # of TrackPad Rows, default is 9\n";
				cout<<"-grey || RGBCapture render in grayscale, default is false\n";
				cout<<"-dmbg || Turn on the DepthMapCapture background, default is false\n";
				cout<<"-dmdo || Turn off the DepthMapDetect, default is true\n";
				cout<<"-snapoff || Snap the RGBCapture pixels with the DepthMapCapture pixels, default is on\n";
				cout<<"-mrev || Mirror mode will be set to false, default is true\n";
				cout<<"-crwp || Convert skeleton data from real world to projective, default is off\n";
				cout<<"-aso || Turn off all sockets\n";
				cout<<" \n";
				cout<<"Turn On Features:\n";
				cout<<"-ospf || Turn on the SinglePoint feature\n";
				cout<<"-ogf || Turn on the Gestures feature\n";
				cout<<"-ocf || Turn on the Circle feature\n";
				cout<<"-osldf || Turn on the Slider feature\n";
				cout<<"-otpf || Turn on the TrackPad feature\n";
				cout<<"-outf || Turn on the UserTracking feature\n";
				cout<<"-orgbc || Turn on the RGBCapture feature\n";
				cout<<"-odmc || Turn on the DepthMapCapture feature\n";
				cout<<" \n";
				cout<<"Print Functions:\n";
				cout<<"-pspf || Print out the SinglePoint events\n";
				cout<<"-pgf || Print out the Gesture events\n";
				cout<<"-pcf || Print out the Circle events\n";
				cout<<"-psldf || Print out the Slider events\n";
				cout<<"-ptpf || Print out the TrackPad events\n";
				cout<<"-putf || Print out the UserTracking events\n";
				cout<<" \n";
				cout<<"===============================================================\n";
				cout<<" \n";
				
				exit(1);
			}
			
			// If the argument has a value.
			if(argv[i + 1])
			{
				if(param == "-tpc")
				{
					trackpad_columns = atoi(argv[i + 1]);
					cout<<"AS3OpenNI :: TrackPad Columns: "<< trackpad_columns <<"\n";
				}
			
				if(param == "-tpr")
				{
					trackpad_rows = atoi(argv[i + 1]);
					cout<<"AS3OpenNI :: TrackPad Rows: "<< trackpad_rows <<"\n";
				}
			}
			
			// If it's just for a single function.
			if(param == "-aso")
			{
				_useSockets = false;
				cout<<"AS3OpenNI :: All sockets are off\n";
			}
			
			if(param == "-crwp")
			{
				_convertRealWorldToProjective = true;
				cout<<"AS3OpenNI :: Converting skeleton data from real world to projective is now on\n";
			}
			
			if(param == "-mrev")
			{
				_mirror = false;
				cout<<"AS3OpenNI :: Mirror mode is now off\n";
			}
			
			if(param == "-dmbg")
			{
				_depthMapBackground = true;
				cout<<"AS3OpenNI :: DepthMapCapture background is now on\n";
			}
			
			if(param == "-dmdo")
			{
				_depthMapDetect = false;
				if(!_depthMapBackground) _depthMapBackground = true;
				cout<<"AS3OpenNI :: DepthMapDetect is now off\n";
			}
			
			// Turn on features.
			if(param == "-ospf")
			{
				_featureSinglePoint = true;
				cout<<"AS3OpenNI :: SinglePoint feature turned on\n";
			}
			
			if(param == "-ogf")
			{
				_featureGesture = true;
				cout<<"AS3OpenNI :: Gesture feature turned on\n";
			}
			
			if(param == "-ocf")
			{
				_featureCircle = true;
				cout<<"AS3OpenNI :: Circle feature turned on\n";
			}
			
			if(param == "-osldf")
			{
				_featureSlider = true;
				cout<<"AS3OpenNI :: Slider feature turned on\n";
			}
			
			if(param == "-otpf")
			{
				_featureTrackPad = true;
				cout<<"AS3OpenNI :: TrackPad feature turned on\n";
			}
			
			if(param == "-outf")
			{
				_featureUserTracking = true;
				cout<<"AS3OpenNI :: UserTracking feature turned on\n";
			}
			
			if(param == "-orgbc")
			{
				_featureRGBCapture = true;
				cout<<"AS3OpenNI :: RGBCapture feature turned on\n";
			}
			
			if(param == "-odmc")
			{
				_featureDepthMapCapture = true;
				cout<<"AS3OpenNI :: DepthMapCapture feature turned on\n";
			}
			
			// Print out feature events.
			if(param == "-pspf")
			{
				_printSinglePoint = true;
				cout<<"AS3OpenNI :: SinglePoint events will print out\n";
			}
			
			if(param == "-pgf")
			{
				_printGesture = true;
				cout<<"AS3OpenNI :: Gesture events will print out\n";
			}
			
			if(param == "-pcf")
			{
				_printCircle = true;
				cout<<"AS3OpenNI :: Circle events will print out\n";
			}
			
			if(param == "-psldf")
			{
				_printSlider = true;
				cout<<"AS3OpenNI :: Slider events will print out\n";
			}
			
			if(param == "-ptpf")
			{
				_printTrackPad = true;
				cout<<"AS3OpenNI :: TrackPad events will print out\n";
			}
			
			if(param == "-putf")
			{
				_printUserTracking = true;
				cout<<"AS3OpenNI :: UserTracking events will print out\n";
			}
			
			if(param == "-snapoff")
			{
				_snapPixels = false;
				cout<<"AS3OpenNI :: Snapping the RGBCapture pixels with the DepthMapCapture pixels is now off\n";
			}
		}
	}
}