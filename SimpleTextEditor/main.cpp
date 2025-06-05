#include "text_editor.h"
#include "console_parser.h"

int main(int argc, char* argv[]) {
	TextEditor* text_editor = new TextEditor();
	Parser* parser = new Parser(text_editor);

	while (1) {
		parser->run();
	}
}