#pragma once
#include <iostream>
#include "commit.h"
#include "point.h"
#include "line.h"

const size_t history_size = 256;

class TextEditor {
private:
	Commit* commits[history_size];
	size_t head;
public:
	Array<Line*> buffer;
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

	int fwriteBuffer(const char* file_name);
	int freadBuffer(const char* file_name);

	Point searchBuffer(const char* str, size_t start_line, size_t start_index);
	
	void copy(size_t length, Text* line);
	int paste(Text* line);

	void undo();
	void redo();
	void addCommit(Commit* new_commit);

	void setCursor(Point point);
	void setCursorZero();

	size_t getHead() const;
};