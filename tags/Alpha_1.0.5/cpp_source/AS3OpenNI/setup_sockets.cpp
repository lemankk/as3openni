#include "setup_sockets.h"

#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
	#include <netdb.h>
	
	extern int POINT_SOCKET, SESSION_SOCKET, SLIDER_SOCKET, USER_TRACKING_SOCKET/*, DEPTH_MAP_SOCKET*/;
	
	struct sockaddr_in _point_serv_addr;
	struct sockaddr_in _session_serv_addr;
	struct sockaddr_in _slider_serv_addr;
	struct sockaddr_in _user_tracking_serv_addr;
	//struct sockaddr_in _depth_map_serv_addr;
	
	int POINT_PORT = 9500;
	int SESSION_PORT = 9501;
	int SLIDER_PORT = 9502;
	int USER_TRACKING_PORT = 9503;
	//int DEPTH_MAP_PORT = 9504;
#else
	#include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include "socket.h"
	
	#define WIN32_LEAN_AND_MEAN

	// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
	#pragma comment (lib, "Ws2_32.lib")
	#pragma comment (lib, "Mswsock.lib")
	#pragma comment (lib, "AdvApi32.lib")

	#define DEFAULT_BUFLEN 512

	extern SOCKET POINT_SOCKET;
	extern SOCKET SESSION_SOCKET;
	extern SOCKET SLIDER_SOCKET;
	extern SOCKET USER_TRACKING_SOCKET;
	//extern SOCKET DEPTH_MAP_SOCKET;

	int POINT_RESULT;
	int SESSION_RESULT;
	int SLIDER_RESULT;
	int USER_TRACKING_RESULT;
	//int DEPTH_MAP_RESULT;
	
	#define WIN_POINT_PORT "9500"
	#define WIN_SESSION_PORT "9501"
	#define WIN_SLIDER_PORT "9502"
	#define WIN_USER_TRACKING_PORT "9503"
	//#define WIN_DEPTH_MAP_PORT "9504"
#endif

#define MAX_PACKET_SIZE 320*240*4
unsigned char *imgMsg = new unsigned char[MAX_PACKET_SIZE];
	
