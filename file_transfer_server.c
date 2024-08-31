#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include "file_transfer_server.h"

// A page of text consists (on average) of 2KB
#define MAX_FILE_LENGTH 2000

const char * get_ip_addr();

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void start_server() {
    int sockfd, new_sockfd;
    int portno = 8200;
    socklen_t clilen;
    char file_contents[MAX_FILE_LENGTH];
    char filename[255];
    struct sockaddr_in serv_addr, cli_addr;
    int byte_read, byte_wrote, file_bytes;

    // Opening socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening the socket.");
    }

    // Resets the server address struct
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    inet_aton(get_ip_addr(), (struct in_addr *)&serv_addr.sin_addr.s_addr);
    serv_addr.sin_port = htons(portno);

    // Binding socket to address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Error during binding.");
    }

    while(1) {
        listen(sockfd, 1);

        printf("Waiting for connection...\n");

        clilen = sizeof(cli_addr);
        new_sockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (new_sockfd < 0) {
            error("Error on accepting client connection.");
        }

        printf("Connection accepted.\n");

        bzero(file_contents, MAX_FILE_LENGTH);
        bzero(filename, 255);

        // Reading filename from socket
        byte_read = read(new_sockfd, filename, 255);
        printf("Filename bytes read: %d", (int) byte_read);

        if (byte_read < 0) {
            error("Error reading from socket.");
        }

        byte_wrote = write(new_sockfd, "Filename read, waiting file", 27);

        if (byte_wrote < 0) {
            error("Error writing to socket.");
        }

        // Reading file bytes from socket
        file_bytes = read(new_sockfd, file_contents, MAX_FILE_LENGTH);

        if (file_bytes < 0) {
            error("Error reading file bytes from socket.");
        }

        byte_wrote = write(new_sockfd, "File recieved correctly!", 24);

        if (byte_wrote < 0) {
            error("Error writing to socket.");
        }

        int fd = open(filename, O_CREAT|O_WRONLY);
        printf("file descriptor: %d\n", fd);

        int b = write(fd, file_contents, file_bytes);
        printf("Bytes wrote to file: %d", b);
        
        close(fd);
        close(new_sockfd);
    }
    close(sockfd);
}

const char * get_ip_addr() {
    char hostname[255];

    if(gethostname(hostname, sizeof(hostname)) < 0) {
        error("Error retreiving the hostname.");
    }

    struct hostent *host_entry = gethostbyname(hostname);

    const char *IPaddr = inet_ntoa(*((struct in_addr *)host_entry->h_addr));


    printf("Hostname: %s\n", hostname);
    printf("IP: %s\n", IPaddr);

    return IPaddr;
}