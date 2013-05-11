#include "Tcp.h"


int Tcp::TCP_Connect(string host, string port)
{
   // create WSADATA object
    WSADATA wsaData;

	int iResult;

    // socket
    SOCKET ConnectSocket = INVALID_SOCKET;

    // holds address info for socket to connect to
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

	 u_long iMode;

	 char value;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (iResult != 0) {
       return -1;
    }

    // set address info
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!


	char* cIp = new char[host.length()+1];
	strcpy(cIp,host.c_str());

	char* cPort = new char[port.length()+1];
	strcpy(cPort,port.c_str());

	
    //resolve server address and port 
    iResult = getaddrinfo(cIp, cPort, &hints, &result);

	delete[] cIp;
	delete[] cPort;

    if( iResult != 0 ) 
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            exit(1);
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

        if (iResult == SOCKET_ERROR)
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            printf ("The server is down... did not connect");
        }
    }



    // no longer need address info for server
    freeaddrinfo(result);



    // if connection failed
    if (ConnectSocket == INVALID_SOCKET) 
    {
      
        WSACleanup();
        exit(1);
    }

	// Set the mode of the socket to be nonblocking
    iMode = 1;

    iResult = ioctlsocket(ConnectSocket, FIONBIO, &iMode);
    if (iResult == SOCKET_ERROR)
    {
        
        closesocket(ConnectSocket);
        WSACleanup();
        exit(1);        
    }

	//disable nagle
    value = 1;
    setsockopt( ConnectSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );

	return ConnectSocket ;
}

int Tcp::TCP_Send(int sock, unsigned char *buffer, int length)
{
    // Send 'length' number of bytes from buffer via provided
    // socket <sock> address
   int sendResult;
   sendResult = send(sock, (char *)buffer, length, 0);
   return sendResult;

   //return 0;

}

int Tcp::TCP_Receive(int sock, unsigned char *buffer, int length)
{
    //Retrieve 'length' number of bytes into 'buffer' from the socket <sock> address
     return recv(sock, (char*) buffer, length, 0);
	 
}

int Tcp::TCP_Disconnect(int sock)
{
    //free and cleanup socket occupied memory
	// if connected
	int socketCloced;
	int wsaCleaned;

	socketCloced = closesocket(sock);
	wsaCleaned = WSACleanup();
    
	if(socketCloced ==0 && wsaCleaned == 0)
	  return 0;
	else
      return -1;
}
