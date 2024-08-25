#include <ctype.h>
#include "encryption.h"
#include "text_elements.h"

char encrypted_letters[26];
char encrypted_numbers[10];
char encrypted_symbols[13];

// encrypts letters, numbers and symbols using the given key.
void encrypt_all_elements(int k);

// encrypts the elements of a given element (letters, numbers or symbols).
// The parameters k and l represent respectively the key and lenght of array.
void encrypt_elements(const char src_array[], char dst_array[], int k, int l);

// Returns the position of the given element in the given array.
// If the element doesn't exist in the array, -1 is returned.
int get_element_pos(const char array[], char value, int length);

void caesar_encrypt(char *str, int k) {
    encrypt_all_elements(k);

    for (int i = 0; str[i] != '\0'; i++) {
        char current = str[i];
        int substitution_pos = -1;

        if (current == '\n' || current == '\t' || current == ' ') {
            continue;
        } else if (isdigit(current)) {
            substitution_pos = get_element_pos(numbers, current, 10);
            str[i] = encrypted_numbers[substitution_pos];
        } else if (isalpha(current)) {
            substitution_pos = get_element_pos(letters, tolower(current), 26);
            if (islower(current)) {
                str[i] = encrypted_letters[substitution_pos];
            } else {
                str[i] = toupper(encrypted_letters[substitution_pos]);
            }
        } else {
            substitution_pos = get_element_pos(symbols, current, 13);
            str[i] = encrypted_symbols[substitution_pos];
        }
    }
}

void encrypt_all_elements(int k) {
    encrypt_elements(letters, encrypted_letters, k, 26);
    encrypt_elements(numbers, encrypted_numbers, k, 10);
    encrypt_elements(symbols, encrypted_symbols, k, 13);
}

void encrypt_elements(const char src_array[], char dst_array[], int k, int l) {
    for (int i = 0; i < l; i++) {
        dst_array[i] = src_array[(i + k) % l];
    }
}

int get_element_pos(const char array[], char value, int length) {
    for (int i = 0; i < length; i++) {
        if (array[i] == value) {
            return i;
        }
    }
    return -1;
}