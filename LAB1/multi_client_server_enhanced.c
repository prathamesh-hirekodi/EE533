#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h> // For waitpid()

void error(const char *msg) //This function handles error messages & exits the program
{
    perror(msg); //prints the error messages
    exit(1);
}

// This function is used to handle the client connection (as advised in the Lab)
void dostuff(int sockfd)
{
    int n;
    char buffer[256]; 

    bzero(buffer, 256); 
    n = read(sockfd, buffer, 255); 
    if (n < 0) error("ERROR reading from socket");

    buffer[n] = '\0'; 
    printf("Here is the message: %s\n", buffer);

    n = write(sockfd, "I got your message", 18);
    if (n < 0) error("ERROR writing to socket");

    close(sockfd); 
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    int pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) {
        fprintf(stderr, "ERROR, No port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

   
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on Binding");

   
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("Server booted up. Server is Listening on Port %d...\n", portno);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            error("ERROR on accept");
            }

        pid = fork();
        if (pid < 0) error("ERROR on fork"); 

        if (pid == 0) {
            close(sockfd); 
            dostuff(newsockfd); 
            exit(0);
        } else {
            close(newsockfd); 
        }
    }

    close(sockfd); 
    return 0;
}