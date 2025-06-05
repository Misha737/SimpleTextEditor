# pragma once

#include "text_editor.h"
#include <functional>
#include "point.h"
#include "buffer.h"

class Parser {
private:
	char console_line[256];
	size_t console_length = 0;
	TextEditor* text_editor;
public:
	Parser(TextEditor* text_editor);

	int readConsoleAll();

	void readConsole();

	int readIntegers(int* integers, size_t number_of_integers);

	size_t parseLength();

	Point parsePoint();

	void run();
};