void setupSockets()
{
	if(_useSockets)
	{
		//-------------------------------------------------------------------------------//
		//------------------------- SEND DATA TO SOCKET SERVER --------------------------//
		//-------------------------------------------------------------------------------//
		
	    #if (XN_PLATFORM == XN_PLATFORM_MACOSX)
	    	
	    	//------------------------- SESSION SOCKET --------------------------//
	        //------------------------------------------------------------------//
	        SESSION_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
	    	if (SESSION_SOCKET < 0) 
	        	error("ERROR opening session socket");
	        
	        memset(&_session_serv_addr, 0, sizeof(struct sockaddr_in));
	    	_session_serv_addr.sin_family = AF_INET;
	    	_session_serv_addr.sin_port = htons(SESSION_PORT);
	    
	    	if (connect(SESSION_SOCKET,(struct sockaddr *) &_session_serv_addr,sizeof(_session_serv_addr)) < 0) 
	        	error("ERROR connecting to session socket");
	    	
	    	if(_featureSinglePoint)
	    	{
		    	//------------------------- POINT SOCKET --------------------------//
		    	//----------------------------------------------------------------//
		    	POINT_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
		    	if (POINT_SOCKET < 0) 
		        	error("ERROR opening point socket");
		        
		        memset(&_point_serv_addr, 0, sizeof(struct sockaddr_in));
		        _point_serv_addr.sin_family = AF_INET;
		    	_point_serv_addr.sin_port = htons(POINT_PORT);
		    	
		    	if (connect(POINT_SOCKET,(struct sockaddr *) &_point_serv_addr,sizeof(_point_serv_addr)) < 0) 
		        	error("ERROR connecting to point socket");
	    	}
	        	
	    	if(_featureSlider)
	    	{
		        //------------------------- SLIDER SOCKET --------------------------//
		    	//-----------------------------------------------------------------//
		        SLIDER_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
		    	if (SLIDER_SOCKET < 0) 
		        	error("ERROR opening slider socket");
		        
		        memset(&_slider_serv_addr, 0, sizeof(struct sockaddr_in));
		    	_slider_serv_addr.sin_family = AF_INET;
		    	_slider_serv_addr.sin_port = htons(SLIDER_PORT);
		    
		    	if (connect(SLIDER_SOCKET,(struct sockaddr *) &_slider_serv_addr,sizeof(_slider_serv_addr)) < 0) 
		        	error("ERROR connecting to slider socket");
	    	}
	    	
	    	if(_featureUserTracking)
	    	{
		    	//------------------------- USER TRACKING SOCKET --------------------------//
		    	//------------------------------------------------------------------------//
		        USER_TRACKING_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
		    	if (USER_TRACKING_SOCKET < 0) 
		        	error("ERROR opening user tracking socket");
		        
		        memset(&_user_tracking_serv_addr, 0, sizeof(struct sockaddr_in));
		        _user_tracking_serv_addr.sin_family = AF_INET;
		        _user_tracking_serv_addr.sin_port = htons(USER_TRACKING_PORT);
		    
		    	if (connect(USER_TRACKING_SOCKET,(struct sockaddr *) &_user_tracking_serv_addr,sizeof(_user_tracking_serv_addr)) < 0) 
		        	error("ERROR connecting to user tracking socket");
	    	}
	    	
	    	/*
	    	//------------------------- DEPTH MAP SOCKET --------------------------//
	    	//--------------------------------------------------------------------//
	        DEPTH_MAP_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
	    	if (DEPTH_MAP_SOCKET < 0) 
	        	error("ERROR opening depth map capture socket");
	        
	        memset(&_depth_map_serv_addr, 0, sizeof(struct sockaddr_in));
	        _depth_map_serv_addr.sin_family = AF_INET;
	        _depth_map_serv_addr.sin_port = htons(DEPTH_MAP_PORT);
	    
	    	if (connect(DEPTH_MAP_SOCKET,(struct sockaddr *) &_depth_map_serv_addr,sizeof(_depth_map_serv_addr)) < 0) 
	        	error("ERROR connecting to depth map capture socket");
	    	}*/
	    	
	    #else
	    
			WSADATA pointData;
			WSADATA sessionData;
			WSADATA sliderData;
			WSADATA userTrackingData;
			
			POINT_SOCKET = INVALID_SOCKET;
			SESSION_SOCKET = INVALID_SOCKET;
			SLIDER_SOCKET = INVALID_SOCKET;
			USER_TRACKING_SOCKET = INVALID_SOCKET;
			
	    	struct addrinfo *pointResult = NULL, 
	    					*sessionResult = NULL,
	    					*sliderResult = NULL,
	    					*userTrackingResult = NULL,
	    					*pointObj = NULL, 
	    					*sessionObj = NULL,
	    					*sliderObj = NULL,
	    					*userTrackingObj = NULL,
	    					pointHints, 
	    					sessionHints,
	    					sliderHints,
	    					userTrackingHints;
	    					
	    	char *pointBuf = "This is a point test.";
	    	char *sessionBuf = "This is a session test.";
	    	char *sliderBuf = "This is a slider test.";
	    	char *userTrackingBuf = "This is a user tracking test.";
	    	int recvbuflen = DEFAULT_BUFLEN;
	    	
	    	//------------------------- SESSION SOCKET --------------------------//
	    	//------------------------------------------------------------------//
	    	SESSION_RESULT = WSAStartup(MAKEWORD(2,2), &sessionData);
	    	if(SESSION_RESULT != 0) 
	    	{
	        	printf("WSAStartup session failed: %d\n", SESSION_RESULT);
	        	return;
	    	}
	    	
	    	// Setup session hints.
	    	ZeroMemory(&sessionHints, sizeof(sessionHints));
	    	sessionHints.ai_family = AF_UNSPEC;
	   		sessionHints.ai_socktype = SOCK_STREAM;
	    	sessionHints.ai_protocol = IPPROTO_TCP;
	    	
	    	SESSION_SOCKET = getaddrinfo("127.0.0.1", WIN_SESSION_PORT, &sessionHints, &sessionResult);
	    	if(SESSION_RESULT != 0) 
	    	{
	        	printf("Getaddrinfo failed on session socket server: %d\n", SESSION_SOCKET);
	        	WSACleanup();
	        	return;
	        }
	        
	        // Attempt to connect to an address until one succeeds on the session server.
	    	for(sessionObj=sessionResult; sessionObj != NULL; sessionObj=sessionObj->ai_next) 
	    	{
	        	// Create the session socket.
	        	SESSION_SOCKET = socket(sessionObj->ai_family, sessionObj->ai_socktype, sessionObj->ai_protocol);
	        	if (SESSION_SOCKET == INVALID_SOCKET) 
	        	{
	            	printf("Error at session socket(): %ld\n", WSAGetLastError());
	            	return;
	        	}
	        	
	        	// Connect to the session server.
	        	SESSION_RESULT = connect(SESSION_SOCKET, sessionObj->ai_addr, (int)sessionObj->ai_addrlen);
	        	if(SESSION_RESULT == SOCKET_ERROR) 
	        	{
	            	SESSION_SOCKET = INVALID_SOCKET;
	            	continue;
	        	}
	        	
	        	break;
	    	}
	    	
	    	if(SESSION_SOCKET == INVALID_SOCKET) 
	    	{
	        	printf("Unable to connect to session server!\n");
	        	return;
	    	}
	    	
	    	// Send an initial buffer to session server.
	    	SESSION_RESULT = send(SESSION_SOCKET, sessionBuf, (int)strlen(sessionBuf), 0);
	    	if(SESSION_RESULT == SOCKET_ERROR) 
	    	{
	        	printf("Send failed to session server: %d\n", WSAGetLastError());
	        	return;
	    	}
	    	
	    	if(_featureSinglePoint)
	    	{
		    	//------------------------- POINT SOCKET --------------------------//
		    	//----------------------------------------------------------------//
		    	POINT_RESULT = WSAStartup(MAKEWORD(2,2), &pointData);
		    	if(POINT_RESULT != 0) 
		    	{
		        	printf("WSAStartup point failed: %d\n", POINT_RESULT);
		        	return;
		    	}
		    	
		    	// Setup point hints.
		    	ZeroMemory(&pointHints, sizeof(pointHints));
		    	pointHints.ai_family = AF_UNSPEC;
		   		pointHints.ai_socktype = SOCK_STREAM;
		    	pointHints.ai_protocol = IPPROTO_TCP;
		    	
		    	// Resolve the server address and port
		    	POINT_RESULT = getaddrinfo("127.0.0.1", WIN_POINT_PORT, &pointHints, &pointResult);
		    	if(POINT_RESULT != 0) 
		    	{
		        	printf("Getaddrinfo failed on point server: %d\n", POINT_RESULT);
		        	WSACleanup();
		        	return;
		        }
		        
		        // Attempt to connect to an address until one succeeds on the point server.
		    	for(pointObj=pointResult; pointObj != NULL; pointObj=pointObj->ai_next) 
		    	{
		        	// Create the point socket.
		        	POINT_SOCKET = socket(pointObj->ai_family, pointObj->ai_socktype, pointObj->ai_protocol);
		        	if (POINT_SOCKET == INVALID_SOCKET) 
		        	{
		            	printf("Error at point socket(): %ld\n", WSAGetLastError());
		            	return;
		        	}
		        	
		        	// Connect to the point server.
		        	POINT_RESULT = connect(POINT_SOCKET, pointObj->ai_addr, (int)pointObj->ai_addrlen);
		        	if (POINT_RESULT == SOCKET_ERROR) 
		        	{
		            	POINT_SOCKET = INVALID_SOCKET;
		            	continue;
		        	}
		        	
		        	break;
		    	}
		    	
		    	if(POINT_SOCKET == INVALID_SOCKET) 
		    	{
		        	printf("Unable to connect to point server!\n");
		        	return;
		    	}
		    	
		    	// Send an initial buffer to point server.
		    	POINT_RESULT = send(POINT_SOCKET, pointBuf, (int)strlen(pointBuf), 0);
		    	if(POINT_RESULT == SOCKET_ERROR) 
		    	{
		        	printf("Send failed to point server: %d\n", WSAGetLastError());
		        	return;
		    	}
	    	}
	        
	    	if(_featureSlider)
	    	{
		    	//------------------------- SLIDER SOCKET --------------------------//
		    	//------------------------------------------------------------------//
		    	SLIDER_RESULT = WSAStartup(MAKEWORD(2,2), &sliderData);
		    	if(SLIDER_RESULT != 0) 
		    	{
		        	printf("WSAStartup slider failed: %d\n", SLIDER_RESULT);
		        	return;
		    	}
		    	
		    	// Setup slider hints.
		    	ZeroMemory(&sliderHints, sizeof(sliderHints));
		    	sliderHints.ai_family = AF_UNSPEC;
		   		sliderHints.ai_socktype = SOCK_STREAM;
		    	sliderHints.ai_protocol = IPPROTO_TCP;
		    	
		        SLIDER_SOCKET = getaddrinfo("127.0.0.1", WIN_SLIDER_PORT, &sliderHints, &sliderResult);
		    	if(SLIDER_RESULT != 0) 
		    	{
		        	printf("Getaddrinfo failed on slider socket server: %d\n", SLIDER_SOCKET);
		        	WSACleanup();
		        	return;
		        }
				
				// Attempt to connect to an address until one succeeds on the slider server.
		    	for(sliderObj=sliderResult; sliderObj != NULL; sliderObj=sliderObj->ai_next) 
		    	{
		        	// Create the session socket.
		        	SLIDER_SOCKET = socket(sliderObj->ai_family, sliderObj->ai_socktype, sliderObj->ai_protocol);
		        	if (SLIDER_SOCKET == INVALID_SOCKET) 
		        	{
		            	printf("Error at slider socket(): %ld\n", WSAGetLastError());
		            	return;
		        	}
		        	
		        	// Connect to the slider server.
		        	SLIDER_RESULT = connect(SLIDER_SOCKET, sliderObj->ai_addr, (int)sliderObj->ai_addrlen);
		        	if(SLIDER_RESULT == SOCKET_ERROR) 
		        	{
		            	SLIDER_SOCKET = INVALID_SOCKET;
		            	continue;
		        	}
		        	
		        	break;
		    	}
		    	
		    	if(SLIDER_SOCKET == INVALID_SOCKET) 
		    	{
		        	printf("Unable to connect to slider server!\n");
		        	return;
		    	}
		    	
		    	// Send an initial buffer to slider server.
		    	SLIDER_RESULT = send(SLIDER_SOCKET, sliderBuf, (int)strlen(sliderBuf), 0);
		    	if(SLIDER_RESULT == SOCKET_ERROR) 
		    	{
		        	printf("Send failed to slider server: %d\n", WSAGetLastError());
		        	return;
		    	}
	    	}
	    	
	    	if(_featureUserTracking)
	    	{
		    	//------------------------- USER TRACKING SOCKET --------------------------//
		    	//------------------------------------------------------------------------//
		    	USER_TRACKING_RESULT = WSAStartup(MAKEWORD(2,2), &userTrackingData);
		    	if(USER_TRACKING_RESULT != 0) 
		    	{
		        	printf("WSAStartup user tracking failed: %d\n", USER_TRACKING_RESULT);
		        	return;
		    	}
		    	
		    	// Setup user tracking hints.
		    	ZeroMemory(&userTrackingHints, sizeof(userTrackingHints));
		    	userTrackingHints.ai_family = AF_UNSPEC;
		    	userTrackingHints.ai_socktype = SOCK_STREAM;
		    	userTrackingHints.ai_protocol = IPPROTO_TCP;
		    	
		    	USER_TRACKING_SOCKET = getaddrinfo("127.0.0.1", WIN_USER_TRACKING_PORT, &userTrackingHints, &userTrackingResult);
		    	if(USER_TRACKING_RESULT != 0) 
		    	{
		        	printf("Getaddrinfo failed on user tracking socket server: %d\n", USER_TRACKING_SOCKET);
		        	WSACleanup();
		        	return;
		        }
				
				// Attempt to connect to an address until one succeeds on the user tracking server.
		    	for(userTrackingObj=userTrackingResult; userTrackingObj != NULL; userTrackingObj=userTrackingObj->ai_next) 
		    	{
		        	// Create the session socket.
		    		USER_TRACKING_SOCKET = socket(userTrackingObj->ai_family, userTrackingObj->ai_socktype, userTrackingObj->ai_protocol);
		        	if (USER_TRACKING_SOCKET == INVALID_SOCKET)
		        	{
		            	printf("Error at user tracking socket(): %ld\n", WSAGetLastError());
		            	return;
		        	}
		        	
		        	// Connect to the user tracking server.
		        	USER_TRACKING_RESULT = connect(USER_TRACKING_SOCKET, userTrackingObj->ai_addr, (int)userTrackingObj->ai_addrlen);
		        	if(USER_TRACKING_RESULT == SOCKET_ERROR) 
		        	{
		        		USER_TRACKING_SOCKET = INVALID_SOCKET;
		            	continue;
		        	}
		        	
		        	break;
		    	}
		    	
		    	if(USER_TRACKING_SOCKET == INVALID_SOCKET) 
		    	{
		        	printf("Unable to connect to user tracking server!\n");
		        	return;
		    	}
		    	
		    	// Send an initial buffer to user tracking server.
		    	USER_TRACKING_RESULT = send(USER_TRACKING_SOCKET, userTrackingBuf, (int)strlen(userTrackingBuf), 0);
		    	if(USER_TRACKING_RESULT == SOCKET_ERROR) 
		    	{
		        	printf("Send failed to user tracking server: %d\n", WSAGetLastError());
		        	return;
		    	}
	    	}
	    	
		#endif
		
	    //-------------------------------------------------------------------------------//
		//------------------------- SEND DATA TO SOCKET SERVER --------------------------//
		//-------------------------------------------------------------------------------//
	}
}
	
void sendToSocket(int socket, const char *data)
{
	#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
		if(write(socket, data, (strlen(data) + 1)) < 0) 
		{
			error("ERROR writing to socket");
		}
	#else
		send(socket, data, (strlen(data) + 1), 0);
	#endif
}

/*
void sendByteToSocket(int socket, FIBITMAP* dib)
{
	//int m_len = 1 + 1 + sizeof(int);
	//memcpy(imgMsg, dib, sizeof(dib));
	
	#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
		if(write(socket, (char*)dib, MAX_PACKET_SIZE) < 0) 
		{
			error("ERROR writing to socket");
		}
	#else
		send(socket, (char*)dib, MAX_PACKET_SIZE, 0);
	#endif
}*/