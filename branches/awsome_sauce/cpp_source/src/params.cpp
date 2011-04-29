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
				cout<<"-dmbg || Turn on the DepthMapCapture background, default is false\n";
				cout<<"-mrev || Mirror mode will be set to false, default is true\n";
				cout<<"-aso || Turn off the socket server\n";
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
				cout<<"===============================================================\n";
				cout<<" \n";
				
				exit(1);
			}
			
			// If the argument has a value.
			if(argv[i + 1])
			{
				if(param == "-tpc")
				{
					g_intTrackpadColumns = atoi(argv[i + 1]);
					cout<<"AS3OpenNI-Bridge :: TrackPad Columns: "<< g_intTrackpadColumns <<"\n";
				}
			
				if(param == "-tpr")
				{
					g_intTrackpadRows = atoi(argv[i + 1]);
					cout<<"AS3OpenNI-Bridge :: TrackPad Rows: "<< g_intTrackpadRows <<"\n";
				}
			}
			
			// If it's just for a single function.
			if(param == "-aso")
			{
				g_bUseSockets = false;
				cout<<"AS3OpenNI-Bridge :: The socket server is off\n";
			}
			
			if(param == "-mrev")
			{
				g_bMirror = false;
				cout<<"AS3OpenNI-Bridge :: Mirror mode is now false\n";
			}
			
			if(param == "-dmbg")
			{
				g_bDrawBackground = true;
				cout<<"AS3OpenNI-Bridge :: DepthMapCapture background is now on\n";
			}
			
			/*// Turn on features.
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
			}*/
			
			if(param == "-outf")
			{
				g_bFeatureUserTracking = true;
				cout<<"AS3OpenNI-Bridge :: UserTracking feature turned on\n";
			}
			
			if(param == "-orgbc")
			{
				g_bFeatureRGBCapture = true;
				cout<<"AS3OpenNI-Bridge :: RGBCapture feature turned on\n";
			}
			
			if(param == "-odmc")
			{
				g_bFeatureDepthMapCapture = true;
				cout<<"AS3OpenNI-Bridge :: DepthMapCapture feature turned on\n";
			}
		}
	}
}