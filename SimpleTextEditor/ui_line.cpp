#include "ui_line.h"

void UIPureLine::printFields()
{
	std::cout << "1 - text" << std::endl;
}

Text* UIPureLine::getTextFromField(size_t* out = nullptr)
{
	if (out != nullptr)
		*out = 0;
	return &line->text;
}

void UIContactLine::printFields()
{
	std::cout << "1 - name" << std::endl;
	std::cout << "2 - surname" << std::endl;
	std::cout << "3 - email" << std::endl;
}

Text* UIContactLine::getTextFromField(size_t* out = nullptr)
{
	std::cout << "1 - name" << std::endl;
	std::cout << "2 - surname" << std::endl;
	std::cout << "3 - email" << std::endl;
	size_t input = tools->readInteger();
	if (out != nullptr)
		*out = input;
	switch (input)
	{
	case 1:
		return &line->name;
	case 2:
		return &line->surname;
		break;
	case 3:
		return &line->email;
		break;
	default:
		return nullptr;
	}
}

void UICheckListLine::printFields()
{
	std::cout << "1 - info" << std::endl;
	std::cout << "2 - status" << std::endl;
}

Text* UICheckListLine::getTextFromField(size_t* out = nullptr)
{
	if (out != nullptr)
		*out = 0;
	return &line->info;
}
