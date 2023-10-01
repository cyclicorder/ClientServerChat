#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>


int main()

{

    int serverSocketFD= socket(AF_INET,SOCK_STREAM, 0);

    if(serverSocketFD == -1)
    {
        perror("socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    printf("[+]TCP server socket created.\n");        //new

    char *ip ="127.0.0.1";
    int port =5456;

    struct sockaddr_in serverAddress;
    memset(&serverAddress,'\0',sizeof(serverAddress)); //new
    serverAddress.sin_family =AF_INET;
    serverAddress.sin_port =htons(port);
    //serverAddress.sin_addr.s_addr =INADDR_ANY;
    serverAddress.sin_addr.s_addr =inet_addr(ip); //new


    int bind_result= bind(serverSocketFD, (struct sockaddr*)&serverAddress,sizeof(serverAddress));


    if(bind_result ==-1)
    {
        perror("Bind failed");
        exit(1);
    }

    printf("[+]Bind to the port number: %d\n", port);  //new

    //get the hostname

    
    char hostinfo[1024];
    if (gethostname(hostinfo, sizeof(hostinfo)) == -1) {
        perror("Get hostname failed");
        exit(EXIT_FAILURE);
    }

    //listening for incoming connection request

    int listen_result= listen(serverSocketFD,10); //10 clients are stored in the queue

    if(listen_result ==-1)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("server: started server on '%s' at %s: '%d'\n", hostinfo, inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port));

    int clientSocketFD[20],i; //array of storing 20 client socket descriptor


    for (i=0; i<20;i++)
    {
        clientSocketFD[i]=0;   //setting all the clientSocketFD =0 indicates, there is no connected clients

    
    }

    




    fd_set readfds;

    char message[1024];

    int max_FD,temp_FD;


    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(serverSocketFD,&readfds);  //monitoring the client socket
        
        //selecting the maximum_FD value

        max_FD =serverSocketFD;

        for (i =0 ;i<20;i++)
        {
            temp_FD =clientSocketFD[i];
            
            if(temp_FD>0)
                FD_SET(temp_FD,&readfds);

            if (temp_FD>max_FD)
                max_FD =temp_FD;
        }

        int select_result= select(max_FD+1, &readfds ,NULL, NULL ,NULL);

        if(select_result <0 )
        {
            perror("There is an error in selecting the process\n");
            exit(EXIT_FAILURE);
        }

        
        //processing new connection

        if(FD_ISSET(serverSocketFD,&readfds))   //If the server socket is in the set, it means there is a new connection request pending.
        {
            
            int client_new_sock=accept(serverSocketFD,NULL,NULL);

            if(client_new_sock == -1){
                perror("Accept operation failed\n");
                exit(EXIT_FAILURE);

            }

            printf("New connection has been established and the socket fd is:  %d\n", client_new_sock);

            for(i =0 ;i<20;i++)
            {
                if(clientSocketFD[i]==0)
                {
                    clientSocketFD[i]=client_new_sock;
                    break;
                }
            }    
            
        }


        // processing client message
        int current_clientFD;

        for (i =0 ;i<20;i++)
        {
            current_clientFD =clientSocketFD[i];

            if(FD_ISSET(current_clientFD,&readfds))
            {
                if(read(current_clientFD,message,1024)== 0)
                {
                    getpeername(current_clientFD,(struct sockaddr *)&serverAddress,(socklen_t *)&serverAddress);

                    printf("Host disconnected, ip %s, port %d\n", inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port));
                    close(current_clientFD); //closing the current client

                    clientSocketFD[i]=0;
                }

                else{
                    for (int j=0;j<20;j++)
                    {
                        if (clientSocketFD[j] != 0 && clientSocketFD[j] != current_clientFD) {
                            send(clientSocketFD[j], message, strlen(message), 0);
                    }
                }
            }

        }


    }






}


}
