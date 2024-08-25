#include <stdio.h>
#include "encryption.h"
#include "decryption.h"
#include "file_encryptor.h"

int main(void){
    encrypt_file("example.txt", 3);
    
    return 0;
}