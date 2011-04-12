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
				cout<<"-snap || Snap the RGBCapture pixels with the DepthMapCapture pixels, default is false\n";
				cout<<"-grey || RGBCapture render in grayscale, default is false\n";
				cout<<"-rgbq 0 || RGBCapture quality, 0 = Low, 1 = Average, 2 = High, 3 = Super High, default is 0\n";
				cout<<"-dmq 0 || DepthMapCapture quality, 0 = Low, 1 = Average, 2 = High, 3 = Super High, default is 0\n";
				cout<<"-mrev || Mirror mode will be set to false, default is true\n";
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
				cout<<"-prgbc || Print out the RGBCapture file size\n";
				cout<<"-pdmc || Print out the DepthMapCapture file size\n";
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
					cout<<"TrackPad Columns: "<< trackpad_columns <<"\n";
				}
			
				if(param == "-tpr")
				{
					trackpad_rows = atoi(argv[i + 1]);
					cout<<"TrackPad Rows: "<< trackpad_rows <<"\n";
				}
				
				if(param == "-rgbq")
				{
					rgb_quality = atoi(argv[i + 1]);
					cout<<"RGBCapture Quality: "<< rgb_quality <<"\n";
				}
				
				if(param == "-dmq")
				{
					depthmap_quality = atoi(argv[i + 1]);
					cout<<"DepthMapCapture Quality: "<< depthmap_quality <<"\n";
				}
			}
			
			// If it's just for a single function.
			if(param == "-aso")
			{
				_useSockets = false;
				cout<<"All sockets are off\n";
			}
			
			if(param == "-mrev")
			{
				_mirror = false;
				cout<<"Mirror mode is now false\n";
			}
			
			// Turn on features.
			if(param == "-ospf")
			{
				_featureSinglePoint = true;
				cout<<"SinglePoint feature turned on\n";
			}
			
			if(param == "-ogf")
			{
				_featureGesture = true;
				cout<<"Gesture feature turned on\n";
			}
			
			if(param == "-ocf")
			{
				_featureCircle = true;
				cout<<"Circle feature turned on\n";
			}
			
			if(param == "-osldf")
			{
				_featureSlider = true;
				cout<<"Slider feature turned on\n";
			}
			
			if(param == "-otpf")
			{
				_featureTrackPad = true;
				cout<<"TrackPad feature turned on\n";
			}
			
			if(param == "-outf")
			{
				_featureUserTracking = true;
				cout<<"UserTracking feature turned on\n";
			}
			
			if(param == "-orgbc")
			{
				_featureRGBCapture = true;
				cout<<"RGBCapture feature turned on\n";
			}
			
			if(param == "-odmc")
			{
				_featureDepthMapCapture = true;
				cout<<"DepthMapCapture feature turned on\n";
			}
			
			// Print out feature events.
			if(param == "-pspf")
			{
				_printSinglePoint = true;
				cout<<"SinglePoint events will print out\n";
			}
			
			if(param == "-pgf")
			{
				_printGesture = true;
				cout<<"Gesture events will print out\n";
			}
			
			if(param == "-pcf")
			{
				_printCircle = true;
				cout<<"Circle events will print out\n";
			}
			
			if(param == "-psldf")
			{
				_printSlider = true;
				cout<<"Slider events will print out\n";
			}
			
			if(param == "-ptpf")
			{
				_printTrackPad = true;
				cout<<"TrackPad events will print out\n";
			}
			
			if(param == "-putf")
			{
				_printUserTracking = true;
				cout<<"UserTracking events will print out\n";
			}
			
			if(param == "-prgbc")
			{
				_printRGBCapture = true;
				cout<<"Print out the RGBCapture file size\n";
			}
			
			if(param == "-pdmc")
			{
				_printDepthMapCapture = true;
				cout<<"Print out the DepthMapCapture file size\n";
			}
			
			if(param == "-grey")
			{
				_rgbGoGrey = true;
				cout<<"RGBCapture render in grayscale\n";
			}
			
			if(param == "-snap")
			{
				_snapPixels = true;
				cout<<"Snapping the RGBCapture pixels with the DepthMapCapture pixels is on\n";
			}
		}
	}
}