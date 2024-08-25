#include <ctype.h>
#include "decryption.h"
#include "text_elements.h"

char decrypted_letters[26];
char decrypted_numbers[10];
char decrypted_symbols[13];

// Decrypts all the elements of a given type using the given key (parameter k).
void decrypt_elements(const char src_array[], char dst_array[], int k, int l);

// Decrypts letters, numbers and symbols using the given key.
void decrypt_all_elements(int k);

// Returns the position of the given element in the given array.
// If the element doesn't exist in the array, -1 is returned.
int get_element_position(const char array[], char value, int length);

void caesar_decrypt(char *str, int k) {
    decrypt_all_elements(k);

    for (int i = 0; str[i] != '\0'; i++) {
        char current = str[i];
        int substitution_pos = -1;

        if (current == '\n' || current == '\t' || current == ' ') {
            continue;
        } else if (isdigit(current)) {
            substitution_pos = get_element_position(numbers, current, 10);
            str[i] = decrypted_numbers[substitution_pos];
        } else if (isalpha(current)) {
           substitution_pos = get_element_position(letters, tolower(current), 26);
           if (islower(current)) {
                str[i] = decrypted_letters[substitution_pos];
            } else {
                str[i] = toupper(decrypted_letters[substitution_pos]);
            } 
        } else {
            substitution_pos = get_element_position(symbols, current, 13);
            str[i] = decrypted_symbols[substitution_pos];
        }
    }
}

void decrypt_all_elements(int k) {
    decrypt_elements(letters, decrypted_letters, k, 26);
    decrypt_elements(numbers, decrypted_numbers, k, 10);
    decrypt_elements(symbols, decrypted_symbols, k, 13);
}

void decrypt_elements(const char src_array[], char dst_array[], int k, int l) {
    for (int i = 0; i < l; i++) {
        if ((i - k) >= 0) {
            dst_array[i] = src_array[i - k];
        } else {
            dst_array[i] = src_array[l + (i - k)];
        }
    }
}



int get_element_position(const char array[], char value, int length) {
    for (int i = 0; i < length; i++) {
        if (array[i] == value) {
            return i;
        }
    }
    return -1;
}

