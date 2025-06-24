#pragma once
#include "array.h"
#include "point.h"
#include "text.h"
#include "text_editor.h"

class UITools {
public:
	TextEditor* text_editor;
	char console_line[256];
	size_t console_length = 0;

	UITools(TextEditor* textEditor);

	int readConsoleAll();
	void readConsole();
	void readIntegers(Array<int>& integers, size_t number_of_integers);
	int readInteger();
	bool readBool();
	size_t parseLength(Text* line);
	Point parsePoint();

	void append_text(Text* text);
	void insert_text(Text* text);
};