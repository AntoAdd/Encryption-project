#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "file_encryptor.h"
#include "encryption.h"

int get_start_of_filename(const char *file_path);

int get_filename_length(const char *file_path);

void filename_from_path(const char *file_path, char *filename);

void encrypt_file(const char *file_path, int k) {
    int fd = open(file_path, O_RDWR);

    // system call lseek is used to retrive the total number of bytes of the file.
    off_t size = lseek(fd, 0L, SEEK_END);

    char file_content[(int) size + 1];

    // rewinds the offset to start of the file
    off_t f_start = lseek(fd, 0L, SEEK_SET);

    ssize_t read_bytes = read(fd, file_content, (size_t) size);

    file_content[size] = '\0';

    caesar_encrypt(file_content, k);

    const char *encrypted_content = file_content;

    // Repositioning the offset to the first byte to override the file.
    lseek(fd, 0L, SEEK_SET);

    // Write the encrypted content to file.
    ssize_t wrote_bytes = write(fd, encrypted_content, (size_t) size);

    int filename_length = get_filename_length(file_path);

    char filename[filename_length];

    filename_from_path(file_path, filename);

    char old_filename[filename_length + 5];
    char new_filename[filename_length + 5];

    strcpy(old_filename, filename);
    strcat(old_filename, ".txt");

    strcpy(new_filename, filename);
    strcat(new_filename, ".cpt");

    rename(old_filename, new_filename);

    // Using fchmod system call to set read only permission for all users.
    int rc = fchmod(fd, S_IRUSR|S_IRGRP|S_IROTH);

    close(fd);
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