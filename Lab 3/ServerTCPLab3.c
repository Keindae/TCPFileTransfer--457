#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
//Matt Noblett
//Lab 3 - TCP Server/Client
//Server File
//1/18/17


void* handleclient(void *arg){
    int clientsocket = *(int *)arg;
    char line [5000];

    while(1){
        int fromclient = recv(clientsocket, line, 5000, 0);
        if(!strcmp(line, "Quit")){
            printf("Client has disconnected");
            close(clientsocket);
            exit(0);
        }
        else if(fromclient){
            printf("Message from Client: %s\n Please enter a message to send: ", line);

        }
        bzero(line, sizeof(line));
    }
}

int main( int argc, char **agv){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serveraddr, clientaddr;
    serveraddr.sin_family = AF_INET;
    printf("Please enter a port number to listen to: \n");
    int port;
    int result = scanf("%d", &port);
    if(result == EOF){
        printf("End of File");
            }
    serveraddr.sin_port = htons(port);
    //INADDR_ANY is a constant, which allows for computer programs to be portable. It will listen
    //to all of the address that the computer has. 
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    
    //Says, "This is our address on our side. Bind isn't going to see if anyone is listening, 
    //It will make sure that the port number is not being used, and will return an error if it is. 
   int e = bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if(e < 0){
        printf("There was an error binding the address\n");
        return 1;
    }
    //This tells the server to start listening to incoming connections. 
    listen(sockfd, 10);
    printf("Waiting for a Client:\n");
     int len = sizeof(clientaddr);
     //This accepts an incoming client
    //It will return a diffrent address
     int clientsocket = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
     pthread_t child;
     pthread_create(&child, NULL, handleclient,&clientsocket);
     //Cleans up the child once it get done running,
     //because we don't care about the
     //return value of the child. 
     pthread_detach(child);
     char line[5000];
     while(1){
         printf("Enter a message to send to client: ");
         bzero(line, sizeof(line));
         fgets(line, 5000, stdin);
         size_t len = strlen(line);
         if(len > 0 && line[len -1] == '\n'){
             line[--len] = 0;
         }
         send(clientsocket, line, len, 0);
         if(!strcmp(line, "Quit")){
             close(clientsocket);
             exit(0);
         }
     }
     return 0;
}

