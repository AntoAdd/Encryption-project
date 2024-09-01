#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "file_encryptor.h"
#include "encryption.h"
#include "file_utils.h"



int encrypt_file(const char *file_path, int k) {
    if (is_encrypted_file(file_path)) {
        return -1;
    }

    int fd = open(file_path, O_RDWR);

    // system call lseek is used to retrive the total number of bytes of the file.
    off_t size = lseek(fd, 0L, SEEK_END);

    char file_content[(int) size + 1];

    // rewinds the offset to start of the file.
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
    return 0;
}