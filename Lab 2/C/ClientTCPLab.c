#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//Matt Noblett
//Lab 2 - TCP Server/Client
//Client File
//1/18/17
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

    printf("Enter a message: ");
    char line[5000];
    int len = sizeof(serveraddr);
    scanf("%s", &line);
    send(sockfd,line, strlen(line)+1, 0);
    listen(sockfd, 10);
    char echoline[5000]; 
	//Getting the sent message back from the server. 
    int n = recv(sockfd, line, 5000, 0);
    printf("Echo from Server: %s\n", line);
    close(sockfd);
    return 0;
}
