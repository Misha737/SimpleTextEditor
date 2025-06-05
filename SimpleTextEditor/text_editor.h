#pragma once
#include <iostream>
#include "commit.h"
#include "buffer.h"
#include "point.h"

const size_t history_size = 256;

class TextEditor {
private:
	Commit* commits[history_size];
	size_t head;
public:
	Buffer* buffer;
	Point point;

	TextEditor();
	~TextEditor();

	void insert(const char append[]);
	void append(const char append[]);
	void deleteChars(size_t length);

	void cleanBuffer();

	void newLine();
	void deleteLine();

	void printBuffer();

	int fwriteBuffer(const char* file_name);
	int freadBuffer(const char* file_name);

	Point searchBuffer(const char* str, size_t start_line, size_t start_index);
	
	void copy(size_t length);
	int paste();

	void undo();
	void redo();
	void addCommit(Commit* new_commit);

	void setCursor(Point point);
	void setCursorZero();

	const char* getCharByLineCursor(size_t index_of_line) const;
	const char* getCharByCursor() const;
	const char* getCharByLineCursorLast() const;

	size_t getHead() const;
	size_t getLineLength(size_t line_index) const;
};