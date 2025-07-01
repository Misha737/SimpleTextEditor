#pragma once

#include "text.h"
#include <windows.h>

typedef char* (*ptr_encrypt)(char*, int);
typedef char* (*ptr_decrypt)(char*, int);

class CaesarCipher {
private:
	HINSTANCE handle;
	ptr_encrypt encrypt_ptr;
	ptr_decrypt decrypt_ptr;
	int key;
public:
	CaesarCipher(int key);
	~CaesarCipher();
	
	void encrypt(Text& text);
	void decrypt(Text& text);
};