#pragma once

#include "console_parser.h"

#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <iostream>

Parser::Parser(TextEditor* text_editor) {
	this->text_editor = text_editor;
	for (size_t i = 0; i < sizeof(console_line) / sizeof(char); i++)
		console_line[i] = '\n';
}

int Parser::readConsoleAll() {
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

void Parser::readConsole() {
	int end = readConsoleAll();
	if (end == 1) {
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF);
	}
}

int Parser::readIntegers(int* integers, size_t number_of_integers) {
	readConsole();
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

size_t Parser::parseLength() {
	printf("\nChoose number of symbols: ");
	while (true) {
		int inputs[1];
		int errors = readIntegers(inputs, 1);
		if (errors == 1) {
			printf("\nEnter 1 integers, for example \"3\"\n");
			continue;
		}
		size_t line_len = text_editor->getLineLength(text_editor->point.line);
		if (line_len == 0) {
			printf("\nNothing do\n");
			return 0;
		}
		size_t right_len = line_len - text_editor->point.index - 1;
		if (inputs[0] > right_len)
			return right_len + 1;

		return inputs[0];
		break;
	}
}

Point Parser::parsePoint() {
	printf("\nChoose line and index: ");
	while (1) {
		Point point;
		int integers[2];
		int errors = readIntegers(integers, sizeof(integers) / sizeof(int));
		if (errors == 1) {
			printf("\nEnter two integers, for example \"1 2\"\n");
			continue;
		}
		point.line = integers[0] - 1;
		point.index = integers[1] - 1;
		if (point.line >= text_editor->buffer->getLines() || point.line < 0) {
			printf("\nEnter an index of line in range from 1 to %d\n", (int)(text_editor->buffer->getLines()));
			continue;
		}
		if (point.index > text_editor->getLineLength(point.line) || point.index < 0) {
			printf("\nEnter an index of char in range from 1 to %d\n", (int)text_editor->getLineLength(point.line) + 1);
			continue;
		}
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
		Point temp_point = text_editor->point;
		text_editor->point.index = text_editor->getLineLength(temp_point.line);
		int status;
		console_length = 0;
		do
		{
			status = readConsoleAll();
			text_editor->append(console_line);
		} while (status == 1);
		const char* append = text_editor->getCharByLineCursor(text_editor->point.index);
		text_editor->addCommit(new CommitChars(append, console_length, "", 0, text_editor->point));
		text_editor->point = temp_point;
	}
	break;
	case '2':
		printf("\nNew line is started\n");
		text_editor->newLine();
		text_editor->addCommit(new CommitLine());
		break;
	case '3':
		printf("\nEnter the file name for saving: ");
		readConsole();
		if (text_editor->fwriteBuffer(console_line) == 0)
			printf("\nText has been saved successfully\n");
		break;
	case '4':
	{
		printf("\nEnter the file name for loading: ");
		readConsole();
		Buffer* old_buffer = text_editor->buffer->copy();
		if (text_editor->freadBuffer(console_line) == 0) {
			text_editor->addCommit(new CommitBuffer(text_editor->buffer, old_buffer));
			printf("\nText has been loaded successfully\n");
		}
		else {
			std::cout << "[Error] Couldn't load a file" << std::endl;
		}
		delete old_buffer;
		break;
	}
	case '5':
		text_editor->printBuffer();
		break;
	case '6':
	{
		printf("\nEnter a text to insert: ");
		int status_reading;
		console_length = 0;
		do
		{
			status_reading = readConsoleAll();
			text_editor->insert(console_line);
		} while (status_reading == 1);
		text_editor->addCommit(new CommitChars(text_editor->getCharByCursor(), console_length, "", 0, text_editor->point));
		break;
	}
	case '7':
	{
		printf("\nEnter text to search: ");
		size_t found_counter = 0;
		readConsole();
		Point point = text_editor->searchBuffer(console_line, 0, 0);
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
			point = text_editor->searchBuffer(console_line, point.line, point.index + 1);
		}
		break;
	}
	case '8':
	{
		size_t length = parseLength();
		if (length != 0) {
			text_editor->addCommit(new CommitChars("", 0, text_editor->getCharByCursor(), length, text_editor->point));
			text_editor->deleteChars(length);
		}
		break;
	}
	case 'r':
	{
		printf("\nEnter a text to replace: ");
		Point temp_point = text_editor->point;
		int status_reading;
		console_length = 0;
		do
		{
			status_reading = readConsoleAll();
			text_editor->insert(console_line);
		} while (status_reading == 1);
		const char* chars_after = text_editor->getCharByCursor();
		text_editor->point.index += console_length;
		const char* chars_before = text_editor->getCharByCursor();
		text_editor->addCommit(new CommitChars(chars_after, console_length, chars_before, console_length, temp_point));
		text_editor->deleteChars(console_length);
		text_editor->point = temp_point;
		break;
	}
	break;
	case 'd':
		text_editor->addCommit(new CommitBuffer(new Buffer(), text_editor->buffer));
		text_editor->cleanBuffer();
		text_editor->newLine();
		std::cout << "\nA buffer has been cleaned\n";
		break;
	case 'c':
	{
		size_t length = parseLength();
		if (length != 0) {
			text_editor->copy(length);
			std::cout << "A text has been copied" << std::endl;
		}

		break;
	}
	case 'x':
	{
		size_t length = parseLength();
		if (length != 0) {
			text_editor->addCommit(new CommitChars("", 0, text_editor->getCharByCursor(), length, text_editor->point));
			text_editor->copy(length);
			text_editor->deleteChars(length);
			std::cout << "A text has been cut" << std::endl;
		}

		break;
	}
	case 'p':
	{
		int length_pasted;
		if (length_pasted = text_editor->paste() == -1) {
			std::cout << "\nText pasting failed\n";
		}
		text_editor->addCommit(new CommitChars(text_editor->getCharByCursor(), length_pasted, "", 0, text_editor->point));
		std::cout << "\nA text has been pasted" << std::endl;

		break;
	}
	case 'u':
		if (text_editor->getHead() > 0) {
			text_editor->undo();
			std::cout << "\nUndo" << std::endl;
		}
		else {
			std::cout << "\nStart of history" << std::endl;
		}
		break;
	case 'e':
		if (text_editor->getHead() < history_size - 1) {
			text_editor->redo();
			std::cout << "\nRedo" << std::endl;
		}
		else {
			std::cout << "\nEnd of history" << std::endl;
		}
		break;
	case 's':
		text_editor->setCursor(parsePoint());
		break;
	case 'h':
		std::cout << "\nline: " << text_editor->point.line + 1 << std::endl;
		std::cout << "Index: " << text_editor->point.index + 1 << std::endl;
		break;
	default:
		std::cout << "\nThe command is not implemented :(\n";
		break;
	}
}