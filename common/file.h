#ifndef FILE_H
#define FILE_H

// file.c
typedef struct {
	void *buffer;
	long size;
} file;

file load_file(const char *path);
void free_file(file f);

#endif
