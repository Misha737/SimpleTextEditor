#include "ui_line.h"

void UIPureLine::printFields()
{
	std::cout << "1 - text" << std::endl;
}

Text* UIPureLine::getTextFromField()
{
	return &line->text;
}

void UIContactLine::printFields()
{
	std::cout << "1 - name" << std::endl;
	std::cout << "2 - surname" << std::endl;
	std::cout << "3 - email" << std::endl;
}

Text* UIContactLine::getTextFromField()
{
	std::cout << "1 - name" << std::endl;
	std::cout << "2 - surname" << std::endl;
	std::cout << "3 - email" << std::endl;
	switch (tools->readInteger())
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

Text* UICheckListLine::getTextFromField()
{
	return &line->info;
}
