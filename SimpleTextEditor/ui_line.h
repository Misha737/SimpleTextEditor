#pragma once
#include "line.h"
#include "ui_tools.h"


class UILine {
protected:
	UITools* tools;
	virtual void printFields() = 0;
public:
	UILine(UITools& new_ui_tools) : tools(&new_ui_tools) {}
	virtual Text* getTextFromField(size_t* out) = 0;
};

class UIPureLine : public UILine {
private:
	void printFields() override;
	PureLine* line;
public:
	UIPureLine(PureLine* new_pure_line, UITools& new_ui_tools) :
		UILine(new_ui_tools), line(new_pure_line) {};
	Text* getTextFromField(size_t* out) override;
};

class UIContactLine : public UILine {
private:
	void printFields() override;
	ContactLine* line;
public:
	UIContactLine(ContactLine* new_contact_line, UITools& new_ui_tools) :
		UILine(new_ui_tools), line(new_contact_line) {};
	Text* getTextFromField(size_t* out) override;
};

class UICheckListLine : public UILine {
private:
	void printFields() override;
	CheckListLine* line;
public:
	UICheckListLine(CheckListLine* new_line, UITools& new_ui_tools) :
		UILine(new_ui_tools), line(new_line) {
	};
	Text* getTextFromField(size_t* out) override;
};