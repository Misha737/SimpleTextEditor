#include "line.h"
#include "caesar_cipher.h"

ContactLine::ContactLine() :
	name(Text()),
	surname(Text()),
	email(Text())
{
}

ContactLine::ContactLine(const Text& text) :
	name(Text()),
	surname(Text()),
	email(Text())
{
	size_t start = deserializeField(0, name, &text);
	start = deserializeField(start, surname, &text);
	deserializeField(start, email, &text);
}

Text ContactLine::toText()
{
	Text text("Contact - ");
	text += name;
	text += " ";
	text += surname;
	text += ", E-mail: ";
	text += email;
	return text;
}

Text* ContactLine::serialize()
{
	Text* text = new Text("");

	appendSerializedField(text, name);
	appendSerializedField(text, surname);
	appendSerializedField(text, email);

	return text;
}

void ContactLine::encrypt(int key)
{
	CaesarCipher cipher(key);
	cipher.encrypt(name);
	cipher.encrypt(surname);
	cipher.encrypt(email);
}

void ContactLine::decrypt(int key)
{
	CaesarCipher cipher(key);
	cipher.decrypt(name);
	cipher.decrypt(surname);
	cipher.decrypt(email);

}

Line* ContactLine::copy() const
{
	ContactLine* line = new ContactLine();
	line->name = Text(this->name);
	line->surname = Text(this->surname);
	line->email = Text(this->email);
	return line;
}

Text* ContactLine::getTextFromField(size_t form)
{
	switch (form)
	{

	case 1:
		return &this->name;
	case 2:
		return &this->surname;
		break;
	case 3:
		return &this->email;
		break;
	default:
		return nullptr;
	}
}

Text ContactLine::getLineType()
{
	return Text("0");
}

CheckListLine::CheckListLine() :
	info(Text()),
	status(false)
{
}

CheckListLine::CheckListLine(const Text& text) : info()
{
	this->status = (bool)text[0];
	deserializeField(1, this->info, &text);
}

Text CheckListLine::toText()
{
	Text text = Text("[ ");
	if (status)
		text.add('+');
	text += " ] ";
	text += this->info;
	return text;
}

Text* CheckListLine::serialize()
{
	Text* text = new Text();
	text->add((char)status);
	appendSerializedField(text, info);
	return text;
}

void CheckListLine::encrypt(int key)
{
	CaesarCipher cipher(key);
	cipher.encrypt(info);
}

void CheckListLine::decrypt(int key)
{
	CaesarCipher cipher(key);
	cipher.decrypt(info);

}

Line* CheckListLine::copy() const
{
	CheckListLine* line = new CheckListLine();
	line->info = Text(this->info);
	line->status = this->status;
	return line;
}

Text* CheckListLine::getTextFromField(size_t form)
{
	return &this->info;
}

Text CheckListLine::getLineType()
{
	return Text("1");
}

PureLine::PureLine() : text(Text())
{
}

PureLine::PureLine(const Text& text) : text(text)
{
}

Text PureLine::toText()
{
	return Text(text);
}

Text* PureLine::serialize()
{
	Text* text = new Text(this->text);
	return text;
}

void PureLine::encrypt(int key)
{
	CaesarCipher cipher(key);
	cipher.encrypt(text);
}

void PureLine::decrypt(int key)
{
	CaesarCipher cipher(key);
	cipher.decrypt(text);
}

Line* PureLine::copy() const
{
	PureLine* line = new PureLine();
	line->text = Text(this->text);
	return line;
}

Text* PureLine::getTextFromField(size_t form)
{
	return &this->text;
}

Text PureLine::getLineType()
{
	return Text("2");
}

void Line::appendSerializedField(Text* dest, const Text& src)
{
	char byte_number[sizeof(size_t)]{};
	size_t src_len = src.length();
	std::memcpy(byte_number, &src_len, sizeof(size_t));
	dest->insertRange(byte_number, sizeof(size_t), dest->length());
	*dest += src;
}

size_t Line::deserializeField(size_t start, Text& dest, const Text* src)
{
	try {
		size_t len;
		const char* src_array = src->getArray();
		std::memcpy(&len, src_array + start, sizeof(size_t));
		dest.insertRange(src_array + start + sizeof(size_t), len, 0);
		return start + sizeof(size_t) + len;
	}
	catch (std::exception ex) {
		throw std::exception("Deserialization error");
	}
}