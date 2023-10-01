#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>


//int main(int argc, char *argv[])
int main()

{
/*  if (argc!=3)
    {
        fprintf(stderr,"Usage: %s <host> <port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }*/
  


    //char *host =argv[1];

    //int port = atoi(argv[2]);

    char *ip = "127.0.0.1";
    int port = 5456;

    int clientSocketFD= socket(AF_INET,SOCK_STREAM, 0);

    if(clientSocketFD == -1)
    {
        printf("socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    printf("[+]TCP server socket created.\n");


    struct sockaddr_in serverAddress;
    memset(&serverAddress,'\0',sizeof(serverAddress));

    serverAddress.sin_family =AF_INET;
    serverAddress.sin_port =htons(port);

    serverAddress.sin_addr.s_addr =inet_addr(ip);


    int connect_result= connect(clientSocketFD,(struct sockaddr *)&serverAddress,sizeof(serverAddress));

    if(connect_result ==-1)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n",ip,port);



    fd_set readfds;

    char message[1024];

    int max_FD;


    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(clientSocketFD,&readfds);  //monitoring the client socket
        FD_SET(STDIN_FILENO,&readfds); //monitoring the standard input file descriptor

        //selecting the maximum_FD value



        if(STDIN_FILENO>clientSocketFD)
        {
            max_FD =STDIN_FILENO;
        }
        else{
            max_FD =clientSocketFD;

        }

        int select_result= select(max_FD+1, &readfds ,NULL, NULL ,NULL);

        if(select_result <0)
        {
            perror("There is an error in selecting the process\n");
            exit(EXIT_FAILURE);
        }

        //sending message to the server
        
        if(FD_ISSET(STDIN_FILENO,&readfds))
        {
            memset(message,0,1024);
            if(fgets(message,1024-1,stdin)!=NULL)
            {
                send(clientSocketFD,message,1024,0);
            }
        }

        //showing response from the server

        if(FD_ISSET(clientSocketFD,&readfds))
        {
            memset(message,0,1024);
            int response=recv(clientSocketFD, message, 1024,0);

            if(response<=0){
                perror("server has been disconnected\n");
                break;

            }

            printf("Server reply:%s\n",message);
           
            
        }


    }

    close(clientSocketFD);

    return 0;
}
