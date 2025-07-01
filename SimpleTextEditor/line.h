#pragma once

#include "text.h"
#include "point.h"

class Line {
public:
	static const size_t elements = 0;
	virtual Text toText() = 0;
	virtual Line* copy() const = 0;
	virtual Text* getTextFromField(size_t form) = 0;
	virtual Text* serialize() = 0;
	virtual void encrypt(int key) = 0;
	virtual void decrypt(int key) = 0;
	virtual Text getLineType() = 0;
protected:
	void appendSerializedField(Text* dest, const Text& src);
	size_t deserializeField(size_t start, Text& dest, const Text* src);
};

class ContactLine : public Line {
public:
	static const size_t elements = 3;
	Text name;
	Text surname;
	Text email;

	ContactLine();
	ContactLine(const Text& text);

	Text toText() override;
	Text* serialize() override;
	void encrypt(int key) override;
	void decrypt(int key) override;
	Line* copy() const override;
	Text* getTextFromField(size_t form) override;
	Text getLineType() override;
};

class CheckListLine : public Line {
public:
	static const size_t elements = 2;
	Text info;
	bool status;

	CheckListLine();
	CheckListLine(const Text& text);

	Text toText() override;
	Text* serialize() override;
	void encrypt(int key) override;
	void decrypt(int key) override;
	Line* copy() const override;
	Text* getTextFromField(size_t form) override;
	Text getLineType() override;
};

class PureLine : public Line {
public:
	static const size_t elements = 1;
	Text text;

	PureLine();
	PureLine(const Text& text);

	Text toText() override;
	Text* serialize() override;
	void encrypt(int key) override;
	void decrypt(int key) override;
	Line* copy() const override;
	Text* getTextFromField(size_t form) override;
	Text getLineType() override;
};