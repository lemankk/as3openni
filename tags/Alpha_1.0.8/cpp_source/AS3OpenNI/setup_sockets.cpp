#include "setup_sockets.h"

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	#include <windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include "socket.h"
	
	//#define WIN32_LEAN_AND_MEAN
	
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
	extern SOCKET SECOND_USER_TRACKING_SOCKET;
	
	int POINT_RESULT;
	int SESSION_RESULT;
	int SLIDER_RESULT;
	int USER_TRACKING_RESULT;
	int DEPTH_MAP_RESULT;
	int RGB_RESULT;
	int SECOND_USER_TRACKING_RESULT;
	
	#define WIN_POINT_PORT "9500"
	#define WIN_SESSION_PORT "9501"
	#define WIN_SLIDER_PORT "9502"
	#define WIN_USER_TRACKING_PORT "9503"
	#define WIN_DEPTH_MAP_PORT "9504"
	#define WIN_RGB_PORT "9505"
	#define WIN_SECOND_USER_TRACKING_PORT "9506"
#else
	#include <netdb.h>
	extern int POINT_SOCKET, SESSION_SOCKET, SLIDER_SOCKET, USER_TRACKING_SOCKET, DEPTH_MAP_SOCKET, RGB_SOCKET, SECOND_USER_TRACKING_SOCKET;
	
	struct sockaddr_in _point_serv_addr;
	struct sockaddr_in _session_serv_addr;
	struct sockaddr_in _slider_serv_addr;
	struct sockaddr_in _user_tracking_serv_addr;
	struct sockaddr_in _depth_map_serv_addr;
	struct sockaddr_in _rgb_serv_addr;
	struct sockaddr_in _second_user_tracking_serv_addr;
	
	int POINT_PORT = 9500;
	int SESSION_PORT = 9501;
	int SLIDER_PORT = 9502;
	int USER_TRACKING_PORT = 9503;
	int DEPTH_MAP_PORT = 9504;
	int RGB_PORT = 9505;
	int SECOND_USER_TRACKING_PORT = 9506;
#endif
	
