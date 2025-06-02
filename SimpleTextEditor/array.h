#pragma once
#include <stdlib.h>

typedef struct {
	size_t line;
	size_t index;
} Point;

class Buffer {
public:
	char** buffer;
	size_t lines;

	Buffer();

	void insert(size_t line_index, const char append[], size_t insert_pos);

	void append(const char append[]);

	void clear_buffer();

	void new_line();

	void init_buffer();

	void print_buffer();

	int fwrite_buffer(const char* file_name);

	int fread_buffer(const char* file_name);

	Point search_buffer(const char* str, size_t start_line, size_t start_index);
};