#include "text_editor.h"
#include "ui.h"

void test_array() {
	Text array = "Array<int>(a, 3)";
	Text array2 = Text(array);

	std::cout << array << std::endl;
	std::cout << array2 << std::endl;
}

void testLine() {
	CheckListLine line;
	line.info.insertRange("Misha", 5, 0);
	line.status = true;
	std::cout << line.toText() << std::endl;
	Text* serializedText = line.serialize();
	std::cout << serializedText << std::endl;


	CheckListLine* newLine = new CheckListLine(*serializedText);
	std::cout << newLine->toText() << std::endl;
}

void textCipher() {
	ContactLine line;
	line.name.insertRange("abc", 3, 0);
	line.surname.insertRange("def", 3, 0);
	line.email.insertRange("misha@gmail.com", 15, 0);
	line.encrypt(1);
	std::cout << line.toText() << std::endl;
	line.decrypt(1);
	std::cout << line.toText() << std::endl;
}

int main(int argc, char* argv[]) {
	TextEditor* text_editor = new TextEditor();
	UI* parser = new UI(text_editor);

	while (1) {
		parser->run();
	}
	//test_array();
	//testLine();
	//textCipher();
}