#pragma once
#include "point.h"
#include "line.h"

class Commit {
public:
	virtual ~Commit() {}
};

class CommitChars : public Commit {
private:
	void setData(const char* after, size_t after_len, const char* before, size_t before_len);
public:
	char* after = nullptr;
	char* before = nullptr;
	Point point;

	CommitChars(const char* after, size_t after_len, const char* before, size_t before_len, Point point);
	~CommitChars() override;
	void setCursor(Point point);
};

class CommitLine : public Commit {
public:
	~CommitLine() override;
};

class CommitBuffer : public Commit {
public:
	Array<Line*>* after;
	Array<Line*>* before;
	CommitBuffer(const Array<Line*>* after, const Array<Line*>* before);
	~CommitBuffer() override;
};