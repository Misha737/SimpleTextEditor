#pragma once
#include "text_editor.h"
#include <iostream>
#include <string.h>
#include <windows.h>

TextEditor::TextEditor() {
	head = 0;
	buffer = new Buffer();
	point.line = 0;
	point.index = 0;
	for (size_t i = 0; i < history_size; i++)
		commits[i] = nullptr;
	commits[0] = new CommitChars("", 0, "", 0, point);
}

TextEditor::~TextEditor() {
	for (size_t i = 0; i < history_size; i++) {
		if (commits[i] == nullptr)
			break;
		delete commits[i];
	}
	delete buffer;
}

void TextEditor::insert(const char append[]) {
	if (buffer->getLines() <= point.line) {
		throw std::out_of_range("Line position is longer then number of lines");
	}
	buffer->insert(append, point);
}

void TextEditor::append(const char append[]) {
	//Point temp_point = point;
	//point.index = strlen((*buffer)[point.line]);
	insert(append);
	//point = temp_point;
}

void TextEditor::cleanBuffer() {
	buffer->cleanBuffer();
	setCursorZero();
}

void TextEditor::newLine()
{
	buffer->newLine();
}

void TextEditor::deleteLine() {
	if (point.line == buffer->getLines() - 1)
		point.line--;
	buffer->deleteLine();
}

void TextEditor::printBuffer() {
	printf("\n=========\n");
	for (int i = 0; i < buffer->getLines(); i++) {
		std::cout << (*buffer)[i] << std::endl;
	}
	printf("=========\n");
}

int TextEditor::fwriteBuffer(const char* file_name) {
	FILE* file = NULL;
	int error;
	if ((error = fopen_s(&file, file_name, "w")) != 0) {
		printf("Couldn't create file for writing. Code: %d", error);
		return 1;
	}
	for (int i = 0; i < buffer->getLines() - 1; i++) {
		fprintf(file, "%s\n", (*buffer)[i]);
	}
	fprintf(file, "%s", (*buffer)[buffer->getLines() - 1]);
	fclose(file);
	return 0;
}

int TextEditor::freadBuffer(const char* file_name) {
	FILE* file = NULL;
	int error;
	if ((error = fopen_s(&file, file_name, "r")) != 0) {
		printf("Couldn't create file for writing. Code: %d", error);
		return 1;
	}
	cleanBuffer();
	newLine();
	char line[256];
	while (fgets(line, sizeof(line), file)) {
		size_t end = strcspn(line, "\n");
		int is_new_line = line[end] == '\n';
		line[end] = '\0';
		append(line);
		if (end < sizeof(line) - 1 && is_new_line) {
			newLine();
			point.line++;
		}
	}

	fclose(file);
	return 0;
}

Point TextEditor::searchBuffer(const char* str, size_t start_line, size_t start_index) {
	Point point;
	size_t str_length = strlen(str);
	if (str_length == 0) {
		point.line = -1;
		point.index = 1;
		return point;
	}
	for (size_t i_line = start_line; i_line < buffer->getLines(); i_line++) {
		size_t line_length = strlen((*buffer)[i_line]);

		if (line_length < str_length)
			continue;
		for (size_t i_index = start_index; i_index < line_length - str_length + 1; i_index++) {
			point.index = i_index;
			char* curr_line = (*buffer)[i_line];
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

void TextEditor::deleteChars(size_t length) {
	Vector vector;
	vector.point = point;
	vector.length = length;
	buffer->deleteChars(vector);
}

void TextEditor::copy(size_t length) {
	if (!OpenClipboard(nullptr)) return;

	EmptyClipboard();

	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, length + 1);
	if (hMem == nullptr) {
		CloseClipboard();
		return;
	}

	char* dest = static_cast<char*>(GlobalLock(hMem));
	if (dest == nullptr) {
		GlobalFree(hMem);
		CloseClipboard();
		return;
	}

	strncpy_s(dest, length + 1, getCharByCursor(), length);
	dest[length] = '\0';

	GlobalUnlock(hMem);

	SetClipboardData(CF_TEXT, hMem);

	CloseClipboard();
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
	return (int)strlen(text);
}


void TextEditor::undo() {
	if (head <= 0)
		return;

	Commit* commit = commits[head];
	head--;

	if (CommitChars* commit_chars = dynamic_cast<CommitChars*>(commit)) {
		setCursor(commit_chars->point);
		deleteChars(strlen(commit_chars->after));
		insert(commit_chars->before);
	}
	else if (CommitLine* commit_line = dynamic_cast<CommitLine*>(commit)) {
		deleteLine();
	}
	else if (CommitBuffer* commit_buffer = dynamic_cast<CommitBuffer*>(commit)) {
		delete buffer;
		buffer = commit_buffer->before->copy();
		setCursorZero();
	}
}

void TextEditor::redo() {
	if (head >= history_size - 1)
		return;
	if (commits[head + 1] == nullptr) {
		std::cout << "\nEnd of the history" << std::endl;
		return;
	}

	head++;
	Commit* commit = commits[head];

	if (CommitChars* commit_chars = dynamic_cast<CommitChars*>(commit)) {
		setCursor(commit_chars->point);
		deleteChars(strlen(commit_chars->before));
		insert(commit_chars->after);
	}
	else if (CommitLine* commit_line = dynamic_cast<CommitLine*>(commit)) {
		newLine();
	}
	else if (CommitBuffer* commit_buffer = dynamic_cast<CommitBuffer*>(commit)) {
		delete buffer;
		buffer = commit_buffer->after->copy();
		setCursorZero();
	}
}

void TextEditor::addCommit(Commit* new_commit) {
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
		commits[history_size - 1] = nullptr;
	}

	for (size_t i = head; i < history_size; i++) {
		if (commits[i] == nullptr)
			break;
		delete commits[i];
		commits[i] = nullptr;
	}

	commits[head] = new_commit;
}

void TextEditor::setCursor(Point point) {
	this->point = point;
}

void TextEditor::setCursorZero() {
	point.index = 0;
	point.line = 0;
}

const char* TextEditor::getCharByCursor() const
{
	return getCharByLineCursor(point.index);
}

const char* TextEditor::getCharByLineCursorLast() const
{
	return getCharByLineCursor(strlen((*buffer)[point.line]));
}

const char* TextEditor::getCharByLineCursor(size_t index_of_line) const
{
	return &(*buffer)[point.line][index_of_line];
}

size_t TextEditor::getHead() const
{
	return head;
}

size_t TextEditor::getLineLength(size_t line_index) const
{
	if (line_index >= buffer->getLines())
		throw std::out_of_range("Line index is loner than number of lines");
	return strlen((*buffer)[line_index]);
}