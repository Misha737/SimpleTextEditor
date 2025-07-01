# pragma once

#include "text_editor.h"
#include <functional>
#include "point.h"
#include "ui_tools.h"
#include "ui_line.h"

class UI {
private:
	UITools tools;
	TextEditor* text_editor;
	UILine* ui_line;
public:
	UI(TextEditor* text_editor);
	~UI();

	void setUILine();
	void run();
};