#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "user32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "5000"

void mousemove(int x, int y)
{
	INPUT ip;
	ip.type=INPUT_MOUSE;
	ip.mi.dx = x;
	ip.mi.dy = y;
	ip.mi.dwFlags = MOUSEEVENTF_MOVE;
	ip.mi.time = 0;
	ip.mi.dwExtraInfo= 0;
	SendInput(1, &ip, sizeof(INPUT));

}
int flagp = 1;
int flagn = 1;
void movewheel(int dir)
{
	INPUT ip;
	ip.type=INPUT_MOUSE;
	//ip.mi.dx = x;
	//ip.mi.dy = y;
	if (dir == 1)
		ip.mi.mouseData = 0x150;
	else
		ip.mi.mouseData = -0x150;
	ip.mi.dwFlags = MOUSEEVENTF_WHEEL;
	ip.mi.time = 0;
	ip.mi.dwExtraInfo= 0;
	SendInput(1, &ip, sizeof(INPUT));

}

void leftclick()
{
	INPUT ip;
	ip.type=INPUT_MOUSE;
	//ip.mi.dx = x;
	//ip.mi.dy = y;

	ip.mi.time = 0;
	ip.mi.dwExtraInfo= 0;
	ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &ip, sizeof(INPUT));

	ip.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &ip, sizeof(INPUT));
}

int __cdecl main(void) 
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    u_long iMode = 1;
    iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);
    if (iResult != NO_ERROR)
  	printf("ioctlsocket failed with error: %ld\n", iResult);
  

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    do {
    		// Accept a client socket
	if (kbhit() && _getch() == 'q')
	{
		closesocket(ListenSocket);
		break;
	}

    	//printf("Waiting for client\n");
    	ClientSocket = accept(ListenSocket, NULL, NULL);
    	//printf("client connected\n");
    	if (ClientSocket == INVALID_SOCKET) {
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			//printf("block\n");
			Sleep(25);
			continue;
		}
        	printf("accept failed with error: %d\n", WSAGetLastError());
        	closesocket(ListenSocket);
        	WSACleanup();
        	return 1;
    	}

    // No longer need server socket
    //closesocket(ListenSocket);

    // Receive until the peer shuts down the connection
    //do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

        if (iResult > 0) {
		if (!strncmp(recvbuf, "LC", 2))
		{
			printf("click %s\n", recvbuf);
			leftclick();
			continue;
		}

		char *str = strtok(recvbuf, "\n");
		int count = 0;
		float xyz[3],x,y,z;
		
		//printf("%s\n", str);
		char *xyzstr = strtok(str, ",");
		//printf("%s\n", xyz);
		int i=0;
		while(xyzstr!=NULL)
		{
			xyz[i++] = atof(xyzstr);
			//printf("%f\n",x);
			xyzstr = strtok(NULL, ",");
		}
		x=xyz[0];
		y=xyz[1];
		z=xyz[2];
		if (x>6.0) {
			if (flagp == 1) {
				flagn = 0;
				movewheel(1);
			}
		}
		else
		if (x<-6.0) {
			if (flagn == 1) {
				flagp = 0;
				movewheel(-1);
			}
		}

		if (x <-1.0 && flagn == 0)
			flagn = -1;
		if (x > 1.0 && flagn == -1)
			flagn = 1;

		if (x > 1.0 && flagp == 0)
			flagp = -1;

		if (x < -1.0 && flagp == -1)
			flagp = 1;
		//printf("flagp %d\n", flagp);
		//printf("flagn %d\n", flagn);
		//printf("y %f\n",y);
		//mousemove(-z*100,-x*100);
		printf("%f %f %f\n",x,y,z);


        	/*// Echo the buffer back to the sender
            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);*/
        }
        else if (iResult == 0) {
	     printf("Connection closing...\n");
	}
        else  {
	    if (WSAGetLastError() == WSAEWOULDBLOCK)
		    continue;
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (1);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}

