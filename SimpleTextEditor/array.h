#pragma once
#include <stdlib.h>

//#define line buffer[lines - 1]

extern char** buffer;
extern size_t lines;

typedef struct {
	int line;
	int index;
} Point;

void insert(size_t line_index, const char append[], size_t insert_pos);

void append(const char append[]);

void clear_buffer();

void new_line();

void init_buffer();

void print_buffer();

int fwrite_buffer(const char* file_name);

int fread_buffer(const char* file_name);

Point search_buffer(const char* str);