#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "file_decryptor.h"
#include "decryption.h"
#include "file_utils.h"

void decrypt_file(const char *file_path, int k) {
    // Checks if the file is encrypted.
    if (!is_encrypted_file(file_path)) {
        printf("Error: file \"%s\" is not encrypted!\n", file_path);
        return;
    }

    // Using chmod system call to set read and write permissions.
    chmod(file_path, S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH);

    int fd = open(file_path, O_RDWR);

    // system call lseek is used to retrive the total number of bytes of the file.
    off_t size = lseek(fd, 0L, SEEK_END);

    char file_content[(int) size + 1];

    // rewinds the offset to start of the file.
    off_t f_start = lseek(fd, 0L, SEEK_SET);

    ssize_t read_bytes = read(fd, file_content, (size_t) size);

    file_content[size] = '\0';

    caesar_decrypt(file_content, k);

    lseek(fd, 0L, SEEK_SET);

    // Write the encrypted content to file.
    ssize_t wrote_bytes = write(fd, file_content, (size_t) size);

    int filename_length = get_filename_length(file_path);

    char filename[filename_length];

    filename_from_path(file_path, filename);

    char old_filename[filename_length + 5];
    char new_filename[filename_length + 5];

    strcpy(old_filename, filename);
    strcat(old_filename, ".cpt");

    strcpy(new_filename, filename);
    strcat(new_filename, ".txt");

    rename(old_filename, new_filename);

    close(fd);
}