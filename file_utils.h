#ifndef FILE_UTILS_H
#define FILE_UTILS_H
#include <stdbool.h>

// Reurns true if the file specifyed by the path is encrypted (.cpt extension),
// false otherwise.
extern bool is_encrypted_file(const char *file_path);

// Returns true if the file specyfied by the path is a text file (.txt extension),
// false otherwise.
extern bool is_text_file(const char *file_path);

// Returns the index of the first filename character in the path string.
extern int get_start_of_filename(const char *file_path);

// Returns the length of filename (number of characters in the name).
extern int get_filename_length(const char *file_path);

// Extract the filename substring from path string and save it in filename.
extern void filename_from_path(const char *file_path, char *filename);

#endif