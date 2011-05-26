#include <XnTypes.h>
#include <signal.h>
#include <math.h>

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	#include <pthread/pthread.h>
	#include <windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
	
	#pragma comment(lib, "Ws2_32.lib")
	#pragma comment(lib, "pthreadVC2.lib")
#else
	#include <netdb.h>
	#include <pthread.h>
#endif

typedef void (*callback)(void);

class network 
{
  	public:
		int init(callback cb);
		void closeConnection();
		int sendData(unsigned char *buffer, int length);
		int getData(unsigned char *buffer, int length);
		void sendMessage(const char *msg);
		void sendMessage(int first, int second, int value);
		void sendMessage(int first, int second, unsigned char *data, const int len);
		void waitForClient();
	private:
		#if (XN_PLATFORM == XN_PLATFORM_WIN32)
			int initServer(addrinfo si_type, PCSTR conf_port, PCSTR label);
		#else
			int initServer(addrinfo si_type, const char *conf_port, const char *label);
		#endif
};