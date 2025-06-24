#include "ui_tools.h"

UITools::UITools(TextEditor* textEditor) : text_editor(textEditor)
{
	for (size_t i = 0; i < sizeof(console_line) / sizeof(char); i++)
		console_line[i] = '\n';
}

int UITools::readConsoleAll() {
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

void UITools::readConsole() {
	int end = readConsoleAll();
	if (end == 1) {
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF);
	}
}

void UITools::readIntegers(Array<int>& integers, size_t number_of_integers) {
	readConsole();
	const char* start = console_line;
	char* end;

	for (int i = 0; i < number_of_integers; i++) {
		long number = strtol(start, &end, 10);

		if (errno == ERANGE || end == start)
			throw std::invalid_argument("Invalid value");
		integers.add((int)number);

		start = end;
	}
}

int UITools::readInteger()
{
	Array<int> integer;
	readIntegers(integer, 1);

	return integer.last();
}

bool UITools::readBool()
{
	Array<int> value;
	readIntegers(value, 1);
	if (value[0] == 1 || value[0] == 2)
		return value[0];
	else {
		throw std::invalid_argument("Enter 1 or 0");
	}
}

size_t UITools::parseLength(Text* line) {
	printf("\nChoose number of symbols: ");
	while (true) {
		Array<int> integers;
		readIntegers(integers, 1);
		size_t line_len = line->length();
		if (line_len == 0) {
			printf("\nNothing do\n");
			return 0;
		}
		size_t right_len = line_len - text_editor->point.index - 1;
		if (integers[0] > right_len)
			return right_len + 1;

		return integers[0];
	}
}

Point UITools::parsePoint() {
	printf("\nChoose line and index: ");
	while (1) {
		Point point;
		Array<int> integers;
		try
		{
			readIntegers(integers, 2);
		}
		catch (const std::invalid_argument&)
		{
			printf("\nEnter two integers, for example \"1 2\"\n");
			continue;
		}
		point.line = integers[0] - 1;
		point.index = integers[1] - 1;
		if (point.line >= text_editor->buffer.length() || point.line < 0) {
			printf("\nEnter an index of line in range from 1 to %d\n", (int)(text_editor->buffer.length()));
			continue;
		}
		/*if (point.index > text_editor->getLineLength(point.line) || point.index < 0) {
			printf("\nEnter an index of char in range from 1 to %d\n", (int)text_editor->getLineLength(point.line) + 1);
			continue;
		}*/
		return point;
	}
}

void UITools::append_text(Text* text) {
	Point temp_point = text_editor->point;
	text_editor->point.index = text->length();
	insert_text(text);
	text_editor->point = temp_point;
}

void UITools::insert_text(Text* text)
{
	console_length = 0;
	int status;
	do
	{
		status = readConsoleAll();
		Text console_text(console_line);
		text->insertRange(console_text, text_editor->point.index);
	} while (status == 1);
	const char* append = &(*text)[text_editor->point.index];
	text_editor->addCommit(new CommitChars(append, console_length, "", 0, text_editor->point));
}