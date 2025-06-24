#include "text_editor.h"
#include "console_parser.h"

void test_array() {
	Text array = "Array<int>(a, 3)";
	Text array2 = Text(array);

	std::cout << array << std::endl;
	std::cout << array2 << std::endl;
}

int main(int argc, char* argv[]) {
	TextEditor* text_editor = new TextEditor();
	Parser* parser = new Parser(text_editor);

	while (1) {
		parser->run();
	}
	//test_array();
}