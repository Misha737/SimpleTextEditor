#pragma once
#include "point.h"

class Buffer {
private:
	const size_t SIZE_SEGM = 64;
	size_t segments = 0;
	char** content;
	size_t lines;
public:
	Buffer();
	~Buffer();
	void initBuffer();
	void newLine();
	void deleteLine();
	size_t getLines() const;
	void cleanBuffer();
	Buffer* copy() const;
	char* operator[](size_t index);
	const char* operator[](size_t index) const;
	void insert(const char append[], Point point);
	void deleteChars(Vector vector);
};