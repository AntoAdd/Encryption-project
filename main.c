#include <stdio.h>
#include "encryption.h"

int main(void){
    char sentence[] = "Abcd1239?!";

    printf("Before encryotion: %s\n", sentence);

    caesar_encrypt(sentence, 3);

    printf("After encryption: %s\n\n", sentence);
    
    return 0;
}