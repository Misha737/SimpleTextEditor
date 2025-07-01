#pragma once
#include "text_editor.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>

TextEditor::TextEditor() {
	head = 0;
	buffer = new Array<Line*>();
	newLine(new PureLine());
	point.line = 0;
	point.index = 0;
	for (size_t i = 0; i < history_size; i++)
		commits[i] = nullptr;
	commits[0] = new CommitChars("", 0, "", 0, point, 0);
}

TextEditor::~TextEditor() {
	for (size_t i = 0; i < history_size; i++) {
		if (commits[i] == nullptr)
			break;
		delete commits[i];
	}
	delete buffer;
}

void TextEditor::insert(Text& text, const char append[])
{
}

void TextEditor::append(Text& text, const char append[])
{
}

void TextEditor::deleteChars(Text&, size_t length)
{
}

void TextEditor::cleanBuffer() {
	buffer->clean();
	newLine(new PureLine);
	setCursorZero();
}

void TextEditor::newLine(Line* line)
{
	buffer->add(line);
}

void TextEditor::deleteLine() {
	if (point.line == buffer->length() - 1)
		point.line--;
	buffer->pop();
}

void TextEditor::printBuffer() {
	printf("\n=========\n");
	for (int i = 0; i < buffer->length(); i++) {
		Text text = (*buffer)[i]->toText();
		std::cout << (*buffer)[i]->toText() << std::endl;
	}
	printf("=========\n");
}

void TextEditor::fwriteBuffer(const char* file_name) {
	fwriteBuffer(file_name, this->buffer);
}

void TextEditor::fwriteBuffer(const char* file_name, Array<Line*>* buffer)
{
	std::ofstream file(file_name);
	if (!file) {
		throw std::exception("Couldn't open file");
	}

	for (size_t i = 0; i < buffer->length(); i++) {
		Line* line = (*buffer)[i];
		Text* serialized = line->serialize();
		file.write(line->getLineType().getArray(), 1);
		file.write(serialized->getArray(), serialized->length());
		file.write("\n", 1);
		delete serialized;
	}

	file.close();
}

void TextEditor::freadBuffer(const char* file_name) {
	freadBuffer(file_name, this->buffer);
}

void TextEditor::freadBuffer(const char* file_name, Array<Line*>* buffer)
{
	std::ifstream file(file_name);
	if (!file) {
		throw std::exception("Couldn't open file");
	}

	buffer->clean();

	std::string line;
	while (std::getline(file, line)) {
		Text deserialize;
		deserialize.insertRange(&line.c_str()[1], line.length() - 1, 0);
		switch (line[0]) {
		case '0':
			buffer->add(new ContactLine(deserialize));
			break;
		case '1':
			buffer->add(new CheckListLine(deserialize));
			break;
		case '2':
			buffer->add(new PureLine(deserialize));
			break;
		default:
			continue;
		}
		//buffer->last()->decrypt(key);
	}

	file.close();

}

Point TextEditor::searchBuffer(const char* str, size_t start_line, size_t start_index) {
	//Point point;
	//size_t str_length = strlen(str);
	//if (str_length == 0) {
	//	point.line = -1;
	//	point.index = 1;
	//	return point;
	//}
	//for (size_t i_line = start_line; i_line < buffer->getLines(); i_line++) {
	//	size_t line_length = strlen((*buffer)[i_line]);

	//	if (line_length < str_length)
	//		continue;
	//	for (size_t i_index = start_index; i_index < line_length - str_length + 1; i_index++) {
	//		point.index = i_index;
	//		char* curr_line = (*buffer)[i_line];
	//		for (int i_str = 0; i_str < str_length; i_str++) {
	//			if (curr_line[i_index + i_str] != str[i_str]) {
	//				break;
	//			}
	//			if (i_str == str_length - 1) {
	//				point.line = i_line;
	//				point.index = i_index;
	//				return point;
	//			}
	//		}
	//	}
	//	start_index = 0;
	//}
	//point.line = -1;
	//point.index = 0;
	return point;
}

void TextEditor::copy(size_t length, Text* line) {
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

	strncpy_s(dest, length + 1, line[point.index].getArray(), length);
	dest[length] = '\0';

	GlobalUnlock(hMem);

	SetClipboardData(CF_TEXT, hMem);

	CloseClipboard();
}

int TextEditor::paste(Text* line) {
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

	//append(text);

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

		Line* line = (*buffer)[commit_chars->point.line];
		Text& text = *line->getTextFromField(commit_chars->form);

		deleteChars(text, strlen(commit_chars->after));
		append(text, commit_chars->before);
	}
	else if (CommitLine* commit_line = dynamic_cast<CommitLine*>(commit)) {
		deleteLine();
	}
	else if (CommitBuffer* commit_buffer = dynamic_cast<CommitBuffer*>(commit)) {
		buffer->clean();
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

		Line* line = (*buffer)[commit_chars->point.line];
		Text& text = *line->getTextFromField(commit_chars->form);

		deleteChars(text, strlen(commit_chars->before));
		append(text, commit_chars->after);
	}
	else if (CommitLine* commit_line = dynamic_cast<CommitLine*>(commit)) {
		newLine(commit_line->line_type->copy());
	}
	else if (CommitBuffer* commit_buffer = dynamic_cast<CommitBuffer*>(commit)) {
		buffer->clean();
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
	if (buffer->length() <= point.line) {
		throw std::out_of_range("Line position is longer then number of lines");
	}
	this->point = point;
}

void TextEditor::setCursorZero() {
	point.index = 0;
	point.line = 0;
}

size_t TextEditor::getHead() const
{
	return head;
}

void TextEditor::encryptBuffer(int key)
{
	for (size_t i = 0; i < buffer->length(); i++) {
		Line* line = (*buffer)[i];
		line->encrypt(key);
	}
}

void TextEditor::decryptBuffer(int key)
{
	for (size_t i = 0; i < buffer->length(); i++) {
		Line* line = (*buffer)[i];
		line->decrypt(key);
	}
}

void TextEditor::saveEncrypted(int key, const char* path)
{
	encryptBuffer(key);
	fwriteBuffer(path);
	decryptBuffer(key);
}

void TextEditor::loadEncrypted(int key, const char* path)
{
	freadBuffer(path);
	decryptBuffer(key);
}

void TextEditor::encryptFile(
	int key,
	const char* input_path,
	const char* output_path,
	std::function<void(int)> algorithm)
{
	Array<Line*>* temp_buffer = new Array<Line*>();
	Array<Line*>* support_buffer = buffer;
	buffer = temp_buffer;
	freadBuffer(input_path, temp_buffer);
	algorithm(key);
	fwriteBuffer(output_path, temp_buffer);
	delete temp_buffer;
	buffer = support_buffer;
}