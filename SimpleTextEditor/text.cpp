#include "text.h"

Text::Text() : Array<char>()
{
}

Text::Text(const char* text) : Array(text, strlen(text))
{
}

Text::Text(const Array<char>& text) : Array<char>(text)
{
}

Text& Text::operator+=(const Text& other)
{
	this->insertRange(other, this->length());
	return *this;
}

Text& Text::operator+=(const char* other)
{
	*this += Text(other);
	return *this;
}

Text::~Text()
{
}

Text* Text::copy() const
{
	return new Text(*copy());
}

std::ostream& operator<<(std::ostream& os, const Text& text) {
	for (size_t i = 0; i < text.length(); i++) {
		os << text[i];
	}
	return os;
}
