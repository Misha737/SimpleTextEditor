#include "buffer.h"
#include <iostream>

Buffer::Buffer()
{
	initBuffer();
}

Buffer::~Buffer()
{
	for (size_t i = 0; i < lines; i++) {
		delete[] content[i];
	}
	delete[] content;
}

void Buffer::initBuffer() {
	lines = 0;
	content = (char**)malloc(SIZE_SEGM * sizeof(char*));
	if (content == NULL) {
		printf("Couldn't create a buffer");
		exit(1);
	}
	segments++;
	newLine();
}

void Buffer::newLine() {
	content[lines] = (char*)malloc(1);
	if (content[lines] == NULL)
		exit(1);
	content[lines][0] = '\0';
	lines++;
	if (lines + 1 >= segments * SIZE_SEGM) {
		segments++;
		char** temp = (char**)realloc(content, segments * SIZE_SEGM * sizeof(char*));
		if (temp == NULL) {
			printf("Error reallocation of memory\0");
			exit(1);
		}
		content = temp;
	}
}

void Buffer::deleteLine() {
	delete[] content[lines - 1];
	//delete text_editor[lines - 1];
	content[lines - 1] = nullptr;
	lines--;
}

size_t Buffer::getLines() const
{
	return lines;
}

void Buffer::cleanBuffer()
{
	for (int i = 0; i < lines; i++) {
		free(content[i]);
	}
	lines = 0;
	segments = 0;
}

Buffer* Buffer::copy() const
{
	char** new_content = new char* [lines];
	if (new_content == nullptr) {
		std::cout << "[Error] Failed to create a copy of a buffer" << std::endl;
		exit(1);
	}

	for (size_t i = 0; i < lines; i++) {
		size_t line_len = strlen(content[i]) + 1;
		new_content[i] = new char[line_len];
		if (new_content[i] == nullptr) {
			std::cout << "[Error] Failed to create a copy of a line of a buffer" << std::endl;
			exit(1);
		}
		strncpy_s(new_content[i], line_len, content[i], line_len);
	}

	Buffer* new_buffer = new Buffer();
	new_buffer->content = new_content;
	new_buffer->lines = lines;
	return new_buffer;
}

char* Buffer::operator[](size_t index)
{
	if (index >= lines) {
		throw std::out_of_range("Index is longer than number of lines");
	}
	return content[index];
}

const char* Buffer::operator[](size_t index) const
{
	if (index >= lines) {
		throw std::out_of_range("Index is longer than number of lines");
	}
	return content[index];
}

void Buffer::insert(const char append[], Point point) {
	char* buffer_line = content[point.line];

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

	content[point.line] = (char*)realloc(buffer_line, full_size + 1);
	if (content[point.line] == NULL) {
		printf("Error reallocation of memory\0");
		exit(1);
	}
	buffer_line = content[point.line];
	buffer_line[full_size] = '\0';

	for (size_t i = full_size - 1; i >= append_size + point.index; i--) {
		buffer_line[i] = buffer_line[i - append_size];
	}

	memcpy_s(buffer_line + point.index, full_size, append, append_size);
}

void Buffer::deleteChars(Vector vector) {
	char* line = content[vector.point.line];

	for (size_t i = vector.point.index + vector.length; i <= strlen(line); i++) {
		line[i - vector.length] = line[i];
	}
	char* temp = (char*)realloc(line, strlen(line) + 1);
	if (temp == nullptr) {
		printf("Error reallocation of memory\0");
		exit(1);
	}
	content[vector.point.line] = temp;
}