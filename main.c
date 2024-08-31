#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "encryption.h"
#include "decryption.h"
#include "file_encryptor.h"
#include "file_decryptor.h"
#include "file_transfer_client.h"



int main(int argc, char* argv[]){
    pid_t pid = fork();

    // if it's the child process created
    if (pid == 0) {
        char *args[] = {"./file_transfer_server/server", NULL};
        execve("./file_transfer_server/server", args, NULL);
    } else {
        if (strcmp(argv[1], "1") == 0) {
            return send_file("example.txt", "antonio-VMware-Virtual-Platform");
        }
        printf("--------------\n");
        printf("Executing parent...\n");
        sleep(30);
        printf("Executing parent...\n");
        sleep(30);
        printf("Executing parent...\n");
        return kill(pid, SIGTERM);
    }
    return kill(pid, SIGTERM);
}

