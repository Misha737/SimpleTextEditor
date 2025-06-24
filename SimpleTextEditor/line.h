#pragma once

#include "text.h"
#include "point.h"

class Line {
public:
	static const size_t elements = 0;
	virtual Text toText() = 0;
	//void append(Text append, size_t index);
	//void append(Text append);
	//void deleteChars(Vector vector);
};

class ContactLine : public Line {
public:
	static const size_t elements = 3;
	Text name;
	Text surname;
	Text email;

	ContactLine();

	Text toText() override;
};

class CheckListLine : public Line {
public:
	static const size_t elements = 2;
	Text info;
	bool status;

	CheckListLine();

	Text toText() override;
};

class PureLine : public Line {
public:
	static const size_t elements = 1;
	Text text;

	PureLine();

	Text toText() override;
};