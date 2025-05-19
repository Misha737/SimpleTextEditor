#pragma once
#include "./array.h"
#include <stdio.h>
#include <string.h>

char** buffer = NULL;
size_t lines = 0;

void insert(size_t line_index, const char append[], size_t insert_pos) {
	if (lines <= line_index) {
		printf("Line position is longer then number of lines");
		exit(1);
	}
	char* buffer_line = buffer[line_index];
	if (buffer_line == NULL || append == NULL)
		exit(1);

	size_t src_size = strlen(buffer_line);
	size_t append_size = strlen(append);
	size_t full_size = src_size + append_size;

	if (append_size == 0) {
		return;
	}

	if (insert_pos > src_size) {
		printf("Insert position is longer then src length");
		exit(1);
	}

	buffer[line_index] = (char*)realloc(buffer_line, full_size + 1);
	if (buffer[line_index] == NULL) {
		printf("Error reallocation of memory\0");
		exit(1);
	}
	buffer_line = buffer[line_index];
	buffer_line[full_size] = '\0';

	for (size_t i = full_size - 1; i >= append_size + insert_pos; i--) {
		buffer_line[i] = buffer_line[i - append_size];
	}

	memcpy_s(buffer_line + insert_pos, full_size, append, append_size);
}

void append(const char append[]) {
	insert(lines - 1, append, strlen(buffer[lines - 1]));
}

void clear_buffer() {
	for (int i = 0; i < lines; i++) {
		free(buffer[i]);
	}
	lines = 0;
	//new_line();
}

void new_line() {
	buffer[lines] = (char*)malloc(1);
	if (buffer[lines] == NULL)
		exit(1);
	buffer[lines][0] = '\0';
	lines++;
}

void init_buffer() {
	lines = 0;
	// TODO: if there is more than 50 lines
	buffer = (char**)malloc(50 * sizeof(char**));
	if (buffer == NULL) {
		printf("Couldn't create a buffer");
		exit(1);
	}
	new_line();
}


void print_buffer() {
	printf("\n=========\n");
	for (int i = 0; i < lines; i++) {
		printf("%s\n", buffer[i]);
	}
	printf("=========\n");
}

int fwrite_buffer(const char* file_name) {
	FILE* file = fopen(file_name, "w");
	if (file == NULL) {
		printf("Couldn't create file for writing");
		return 1;
	}
	for (int i = 0; i < lines; i++) {
		fprintf(file, "%s\n", buffer[i]);
	}
	fclose(file);
	return 0;
}

int fread_buffer(const char* file_name) {
	FILE* file = fopen(file_name, "r");
	if (file == NULL) {
		printf("Couldn't open the file for writing\n");
		return 1;
	}
	clear_buffer();
	char line[256];
	int is_new_line = 0;
	while (fgets(line, sizeof(line), file)) {
		size_t end = strcspn(line, "\n");
		if (end < sizeof(line) - 1 && line[end] == '\n') {
			new_line();
		}
		line[end] = '\0';
		append(line);
	}

	fclose(file);
	return 0;
}

Point search_buffer(const char* str) {
	Point point;
	size_t str_length = strlen(str);
	if (str_length == 0) {
		point.line = -1;
		point.index = 1;
		return point;
	}
	for (int i_line = 0; i_line < lines; i_line++) {
		size_t line_length = strlen(buffer[i_line]);
		point.line = i_line;
		if (line_length < str_length)
			continue;
		for (int i_index = 0; i_index < line_length - str_length + 1; i_index++) {
			point.index = i_index;
			char* curr_line = buffer[i_line];
			for (int i_str = 0; i_str < str_length; i_str++) {
				if (curr_line[i_index + i_str] != str[i_str]) {
					break;
				}
				if (i_str == str_length - 1) {
					return point;
				}
			}
		}
	}
	point.line = -1;
	point.index = 0;
	return point;
}