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
#include "file_utils.h"

#define MAX_FILE_LENGTH 2000

void client_error(const char *msg) {
    perror(msg);
    exit(1);
}

int send_file(const char *file_path, const char *hostname) {
    if (!is_encrypted_file(file_path)) {
        return -2;
    }

    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int portno = 8200;
    int byte_read, byte_wrote;
    off_t file_bytes_read;

    char file_contents[MAX_FILE_LENGTH];
    char filename[255];
    char message[255];

    // Creating a new socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        return -4;
    }

    server = gethostbyname(hostname);

    if (server == NULL) {
        return -5;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        return -6;
    }
        

    bzero(filename, 255);
    bzero(message, 255);
    bzero(file_contents, MAX_FILE_LENGTH);

    filename_from_path(file_path, filename);

    byte_wrote = write(sockfd, filename, strlen(filename));

    if (byte_wrote < 0) {
        return -7;
    }

    byte_read = read(sockfd, message, 255);

    if (byte_read < 0) {
        return -8;
    }

    printf("%s\n", message);

    // Reading file to transfer bytes
    int fd = open(file_path, O_RDONLY);

    file_bytes_read = lseek(fd, 0L, SEEK_END);
    printf("Client: file bytes read %d\n", (int) file_bytes_read);
    lseek(fd, 0L, SEEK_SET);

    file_bytes_read = read(fd, file_contents, (size_t) file_bytes_read);
    printf("Client: file bytes read %d\n", (int) file_bytes_read);
    close(fd);

    byte_wrote = write(sockfd, file_contents, (size_t) file_bytes_read);

    if (byte_wrote < 0) {
        return -7;
    }

    byte_read = read(sockfd, message, 255);

    if (byte_read < 0) {
        return -8;
    }

    if (strcmp(message, "Error: file already exists.") == 0) {
        return -3;
    }

    printf("%s\n", message);

    close(sockfd);
    return 0;
}