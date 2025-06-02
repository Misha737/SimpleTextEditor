#pragma once
#include "./array.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <windows.h>

const size_t SIZE_SEGM = 64;
size_t segments = 0;

Buffer::Buffer() {
	buffer = NULL;
	lines = 0;
	init_buffer();
	for (size_t i = 0; i < history_size; i++) {
		history[i].lines = -1;
	}
	curr_buffer = 0;
	history[curr_buffer].buffer = buffer;
	history[curr_buffer].lines = lines;
}

void Buffer::insert(size_t line_index, const char append[], size_t insert_pos) {
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

void Buffer::append(const char append[]) {
	insert(lines - 1, append, strlen(buffer[lines - 1]));
}

void Buffer::clear_buffer() {
	for (int i = 0; i < lines; i++) {
		free(buffer[i]);
	}
	lines = 0;
	segments = 0;
	//new_line();
}

void Buffer::new_line() {
	buffer[lines] = (char*)malloc(1);
	if (buffer[lines] == NULL)
		exit(1);
	buffer[lines][0] = '\0';
	lines++;
	if (lines + 1 >= segments * SIZE_SEGM) {
		segments++;
		char** temp = (char**)realloc(buffer, segments * SIZE_SEGM * sizeof(char*));
		if (temp == NULL) {
			printf("Error reallocation of memory\0");
			exit(1);
		}
		buffer = temp;
	}
}

void Buffer::init_buffer() {
	lines = 0;
	buffer = (char**)malloc(SIZE_SEGM * sizeof(char*));
	if (buffer == NULL) {
		printf("Couldn't create a buffer");
		exit(1);
	}
	segments++;
	new_line();
}


void Buffer::print_buffer() {
	printf("\n=========\n");
	for (int i = 0; i < lines; i++) {
		std::cout << buffer[i] << std::endl;
	}
	printf("=========\n");
}

int Buffer::fwrite_buffer(const char* file_name) {
	FILE* file = NULL;
	int error;
	if ((error = fopen_s(&file, file_name, "w")) != 0) {
		printf("Couldn't create file for writing. Code: %d", error);
		return 1;
	}
	for (int i = 0; i < lines - 1; i++) {
		fprintf(file, "%s\n", buffer[i]);
	}
	fprintf(file, "%s", buffer[lines - 1]);
	fclose(file);
	return 0;
}

int Buffer::fread_buffer(const char* file_name) {
	FILE* file = NULL;
	int error;
	if ((error = fopen_s(&file, file_name, "r")) != 0) {
		printf("Couldn't create file for writing. Code: %d", error);
		return 1;
	}
	clear_buffer();
	new_line();
	char line[256];
	while (fgets(line, sizeof(line), file)) {
		size_t end = strcspn(line, "\n");
		int is_new_line = line[end] == '\n';
		line[end] = '\0';
		append(line);
		if (end < sizeof(line) - 1 && is_new_line) {
			new_line();
		}
	}

	fclose(file);
	return 0;
}

Point Buffer::search_buffer(const char* str, size_t start_line, size_t start_index) {
	Point point;
	size_t str_length = strlen(str);
	if (str_length == 0) {
		point.line = -1;
		point.index = 1;
		return point;
	}
	for (size_t i_line = start_line; i_line < lines; i_line++) {
		size_t line_length = strlen(buffer[i_line]);

		if (line_length < str_length)
			continue;
		for (size_t i_index = start_index; i_index < line_length - str_length + 1; i_index++) {
			point.index = i_index;
			char* curr_line = buffer[i_line];
			for (int i_str = 0; i_str < str_length; i_str++) {
				if (curr_line[i_index + i_str] != str[i_str]) {
					break;
				}
				if (i_str == str_length - 1) {
					point.line = i_line;
					point.index = i_index;
					return point;
				}
			}
		}
		start_index = 0;
	}
	point.line = -1;
	point.index = 0;
	return point;
}

void Buffer::delete_chars(Point point, size_t length) {
	char* line = buffer[point.line];

	for (size_t i = point.index + length; i <= strlen(line); i++) {
		line[i - length] = line[i];
	}
	char* temp = (char*)realloc(line, strlen(line) + 1);
	if (temp == nullptr) {
		printf("Error reallocation of memory\0");
		exit(1);
	}
	buffer[point.line] = temp;
}

void Buffer::copy(Vector vector) {
	if (OpenClipboard(0))
	{
		EmptyClipboard();
		// TODO: some problem
		char* hBuff = (char*)GlobalAlloc(GMEM_FIXED, vector.length + 1);
		if (hBuff == nullptr) {
			return;
		}

		char* dest = (char*)GlobalLock(hBuff);
		if (dest == nullptr) {
			GlobalFree(hBuff);
			return;
		}
		strncpy_s(dest, 2048, buffer[vector.point.line] + vector.point.index, vector.length + 1);
		SetClipboardData(CF_TEXT, hBuff);
		CloseClipboard();
	}
}

int Buffer::paste(Point vector) {
	if (!OpenClipboard(nullptr)) return -1;

	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr) {
		CloseClipboard();
		return -1;
	}

	char* text = static_cast<char*>(GlobalLock(hData));
	if (text == nullptr) {
		CloseClipboard();
		return -1;
	}

	insert(vector.line, text, vector.index);

	GlobalUnlock(hData);
	CloseClipboard();
	return 0;
}


void Buffer::undo() {
	if (curr_buffer > 0) {
		curr_buffer--;
		set_buffer_from_history();
	}
}

void Buffer::redo() {
	if (curr_buffer >= history_size - 1)
		return;
	if (history[curr_buffer + 1].lines == -1)
		return;
	curr_buffer++;
	set_buffer_from_history();

}

void Buffer::delete_buffer(struct HistoryItem history_item) {
	for (size_t i = 0; i < history_item.lines; i++) {
		delete[] history_item.buffer[i];
	}
	delete[] history_item.buffer;
}

void Buffer::update_history() {
	struct HistoryItem history_item;
	history_item.lines = lines;
	history_item.buffer = new char* [lines];
	for (size_t i = 0; i < lines; i++) {
		history_item.buffer[i] = new char[strlen(buffer[i]) + 1];
		strcpy_s(history_item.buffer[i], strlen(buffer[i]) + 1, buffer[i]);
	}
	if (curr_buffer == history_size - 1) {
		delete_buffer(history[0]);
		history[0].lines = -1;
		for (size_t i = 0; i < history_size - 1; i++) {
			history[i] = history[i + 1];
		}
		history[history_size - 1] = history_item;

	}
	else if (history[curr_buffer + 1].lines != -1) {
		for (size_t i = curr_buffer + 1; i < history_size; i++) {
			if (history[i].lines == -1)
				break;
			delete_buffer(history[i]);
			history[i].lines = -1;
		}
		curr_buffer++;
		history[curr_buffer] = history_item;
	}
	else {
		history[++curr_buffer] = history_item;
	}
	set_buffer_from_history();
}

void Buffer::set_buffer_from_history() {
	buffer = history[curr_buffer].buffer;
	lines = history[curr_buffer].lines;
}