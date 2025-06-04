#pragma once
#include "./array.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <windows.h>

const size_t SIZE_SEGM = 64;
size_t segments = 0;



TextEditor::TextEditor() {
	buffer = NULL;
	lines = 0;
	init_buffer();
	head = 0;
	//for (size_t i = 0; i < history_size; i++) {
	//	history[i].lines = -1;
	//}
	//curr_buffer = 0;
	//history[curr_buffer].buffer = buffer;
	//history[curr_buffer].lines = lines;
	point.line = 0;
	point.index = 0;
}

TextEditor::~TextEditor() {
	size_t i;
	while (commits[i] != nullptr) {
		delete commits[i];
	}
}

void TextEditor::insert(const char append[]) {
	if (lines <= point.line) {
		printf("Line position is longer then number of lines");
		exit(1);
	}
	char* buffer_line = buffer[point.line];
	if (buffer_line == NULL || append == NULL)
		exit(1);

	size_t src_size = strlen(buffer_line);
	size_t append_size = strlen(append);
	size_t full_size = src_size + append_size;

	if (append_size == 0) {
		return;
	}

	if (point.index > src_size) {
		printf("Insert position is longer then src length");
		exit(1);
	}

	buffer[point.line] = (char*)realloc(buffer_line, full_size + 1);
	if (buffer[point.line] == NULL) {
		printf("Error reallocation of memory\0");
		exit(1);
	}
	buffer_line = buffer[point.line];
	buffer_line[full_size] = '\0';

	for (size_t i = full_size - 1; i >= append_size + point.index; i--) {
		buffer_line[i] = buffer_line[i - append_size];
	}

	memcpy_s(buffer_line + point.index, full_size, append, append_size);
}

void TextEditor::append(const char append[]) {
	Point temp_point = point;
	point.line = lines - 1;
	point.index = strlen(buffer[lines - 1]);
	insert(append);
	point = temp_point;
}

void TextEditor::clear_buffer() {
	for (int i = 0; i < lines; i++) {
		free(buffer[i]);
	}
	lines = 0;
	segments = 0;
	set_cursor_zero();
	//new_line();
}

void TextEditor::new_line() {
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

void TextEditor::delete_line() {
	delete[] buffer[lines - 1];
	delete buffer[lines - 1];
	buffer[lines] = nullptr;
	lines--;
}

void TextEditor::init_buffer() {
	lines = 0;
	buffer = (char**)malloc(SIZE_SEGM * sizeof(char*));
	if (buffer == NULL) {
		printf("Couldn't create a buffer");
		exit(1);
	}
	segments++;
	new_line();
}


void TextEditor::print_buffer() {
	printf("\n=========\n");
	for (int i = 0; i < lines; i++) {
		std::cout << buffer[i] << std::endl;
	}
	printf("=========\n");
}

int TextEditor::fwrite_buffer(const char* file_name) {
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

int TextEditor::fread_buffer(const char* file_name) {
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

Point TextEditor::search_buffer(const char* str, size_t start_line, size_t start_index) {
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

void TextEditor::delete_chars(size_t length) {
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
	set_cursor_zero();
}

void TextEditor::copy(size_t length) {
	if (OpenClipboard(0))
	{
		EmptyClipboard();
		// TODO: some problem
		char* hBuff = (char*)GlobalAlloc(GMEM_FIXED,  length + 1);
		if (hBuff == nullptr) {
			return;
		}

		char* dest = (char*)GlobalLock(hBuff);
		if (dest == nullptr) {
			GlobalFree(hBuff);
			return;
		}
		strncpy_s(dest, 2048, buffer[point.line] + point.index, length + 1);
		SetClipboardData(CF_TEXT, hBuff);
		CloseClipboard();
	}
}

int TextEditor::paste() {
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

	insert(text);

	GlobalUnlock(hData);
	CloseClipboard();
	return 0;
}


void TextEditor::undo() {
	/*if (curr_buffer > 0) {
		curr_buffer--;
		set_buffer_from_history();
	}*/

	if (head <= 0)
		return;

	head--;
	CommitChars* commit = commits[head];
	if (commit->text == NULL) {

	}
	else {

		set_cursor(commit->vector.point);
		delete_chars(commit->vector.length);
		insert(commit->text);
	}
}

void TextEditor::redo() {
	if (curr_buffer >= history_size - 1)
		return;
	if (history[curr_buffer + 1].lines == -1)
		return;
	curr_buffer++;
	set_buffer_from_history();

}

//void TextEditor::delete_buffer(struct HistoryItem history_item) {
//	for (size_t i = 0; i < history_item.lines; i++) {
//		delete[] history_item.buffer[i];
//	}
//	delete[] history_item.buffer;
//}

void TextEditor::add_commit(Commit* new_commit) {
	//CommitChars* new_commit = new CommitChars(text, vector);

	if (new_commit == nullptr) {
		std::cout << "[error] A fault of a commit creating" << std::endl;
		exit(1);
	}

	if (head < history_size - 1) {
		head++;
	}
	else {
		delete commits[0];
		for (size_t i = 0; i < history_size - 1; i++) {
			commits[i] = commits[i + 1];
		}
		commits[history_size] = nullptr;
	}

	for (size_t i = head; i < history_size; i++) {
		if (commits[head] == nullptr)
			break;
		delete commits[i];
	}

	commits[head] = new_commit;
}

//void TextEditor::add_commit() {
//	struct HistoryItem history_item;
//	history_item.lines = lines;
//	history_item.buffer = new char* [lines];
//	for (size_t i = 0; i < lines; i++) {
//		history_item.buffer[i] = new char[strlen(buffer[i]) + 1];
//		strcpy_s(history_item.buffer[i], strlen(buffer[i]) + 1, buffer[i]);
//	}
//	if (curr_buffer == history_size - 1) {
//		delete_buffer(history[0]);
//		history[0].lines = -1;
//		for (size_t i = 0; i < history_size - 1; i++) {
//			history[i] = history[i + 1];
//		}
//		history[history_size - 1] = history_item;
//
//	}
//	else if (history[curr_buffer + 1].lines != -1) {
//		for (size_t i = curr_buffer + 1; i < history_size; i++) {
//			if (history[i].lines == -1)
//				break;
//			delete_buffer(history[i]);
//			history[i].lines = -1;
//		}
//		curr_buffer++;
//		history[curr_buffer] = history_item;
//	}
//	else {
//		history[++curr_buffer] = history_item;
//	}
//	set_buffer_from_history();
//}

//void TextEditor::set_buffer_from_history() {
//	buffer = history[curr_buffer].buffer;
//	lines = history[curr_buffer].lines;
//}

void TextEditor::set_cursor(Point point) {
	this->point = point;
}

void TextEditor::set_cursor_zero() {
	point.index = 0;
	point.line = 0;
}