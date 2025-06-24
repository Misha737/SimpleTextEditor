#include "line.h"

ContactLine::ContactLine() :
	name(Text()),
	surname(Text()),
	email(Text())
{
}

Text ContactLine::toText()
{
	Text text("Contact - ");
	text += name;
	text += " ";
	text += surname;
	text += ", E-mail: ";
	text += email;
	return text;
}

CheckListLine::CheckListLine() :
	info(Text()),
	status(false)
{
}

Text CheckListLine::toText()
{
	Text text = Text("[ ");
	text += (char*)(&this->status);
	text += " ] ";
	text += this->info;
	return text;
}

PureLine::PureLine() : text(Text())
{
}

Text PureLine::toText()
{
	return Text(text);
}