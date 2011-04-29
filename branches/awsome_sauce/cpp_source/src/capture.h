#include <XnOpenNI.h>
#include <XnCppWrapper.h>

using namespace xn;
extern ImageGenerator g_ImageGenerator;
extern DepthGenerator g_DepthGenerator;
extern UserGenerator g_UserGenerator;
extern XnBool g_bDrawBackground;
extern XnBool g_bDrawPixels;

class capture 
{
	public:
		void getRGB(unsigned char* g_ucImageBuffer);
		void getDepthMap(unsigned char* g_ucDepthBuffer);
};