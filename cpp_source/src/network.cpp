#include "network.h"
#include <iostream>
using namespace std;

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	SOCKET dataSocket = INVALID_SOCKET;
	SOCKET dataClientSocket = INVALID_SOCKET;
	PCSTR confPort = "9400";
#else
	#include <netdb.h>
	#include <errno.h>
	
	#define confPort "9400"
	int dataSocket;
	int dataClientSocket;
#endif

//---------------------------------------------------------------------------
// Namespaces
//---------------------------------------------------------------------------
using namespace std;

//---------------------------------------------------------------------------
// Definitions
//---------------------------------------------------------------------------
#define MAX_PACKET_SIZE 1280*1024*4

//---------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------
extern int g_Exit;
unsigned char *msg = new unsigned char[MAX_PACKET_SIZE];
callback networkCallback;
struct addrinfo siData, *serverResult = NULL, *serverObj = NULL;

//---------------------------------------------------------------------------
// Public Methods
//---------------------------------------------------------------------------
void network::sendMessage(const char *data) 
{
	string _msg = data;
	int len = strlen(data) + 1;
	int first = 3;
	int second = 0;
	int m_len = 1 + 1 + sizeof(int);
	memcpy(msg, &first, 1);
	memcpy(msg + 1, &second, 1);
	memcpy(msg + 2, &len, sizeof(int));
	memcpy(msg + m_len, data, len);
	sendData(msg,len+m_len);
}

void network::sendMessage(int first, int second, int value) 
{
	unsigned char buff[2 + sizeof(int) * 2];
	buff[0] = first;
	buff[1] = second;
	int size = sizeof(int);
	memcpy(buff+2, &size, sizeof(int));
	memcpy(buff+6, &value, sizeof(int));
	sendData(buff, 2 + sizeof(int) * 2);
}

void network::sendMessage(int first, int second, unsigned char *data, int len) 
{
	int m_len = 1 + 1 + sizeof(int);
	memcpy(msg, &first, 1);
	memcpy(msg + 1, &second, 1);
	memcpy(msg + 2, &len, sizeof(int));
	memcpy(msg + m_len, data, len);
	sendData(msg, m_len + len);
}

int network::sendData(unsigned char *buffer, int length)
{
	#if (XN_PLATFORM == XN_PLATFORM_WIN32)
		if(dataClientSocket != INVALID_SOCKET) 
		{
			return send(dataClientSocket, (char*)buffer, length, 0);
		}
	#else
		if(dataClientSocket != 0) 
		{
			return send(dataClientSocket, (char*)buffer, length, 0);
		}
	#endif
	return 0;
}

int network::getData(unsigned char *buffer, int length)
{
	#if (XN_PLATFORM == XN_PLATFORM_WIN32)
		if(dataClientSocket != INVALID_SOCKET) 
		{
			return recv(dataClientSocket, (char*)buffer, 1024, 0);
		}
	#else
		if(dataClientSocket != 0) 
		{
			return recv(dataClientSocket, (char*)buffer, 1024, 0);
		}
	#endif
	return 0;	
}

void network::waitForClient() 
{
	printf("AS3OpenNI-Bridge :: Waiting for client.\n");
	dataClientSocket = accept(dataSocket, NULL, NULL);
	
	#if (XN_PLATFORM == XN_PLATFORM_WIN32)
		if (dataClientSocket == INVALID_SOCKET && !g_Exit)
		{
			printf("AS3OpenNI-Bridge :: Error on accept() for data, exit data thread. %d\n", WSAGetLastError());
			closesocket(dataSocket);
			WSACleanup();
		}
	#else
		if (0 > dataClientSocket && !g_Exit) 
		{
			printf("AS3OpenNI-Bridge :: Error on accept() for data, exit data thread. %s\n", strerror(errno));
			close(dataSocket);
		}
	#endif

	printf("AS3OpenNI-Bridge :: Got data client.\n");
	networkCallback();
}

