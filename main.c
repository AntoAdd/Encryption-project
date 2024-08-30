#include <stdio.h>
#include <pthread.h>
#include "encryption.h"
#include "decryption.h"
#include "file_encryptor.h"
#include "file_decryptor.h"
#include "file_transfer_server.h"

// only for test (sleep function)
#include <unistd.h>

void *server_fun(void *arg);

int main(void){
    pthread_t server_thread;
    if (pthread_create(&server_thread, NULL, server_fun, NULL) != 0) {
        printf("Error creating server thread.");
        return -1;
    }

    sleep(5);

    printf("After thread creation\n");
    
    
    return 0;
}

void *server_fun(void *arg) {
    printf("Starting file transfer server...\n");
    start_server();
}