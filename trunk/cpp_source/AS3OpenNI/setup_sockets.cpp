#include "setup_sockets.h"

int POINT_RESULT;
int SESSION_RESULT;
int SLIDER_RESULT;
int USER_TRACKING_RESULT;
int DEPTH_MAP_RESULT;
int RGB_RESULT;

#define POINT_PORT "9500"
#define SESSION_PORT "9501"
#define SLIDER_PORT "9502"
#define USER_TRACKING_PORT "9503"
#define DEPTH_MAP_PORT "9504"
#define RGB_PORT "9505"

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	#include <windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include "socket.h"
	
	// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
	#pragma comment (lib, "Ws2_32.lib")
	#pragma comment (lib, "Mswsock.lib")
	#pragma comment (lib, "AdvApi32.lib")
	
	extern SOCKET POINT_SOCKET;
	extern SOCKET SESSION_SOCKET;
	extern SOCKET SLIDER_SOCKET;
	extern SOCKET USER_TRACKING_SOCKET;
	extern SOCKET DEPTH_MAP_SOCKET;
	extern SOCKET RGB_SOCKET;
#else
	#include <netdb.h>
	extern int POINT_SOCKET, SESSION_SOCKET, SLIDER_SOCKET, USER_TRACKING_SOCKET, DEPTH_MAP_SOCKET, RGB_SOCKET;
#endif

