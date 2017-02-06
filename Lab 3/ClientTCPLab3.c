#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
//Matt Noblett
//Lab 2 - TCP Server/Client
//Client File
//1/18/17


void* fromserver(void* arg){

    int sockfd = *(int *)arg;
    char sm[5000];

    while(1){
        int server_message = recv(sockfd, sm, 5000, 0);

        if(!strcmp(sm, "Exit")){
            printf("Server has Disconnected. \n");
            exit(0);
        }
        else if(server_message){
            printf("Message from Server: %s\n Please enter a message to send to server: ", sm);
        }
        //Zero's out the array of characters.
        bzero(sm, sizeof(sm));
    }
}



int main(int argc, char** argv){
    //Second Parameter is determining the transport layer protocol. Sock is telling it to use TCP
    //Returns an integer. There is alot done behind the scenes.
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        printf("There was an error creating the socket\n");
        return 1;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    //Port Number - Specify the application on a computer that is supposed to be receiving the packet.
    printf("Please enter a port number to connect to: \n");
    int port;
    int result = scanf("%d", &port);

    if(result == EOF){
        printf("End of File");
    }
    serveraddr.sin_port = htons(port);
    //Takes the text version of an address, and converts it to the binary version
    //In this example, it is a special address, doesn't work like most. This will always refer back 
    //to the computer you are on. "Loop Back Address" 127.0.0.1
    printf("Please enter the IP Address: ");
    char ip[5000];
    scanf("%s", &ip);
    serveraddr.sin_addr.s_addr = inet_addr(ip);

    //Takes the address and socket, and associates them together, permenently. 
    //It will make sure that there is someone "listening" at the address first. 
    //If there isn't any there, it will return an error.
    //Last parameter is the length of the address
    int e = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    if(e < 0){
        printf("There was an error with connecting\n");
        return 1;
    }

    pthread_t child;
    pthread_create(&child, NULL, fromserver, &sockfd);
    pthread_detach(child);

    char line[5000];
    while(1){
        printf("Enter a message: ");
        bzero(line, sizeof(line));
        fgets(line, 5000, stdin);
        size_t len = strlen(line);
        if(len > 0 && line[len-1] == '\n'){
            line[--len] =0;
        }
        send(sockfd, line, len, 0);
        if(!strcmp(line, "Quit")){
            exit(0);
        }
    }
    return 0;
}
