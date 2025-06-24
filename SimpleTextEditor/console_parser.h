# pragma once

#include "text_editor.h"
#include <functional>
#include "point.h"
#include "ui_tools.h"
#include "ui_line.h"

class Parser {
private:
	UITools tools;
	TextEditor* text_editor;
	UILine* ui_line;
public:
	Parser(TextEditor* text_editor);
	~Parser();

	void setUILine();
	void run();
};