#include <stdio.h>
#include "encryption.h"
#include "decryption.h"
#include "file_encryptor.h"
#include "file_decryptor.h"

int main(void){
    decrypt_file("example.cpt", 3);
    
    return 0;
}