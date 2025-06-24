#pragma once

#include "array.h"
#include <iostream>

class Text : public Array<char>{
public:
	Text();
	~Text();
	Text(const char* text);
	Text(const Array<char>& text);
	Text& operator+=(const Text& other);
	Text& operator+=(const char* other);
	Text* copy() const;
	friend std::ostream& operator<<(std::ostream& os, const Text& text);
};