void setupSockets()
{
	if(_useSockets)
	{
		//-------------------------------------------------------------------------------//
		//------------------------- SEND DATA TO SOCKET SERVER --------------------------//
		//-------------------------------------------------------------------------------//
		
		struct addrinfo	sessionHints, 
						pointHints,
						sliderHints,
						userTrackingHints,
						depthMapHints,
						rgbHints,
						
						*pointResult = NULL, 
						*sessionResult = NULL,
						*sliderResult = NULL,
						*userTrackingResult = NULL,
						*depthMapResult = NULL,
						*rgbResult = NULL,
						
						*pointObj = NULL, 
						*sessionObj = NULL,
						*sliderObj = NULL,
						*userTrackingObj = NULL,
						*depthMapObj = NULL,
						*rgbObj = NULL;
		
	    #if (XN_PLATFORM == XN_PLATFORM_WIN32)
	    	
	    	WSADATA pointData;
			WSADATA sessionData;
			WSADATA sliderData;
			WSADATA userTrackingData;
			WSADATA depthMapData;
			WSADATA rgbData;
			
			POINT_SOCKET = INVALID_SOCKET;
			SESSION_SOCKET = INVALID_SOCKET;
			SLIDER_SOCKET = INVALID_SOCKET;
			USER_TRACKING_SOCKET = INVALID_SOCKET;
			DEPTH_MAP_SOCKET = INVALID_SOCKET;
			RGB_SOCKET = INVALID_SOCKET;
	    	
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
	    	
	    	SESSION_SOCKET = getaddrinfo("127.0.0.1", SESSION_PORT, &sessionHints, &sessionResult);
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
		    	POINT_RESULT = getaddrinfo("127.0.0.1", POINT_PORT, &pointHints, &pointResult);
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
		    	
		        SLIDER_SOCKET = getaddrinfo("127.0.0.1", SLIDER_PORT, &sliderHints, &sliderResult);
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
		    	
		    	USER_TRACKING_SOCKET = getaddrinfo("127.0.0.1", USER_TRACKING_PORT, &userTrackingHints, &userTrackingResult);
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
	    	}
	    	
	    	if(_featureDepthMapCapture)
	    	{
		    	//------------------------- DEPTH MAP SOCKET --------------------------//
		    	//---------------------------------------------------------------------//
		    	DEPTH_MAP_RESULT = WSAStartup(MAKEWORD(2,2), &depthMapData);
		    	if(DEPTH_MAP_RESULT != 0) 
		    	{
		        	printf("WSAStartup depth map capture failed: %d\n", DEPTH_MAP_RESULT);
		        	return;
		    	}
		    	
		    	// Setup depth map capture hints.
		    	ZeroMemory(&depthMapHints, sizeof(depthMapHints));
		    	depthMapHints.ai_family = AF_UNSPEC;
		    	depthMapHints.ai_socktype = SOCK_STREAM;
		    	depthMapHints.ai_protocol = IPPROTO_TCP;
		    	
		    	DEPTH_MAP_SOCKET = getaddrinfo("127.0.0.1", DEPTH_MAP_PORT, &depthMapHints, &depthMapResult);
		    	if(DEPTH_MAP_RESULT != 0) 
		    	{
		        	printf("Getaddrinfo failed on depth map capture socket server: %d\n", DEPTH_MAP_SOCKET);
		        	WSACleanup();
		        	return;
		        }
				
				// Attempt to connect to an address until one succeeds on the depth map capture server.
		    	for(depthMapObj=depthMapResult; depthMapObj != NULL; depthMapObj=depthMapObj->ai_next) 
		    	{
		        	// Create the session socket.
		    		DEPTH_MAP_SOCKET = socket(depthMapObj->ai_family, depthMapObj->ai_socktype, depthMapObj->ai_protocol);
		        	if (DEPTH_MAP_SOCKET == INVALID_SOCKET)
		        	{
		            	printf("Error at depth map capture socket(): %ld\n", WSAGetLastError());
		            	return;
		        	}
		        	
		        	// Connect to the depth map capture server.
		        	DEPTH_MAP_RESULT = connect(DEPTH_MAP_SOCKET, depthMapObj->ai_addr, (int)depthMapObj->ai_addrlen);
		        	if(DEPTH_MAP_RESULT == SOCKET_ERROR) 
		        	{
		        		DEPTH_MAP_SOCKET = INVALID_SOCKET;
		            	continue;
		        	}
		        	
		        	break;
		    	}
		    	
		    	if(DEPTH_MAP_SOCKET == INVALID_SOCKET) 
		    	{
		        	printf("Unable to connect to depth map capture server!\n");
		        	return;
		    	}
	    	}
	    	
	    	if(_featureRGBCapture)
	    	{
		    	//------------------------- RGB SOCKET --------------------------//
		    	//---------------------------------------------------------------//
		    	RGB_RESULT = WSAStartup(MAKEWORD(2,2), &rgbData);
		    	if(RGB_RESULT != 0) 
		    	{
		        	printf("WSAStartup rgb capture failed: %d\n", RGB_RESULT);
		        	return;
		    	}
		    	
		    	// Setup rgb capture hints.
		    	ZeroMemory(&rgbHints, sizeof(rgbHints));
		    	rgbHints.ai_family = AF_UNSPEC;
		    	rgbHints.ai_socktype = SOCK_STREAM;
		    	rgbHints.ai_protocol = IPPROTO_TCP;
		    	
		    	RGB_SOCKET = getaddrinfo("127.0.0.1", RGB_PORT, &rgbHints, &rgbResult);
		    	if(RGB_RESULT != 0) 
		    	{
		        	printf("Getaddrinfo failed on rgb capture socket server: %d\n", RGB_SOCKET);
		        	WSACleanup();
		        	return;
		        }
				
				// Attempt to connect to an address until one succeeds on the rgb capture server.
		    	for(rgbObj=rgbResult; rgbObj != NULL; rgbObj=rgbObj->ai_next) 
		    	{
		        	// Create the session socket.
		    		RGB_SOCKET = socket(rgbObj->ai_family, rgbObj->ai_socktype, rgbObj->ai_protocol);
		        	if (RGB_SOCKET == INVALID_SOCKET)
		        	{
		            	printf("Error at rgb capture socket(): %ld\n", WSAGetLastError());
		            	return;
		        	}
		        	
		        	// Connect to the rgb capture server.
		        	RGB_RESULT = connect(RGB_SOCKET, rgbObj->ai_addr, (int)rgbObj->ai_addrlen);
		        	if(RGB_RESULT == SOCKET_ERROR) 
		        	{
		        		RGB_SOCKET = INVALID_SOCKET;
		            	continue;
		        	}
		        	
		        	break;
		    	}
		    	
		    	if(RGB_SOCKET == INVALID_SOCKET) 
		    	{
		        	printf("Unable to connect to rgb capture server!\n");
		        	return;
		    	}
	    	}
	    	
	    #else
	    
	    	//------------------------- SESSION SOCKET --------------------------//
	        //------------------------------------------------------------------//
	    	memset(&sessionHints, 0, sizeof(struct addrinfo));
	    	sessionHints.ai_family = PF_UNSPEC;
	    	sessionHints.ai_socktype = SOCK_STREAM;
	    	sessionHints.ai_protocol = IPPROTO_TCP;
	        
	        SESSION_RESULT = getaddrinfo("127.0.0.1", SESSION_PORT, &sessionHints, &sessionResult);
	        if(SESSION_RESULT < 0)
	        	error("ERROR opening session socket");
	    	
	        for(sessionObj=sessionResult; sessionObj != NULL; sessionObj=sessionObj->ai_next) 
	    	{
	    		SESSION_SOCKET = socket(sessionObj->ai_family, sessionObj->ai_socktype, sessionObj->ai_protocol);
	    		if (SESSION_SOCKET < 0) 
	    			error("ERROR setting up session socket");
	    	
	    		SESSION_RESULT = connect(SESSION_SOCKET, sessionObj->ai_addr, (int)sessionObj->ai_addrlen);
	    		if (SESSION_RESULT < 0) 
	    			error("ERROR connecting session socket");
	    	}
	    	
	    	if(_featureSinglePoint)
	    	{
		    	//------------------------- POINT SOCKET --------------------------//
		    	//----------------------------------------------------------------//
	    		memset(&pointHints, 0, sizeof(struct addrinfo));
		    	pointHints.ai_family = PF_UNSPEC;
		    	pointHints.ai_socktype = SOCK_STREAM;
		    	pointHints.ai_protocol = IPPROTO_TCP;
		        
		        POINT_RESULT = getaddrinfo("127.0.0.1", POINT_PORT, &pointHints, &pointResult);
		        if(POINT_RESULT < 0)
		        	error("ERROR opening point socket");
		    	
		        for(pointObj=pointResult; pointObj != NULL; pointObj=pointObj->ai_next) 
		    	{
		    		POINT_SOCKET = socket(pointObj->ai_family, pointObj->ai_socktype, pointObj->ai_protocol);
		    		if (POINT_SOCKET < 0) 
		    			error("ERROR setting up point socket");
		    	
		    		POINT_RESULT = connect(POINT_SOCKET, pointObj->ai_addr, (int)pointObj->ai_addrlen);
		    		if (POINT_RESULT < 0) 
		    			error("ERROR connecting point socket");
		    	}
	    	}
	        	
	    	if(_featureSlider)
	    	{
		        //------------------------- SLIDER SOCKET --------------------------//
		    	//------------------------------------------------------------------//
	    		memset(&sliderHints, 0, sizeof(struct addrinfo));
		    	sliderHints.ai_family = PF_UNSPEC;
		    	sliderHints.ai_socktype = SOCK_STREAM;
		    	sliderHints.ai_protocol = IPPROTO_TCP;
		        
		        SLIDER_RESULT = getaddrinfo("127.0.0.1", SLIDER_PORT, &sliderHints, &sliderResult);
		        if(SLIDER_RESULT < 0)
		        	error("ERROR opening slider socket");
		    	
		        for(sliderObj=sliderResult; sliderObj != NULL; sliderObj=sliderObj->ai_next) 
		    	{
		    		SLIDER_SOCKET = socket(sliderObj->ai_family, sliderObj->ai_socktype, sliderObj->ai_protocol);
		    		if (SLIDER_SOCKET < 0) 
		    			error("ERROR setting up slider socket");
		    	
		    		SLIDER_RESULT = connect(SLIDER_SOCKET, sliderObj->ai_addr, (int)sliderObj->ai_addrlen);
		    		if (SLIDER_RESULT < 0) 
		    			error("ERROR connecting slider socket");
		    	}
	    	}
	    	
	    	if(_featureUserTracking)
	    	{
		    	//------------------------- USER TRACKING SOCKET ---------------------------------//
		    	//--------------------------------------------------------------------------------//
	    		memset(&userTrackingHints, 0, sizeof(struct addrinfo));
		    	userTrackingHints.ai_family = PF_UNSPEC;
		    	userTrackingHints.ai_socktype = SOCK_STREAM;
		    	userTrackingHints.ai_protocol = IPPROTO_TCP;

		        USER_TRACKING_RESULT = getaddrinfo("127.0.0.1", USER_TRACKING_PORT, &userTrackingHints, &userTrackingResult);
		        if(USER_TRACKING_RESULT < 0)
		        	error("ERROR opening user tracking socket");
		    	
		        for(userTrackingObj=userTrackingResult; userTrackingObj != NULL; userTrackingObj=userTrackingObj->ai_next) 
		    	{
		    		USER_TRACKING_SOCKET = socket(userTrackingObj->ai_family, userTrackingObj->ai_socktype, userTrackingObj->ai_protocol);
		    		if (USER_TRACKING_SOCKET < 0) 
		    			error("ERROR setting up user tracking socket");
		    	
		    		USER_TRACKING_RESULT = connect(USER_TRACKING_SOCKET, userTrackingObj->ai_addr, (int)userTrackingObj->ai_addrlen);
		    		if (USER_TRACKING_RESULT < 0) 
		    			error("ERROR connecting user tracking socket");
		    	}
	    	}
	    	
	    	if(_featureDepthMapCapture)
	    	{
		    	//------------------------- DEPTH MAP SOCKET -------------------------//
		    	//--------------------------------------------------------------------//
	    		memset(&depthMapHints, 0, sizeof(struct addrinfo));
		    	depthMapHints.ai_family = PF_UNSPEC;
		    	depthMapHints.ai_socktype = SOCK_STREAM;
		    	depthMapHints.ai_protocol = IPPROTO_TCP;
		        
		        DEPTH_MAP_RESULT = getaddrinfo("127.0.0.1", DEPTH_MAP_PORT, &depthMapHints, &depthMapResult);
		        if(DEPTH_MAP_RESULT < 0)
		        	error("ERROR opening depth map socket");
		    	
		        for(depthMapObj=depthMapResult; depthMapObj != NULL; depthMapObj=depthMapObj->ai_next) 
		    	{
		    		DEPTH_MAP_SOCKET = socket(depthMapObj->ai_family, depthMapObj->ai_socktype, depthMapObj->ai_protocol);
		    		if (DEPTH_MAP_SOCKET < 0) 
		    			error("ERROR setting up depth map socket");
		    	
		    		DEPTH_MAP_RESULT = connect(DEPTH_MAP_SOCKET, depthMapObj->ai_addr, (int)depthMapObj->ai_addrlen);
		    		if (DEPTH_MAP_RESULT < 0) 
		    			error("ERROR connecting depth map socket");
		    	}
	    	}
	    	
	    	if(_featureRGBCapture)
	    	{
		    	//------------------------- RGB SOCKET -------------------------------//
		    	//--------------------------------------------------------------------//
	    		memset(&rgbHints, 0, sizeof(struct addrinfo));
		    	rgbHints.ai_family = PF_UNSPEC;
		    	rgbHints.ai_socktype = SOCK_STREAM;
		    	rgbHints.ai_protocol = IPPROTO_TCP;
		        
		        RGB_RESULT = getaddrinfo("127.0.0.1", RGB_PORT, &rgbHints, &rgbResult);
		        if(RGB_RESULT < 0)
		        	error("ERROR opening rgb socket");
		    	
		        for(rgbObj=rgbResult; rgbObj != NULL; rgbObj=rgbObj->ai_next) 
		    	{
		    		RGB_SOCKET = socket(rgbObj->ai_family, rgbObj->ai_socktype, rgbObj->ai_protocol);
		    		if (RGB_SOCKET < 0) 
		    			error("ERROR setting up rgb socket");
		    	
		    		RGB_RESULT = connect(RGB_SOCKET, rgbObj->ai_addr, (int)rgbObj->ai_addrlen);
		    		if (RGB_RESULT < 0) 
		    			error("ERROR connecting rgb socket");
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
	send(socket, data, (strlen(data) + 1), 0);
}