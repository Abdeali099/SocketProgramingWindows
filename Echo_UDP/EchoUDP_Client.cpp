/* -- UDP Echo Client -- */

#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"	// IP address of UDP server
#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data

int main()
{
    /* Reference : https://learn.microsoft.com/en-us/windows/win32/winsock/initializing-winsock */

	// -- Initialise winsock
	WSADATA wsa;

	printf("\n Initializing Winsock...");

	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	printf("Initialized.\n");

	/* Important Structure & Variable */

	struct sockaddr_in serverAddrStruct;
	int clientSocket,sendStatus,receiveStatus, slen=sizeof(serverAddrStruct);
	char buf[BUFLEN];
	char message[BUFLEN];

	   /* --- create socket : Clients --- */

    clientSocket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (clientSocket == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	/* --- setup address structure --- */
	memset((char *) &serverAddrStruct, 0, sizeof(serverAddrStruct));

	serverAddrStruct.sin_family = AF_INET;
	serverAddrStruct.sin_port = htons(PORT);
	serverAddrStruct.sin_addr.S_un.S_addr = inet_addr(SERVER);

	/* -- starts  communication -- */

	while(1)
	{
		printf("\n =>Enter message : ");
		gets(message);

		/* --- send the message --- */

		sendStatus=sendto(clientSocket, message, strlen(message) , 0 , (struct sockaddr *) &serverAddrStruct, slen);

		if (sendStatus == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d" , WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		/* -- receive a Echo and print it -- */

		// -- clear the buffer by filling null, it might have previously received data
		memset(buf,'\0', BUFLEN);

		/* --- try to receive some data, this is a blocking call --- */
		receiveStatus=recvfrom(clientSocket, buf, BUFLEN, 0, (struct sockaddr *) &serverAddrStruct, &slen);

		if (receiveStatus == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d" , WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		printf("\n Echo from server : %s",buf);

		   if(strcmp(buf,"Bye")==0 || strcmp(buf,"bye")==0)
            {
                printf("\n Server is Closed !! ");
                break;
            }


	}


	closesocket(clientSocket);

	 /* Clear WSA (Window socket API) */
	WSACleanup();

	return 0;
}
