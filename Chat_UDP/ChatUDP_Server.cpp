/* -- UDP Chat Server -- */

#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library (if compiler not recognized WSA then use this);

#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data

int main()
{
/* Reference : https://learn.microsoft.com/en-us/windows/win32/winsock/initializing-winsock */

	//Initialise winsock

	WSADATA wsa;

	printf("\n Initializing Winsock...");

	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)

	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	printf(" Initialized.\n");

	/* Important Structure & Variable */

    SOCKET serverSocket; // int serverSocket; will also work
	struct sockaddr_in serverAddrStruct, clientsAddrStruct;
	int bindStatus,sendingStatus,clientAddrLength ,recv_len;
	char buf[BUFLEN];
	char message[BUFLEN];
	clientAddrLength = sizeof(clientsAddrStruct) ;

    /* --- create socket : Server --- */

    serverSocket = socket(AF_INET , SOCK_DGRAM , 0 );

	if(serverSocket == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
	}

	printf(" Socket created.\n");

	/* --- Adding Values to struct sockaddr_in for bind  --- */
	serverAddrStruct.sin_family = AF_INET;
	serverAddrStruct.sin_addr.s_addr = INADDR_ANY;
	serverAddrStruct.sin_port = htons( PORT );

	/* --- Bind server  --- */

	bindStatus=bind(serverSocket ,(struct sockaddr *)&serverAddrStruct , sizeof(serverAddrStruct));

	if(bindStatus<0)
        {
                printf("\n Cannot bind port ");
                exit(EXIT_FAILURE);
        }

        else
        {
                printf("\n Bind Successful ");
        }



	 /* ---  Start server to Listen --- */

	while(1)
	{
     fflush(stdout);

		// -- clear the buffer by filling null, it might have previously received data -- //
		memset(buf,'\0', BUFLEN);

		// -- try to receive some data, this is a blocking call -- //

		recv_len = recvfrom(serverSocket, buf, BUFLEN, 0, (struct sockaddr *) &clientsAddrStruct, &clientAddrLength);

		if (recv_len == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d" , WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		printf("\n => Client : %s \n" , buf);

		// -- now send to client

		printf("\n => Server : ");
		gets(message);

		sendingStatus=sendto(serverSocket, message, recv_len, 0, (struct sockaddr*) &clientsAddrStruct, clientAddrLength);

		if (sendingStatus == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d" , WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		 if(strcmp(message,"Bye")==0 || strcmp(message,"bye")==0)
            {
                printf("\n Server is Closed !! ");
                break;
            }

	} // while loop close

	closesocket(serverSocket);

	 /* Clear WSA (Window socket API) */
	WSACleanup();

	return 0;
}
