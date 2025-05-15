#pragma once
#include <stdlib.h>

//#define line buffer[lines - 1]

extern char** buffer;
extern size_t lines;

void insert(size_t line_index, const char append[], size_t insert_pos);

void new_line();

void init_buffer();

void print_buffer();