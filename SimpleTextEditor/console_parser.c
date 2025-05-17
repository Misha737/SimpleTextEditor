#pragma once

#include "console_parser.h"

#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>

char console_line[256];

int read_console_all() {
	memset(console_line, '\0', sizeof(console_line));
	fgets(console_line, sizeof(console_line), stdin);
	size_t end = strcspn(console_line, "\n");
	console_line[end] = '\0';
	if (end >= sizeof(console_line) - 1) {
		return 1;
	}
	return 0;
}

void read_console() {
	int end = read_console_all();
	if (end == 1) {
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF);
	}
}

int read_integers(int* integers, size_t number_of_integers) {
	read_console();
	const char* start = console_line;
	char* end;

	for (int i = 0; i < number_of_integers; i++) {
		long number = strtol(start, &end, 10);

		if (errno == ERANGE || end == start)
			return 1;
		integers[i] = (int)number;

		start = end;
	}
}

void parser() {
	printf("\nChoose the command: ");
	char input = _getche();
	switch (input)
	{
	case '1':
		printf("\nEnter text to append: ");
		int status;
		while ((status = read_console_all()) == 1)
			append(console_line);
		append(console_line);
		break;
	case '2':
		printf("\nNew line is started");
		new_line();
		break;
	case '3':
		printf("\nEnter the file name for saving: ");
		read_console();
		if (fwrite_buffer(console_line) == 0)
			printf("\nText has been saved successfully\n");
		break;
	case '4':
		printf("\nEnter the file name for loading: ");
		read_console();
		if (fread_buffer(console_line) == 0) {
			printf("\nText has been loaded successfully\n");
		}
		break;
	case '5':
		print_buffer();
		break;
	case '6':
		printf("\nChoose line and index to insert: \n");
		int integers[2];
		int errors = read_integers(integers, sizeof(integers) / sizeof(int));
		if (errors == 1) {
			printf("\nEnter two integers\n");
		}
		break;
	case '7':
		printf("7");
		break;
	case '8':
		clear_buffer();
		new_line();
		printf("\nA buffer has been cleared\n");
		break;
	default:
		printf("\nThe command is not implemented :(\n");
		break;
	}
}