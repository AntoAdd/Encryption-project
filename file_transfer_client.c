#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include "file_transfer_client.h"

#define MAX_FILE_LENGTH 2000

void client_error(const char *msg) {
    perror(msg);
    exit(1);
}

int send_file(const char *file_path, const char *hostname) {
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int portno = 8200;
    int byte_read, byte_wrote, file_bytes_read;

    char file_contents[MAX_FILE_LENGTH];
    char filename[255];
    char message[255];

    // Creating a new socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        client_error("Error creating socket.");
    }

    server = gethostbyname(hostname);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        client_error("Connection error.");

    bzero(filename, 255);
    bzero(message, 255);
    bzero(file_contents, MAX_FILE_LENGTH);

    // TODO: derive filename from path
    printf("Enter filename to transfer: \n");
    scanf("%s", filename);

    byte_wrote = write(sockfd, filename, 255);

    if (byte_wrote < 0) {
        client_error("Error writing to socket.");
    }

    byte_read = read(sockfd, message, 255);

    if (byte_read < 0) {
        client_error("Error reading from socket.");
    }

    printf("%s\n", message);

    // Reading file to transfer bytes
    int fd = open(file_path, O_RDONLY);

    off_t sifile_bytes_read = lseek(fd, 0L, SEEK_END);
    lseek(fd, 0L, SEEK_SET);

    file_bytes_read = read(fd, file_contents, (size_t) file_bytes_read);
    close(fd);

    byte_wrote = write(sockfd, file_contents, (size_t) file_bytes_read);

    if (byte_wrote < 0) {
        client_error("Error writing to socket.");
    }

    byte_read = read(sockfd, message, 255);

    if (byte_read < 0) {
        client_error("Error reading from socket.");
    }

    printf("%s\n", message);

    close(sockfd);
    return 0;
}