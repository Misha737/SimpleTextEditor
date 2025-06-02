# pragma once

#include "array.h"

class Parser {
private:
	char console_line[256];
	Buffer* buffer;
public:
	Parser(Buffer* buffer);

	int read_console_all();

	void read_console();

	int read_integers(int* integers, size_t number_of_integers);

	void run();
};