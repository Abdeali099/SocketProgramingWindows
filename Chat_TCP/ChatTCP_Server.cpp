/* --- TCP Chat server --- */

#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include<iostream>
using namespace std;


#define msgsize 100
char msgOfClient[msgsize];
char msgOfServer[msgsize];

int main ()
{
    /* Reference : https://learn.microsoft.com/en-us/windows/win32/winsock/initializing-winsock */

    printf("\n Initializing Winsock...");

    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    printf("\n Initialized.\n");


    /* Important Structure & Variable */

        int serverSocket , acceptedSocket; /* Socket Reference (descriptor)*/
        struct sockaddr_in clientAddrStruct, serverAddrStruct;
        int bindStatus,listenStatus,receiveStatus,sendStatus,clientAddressLength;

        /* --- create socket : Server --- */

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        if(serverSocket<0)
        {
                printf("\n Error in socket creation ");
                exit(EXIT_FAILURE);
        }
        else
        {
                printf("\n Socket creation successful : %d",serverSocket);
        }

        /* --- Adding Values to struct sockaddr_in for bind  --- */


        serverAddrStruct.sin_family = AF_INET;
        serverAddrStruct.sin_addr.s_addr = inet_addr("127.0.0.1");
        serverAddrStruct.sin_port = htons( 8888 );

        // cout<<"\n htons( 8888 ) = "<<htons( 8888 )<<endl;
        // cout<<"\n ntohs( 47138 ) = "<<ntohs( 47138 )<<endl;

        /* --- Bind server  --- */

       bindStatus = bind(serverSocket, (struct sockaddr *) &serverAddrStruct, sizeof(serverAddrStruct));

        if(bindStatus<0)
        {
                printf("\n Cannot bind port ");
                exit(EXIT_FAILURE);
        }

        else
        {
                printf("\n Bind Successful ");
        }

         /* ---  Start server to Listen on Specified port --- */

        listenStatus=listen(serverSocket,5); /* Can accept 5 request as Queue */

        if(listenStatus<0)
        {
                printf("\n Error in Listen ");
                exit(EXIT_FAILURE);
        }

        else
        {
                printf("\n Successfully Listen ");
        }

        /* Accepting Request Client !! */

                printf("\n Awaiting connection request on port (Server Port) : %u\n", ntohs(47138));

                clientAddressLength = sizeof(clientAddrStruct);

                acceptedSocket = accept(serverSocket, (struct sockaddr *) &clientAddrStruct, &clientAddressLength);

                if(acceptedSocket<0) {
                        printf("\n Error accepting connection ");
                        exit(EXIT_FAILURE);
                }

                else
                {
                        printf("\n New socket created successfully : %d", acceptedSocket);
                        printf("\n Connection accepted from client : %s , port : %d \n",inet_ntoa(clientAddrStruct.sin_addr),clientAddrStruct.sin_port);
                }


                /* --- Client sends data --- */
                while(1)
                {
                        /* Setting memory for Msg*/
                        memset(msgOfClient,0,msgsize);

                        /* receive segments */
                receiveStatus = recv(acceptedSocket, msgOfClient, msgsize, 0); /* wait for data */

                        if (receiveStatus<0) {
                                continue;
                        }

                        else if (receiveStatus==0) {
                                printf("\n Connection close by client ");
                                closesocket(acceptedSocket);
                                break;
                        }

            printf("\n Client : %s \n", msgOfClient);

       // cout<<"clientAddrStruct.sin_port = "<<clientAddrStruct.sin_port<<endl;
        // cout<<"ntohs(clientAddrStruct.sin_port) = "<< ntohs(clientAddrStruct.sin_port)<<endl;
        // cout<<"htons( clientAddrStruct.sin_port ) = "<<htons( clientAddrStruct.sin_port )<<endl;

                            /* --- Sending Reply --- */

                            printf("\n Server : ");
                            gets(msgOfServer);

                        sendStatus = send(acceptedSocket, msgOfServer, strlen(msgOfServer), 0);

                        if(sendStatus<0) {
                                printf("\n cannot send data ");
                                closesocket(acceptedSocket);
                                exit(1);  //break;
                        }

                        if(strcmp(msgOfServer,"Bye")==0 || strcmp(msgOfServer,"bye")==0){
                            printf("\n Server is Closed !! ");
                            break;
                        }

                } /* connection with 1 client socket close */

                closesocket(acceptedSocket);

                closesocket(serverSocket);

       /* Clear WSA (Window socket API) */
        WSACleanup();

    return 0;
}

/* Note  :-

-> It is a TCP so handel only One client at a Time

htons(8888) = 47138

 -> Client port changes not fixed (below maybe change)

clientAddrStruct.sin_port = 25288

ntohs(25288) = 51298

*/
