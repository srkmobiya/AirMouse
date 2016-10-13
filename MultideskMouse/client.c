#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#pragma comment (lib, "user32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "5000"

SOCKET ConnectSocket = INVALID_SOCKET;
int cursorsize = 25;


HCURSOR cursor =NULL;
void hideCursor()
{
	printf("Hiding cursor");
	cursor = SetCursor(NULL);
}

void showCursor()
{
	SetCursor(cursor);
}

int keyinturrept()
{
	int c = 0;
	if (kbhit())
	{
		c = _getch();
		if (c != 'q')
			return c;
		showCursor();
		closesocket(ConnectSocket);
		exit(-1);
		//break;
	}
	return c;
}

int __cdecl main(int argc, char **argv) 
{
    WSADATA wsaData;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char sendbuf[10];
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }


        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    	u_long iMode = 1;
    	iResult = ioctlsocket(ConnectSocket, FIONBIO, &iMode);
    	if (iResult != NO_ERROR)
  		printf("ioctlsocket failed with error: %ld\n", iResult);

    // Send an initial buffer
    //for (int i=47;i<57;i++){
//	    sendbuf[0] = i;

    //printf("sending\n");
    //for(int i=0;i<10;i++) {
    int sendflag = 0;
    int nVirtKey;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    int remotecontrol = 0;
    int prex=0;
    int prey=0;
    while(1) {
	int ch = keyinturrept();
	char recvbuf[512];
	
	iResult = recv(ConnectSocket, recvbuf, 512, 0);
	if (iResult > 0)
	{
		if (!strncmp(recvbuf, "end", 3)) {
			printf("### recv end\n");
			remotecontrol = 0;
			sendflag = 0;
			showCursor();
		}
	}

	POINT pt;
    	GetCursorPos(&pt);

	if (pt.x == 0 && remotecontrol == 0)
	{
		remotecontrol = 1;
		printf("switching %d\n", sendflag);
		if (sendflag) {
			printf("mouse visible\n");
			showCursor();
			/*	CONSOLE_CURSOR_INFO lpCursor;	
			lpCursor.bVisible = TRUE;
			lpCursor.dwSize = 10;
			SetConsoleCursorInfo(console,&lpCursor);*/
			sendflag = 0;
		}
		else {
			printf("mouse invisible\n");
			hideCursor();
		/*	CONSOLE_CURSOR_INFO lpCursor;	
			lpCursor.bVisible = FALSE;
			lpCursor.dwSize = 20;
			SetConsoleCursorInfo(console,&lpCursor);*/
			sendflag = 1;
		}
	}

	nVirtKey = GetKeyState(VK_LBUTTON);
	//printf("%d\n", nVirtKey);
        if (nVirtKey & 0x8000) {
		//printf("left clicked\n");
		//Sleep(200);
		//continue;
		memset(sendbuf, 0, 10);
		sendbuf[0]='L';
		sendbuf[1]='C';
		if (sendflag) {
			iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    			if (iResult == SOCKET_ERROR) {
     	   			printf("send failed with error: %d\n", WSAGetLastError());
           			closesocket(ConnectSocket);
           			WSACleanup();
           			return 1;
    			}
		}
		Sleep(50);

	}	

	nVirtKey = GetKeyState(VK_RBUTTON);
	//printf("%d\n", nVirtKey);
        if (nVirtKey & 0x8000) {
		//printf("left clicked\n");
		//Sleep(200);
		//continue;
		memset(sendbuf, 0, 10);
		sendbuf[0]='R';
		sendbuf[1]='C';
		if (sendflag) {
			iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    			if (iResult == SOCKET_ERROR) {
     	   			printf("send failed with error: %d\n", WSAGetLastError());
           			closesocket(ConnectSocket);
           			WSACleanup();
           			return 1;
    			}
		}
		Sleep(50);

	}

    	
    	memset(sendbuf, 0, 10);
    	char a[5], b[5];
    	itoa(pt.x, a, 10);
    	itoa(pt.y, b, 10);
    	strcpy(sendbuf, a);
    	strcat(sendbuf, ",");
    	strcat(sendbuf, b);
    	strcat(sendbuf, ",");
	if (sendflag && pt.x != prex && pt.y!=prey) {
    		printf("%s\n", sendbuf);
		prex=pt.x;
		prey=pt.y;
    		iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    		if (iResult == SOCKET_ERROR) {
     	   		printf("send failed with error: %d\n", WSAGetLastError());
           		closesocket(ConnectSocket);
           		WSACleanup();
           		return 1;
    		}
	}
    	Sleep(50);
    }


    printf("Bytes Sent: %ld\n", iResult);

    //shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    /*
    // Receive until the peer closes the connection
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
            printf("Bytes received: %d\n", iResult);
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );

    // cleanup*/
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}

