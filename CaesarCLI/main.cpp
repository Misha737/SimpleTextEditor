#include <iostream>
#include <windows.h>
#include <WinDef.h>
#include <string>

typedef char* (*ptr_encrypt)(char*, int);
typedef char* (*ptr_decrypt)(char*, int);

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

void cli(ptr_encrypt encrypt, ptr_decrypt decrypt) {
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
	HINSTANCE handle = LoadLibrary(TEXT("caesar_cipher.dll"));
	if (handle == nullptr || handle == INVALID_HANDLE_VALUE)
	{
		DWORD error = GetLastError();
		std::cout << "Lib not found" << " " << error << std::endl;
		return 1;
	}
	ptr_encrypt encrypt = (ptr_encrypt)GetProcAddress(handle, "encrypt");

	if (encrypt == NULL) {
		std::cout << "Proc not found" << std::endl;
		return 1;
	}

	ptr_decrypt decrypt = (ptr_decrypt)GetProcAddress(handle, "decrypt");
	if (decrypt == NULL) {
		std::cout << "Proc not found" << std::endl;
		return 1;
	}

	while (true) {
		cli(encrypt, decrypt);
	}

	FreeLibrary(handle);
}