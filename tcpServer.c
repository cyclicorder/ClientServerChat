#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

int main()
{
    int serverSocketFD=socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddress;  //this address will be used to bind the server to listen for the incoming connections

    //initialization

    char* ip ="127.0.0.1";  //server address

    serverAddress.sin_family=AF_INET;
    serverAddress.sin_port =htons(2000); 

    if(strlen(ip)==0)
        serverAddress.sin_addr.s_addr=INADDR_ANY;  //going to listen for any address
    
    else{

        //put the bytes in the right order
        inet_pton(AF_INET,ip,&(serverAddress.sin_addr.s_addr));  //converting it to an unsigned integer and put it in the address that we are giving the pointer

    }


    int binding_res=bind(serverSocketFD,(struct sockaddr*)&serverAddress,sizeof(serverAddress));

    if(binding_res == 0)
        printf("Server Socket bounded successfully");

    int listen_res= listen(serverSocketFD,10);

    struct sockaddr_in clientAddress;  //client address

    int clientAddressSize = sizeof(clientAddress);

   //clientSocketFD is same as the tcpClient

    int clientSocketFD = accept(serverSocketFD, (struct sockaddr*)&clientAddress,&clientAddressSize) ;  //return the FD of the connecting clients.create a FD on ss for each connecting socket and return the client FD and thus it allows accesing the client

    char buffer[1024];

    recv(clientSocketFD, buffer, 1024,0);

    printf("Response was %s\n", buffer);

    return 0;
}