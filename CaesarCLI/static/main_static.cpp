#include <iostream>
#include <windows.h>
#include <WinDef.h>
#include <string>
#include "caesar_cipher.h"

int readInteger() {
	char key_str[256];
	memset(key_str, '\0', 256);
	std::cin >> key_str;
	const char* start = key_str;
	char* end;

	long number = strtol(start, &end, 10);

	if (errno == ERANGE || end == start)
		return 0;

	return (int)number;
}

void cli() {
	std::cout << std::endl;
	char input[256];
	char command[2];
	int key = 0;

	memset(input, '\0', 256);
	memset(command, '\0', 2);


	std::cout << "Enter text: ";
	std::cin >> input;

	std::cout << "\nEnter key: ";
	key = readInteger();
	if (key == 0) return;


	std::cout << "\n1 - encrypt" << std::endl;
	std::cout << "2 - decrypt" << std::endl;
	std::cin >> command;

	switch (command[0])
	{
	case '1':
		std::cout << encrypt(input, key) << std::endl;
		break;
	case '2':
		std::cout << decrypt(input, key) << std::endl;
		break;
	default:
		std::cout << "Command is incorrect" << std::endl;
		break;
	}
}

int main(int argc, char* argv[]) {
	while (true) {
		cli();
	}
}