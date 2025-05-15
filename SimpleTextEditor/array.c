#pragma once
#include "./array.h"
#include <stdio.h>
#include <string.h>

void insert(char** buffer, const char append[], size_t insert_pos) {
	if (*buffer == NULL || append == NULL)
		exit(1);

	size_t src_size = strlen(*buffer);
	size_t append_size = strlen(append);
	size_t full_size = src_size + append_size;

	if (insert_pos >= src_size) {
		printf("Insert position is longer then src length");
		exit(1);
	}

	char* temp = (char*)realloc(*buffer, full_size + 1);
	if (temp == NULL) {
		printf("Error reallocation of memory\0");
		exit(1);
	}
	*buffer = temp;
	temp[full_size] = '\0';

	for (size_t i = full_size - 1; i >= append_size + insert_pos; i--) {
		temp[i] = temp[i - append_size];
	}

	memcpy_s(temp + insert_pos, full_size, append, append_size);
}
