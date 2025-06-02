# pragma once

#include "array.h"
#include <functional>

class Parser {
private:
	char console_line[256];
	size_t console_length = 0;
	Buffer* buffer;
public:
	Parser(Buffer* buffer);

	int read_console_all();

	void read_console();

	int read_integers(int* integers, size_t number_of_integers);

	size_t parse_length();

	Point parse_point();

	void run();
};