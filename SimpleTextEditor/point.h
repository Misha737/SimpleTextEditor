#pragma once

typedef struct {
	size_t line;
	size_t index;
} Point;

typedef struct {
	Point point;
	size_t length;
} Vector;