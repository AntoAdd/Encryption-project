#include <stdio.h>
#include "encryption.h"
#include "decryption.h"

int main(void){
    char sentence[] = "Abcd\t1239?!\n'\"ciao8";

    printf("Before encryption: %s\n\n", sentence);

    caesar_encrypt(sentence, 3);

    printf("After encryption: %s\n\n", sentence);

    caesar_decrypt(sentence, 3);

    printf("After decryption: %s\n\n", sentence);
    
    return 0;
}