#include "commit.h"
#include <string.h>

CommitChars::CommitChars(const char* text, Vector vector) {
	this->vector = vector;
	size_t text_len = strlen(text) + 1;
	this->text = new char[text_len];
	strncpy_s(this->text, text_len, text, text_len);
}

CommitChars::~CommitChars() {
	if (text != nullptr)
		delete[] text;
}

CommitLine::~CommitLine()
{
}
