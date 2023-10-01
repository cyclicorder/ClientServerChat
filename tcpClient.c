#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>

int main()
{
    int socketFD=socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;  //creating server address

    //initialization

    char* ip ="127.0.0.1";  //server address

    address.sin_family=AF_INET;
    address.sin_port =htons(2000);  //put the bytes in the right order
    inet_pton(AF_INET,ip,&address.sin_addr.s_addr);  //converting it to an unsigned integer and put it in the address that we are giving the pointer


    //address.sin_addr.s_addr;
    
    int result= connect(socketFD,(struct sockaddr*) &address,sizeof(address));


    if (result == 0)
        printf("Successfully Connected\n");
/*

    char *line =NULL;
    size_t lineSize =0;
    printf("Start chatting...");

    while(1)
    {
        ssize_t lineCount = getline (&line,&lineSize,stdin);

        if (lineCount>0){

            if(strcmp(line,"exit\n") ==0)
            {
                break;
            }
            
            ssize_t amountSent =send(socketFD, line ,strlen(lineCount),0);
        }


    }
    
    //char* message;
    //message ="GET  \\HTTP/1.1\r\nHost:google.com\r\n\r\n";



    char buffer[1024];

    recv(socketFD,buffer,1024,0);

    printf("Response was %s\n", buffer);



    return 0;
    */
}