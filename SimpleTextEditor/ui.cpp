#pragma once

#include "ui.h"

#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <iostream>

UI::UI(TextEditor* text_editor) : tools(UITools(text_editor)) {
	this->text_editor = text_editor;
	setUILine();
}

UI::~UI()
{
	if (ui_line != nullptr)
		delete ui_line;
}

void UI::setUILine()
{
	if (ui_line != nullptr)
		delete ui_line;
	Line* line = (*text_editor->buffer)[text_editor->point.line];
	if (PureLine* pure_line = dynamic_cast<PureLine*>(line)) {
		ui_line = new UIPureLine(pure_line, tools);
	}
	else if (ContactLine* contact_line = dynamic_cast<ContactLine*>(line)) {
		ui_line = new UIContactLine(contact_line, tools);
	}
	else if (CheckListLine* check_list_line = dynamic_cast<CheckListLine*>(line)) {
		ui_line = new UICheckListLine(check_list_line, tools);
	}
	else {
		throw std::exception("Line type doesn't exist");
	}
}

void UI::run() {
	printf("\nChoose the command: ");
	char input = _getche();
	printf("\033[2J\033[H");
	//system("cls");
	printf("\nChoose the command: %c", input);
	std::cout << std::endl;
	switch (input)
	{
	case '1':
	{
		size_t form;
		Text* text = ui_line->getTextFromField(&form);
		std::cout << "Enter a text to append: ";
		tools.append_text(text);
		break;
	}
	case '2': {
		std::cout << "1 - Pure line" << std::endl;
		std::cout << "2 - Contact line" << std::endl;
		std::cout << "3 - Check list line" << std::endl;
		switch (tools.readInteger())
		{
		case 1:
			text_editor->newLine(new PureLine());
			text_editor->addCommit(new CommitLine(new PureLine()));
			break;
		case 2:
			text_editor->newLine(new ContactLine());
			text_editor->addCommit(new CommitLine(new ContactLine()));
			break;
		case 3:
			text_editor->newLine(new CheckListLine());
			text_editor->addCommit(new CommitLine(new CheckListLine()));
			break;
		default:
			break;
		}
		printf("\nNew line is started\n");
		break;
	}
	case '3':
		printf("\nEnter the file name for saving: ");
		tools.readConsole();
		text_editor->fwriteBuffer(tools.console_line);
		printf("\nText has been saved successfully\n");
		break;
	case '4':
	{
		printf("\nEnter the file name for loading: ");
		tools.readConsole();
		//Buffer* old_buffer = text_editor->buffer->copy();
		try {
			text_editor->freadBuffer(tools.console_line);
			//text_editor->addCommit(new CommitBuffer(text_editor->buffer, old_buffer));
			printf("\nText has been loaded successfully\n");
		}
		catch (std::exception ex) {
			std::cout << "[Error] Couldn't load a file " << ex.what() << std::endl;
		}
		//delete old_buffer;
		break;
	}
	case '5':
		text_editor->printBuffer();
		break;
	case '6':
	{
		size_t form;
		Text* text = ui_line->getTextFromField(&form);
		std::cout << "Enter a text to insert: ";
		tools.insert_text(text);
		break;
	}
	case '7':
	{
		printf("\nEnter text to search: ");
		size_t found_counter = 0;
		tools.readConsole();
		Point point = text_editor->searchBuffer(tools.console_line, 0, 0);
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
			point = text_editor->searchBuffer(tools.console_line, point.line, point.index + 1);
		}
		break;
	}
	case '8':
	{
		size_t form;
		Text* text = ui_line->getTextFromField(&form);
		std::cout << "Enter length to delete" << std::endl;
		size_t length = tools.parseLength(text);
		if (length == 0)
			return;

		text->deleteRange(text_editor->point.index, length);
		break;
	}
	case 'r':
	{
		size_t form;
		Text* text = ui_line->getTextFromField(&form);
		std::cout << "Enter a text to replace: ";

		tools.insert_text(text);
		text->deleteRange(tools.text_editor->point.index + tools.console_length, tools.console_length);

		break;
	}
	break;
	case 'd':
		//text_editor->addCommit(new CommitBuffer(new Array<Line*>(), text_editor->buffer));
		text_editor->cleanBuffer();
		setUILine();
		std::cout << "\nA buffer has been cleaned\n";
		break;
	case 'c':
	{
		size_t from;
		Text* text = ui_line->getTextFromField(&from);
		size_t length = tools.parseLength(text);
		if (length != 0) {
			text_editor->copy(length, text);
			std::cout << "A text has been copied" << std::endl;
		}

		break;
	}
	case 'x':
	{
		size_t from;
		Text* text = ui_line->getTextFromField(&from);
		size_t length = tools.parseLength(text);
		if (length != 0) {
			text_editor->addCommit(new CommitChars("", 0, &text->getArray()[text_editor->point.index], length, text_editor->point, from));
			text_editor->copy(length, text);
			text_editor->deleteChars(*text, length);
			std::cout << "A text has been cut" << std::endl;
		}

		break;
	}
	case 'p':
	{
		size_t form;
		Text* line = ui_line->getTextFromField(&form);
		int length_pasted;
		if (length_pasted = text_editor->paste(line) == -1) {
			std::cout << "\nText pasting failed\n";
		}
		text_editor->addCommit(new CommitChars(&line->getArray()[text_editor->point.index], length_pasted, "", 0, text_editor->point, form));
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
		text_editor->setCursor(tools.parsePoint());
		setUILine();
		break;
	case 'h':
		std::cout << "\nline: " << text_editor->point.line + 1 << std::endl;
		std::cout << "Index: " << text_editor->point.index + 1 << std::endl;
		break;
	case 'j': {
		std::cout << "Enter a key to encrypt: ";
		int input = tools.readInteger();
		text_editor->encryptBuffer(input);
		break;
	}
	case 'k': {
		std::cout << "Enter a key to decrypt: ";
		int input = tools.readInteger();
		text_editor->decryptBuffer(input);
	}
	case 'i': {
		std::cout << "Enter a file path: ";
		tools.readConsole();
		Text path = Text(tools.console_line);
		path.add('\0');
		std::cout << "Enter a key to save in a file: ";
		int key = tools.readInteger();
		text_editor->saveEncrypted(key, path.getArray());
		break;
	}
	case 'o': {
		std::cout << "Enter a encrypted file path: ";
		tools.readConsole();
		Text path = tools.console_line;
		path.add('\0');
		std::cout << "Enter a key to load from a file: ";
		int key = tools.readInteger();
		text_editor->loadEncrypted(key, path.getArray());
		break;
	}
	case 'l': {
		std::cout << "Enter an input file path: ";
		tools.readConsole();
		Text input_path = tools.console_line;
		input_path.add('\0');

		std::cout << "Enter an output file path: ";
		tools.readConsole();
		Text output_path = tools.console_line;
		output_path.add('\0');

		std::cout << "Enter a key: ";
		int key = tools.readInteger();

		std::cout << "1 - encrypt" << std::endl << "2 - decrypt" << std::endl;
		int way = tools.readInteger();

		std::function<void(int)> algorithm;

		if (way == 1)
			algorithm = [this](int key) {
			text_editor->encryptBuffer(key);
			};
		else if (way == 2)
			algorithm = [this](int key) {
			text_editor->decryptBuffer(key);
			};
		text_editor->encryptFile(
			key,
			input_path.getArray(),
			output_path.getArray(),
			algorithm);
		break;
	}
	default:
		std::cout << "\nThe command is not implemented :(\n";
		break;
	}
}