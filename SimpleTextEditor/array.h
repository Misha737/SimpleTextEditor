#pragma once
#include <stdlib.h>

const size_t history_size = 5;

typedef struct {
	size_t line;
	size_t index;
} Point;

typedef struct {
	Point point;
	size_t length;
} Vector;

struct HistoryItem {
	char** buffer;
	size_t lines;
};

class Buffer {
private:
	HistoryItem history[history_size];
	size_t curr_buffer = 0;

	void delete_buffer(struct HistoryItem);

	void set_buffer_from_history();
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

	void delete_chars(Point point, size_t length);
	
	void copy(Vector vector);
	
	int paste(Point vector);

	void undo();

	void redo();

	void update_history();
};