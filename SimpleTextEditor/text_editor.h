#pragma once
#include <iostream>
#include "commit.h"
#include "point.h"
#include "line.h"
#include <functional>

const size_t history_size = 256;

class TextEditor {
private:
	Commit* commits[history_size];
	size_t head;
public:
	Array<Line*>* buffer;
	Point point;

	TextEditor();
	~TextEditor();

	void insert(Text& text, const char append[]);
	void append(Text& text, const char append[]);
	void deleteChars(Text&, size_t length);

	void cleanBuffer();

	void newLine(Line* line);
	void deleteLine();

	void printBuffer();

	void fwriteBuffer(const char* file_name);
	void fwriteBuffer(const char* file_name, Array<Line*>* buffer);
	void freadBuffer(const char* file_name);
	void freadBuffer(const char* file_name, Array<Line*>* buffer);

	Point searchBuffer(const char* str, size_t start_line, size_t start_index);
	
	void copy(size_t length, Text* line);
	int paste(Text* line);

	void undo();
	void redo();
	void addCommit(Commit* new_commit);

	void setCursor(Point point);
	void setCursorZero();

	size_t getHead() const;

	void encryptBuffer(int key);
	void decryptBuffer(int key);
	void saveEncrypted(int key, const char* path);
	void loadEncrypted(int key, const char* path);
	void encryptFile(int key, const char* input_path, const char* output_path,
		std::function<void(int)> algorithm);
};