int network::init(callback cb) 
{
	networkCallback = cb;
	initServer(siData, confPort, "DATA");
	waitForClient();
	return 0;
}

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	int network::initServer(addrinfo si_type, PCSTR conf_port, PCSTR label)
	{
		int iResult;
		WSADATA serverData;

	    iResult = WSAStartup(MAKEWORD(2,2), &serverData);
	    if(iResult != 0) 
	    {
	        printf("WSAStartup session failed: %d\n", iResult);
			return 1;
	    }
	    	
	    // Setup session hints.
	    ZeroMemory(&si_type, sizeof(si_type));
	    si_type.ai_family = AF_INET;
	   	si_type.ai_socktype = SOCK_STREAM;
	    si_type.ai_protocol = IPPROTO_TCP;
		si_type.ai_flags = AI_PASSIVE;
	    	
	    dataSocket = getaddrinfo("127.0.0.1", conf_port, &si_type, &serverResult);
	    if(iResult != 0) 
	    {
	        printf("AS3OpenNI-Bridge :: Getaddrinfo failed on session socket server: %d\n", dataSocket);
	        WSACleanup();
			return 1;
	    }
	        
	    // Attempt to connect to an address until one succeeds on the session server.
	    for(serverObj=serverResult; serverObj != NULL; serverObj=serverObj->ai_next) 
	    {
	        // Create the session socket.
	        dataSocket = socket(serverObj->ai_family, serverObj->ai_socktype, serverObj->ai_protocol);
	        if (dataSocket == INVALID_SOCKET) 
	        {
	            printf("AS3OpenNI-Bridge :: Error at session socket(): %ld\n", WSAGetLastError());
				return 1;
	        }
	        
			iResult = bind(dataSocket, serverObj->ai_addr, (int)serverObj->ai_addrlen);
			if (iResult == SOCKET_ERROR) 
			{
				printf("AS3OpenNI-Bridge :: %s: bind failed: %d\n", label, WSAGetLastError());
				freeaddrinfo(serverObj);
				closesocket(dataSocket);
				WSACleanup();
				return 1;
			}

			freeaddrinfo(serverObj);
			if (listen(dataSocket, SOMAXCONN) == SOCKET_ERROR) 
			{
				printf("AS3OpenNI-Bridge :: %s: listen failed [%ld]\n", label, WSAGetLastError() );
				closesocket(dataSocket);
				WSACleanup();
				return 1;
			}
	        	
	        break;
	    }
	    	
	    if(dataSocket == INVALID_SOCKET) 
	    {
	        printf("AS3OpenNI-Bridge :: Unable to connect to session server!\n");
			return 1;
	    }

		return 0;
	}
#else
	int network::initServer(addrinfo si_type, const char *conf_port, const char *label)
	{
		memset(&si_type, 0, sizeof(struct addrinfo));
		si_type.ai_family = AF_INET;
		si_type.ai_socktype = SOCK_STREAM;
		si_type.ai_protocol = IPPROTO_TCP;
		si_type.ai_flags = AI_PASSIVE;
	
		int iResult = getaddrinfo("127.0.0.1", conf_port, &si_type, &serverResult);
		if (iResult < 0) 
		{
			printf("AS3OpenNI-Bridge :: %s: getaddrinfo failed: %d\n", label, iResult);
			return 1;
		}
	
		for(serverObj=serverResult; serverObj != NULL; serverObj=serverObj->ai_next) 
	    {
	    	dataSocket = socket(serverObj->ai_family, serverObj->ai_socktype, serverObj->ai_protocol);
	    	if (dataSocket < 0)
			{
	    		printf("AS3OpenNI-Bridge :: %s: socket failed [%s]\n", label, strerror(errno));
				freeaddrinfo(serverResult);
				return 1;
			}

			iResult = bind(dataSocket, serverResult->ai_addr, (int)serverResult->ai_addrlen);
			if (iResult < 0) 
			{
				printf("AS3OpenNI-Bridge :: %s: bind failed: %s\n", label, strerror(errno));
				freeaddrinfo(serverResult);
				close(dataSocket);
				return 1;
			}

			freeaddrinfo(serverResult);
			if (listen(dataSocket, 10) < 0) 
			{
				printf("AS3OpenNI-Bridge :: %s: listen failed [%s]\n", label, strerror(errno));
				close(dataSocket);
				return 1;
			}
			
			break;
		}
	
		return 0;
	}
#endif

void network::closeConnection() 
{
	g_Exit = 1;	
	#if (XN_PLATFORM == XN_PLATFORM_WIN32)
		delete [] msg;
		if (dataSocket != INVALID_SOCKET) closesocket(dataSocket);
	#else
		if(dataSocket != 0) close(dataSocket);
	#endif
}
