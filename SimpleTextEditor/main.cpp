#include "array.h"
#include "console_parser.h"

int main(int argc, char* argv[]) {
	Buffer* buffer = new Buffer();
	Parser* parser = new Parser(buffer);

	while (1) {
		parser->run();
	}
}