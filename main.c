#include <stdio.h>
#include "encryption.h"
#include "decryption.h"

int main(void){
    char sentence[] = "Abcd1239?!";

    printf("Before encryotion: %s\n\n", sentence);

    caesar_encrypt(sentence, 3);

    printf("After encryption: %s\n\n", sentence);

    caesar_decrypt(sentence, 3);

    printf("After decryprion: %s\n\n", sentence);
    
    return 0;
}