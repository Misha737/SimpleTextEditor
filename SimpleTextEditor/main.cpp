#include "array.h"
#include "console_parser.h"

int main(int argc, char* argv[]) {
	TextEditor* buffer = new TextEditor();
	Parser* parser = new Parser(buffer);

	while (1) {
		parser->run();
	}
}