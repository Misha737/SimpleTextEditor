#pragma once
#include "array.h"

class Commit {
public:
	virtual ~Commit() {}
};

class CommitChars : public Commit {
public:

	char* text = NULL;
	Vector vector;

	CommitChars(const char* text, Vector vector);
	~CommitChars() override;
};

class CommitLine : public Commit {
public:
	~CommitLine() override;
};