#pragma once
#include "point.h"
#include "line.h"

class Commit {
public:
	virtual ~Commit() {}
};

class CommitChars : public Commit {
private:
	void setData(const char* after, size_t after_len, const char* before, size_t before_len, size_t form);
public:
	char* after = nullptr;
	char* before = nullptr;
	Point point;
	int form;

	CommitChars(const char* after, size_t after_len, const char* before, size_t before_len, Point point, size_t form);
	~CommitChars() override;
	void setCursor(Point point);
};

class CommitLine : public Commit {
public:
	Line* line_type;
	CommitLine(const Line* new_line);
	~CommitLine() override;
};

class CommitBuffer : public Commit {
public:
	Array<Line*>* after;
	Array<Line*>* before;
	CommitBuffer(const Array<Line*>* after, const Array<Line*>* before);
	~CommitBuffer() override;
};