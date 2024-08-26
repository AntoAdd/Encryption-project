#include <string.h>
#include "file_utils.h"

bool is_encrypted_file(const char *file_path) {
    char *dot = strrchr(file_path, '.');

    if (strcmp(dot, ".cpt") == 0) {
        return true;
    }
    return false;
}

bool is_text_file(const char *file_path) {
    char *dot = strrchr(file_path, '.');

    if (strcmp(dot, ".txt")) {
        return true;
    }
    return false;
}

void filename_from_path(const char *file_path, char *filename) {
    int start = get_start_of_filename(file_path);
    int lenght = get_filename_length(file_path);

    for(int i = start, j = 0; i < start + lenght; i++, j++) {
        filename[j] = file_path[i];
    }
}

int get_start_of_filename(const char *file_path) {
    int name_start = 0;
    int i = 0;

    while(file_path[i] != '\0') {
        if (file_path[i] == '/') {
            name_start = i + 1;
        }
        i++; 
    }
    return name_start;
}

int get_filename_length(const char *file_path) {
    int i = get_start_of_filename(file_path);
    int length = 0;

    while(file_path[i] != '.') {
        length++;
        i++;
    }
    return length;
}