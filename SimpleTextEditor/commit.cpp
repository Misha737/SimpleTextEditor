#include "commit.h"
#include <string.h>

void CommitChars::setData(const char* after, size_t after_len, const char* before, size_t before_len, size_t form)
{
	this->after = new char[after_len + 1];
	this->after[after_len] = '\0';
	strncpy_s(this->after, after_len + 1, after, after_len);

	this->before = new char[before_len + 1];
	this->before[before_len] = '\0';
	strncpy_s(this->before, before_len + 1, before, before_len);

	this->form = form;
}

CommitChars::CommitChars(const char* after, size_t after_len, const char* before, size_t before_len, Point point, size_t form) {
	setCursor(point);
	setData(after, after_len, before, before_len, form);
}

void CommitChars::setCursor(Point point) {
	this->point = point;
}

CommitChars::~CommitChars() {
	if (after != nullptr)
		delete[] after;
	if (before != nullptr)
		delete[] before;
}

CommitLine::CommitLine(const Line* new_line)
{
	this->line_type = new_line->copy();
}

CommitLine::~CommitLine() {
}

CommitBuffer::CommitBuffer(const Array<Line*>* after, const Array<Line*>* before) {
	this->after = after->copy();
	this->before = before->copy();
}

CommitBuffer::~CommitBuffer() {
	delete after;
	delete before;
}
