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
				cout<<"-aso || Turn off all sockets\n";
				cout<<" \n";
				cout<<"Turn On Features:\n";
				cout<<"-ospf || Turn on the SinglePoint feature\n";
				cout<<"-ogf || Turn on the Gestures feature\n";
				cout<<"-ocf || Turn on the Circle feature\n";
				cout<<"-osldf || Turn on the Slider feature\n";
				cout<<"-otpf || Turn on the TrackPad feature\n";
				cout<<"-outf || Turn on the UserTracking feature\n";
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
					cout<<"TrackPad Columns: "<< trackpad_columns <<"\n";
				}
			
				if(param == "-tpr")
				{
					trackpad_rows = atoi(argv[i + 1]);
					cout<<"TrackPad Rows: "<< trackpad_rows <<"\n";
				}
			}
			
			// If it's just for a single function.
			if(param == "-aso")
			{
				_useSockets = false;
				cout<<"All sockets are off\n";
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
			
			// Print out feature events.
			if(param == "-pspf")
			{
				_printSinglePointOnly = true;
				cout<<"SinglePoint events will print out\n";
			}
			
			if(param == "-pgf")
			{
				_printGestureOnly = true;
				cout<<"Gesture events will print out\n";
			}
			
			if(param == "-pcf")
			{
				_printCircleOnly = true;
				cout<<"Circle events will print out\n";
			}
			
			if(param == "-psldf")
			{
				_printSliderOnly = true;
				cout<<"Slider events will print out\n";
			}
			
			if(param == "-ptpf")
			{
				_printTrackPadOnly = true;
				cout<<"TrackPad events will print out\n";
			}
			
			if(param == "-putf")
			{
				_printUserTrackingOnly = true;
				cout<<"UserTracking events will print out\n";
			}
		}
	}
}