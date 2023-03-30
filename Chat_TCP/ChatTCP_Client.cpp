/* --- TCP Chat Client --- */

#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include<iostream>
using namespace std;


#define msgSize 100
char msgFromClient[msgSize];
char msgFromServer[msgSize];

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

        int clientSocket; /* socket reference (descriptor)*/
        int connectStatus, receivingStatus, sendingStatus;
        struct sockaddr_in serverAddrStruct;

   /* ---connectStatuseate socket : client --- */

        clientSocket = socket(AF_INET, SOCK_STREAM, 0);

        if(clientSocket<0) {
                printf("\n cannot open socket ");
                exit(EXIT_FAILURE);
        }

        else
        {
                printf("\n Socket connect successfully : %d",clientSocket);
        }

        /* --- Adding Values to struct sockaddr_in to locate server  --- */

        serverAddrStruct.sin_family = AF_INET;
        serverAddrStruct.sin_addr.s_addr = inet_addr("127.0.0.1");
        serverAddrStruct.sin_port = htons(8888);


        /* --- connect to server  ---  */

       connectStatus = connect(clientSocket, (struct sockaddr *) &serverAddrStruct, sizeof(serverAddrStruct));

        if(connectStatus <0)
        {
                printf("\n cannot connect ");
                exit(EXIT_FAILURE);
        }
        else
        {
                printf("\n Connection accepted by server : %s", inet_ntoa(serverAddrStruct.sin_addr));
        }

        /* -- Sending and Receiving (Echo) data --- */

        while (1)
        {

                /* === Sending Segment === */

                /* --- used for empty buffer --- */
                memset(msgFromClient,0,msgSize);

                printf("\n Client : ");
                gets(msgFromClient);

                sendingStatus = send(clientSocket, msgFromClient, strlen(msgFromClient), 0);

                if(sendingStatus<0) {
                        printf("\n cannot send data ");
                        closesocket(clientSocket);
                         exit(EXIT_FAILURE);
                }

                /* === Receiving Segment === */

                memset(msgFromServer,0,msgSize);  // initialize buffer

                receivingStatus = recv(clientSocket, msgFromServer, msgSize, 0); /* wait for data */

                if (receivingStatus<0) {
                        printf("\n cannot receive data ");
                        continue;
                }

                else if (receivingStatus==0) {
                        printf("\n connection close  by server");
                        closesocket(clientSocket);
                        break;
                }

                printf("\n Server : %s \n",msgFromServer);

                        if(strcmp(msgFromClient,"Bye")==0 || strcmp(msgFromClient,"bye")==0){
                            printf("\n Client is Closed !! ");
                            break;
                        }

        } // while loop close

         closesocket(clientSocket);

       /* Clear WSA (Window socket API) */
        WSACleanup();


        return 0;
}