void setupSockets()
{
	if(_useSockets)
	{
		//-------------------------------------------------------------------------------//
		//------------------------- SEND DATA TO SOCKET SERVER --------------------------//
		//-------------------------------------------------------------------------------//
		
	    #if (XN_PLATFORM == XN_PLATFORM_WIN32)
	    	
	    	WSADATA pointData;
			WSADATA sessionData;
			WSADATA sliderData;
			WSADATA userTrackingData;
			WSADATA depthMapData;
			WSADATA rgbData;
			WSADATA secUserTrackingData;
			
			POINT_SOCKET = INVALID_SOCKET;
			SESSION_SOCKET = INVALID_SOCKET;
			SLIDER_SOCKET = INVALID_SOCKET;
			USER_TRACKING_SOCKET = INVALID_SOCKET;
			DEPTH_MAP_SOCKET = INVALID_SOCKET;
			RGB_SOCKET = INVALID_SOCKET;
			SECOND_USER_TRACKING_SOCKET = INVALID_SOCKET;
			
	    	struct addrinfo *pointResult = NULL, 
	    					*sessionResult = NULL,
	    					*sliderResult = NULL,
	    					*userTrackingResult = NULL,
	    					*depthMapResult = NULL,
	    					*rgbResult = NULL,
	    					*secUserTrackingResult = NULL,
	    					
	    					*pointObj = NULL, 
	    					*sessionObj = NULL,
	    					*sliderObj = NULL,
	    					*userTrackingObj = NULL,
	    					*secUserTrackingObj = NULL,
	    					*depthMapObj = NULL,
	    					*rgbObj = NULL,
	    					
	    					pointHints, 
	    					sessionHints,
	    					sliderHints,
	    					userTrackingHints,
	    					secUserTrackingHints,
	    					depthMapHints,
	    					rgbHints;
	    					
	    	char *pointBuf = "This is a point test.";
	    	char *sessionBuf = "This is a session test.";
	    	char *sliderBuf = "This is a slider test.";
	    	char *userTrackingBuf = "This is a user tracking test.";
	    	char *secUserTrackingBuf = "This is a second user tracking test.";
	    	char *depthMapBuf = "This is a depth map capture test.";
	    	char *rgbBuf = "This is a rgb capture test.";
	    	
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
		    	
		    	//------------------------- SECOND USER TRACKING SOCKET --------------------------//
		    	//--------------------------------------------------------------------------------//
		    	SECOND_USER_TRACKING_RESULT = WSAStartup(MAKEWORD(2,2), &secUserTrackingData);
		    	if(SECOND_USER_TRACKING_RESULT != 0) 
		    	{
		        	printf("WSAStartup second user tracking failed: %d\n", SECOND_USER_TRACKING_RESULT);
		        	return;
		    	}
		    	
		    	// Setup second user tracking hints.
		    	ZeroMemory(&secUserTrackingHints, sizeof(secUserTrackingHints));
		    	secUserTrackingHints.ai_family = AF_UNSPEC;
		    	secUserTrackingHints.ai_socktype = SOCK_STREAM;
		    	secUserTrackingHints.ai_protocol = IPPROTO_TCP;
		    	
		    	SECOND_USER_TRACKING_SOCKET = getaddrinfo("127.0.0.1", WIN_SECOND_USER_TRACKING_PORT, &secUserTrackingHints, &secUserTrackingResult);
		    	if(SECOND_USER_TRACKING_RESULT != 0) 
		    	{
		        	printf("Getaddrinfo failed on second user tracking socket server: %d\n", SECOND_USER_TRACKING_SOCKET);
		        	WSACleanup();
		        	return;
		        }
				
				// Attempt to connect to an address until one succeeds on the second user tracking server.
		    	for(secUserTrackingObj=secUserTrackingResult; secUserTrackingObj != NULL; secUserTrackingObj=secUserTrackingObj->ai_next) 
		    	{
		        	// Create the session socket.
		    		SECOND_USER_TRACKING_SOCKET = socket(secUserTrackingObj->ai_family, secUserTrackingObj->ai_socktype, secUserTrackingObj->ai_protocol);
		        	if (SECOND_USER_TRACKING_SOCKET == INVALID_SOCKET)
		        	{
		            	printf("Error at second user tracking socket(): %ld\n", WSAGetLastError());
		            	return;
		        	}
		        	
		        	// Connect to the second user tracking server.
		        	SECOND_USER_TRACKING_RESULT = connect(SECOND_USER_TRACKING_SOCKET, secUserTrackingObj->ai_addr, (int)secUserTrackingObj->ai_addrlen);
		        	if(SECOND_USER_TRACKING_RESULT == SOCKET_ERROR) 
		        	{
		        		SECOND_USER_TRACKING_SOCKET = INVALID_SOCKET;
		            	continue;
		        	}
		        	
		        	break;
		    	}
		    	
		    	if(SECOND_USER_TRACKING_SOCKET == INVALID_SOCKET) 
		    	{
		        	printf("Unable to connect to second user tracking server!\n");
		        	return;
		    	}
		    	
		    	// Send an initial buffer to second user tracking server.
		    	SECOND_USER_TRACKING_RESULT = send(SECOND_USER_TRACKING_SOCKET, secUserTrackingBuf, (int)strlen(secUserTrackingBuf), 0);
		    	if(SECOND_USER_TRACKING_RESULT == SOCKET_ERROR) 
		    	{
		        	printf("Send failed to second user tracking server: %d\n", WSAGetLastError());
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
		    	
		    	DEPTH_MAP_SOCKET = getaddrinfo("127.0.0.1", WIN_DEPTH_MAP_PORT, &depthMapHints, &depthMapResult);
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
		    	
		    	// Send an initial buffer to depth map capture server.
		    	DEPTH_MAP_RESULT = send(DEPTH_MAP_SOCKET, depthMapBuf, (int)strlen(depthMapBuf), 0);
		    	if(DEPTH_MAP_RESULT == SOCKET_ERROR) 
		    	{
		        	printf("Send failed to depth map capture server: %d\n", WSAGetLastError());
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
		    	
		    	RGB_SOCKET = getaddrinfo("127.0.0.1", WIN_RGB_PORT, &rgbHints, &rgbResult);
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
		    	
		    	// Send an initial buffer to rgb capture server.
		    	RGB_RESULT = send(RGB_SOCKET, rgbBuf, (int)strlen(rgbBuf), 0);
		    	if(RGB_RESULT == SOCKET_ERROR) 
		    	{
		        	printf("Send failed to rgb capture server: %d\n", WSAGetLastError());
		        	return;
		    	}
	    	}
	    	
	    #else
	    
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
		    	
		    	//------------------------- SECOND USER TRACKING SOCKET --------------------------//
		    	//--------------------------------------------------------------------------------//
		    	SECOND_USER_TRACKING_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
		    	if (SECOND_USER_TRACKING_SOCKET < 0) 
		        	error("ERROR opening second user tracking socket");
		        
		        memset(&_second_user_tracking_serv_addr, 0, sizeof(struct sockaddr_in));
		        _second_user_tracking_serv_addr.sin_family = AF_INET;
		        _second_user_tracking_serv_addr.sin_port = htons(SECOND_USER_TRACKING_PORT);
		    
		    	if (connect(SECOND_USER_TRACKING_SOCKET,(struct sockaddr *) &_second_user_tracking_serv_addr,sizeof(_second_user_tracking_serv_addr)) < 0) 
		        	error("ERROR connecting to second user tracking socket");
	    	}
	    	
	    	if(_featureDepthMapCapture)
	    	{
		    	//------------------------- DEPTH MAP SOCKET -------------------------//
		    	//--------------------------------------------------------------------//
		        DEPTH_MAP_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
		    	if (DEPTH_MAP_SOCKET < 0) 
		        	error("ERROR opening depth map capture socket");
		        
		        memset(&_depth_map_serv_addr, 0, sizeof(struct sockaddr_in));
		        _depth_map_serv_addr.sin_family = AF_INET;
		        _depth_map_serv_addr.sin_port = htons(DEPTH_MAP_PORT);
		    
		    	if (connect(DEPTH_MAP_SOCKET,(struct sockaddr *) &_depth_map_serv_addr,sizeof(_depth_map_serv_addr)) < 0) 
		        	error("ERROR connecting to depth map capture socket");
	    	}
	    	
	    	if(_featureRGBCapture)
	    	{
		    	//------------------------- RGB SOCKET -------------------------------//
		    	//--------------------------------------------------------------------//
		        RGB_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
		    	if (RGB_SOCKET < 0) 
		        	error("ERROR opening rgb capture socket");
		        
		        memset(&_rgb_serv_addr, 0, sizeof(struct sockaddr_in));
		        _rgb_serv_addr.sin_family = AF_INET;
		        _rgb_serv_addr.sin_port = htons(RGB_PORT);
		    
		    	if (connect(RGB_SOCKET,(struct sockaddr *) &_rgb_serv_addr,sizeof(_rgb_serv_addr)) < 0) 
		        	error("ERROR connecting to rgb capture socket");
	    	}
	    	
		#endif
		
	    //-------------------------------------------------------------------------------//
		//------------------------- SEND DATA TO SOCKET SERVER --------------------------//
		//-------------------------------------------------------------------------------//
	}
}
	
void sendToSocket(int socket, const char *data)
{
	#if (XN_PLATFORM == XN_PLATFORM_WIN32)
		send(socket, data, (strlen(data) + 1), 0);
	#else
		write(socket, data, (strlen(data) + 1));
	#endif
}
