#pragma once

#include "console_parser.h"

#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <iostream>

Parser::Parser(Buffer* buffer) {
	this->buffer = buffer;
}

int Parser::read_console_all() {
	memset(console_line, '\0', sizeof(console_line));
	fgets(console_line, sizeof(console_line), stdin);
	size_t end = strcspn(console_line, "\n");
	console_length += end;
	console_line[end] = '\0';
	if (end >= sizeof(console_line) - 1) {
		return 1;
	}
	return 0;
}

void Parser::read_console() {
	int end = read_console_all();
	if (end == 1) {
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF);
	}
}

int Parser::read_integers(int* integers, size_t number_of_integers) {
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

size_t Parser::parse_length() {
	printf("\nChoose number of symbols: ");
	while (true) {
		Vector vector;
		int inputs[1];
		int errors = read_integers(inputs, 1);
		if (errors == 1) {
			printf("\nEnter 1 integers, for example \"3\"\n");
			continue;
		}

		if (strlen(buffer->buffer[buffer->point.line]) == 0) {
			printf("\nNothing do\n");
			return 0;
		}
		size_t right_len = strlen(buffer->buffer[buffer->point.line]) - buffer->point.index - 1;
		if (inputs[0] > right_len)
			inputs[0] = right_len + 1;

		return inputs[0];
		break;
	}
}

Point Parser::parse_point() {
	printf("\nChoose line and index: ");
	while (1) {
		Point point;
		int integers[2];
		int errors = read_integers(integers, sizeof(integers) / sizeof(int));
		if (errors == 1) {
			printf("\nEnter two integers, for example \"1 2\"\n");
			continue;
		}
		if (integers[0] > buffer->lines || integers[0] < 1) {
			printf("\nEnter an index of line in range from 1 to %d\n", (int)(buffer->lines));
			continue;
		}
		size_t index_of_line = integers[0] - 1;
		if (integers[1] > strlen(buffer->buffer[index_of_line]) + 1 || integers[1] < 1) {
			printf("\nEnter an index of char in range from 1 to %d\n", (int)strlen(buffer->buffer[index_of_line]) + 1);
			continue;
		}
		point.line = integers[0] - 1;
		point.index = integers[1] - 1;
		return point;
	}
}

void Parser::run() {
	printf("\nChoose the command: ");
	char input = _getche();
	printf("\033[2J\033[H");
	//system("cls");
	printf("\nChoose the command: %c", input);
	switch (input)
	{
	case '1':
	{
		printf("\nEnter text to append: ");
		buffer->update_history();
		int status;
		do
		{
			status = read_console_all();
			buffer->append(console_line);
		} while (status == 1);
	}
	break;
	case '2':
		printf("\nNew line is started\n");
		buffer->update_history();
		buffer->new_line();
		break;
	case '3':
		printf("\nEnter the file name for saving: ");
		read_console();
		if (buffer->fwrite_buffer(console_line) == 0)
			printf("\nText has been saved successfully\n");
		break;
	case '4':
		printf("\nEnter the file name for loading: ");
		buffer->update_history();
		read_console();
		if (buffer->fread_buffer(console_line) == 0) {
			printf("\nText has been loaded successfully\n");
		}
		break;
	case '5':
		buffer->print_buffer();
		break;
	case '6':
	{
		buffer->update_history();
		printf("\nEnter a text to insert: ");
		int status_reading;
		do
		{
			status_reading = read_console_all();
			buffer->insert(console_line);
		} while (status_reading == 1);
		break;
	}
	case '7':
	{
		printf("\nEnter text to search: ");
		size_t found_counter = 0;
		read_console();
		Point point = buffer->search_buffer(console_line, 0, 0);
		if (point.line == -1 && point.index == 1) {
			printf("Enter some text to search\n");
			break;
		}

		if (point.line == -1 && point.index == 0)
			printf("Nothing found\n");
		else
			printf("Text is present in this position:\n");

		while (!(point.line == -1 && point.index == 0)) {
			found_counter++;
			printf("%d) %d %d\n", (int)found_counter, (int)point.line + 1, (int)point.index + 1);
			point = buffer->search_buffer(console_line, point.line, point.index + 1);
		}
	}
	break;
	case '8':
	{
		buffer->update_history();
		size_t length = parse_length();
		if (length != 0)
			buffer->delete_chars(length);
	}
	break;
	case 'r':
	{
		buffer->update_history();

			printf("\nEnter a text to insert: ");
			int status_reading;
			console_length = 0;
			do
			{
				status_reading = read_console_all();
				buffer->insert(console_line);
			} while (status_reading == 1);
			buffer->point.index += console_length;
			buffer->delete_chars(console_length);
			break;
	}
	break;
	case 'd':
		buffer->update_history();
		buffer->clear_buffer();
		buffer->new_line();
		std::cout << "\nA buffer has been cleared\n";
		break;
	case 'c':
	{
		size_t length = parse_length();
		if (length != 0)
			buffer->copy(length);

		break;
	}
	case 'x':
	{
		std::cout << "\nChoose number of symbols: ";
		buffer->update_history();

		size_t length = parse_length();
		if (length != 0) {
			buffer->copy(length);
			buffer->delete_chars(length);
		}

		break;
	}
	case 'p':
	{
		buffer->update_history();
		if (buffer->paste() == -1) {
			std::cout << "\nText pasting failed\n";
		}
		break;
	}
	case 'u':
		std::cout << "\nUndo\n";
		buffer->undo();
		break;
	case 'e':
		std::cout << "\nRedo\n";
		buffer->redo();
		break;
	case 's':
		buffer->set_cursor(parse_point());
		break;
	default:
		std::cout << "\nThe command is not implemented :(\n";
		break;
	}
}