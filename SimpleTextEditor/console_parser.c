#pragma once

#include "console_parser.h"

#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include "array.h"

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
	return 0;
}

void parser() {
	printf("\nChoose the command: ");
	char input = _getche();
	switch (input)
	{
	case '1':
		printf("\nEnter text to append: ");
		int status;
		do
		{
			status = read_console_all();
			append(console_line);
		} while (status == 1);
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
		printf("\nChoose line and index to insert: ");
		while (1) {
			int integers[2];
			int errors = read_integers(integers, sizeof(integers) / sizeof(int));
			if (errors == 1) {
				printf("\nEnter two integers, for example \"1 2\"\n");
				continue;
			}
			if (integers[0] > lines || integers[0] < 1) {
				printf("\nEnter an index of line in range from 1 to %d\n", (int)lines);
				continue;
			}
			size_t index_of_line = integers[0] - 1;
			if (integers[1] - 1 > strlen(buffer[index_of_line]) || integers[1] < 1) {
				printf("\nEnter an index of char in range from 1 to %d\n", (int)strlen(buffer[index_of_line]));
				continue;
			}
			printf("Enter a text to insert: ");
			int status_reading;
			do
			{
				status_reading = read_console_all();
				insert(integers[0] - 1, console_line, integers[1] - 1);
			} while (status_reading == 1);
			break;
		}
		break;
	case '7':
		printf("\nEnter text to search: ");
		read_console();
		Point point = search_buffer(console_line);
		if (point.line == -1) {
			if (point.index == 0)
			{
				printf("Nothing found\n");
			}
			else if (point.index == 1) {
				printf("Enter some text to search\n");
			}
			break;
		}
		printf("Text is present in this position: %d %d\n", point.line + 1, point.index + 1);
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